/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "smartcom_netlink.h"
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/time.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/ip.h>
#include <net/netlink.h>
#include <uapi/linux/netlink.h>
#include "securec.h"
#include "smartcom_utils.h"
#ifdef CONFIG_HUAWEI_SCHEDULE_DRIVER
#include "smartcom/smartcom_schedule_driver.h"
#endif
#ifdef CONFIG_HW_SMARTCOM_NSTACK
#include "nstack/nstack.h"
#endif
#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
#include "mpflow/smartcom_mpflow.h"
#endif

#undef HWLOG_TAG
#define HWLOG_TAG smartcom_netlink
HWLOG_REGIST();
MODULE_LICENSE("GPL");

/************************************************************
                    MOCRO   DEFINES
*************************************************************/
DEFINE_MUTEX(smartcom_receive_sem);
DEFINE_MUTEX(smartcom_send_sem);
#define NL_SKB_QUEUE_MAXLEN    64

/********************************
    netlink variables for
    communicate between kernel and apk
*********************************/
static struct sock *g_smartcom_nlfd = NULL; /* netlink socket fd */
/* save user space progress pid when user space netlink socket registering. */
static uint32_t g_user_space_pid = 0;
static struct task_struct *g_smartcom_netlink_task = NULL;
static int g_smartcom_module_state = SMARTCOM_NETLINK_EXIT;
/* tcp protocol use this semaphone to inform smartcom netlink thread when data speed is slow */
static struct semaphore g_smartcom_netlink_sync_sema;
/* Queue of skbs to send to smartcomd */
static struct sk_buff_head g_smartcom_skb_queue;

void smartcom_send_msg2daemon(int cmd, const void *data, int len)
{
	struct nlmsghdr *nlh = NULL;
	struct sk_buff *pskb_out = NULL;

	smartcom_logd(": cmd = %d", cmd);

	if (g_smartcom_module_state != SMARTCOM_NETLINK_INIT ||
	    skb_queue_len(&g_smartcom_skb_queue) > NL_SKB_QUEUE_MAXLEN) {
		smartcom_loge(" : state wrong");
		return;
	}

	/* May be called in any context. */
	pskb_out = nlmsg_new(len, GFP_ATOMIC);
	if (!pskb_out) {
		smartcom_loge(" : Out of memry");
		return; /* Out of memory */
	}

	nlh = nlmsg_put(pskb_out, 0, 0, cmd, len, 0);
	if (!nlh) {
		kfree_skb(pskb_out);
		return; /* Out of memory */
	}

	NETLINK_CB(pskb_out).dst_group = 0; /* For unicast */ /*lint !e545*/
	if (data && (len > 0)) {
		if (memcpy_s((void *)nlmsg_data(nlh), len, (const void *)data, len) != EOK) {
			kfree_skb(pskb_out);
			return;
		}
	}
	skb_queue_tail(&g_smartcom_skb_queue, pskb_out);
	up(&g_smartcom_netlink_sync_sema);

	return;
}

/************************************************************
                    STATIC  FUNCTION  DEFINES
*************************************************************/
/* smartcom common event process function. The event come frome smartcom daemon. */
static void smartcom_common_evt_proc(const struct nlmsghdr *nlh, const uint8_t *data, uint16_t len)
{
	if (nlh == NULL)
		return;

	switch (nlh->nlmsg_type) {
	case NETLINK_SMARTCOM_DK_REG:
	/* save user space progress pid when register netlink socket. */
		g_user_space_pid = nlh->nlmsg_pid;
		smartcom_logd("smartcom netlink receive reg packet: g_user_space_pid = %d", nlh->nlmsg_pid);
		break;
	case NETLINK_SMARTCOM_DK_UNREG:
		smartcom_logd("smartcom netlink receive unreg packet");
		g_user_space_pid = 0;
		break;
	default:
		smartcom_logi("smartcom unsupport packet, the type is %d", nlh->nlmsg_type);
		break;
	}
}


/* netlink socket's callback function,it will be called by system when user space send a message to kernel.
this function will save user space progress pid. */
static void kernel_smartcom_receive(struct sk_buff *__skb)
{
	struct nlmsghdr *nlh = NULL;
	struct sk_buff *skb = NULL;
	void *packet = NULL;
	uint16_t data_len;
	uint8_t submod;

	skb = skb_get(__skb);

	mutex_lock(&smartcom_receive_sem);

	if (skb->len >= NLMSG_HDRLEN) {
		nlh = nlmsg_hdr(skb);
		packet = nlmsg_data(nlh);
		data_len = nlmsg_len(nlh);

		if ((nlh->nlmsg_len >= sizeof(struct nlmsghdr)) &&
			(skb->len >= nlh->nlmsg_len)) {
			submod = (nlh->nlmsg_type & SMARTCOM_SUB_MOD_MASK) >> SMARTCOM_SUB_MOD_MASK_LEN;
			switch (submod) {
			case SMARTCOM_SUB_MOD_COMMON:
				smartcom_common_evt_proc(nlh, packet, data_len);
				break;
#ifdef CONFIG_HUAWEI_SCHEDULE_DRIVER
			case SMARTCOM_SUB_MOD_SCHEDULE_DRIVE:
				smartcom_schedule_driver_evt_proc(nlh->nlmsg_type, packet, data_len);
				break;
#endif
#ifdef CONFIG_HW_SMARTCOM_NSTACK
			case SMARTCOM_SUB_MOD_NSTACK:
				nstack_event_process(nlh->nlmsg_type, packet, data_len);
				break;
#endif
#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
			case SMARTCOM_SUB_MOD_MPFLOW:
				smartcom_mpflow_event_process(nlh->nlmsg_type, packet, data_len);
				break;
#endif
			default:
				smartcom_logi("smartcom netlink unsupport subMod, the subMod is %d", submod);
				break;
			}
			}
	}
	mutex_unlock(&smartcom_receive_sem);
	consume_skb(__skb);
}

/* netlink socket thread,
* 1.it will recieve the message from kernel;
* 2.maybe do some data process job;
* 3.send a message to user space;
*/
static int smartcom_netlink_thread(void* data)
{
	struct sk_buff *skb = NULL;
	smartcom_logd("smartcom_netlink_thread thread start");
	while (1) {
		if (kthread_should_stop()) {
			break;
		}

		/* netlink thread will block at this semaphone when no data coming. */
		smartcom_logd("smartcom_netlink_thread start sema success!");
		down(&g_smartcom_netlink_sync_sema);
		smartcom_logd("get sema success!");

		do {
			skb = skb_dequeue(&g_smartcom_skb_queue);
			if (skb == NULL) {
				continue;
			}
			if (g_user_space_pid) {
				netlink_unicast(g_smartcom_nlfd, skb, g_user_space_pid, MSG_DONTWAIT);
				smartcom_logd("get sema success!");
			} else {
				kfree_skb(skb);
			}
		} while (!skb_queue_empty(&g_smartcom_skb_queue));
	}
	return 0;
}

/* netlink init function. */
static void smartcom_netlink_init(void)
{
	struct netlink_kernel_cfg smartcom_nl_cfg = {
	      .input = kernel_smartcom_receive,
	};

	skb_queue_head_init(&g_smartcom_skb_queue);
	g_smartcom_nlfd = netlink_kernel_create(&init_net, NETLINK_HW_SMARTCOM, &smartcom_nl_cfg);
	if (!g_smartcom_nlfd)
		smartcom_loge(" %s: init failed", __func__);
	else
		smartcom_logi("%s: init success", __func__);

	sema_init(&g_smartcom_netlink_sync_sema, 0);
	g_smartcom_netlink_task = kthread_run(smartcom_netlink_thread, NULL, "smartcom_netlink_thread");
	if (IS_ERR(g_smartcom_netlink_task)) {
		smartcom_logd("%s:smartcom_netlink_thread init ERROR", __func__);
		return;
	}
	if (g_smartcom_netlink_task == NULL)
		smartcom_logd("%s:smartcom_netlink_thread init failed", __func__);
	return;
}

/* netlink deinit function. */
static void smartcom_netlink_deinit(void)
{
	if (g_smartcom_nlfd && (g_smartcom_nlfd->sk_socket)) {
		sock_release(g_smartcom_nlfd->sk_socket);
		g_smartcom_nlfd = NULL;
	}

	if (g_smartcom_netlink_task) {
		kthread_stop(g_smartcom_netlink_task);
		g_smartcom_netlink_task = NULL;
	}
}

static int __init smartcom_netlink_module_init(void)
{
	smartcom_netlink_init();

#ifdef CONFIG_HUAWEI_SCHEDULE_DRIVER
	smartcom_schedule_driver_init();
#endif

#ifdef CONFIG_HW_SMARTCOM_NSTACK
	nstack_init(g_smartcom_nlfd);
#endif

#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
	smartcom_mpflow_init();
#endif

	g_smartcom_module_state = SMARTCOM_NETLINK_INIT;

	return 0;
}

static void __exit smartcom_netlink_module_exit(void)
{
	g_smartcom_module_state = SMARTCOM_NETLINK_EXIT;

#ifdef CONFIG_HUAWEI_SCHEDULE_DRIVER
	smartcom_schedule_driver_clear();
#endif

#ifdef CONFIG_HW_SMARTCOM_NSTACK
	nstack_deinit();
#endif

#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
	smartcom_mpflow_deinit();
#endif

	smartcom_netlink_deinit();
}

module_init(smartcom_netlink_module_init);
module_exit(smartcom_netlink_module_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("smartcom module driver");

