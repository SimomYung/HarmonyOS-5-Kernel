/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This file is performance statistics for data transmission.
 * Author: lianchaofeng@huawei.com
 * Create: 2019-12-10
 */
#include <asm/div64.h>
#include <linux/cpufreq.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/smp.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/tcp.h>
#include <linux/time.h>
#include <linux/types.h>
#include <linux/udp.h>
#include <linux/netdevice.h>
#include <net/inet_sock.h>
#include <securec.h>

#include "hw_pt.h"
#include "../chr/chr_netlink.h"

#define DS_NET_PREFIX "rmnet"
#define WLAN_NET "wlan0"
#define RNIC_UL_DELAY_LIMIT (HZ / 10)
#define RNIC_DL_DELAY_LIMIT (HZ / 20)
#define L4_DL_DELAY_LIMIT (HZ / 10)
#define MAX_LISTENING_UID_NUM 1
#define PACKET_DELAY_DATA_LEN 64
#define QUEUE_PACKET_STATS_DATA_LEN 2048
#define QUEUE_PACKET_ERROR_DATA_LEN 32
#define MIN_APP_UID 10000

#define assign_short(p, val) (*(s16 *)(p) = (val))
#define assign_int(p, val) (*(s32 *)(p) = (val))
#define assign_long(p, val) (*(s64 *)(p) = (val))
#define skip_byte(p, num) ((p) = (p) + (num))

// units: jiffie(==4ms)
#define MAX_UL_TRANS_TIME 10
#define MAX_DL_TRANS_TIME 28

#define MAX_DL_RECV_QUEUE_WAIT_TIME 2000
#define TIMER_EXCEED_MAX (600 * HZ)
#define TIMER_STATISTICS_PEROID (10 * HZ)
#define TIMER_REPORT_BACKOFF_PEROID (3600 * HZ)
#define CHR_TIMER_NOT_START 0
#define CHR_TIMER_STATISTICS 1
#define CHR_TIMER_REPORT_WAIT 2

#define KERNEL_ABNORMAL_PACK_CONTS 10
#define RECV_QUEUE_ABNORMAL_PACK_CONTS 4000
#define KENRL_KEY_LENGTH_MAX 32
#define TCP_HDR_DOFF_QUAD 4
#define TCP_HDR_IHL_QUAD 4

static notify_event *notifier = NULL;

enum DIR {
	DOWNLINK_IN = 0,
	UPLINK = 1,
	DOWNLINK_OUT = 2,
};
static int g_hw_pt_ul_data_time_cost = 0;
static int g_hw_pt_ul_data_count = 0;
static int g_hw_pt_ul_abnormal_data_count = 0;

static int g_hw_pt_dl_data_time_cost = 0;
static int g_hw_pt_dl_in_data_count = 0;
static int g_hw_pt_dl_in_abnormal_data_count = 0;
static int g_hw_pt_dl_out_abnormal_data_count = 0;

static int g_hw_pt_ul_abnormal_data_not_rpt_count = 0;
static int g_hw_pt_dl_abnormal_data_not_rpt_count = 0;

static int g_chr_data_process_timer_state = CHR_TIMER_NOT_START;
static struct timer_list g_kernel_data_process_timer;

static int g_last_app_top_uid = 0;

static struct hw_buffer_queue_info_lst g_buffer_queue_stat[INTERFACE_SIZE];
static unsigned long g_app_large_delay_err_rpt_time = 0;
static unsigned long g_app_queue_stats_rpt_time = 0;
static struct sk_flow *g_flow_list;
static int g_serious_hashcnt = 0;
static const int g_tcp_zero_window_interval = 15 * 60 * 1000;
static unsigned long long g_tcp_zero_window_rpt_time = 0;
 
static spinlock_t g_queue_in_lock;
static spinlock_t g_queue_out_lock;
static spinlock_t g_tcp_zero_window_lock;

void create_flow_list(void)
{
	g_flow_list = kmalloc(sizeof(struct sk_flow) * HASH_MAX,
		GFP_KERNEL);
	if (g_flow_list != NULL) {
		memset_s(g_flow_list,  sizeof(struct sk_flow) * HASH_MAX, 0, sizeof(struct sk_flow) * HASH_MAX);
	}
}

static int get_top_app_index(int uid)
{
	int i;
	for (i = 0; i < CHR_MAX_REPORT_APP_COUNT; i++) {
		if (uid == get_report_top_app(i)) {
			return i;
		}
	}
	return APP_UID_INVALID;
}

static int get_skb_interface(struct sock *sk, struct sk_buff *skb)
{
	struct net_device *dev = NULL;
	struct dst_entry *dst = NULL;
	if (sk == NULL || skb == NULL) {
		return INVALID_INTERFACE;
	}
	dst = sk_dst_get(sk);
	if (dst) {
		dev = dst->dev;
		dst_release(dst);
	} else {
		dev = dev_get_by_index_rcu(sock_net(sk), skb->skb_iif);
	}
	if (dev == NULL) {
		return INVALID_INTERFACE;
	}
	if (strncmp(dev->name, DS_NET_PREFIX, strlen(DS_NET_PREFIX)) == 0) {
		return RMNET_INTERFACE;
	}
	if (strncmp(dev->name, WLAN_NET, strlen(WLAN_NET)) == 0) {
		return WLAN_INTERFACE;
	}
	return INVALID_INTERFACE;
}

void report_queue_conti_large_delay(int count, int duration, int interface, int uid, u8 proto)
{
    if (!notifier)
		return;
	pr_info("%s, %d, %d, %d, %d, %d", __func__, interface, uid, count, duration, proto);
	if (jiffies - g_app_large_delay_err_rpt_time < QUEUE_ERROR_REPORT_TIME) {
        pr_info("%s, in punish time", __func__);
        return;
    }
    char event[QUEUE_PACKET_ERROR_DATA_LEN] = {0};
	char *p = event;
 
    assign_short(p, PACKET_QUEUE_ERROR_RPT);
	skip_byte(p, sizeof(s16));
 
	assign_short(p, 24);
	skip_byte(p, sizeof(s16));
    assign_int(p, uid);
	skip_byte(p, sizeof(int));
    assign_int(p, count);
	skip_byte(p, sizeof(int));
    assign_int(p, duration);
	skip_byte(p, sizeof(int));
    assign_int(p, interface);
	skip_byte(p, sizeof(int));
	assign_int(p, proto);
	skip_byte(p, sizeof(int));
    notifier((struct res_msg_head *)event);
    g_app_large_delay_err_rpt_time = jiffies;
}

static void notify_queue_stat_event(void)
{
	if (!notifier)
		return;
    int dataLen = sizeof(short) * 2 + CHR_MAX_REPORT_APP_COUNT * sizeof(int) +
        sizeof(struct hw_buffer_queue_info_lst) * INTERFACE_SIZE;
    char event[QUEUE_PACKET_STATS_DATA_LEN] = {0};
	char *p = event;
    assign_short(p, PACKET_QUEUE_STAT_RPT);
	skip_byte(p, sizeof(s16));
	assign_short(p, dataLen);
	skip_byte(p, sizeof(s16));
    int index;
    for (index = 0; index < CHR_MAX_REPORT_APP_COUNT; index++) {
        u32 appUid = get_report_top_app(index);
        assign_int(p, appUid);
	    skip_byte(p, sizeof(int));
		int interface = 0;
		for (interface = 0; interface < INTERFACE_SIZE; interface++) {
			struct hw_buffer_queue_info *info = &g_buffer_queue_stat[interface].report_app_stat[index];
			assign_long(p, info->app_recv_pkg_count);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->queue_recv_pkg_count);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->app_recv_large_delay_count);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->queue_recv_large_delay_count);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->app_recv_large_delay_total_delay);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->queue_recv_large_delay_total_delay);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->app_recv_total_delay);
			skip_byte(p, sizeof(u64));
			assign_long(p, info->queue_recv_total_delay);
			skip_byte(p, sizeof(u64));
			assign_int(p, info->app_longest_nurecv_delay);
			skip_byte(p, sizeof(int));
			assign_int(p, info->app_recv_continuously_large_count);
			skip_byte(p, sizeof(int));
			assign_int(p, info->app_recv_continuously_large_max_count);
			skip_byte(p, sizeof(int));
			if (info->app_recv_pkg_count > 0) {
				pr_info("%s, %d, %d, %llu, %llu, %llu, %u", __func__, appUid, interface,
					info->app_recv_pkg_count, info->app_recv_large_delay_count,
					info->app_recv_large_delay_total_delay, info->app_recv_continuously_large_max_count);
			}
		}
    }
    notifier((struct res_msg_head *)event);
}

static void chr_queue_stat_report(void)
{
	spin_lock_bh(&g_queue_out_lock);
	if (jiffies - g_app_queue_stats_rpt_time < QUEUE_REPORT_TIME) {
		spin_unlock_bh(&g_queue_out_lock);
		return;
	}
	g_app_queue_stats_rpt_time = jiffies;
	spin_unlock_bh(&g_queue_out_lock);
    notify_queue_stat_event();
	spin_lock_bh(&g_queue_out_lock);
    memset_s(&g_buffer_queue_stat, sizeof(g_buffer_queue_stat), 0, sizeof(g_buffer_queue_stat));
	spin_unlock_bh(&g_queue_out_lock);
}

static uid_t get_uid_from_sock_new(struct sock *sk)
{
	kuid_t kuid;

	if (!sk || !sk_fullsock(sk))
		return overflowuid;
	kuid = sock_net_uid(sock_net(sk), sk);
	return from_kuid_munged(sock_net(sk)->user_ns, kuid);
}

static bool is_fore_app_socket(struct sock *sk)
{
    uid_t uid = get_uid_from_sock(sk);
    return (get_app_top_uid() > MIN_APP_UID) && (uid == get_app_top_uid());
}

static void chr_kernel_clear_data_record(void)
{
	g_hw_pt_ul_data_time_cost = 0;
	g_hw_pt_ul_data_count = 0;
	g_hw_pt_ul_abnormal_data_count = 0;

	g_hw_pt_dl_data_time_cost = 0;
	g_hw_pt_dl_in_data_count = 0;
	g_hw_pt_dl_in_abnormal_data_count = 0;
	g_hw_pt_dl_out_abnormal_data_count = 0;

	g_hw_pt_ul_abnormal_data_not_rpt_count = 0;
	g_hw_pt_dl_abnormal_data_not_rpt_count = 0;
}

static bool is_chr_report_needed(void)
{
	return (((g_hw_pt_ul_abnormal_data_count +
		g_hw_pt_dl_in_abnormal_data_count) >
		KERNEL_ABNORMAL_PACK_CONTS) ||
		(g_hw_pt_ul_abnormal_data_count >
		KERNEL_ABNORMAL_PACK_CONTS) ||
		(g_hw_pt_dl_in_abnormal_data_count >
		KERNEL_ABNORMAL_PACK_CONTS) ||
		(g_hw_pt_dl_out_abnormal_data_count >
		RECV_QUEUE_ABNORMAL_PACK_CONTS));
}

static void prepare_report_data(struct hw_kernel_delay_info *info_rpt)
{
	info_rpt->uplink_delay_count = g_hw_pt_ul_abnormal_data_count;
	info_rpt->uplink_data_count = g_hw_pt_ul_data_count;
	if (g_hw_pt_ul_abnormal_data_count == 0)
		info_rpt->uplink_time_cost_ave = 0;
	else
		info_rpt->uplink_time_cost_ave = g_hw_pt_ul_data_time_cost /
			g_hw_pt_ul_abnormal_data_count;

	info_rpt->downlink_delay_cnt = g_hw_pt_dl_in_abnormal_data_count;
	info_rpt->downlink_data_count = g_hw_pt_dl_in_data_count;
	if (g_hw_pt_dl_in_abnormal_data_count == 0)
		info_rpt->downlink_time_cost_ave = 0;
	else
		info_rpt->downlink_time_cost_ave = g_hw_pt_dl_data_time_cost /
			g_hw_pt_dl_in_abnormal_data_count;

	info_rpt->downlink_out_wait_cnt = g_hw_pt_dl_out_abnormal_data_count;
	info_rpt->uplink_abnormal_not_report_cnt =
		g_hw_pt_ul_abnormal_data_not_rpt_count;
	info_rpt->downlink_abnormal_not_report_cnt =
		g_hw_pt_dl_abnormal_data_not_rpt_count;
}

static void set_chr_report_timer(int status, int len)
{
	pr_err("%s: ENTER, old status = %d, new status = %d, timerLen = %d \n",
		__func__, g_chr_data_process_timer_state, status, len);
	if (status == CHR_TIMER_NOT_START || len <= 0)
		return;
	if (g_chr_data_process_timer_state == CHR_TIMER_NOT_START && !timer_pending(&g_kernel_data_process_timer)) {
		mod_timer(&g_kernel_data_process_timer, jiffies + len);
		g_chr_data_process_timer_state = status;
	}
}

static bool is_top_app_changed(void)
{
	return (get_app_top_uid() > MIN_APP_UID) && (g_last_app_top_uid != get_app_top_uid());
}

static void notify_packet_delay_event(int uid, struct hw_kernel_delay_info *info_rpt)
{
	char event[PACKET_DELAY_DATA_LEN] = {0};
	char *p = event;

	if (!notifier)
		return;

	assign_short(p, PACKET_DELAY_RPT);
	skip_byte(p, sizeof(s16));

	// data len(2B type + 2B len + 40B DATA), type is short
	assign_short(p, 44);
	skip_byte(p, sizeof(s16));

	assign_int(p, uid);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->uplink_delay_count);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->uplink_data_count);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->uplink_time_cost_ave);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->downlink_delay_cnt);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->downlink_data_count);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->downlink_time_cost_ave);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->downlink_out_wait_cnt);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->uplink_abnormal_not_report_cnt);
	skip_byte(p, sizeof(int));

	assign_int(p, info_rpt->downlink_abnormal_not_report_cnt);

	notifier((struct res_msg_head *)event);
}

static bool is_socket_belong_fg_app(struct sock *sk)
{
	uid_t uid = get_uid_from_sock_new(sk);
	struct hw_kernel_delay_info info_rpt = {0};

	if (is_top_app_changed()) {
		if (g_chr_data_process_timer_state == CHR_TIMER_STATISTICS) {
			if (!del_timer(&g_kernel_data_process_timer))
                return false;

			g_chr_data_process_timer_state = CHR_TIMER_NOT_START;
			if (is_chr_report_needed()) {
				prepare_report_data(&info_rpt);
				notify_packet_delay_event(g_last_app_top_uid, &info_rpt);
				set_chr_report_timer(CHR_TIMER_REPORT_WAIT, TIMER_REPORT_BACKOFF_PEROID);
			}
		}
		chr_kernel_clear_data_record();
		g_last_app_top_uid = get_app_top_uid();
	}
	return (get_app_top_uid() > MIN_APP_UID) && (uid == get_app_top_uid());
}

static void do_commands(struct req_msg_head *msg, u32 len)
{
	if (!msg) {
		pr_err("msg is null\n");
		return;
	}
	return;
}

static void hw_pt_update_counters(int dir, int time_cost)
{
	if (dir == UPLINK) {
		g_hw_pt_ul_abnormal_data_count++;
		g_hw_pt_ul_data_time_cost += time_cost;
	} else if (dir == DOWNLINK_IN) {
		g_hw_pt_dl_in_abnormal_data_count++;
		g_hw_pt_dl_data_time_cost += time_cost;
	} else {
		g_hw_pt_dl_out_abnormal_data_count++;
	}
}

static void hw_pt_exception_happen(int dir, int time_cost)
{
	if (dir == UPLINK)
		g_hw_pt_ul_data_count++;
	else if (dir == DOWNLINK_IN)
		g_hw_pt_dl_in_data_count++;

	if (time_cost >= MAX_UL_TRANS_TIME) {
		switch (g_chr_data_process_timer_state) {
		case CHR_TIMER_NOT_START:
			set_chr_report_timer(CHR_TIMER_STATISTICS, TIMER_STATISTICS_PEROID);
			hw_pt_update_counters(dir, time_cost);
			break;
		case CHR_TIMER_STATISTICS:
			hw_pt_update_counters(dir, time_cost);
			break;
		case CHR_TIMER_REPORT_WAIT:
			if (dir == UPLINK) {
				g_hw_pt_ul_abnormal_data_not_rpt_count++;
			} else {
				g_hw_pt_dl_abnormal_data_not_rpt_count++;
			}
			break;
		default:
			break;
		}
	}
}

int get_time_cost(u64 start_time)
{
	if (jiffies <= start_time) {
		return 0;
	}
	return jiffies_to_msecs(jiffies - start_time);
}

void set_skb_shinfo(struct sk_buff *skb, int bitmask)
{
	struct skb_shared_info *shinfo = NULL;
	if (skb == NULL)
		return;
	shinfo = skb_shinfo(skb);
	if (shinfo != NULL) {
		shinfo->born_stamp = jiffies;
		shinfo->proc_bitmask = bitmask;
	}
}

void hw_pt_set_skb_long_stamp(struct sk_buff *skb)
{
	set_skb_shinfo(skb, PROC_BITMASK_SETTED);
}
EXPORT_SYMBOL(hw_pt_set_skb_long_stamp);

void hw_pt_set_skb_stamp(struct sk_buff *skb)
{
	struct skb_shared_info *shinfo = NULL;
	if (skb == NULL)
		return;
	shinfo = skb_shinfo(skb);
	if (shinfo != NULL) {
		shinfo->born_stamp = jiffies;
		shinfo->proc_bitmask = PROC_BITMASK_SETTED;
	}
}
EXPORT_SYMBOL(hw_pt_set_skb_stamp);

static bool hw_pt_log_exception_need_continue(struct sk_buff *skb)
{
	struct skb_shared_info *shinfo = NULL;
	struct iphdr *ih = NULL;

	shinfo = skb_shinfo(skb);
	if (shinfo == NULL) {
		pr_err("%s ERR: shinfo is NULL \n", __func__);
		return false;
	}

	if ((shinfo->proc_bitmask & PROC_BITMASK_SET_MASK) != PROC_BITMASK_SETTED
		|| shinfo->born_stamp == 0)
		return false;

	if (skb->protocol == ntohs(ETH_P_IP)) {
		ih = ip_hdr(skb);
		if (ih->saddr == ih->daddr)
			return false;
	}
	return true;
}

static bool hw_pt_log_check_recv_skb(struct sk_buff *skb)
{
	struct skb_shared_info *shinfo = NULL;
	struct iphdr *ih = NULL;
	struct ipv6hdr *ip6h = NULL;
	if (skb == NULL) {
		return false;
	}
	shinfo = skb_shinfo(skb);
	if (shinfo == NULL) {
		pr_err("%s ERR: shinfo is NULL \n", __func__);
		return false;
	}

	if (shinfo->born_stamp == 0) {
		return false;
	}

	if (shinfo->proc_bitmask != PROC_BITMASK_SETTED &&
		shinfo->proc_bitmask != PROC_BITMASK_QUEUE_RECV) {
		return false;
	}
	if (skb->protocol == ntohs(ETH_P_IP)) {
		ih = ip_hdr(skb);
		if (ih->saddr == ih->daddr)
			return false;
	}
	if (skb->protocol == htons(ETH_P_IPV6)) {
		ip6h = ipv6_hdr(skb);
		if (ipv6_address_equal(ip6h->saddr, ip6h->daddr))
			return false;
	}
	return true;
}

static void hw_pt_log_exception(struct sock *sk, struct sk_buff *skb, int prot,
	int delay, const char *func, int dir)
{
	struct tcphdr *th = NULL;
	struct skb_shared_info *shinfo = NULL;
	int time_cost;

	if (!hw_pt_log_exception_need_continue(skb))
		return;

	shinfo = skb_shinfo(skb);
	time_cost = jiffies_to_msecs(jiffies - shinfo->born_stamp);
	if (time_cost > TIMER_EXCEED_MAX)
		return;

	switch (prot) {
	case IPPROTO_TCP:
		th = tcp_hdr(skb);
		if (th->fin == 1 || th->rst == 1) {
			break;
		}
		hw_pt_exception_happen(dir, time_cost);
		break;
	case IPPROTO_UDP:
		hw_pt_exception_happen(dir, time_cost);
		break;
	default:
		break;
	}
}

int get_increase_value(u32 value)
{
	u32 origin_value = value;
	u32 new_value = value + 1;
	do_div(origin_value, CONTINUE_LARGEDELAY_COUNT_THRESHOLD);
	do_div(new_value, CONTINUE_LARGEDELAY_COUNT_THRESHOLD);
	if (new_value > origin_value) {
		return 1;
	}
	return 0;
}

bool set_flow_data(struct sock *sk, struct sk_buff *skb, u8 *hash_cnt, u8 *proto)
{
	struct iphdr *iph = NULL;
	struct tcphdr *tcph = NULL;
	struct ipv6hdr *ip6h = NULL;
	struct udphdr *udph = NULL;
	u32 port = 0;
	if (skb == NULL || sk == NULL) {
		return false;
	}
	if (sk->sk_protocol == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		if (tcph == NULL) {
			return false;
		}
		port = tcph->dest;
	}
	if (sk->sk_protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		if (udph == NULL) {
			return false;
		}
		port = udph->dest;
	}
	if (port == 0) {
		return false;
	}
	if (skb->protocol == ntohs(ETH_P_IP)) {
		iph = ip_hdr(skb);
		if (iph == NULL) {
			return false;
		}
		*proto = IPV4_NETWORK;
		*hash_cnt = hash3(iph->saddr, iph->daddr, port);
		return true;
	}
	if (skb->protocol == ntohs(ETH_P_IPV6)) {
		ip6h = ipv6_hdr(skb);
		if (ip6h == NULL) {
			return false;
		}
		*proto = IPV6_NETWORK;
		*hash_cnt = hash3_v6(ip6h->daddr, ip6h->saddr, port);
		return true;
	}
	return false;
}

void set_flow_list_slow_data(u8 hash_cnt, u64 born_stamp)
{
	g_flow_list[hash_cnt].continue_slow_count++;
	if (g_flow_list[hash_cnt].continue_slow_first_time == 0) {
		g_flow_list[hash_cnt].continue_slow_first_time = born_stamp;
	}
	if (g_flow_list[hash_cnt].continue_slow_count >
		g_flow_list[g_serious_hashcnt].continue_slow_count) {
			g_serious_hashcnt = hash_cnt;
	}
}

static void hw_pt_reset_app_continue_info(int uid, int interface)
{
	struct sk_flow info = g_flow_list[g_serious_hashcnt];
	if (info.is_valid == IS_USE &&
		info.continue_slow_count >= CONTINUE_RECV_LARGEDELAY_COUNT_THRESHOLD
	) {
		int duration = get_time_cost(info.continue_slow_first_time);
		report_queue_conti_large_delay(info.continue_slow_count, duration,
			interface, uid, info.proto);
	}
	spin_lock_bh(&g_queue_out_lock);
	memset_s(g_flow_list,  sizeof(struct sk_flow) * HASH_MAX, 0, sizeof(struct sk_flow) * HASH_MAX);
	spin_unlock_bh(&g_queue_out_lock);
}

static void hw_pt_queue_recv_calc(int app_index, int interface, int time_cost)
{
	spin_lock_bh(&g_queue_in_lock);
	struct hw_buffer_queue_info *info = &g_buffer_queue_stat[interface].report_app_stat[app_index];
    info->queue_recv_pkg_count++;
    info->queue_recv_total_delay += time_cost;
    if (time_cost >= QUEUE_LARGE_DELAY_THRESHOLD) {
        info->queue_recv_large_delay_count++;
        info->queue_recv_large_delay_total_delay += time_cost;
    }
	spin_unlock_bh(&g_queue_in_lock);
}

static void hw_pt_app_recv_calc(struct sock *sk, struct sk_buff *skb, int app_index,
	int interface, struct skb_shared_info *shinfo)
{
	u8 hash_cnt;
	u8 proto;
	if (!set_flow_data(sk, skb, &hash_cnt, &proto)) {
		return;
	}
	int time_cost = get_time_cost(shinfo->born_stamp);
	spin_lock_bh(&g_queue_out_lock);
	struct hw_buffer_queue_info *info = &g_buffer_queue_stat[interface].report_app_stat[app_index];
	if (g_flow_list[hash_cnt].is_valid == IS_UNUSE) {
		g_flow_list[hash_cnt].proto = proto;
	}
	g_flow_list[hash_cnt].is_valid = IS_USE;
	if (time_cost < QUEUE_SLOW_DURATION_MAX_THRESHOLD) {
		info->app_recv_pkg_count++;
    	info->app_recv_total_delay += time_cost;
	}
    if (time_cost >= QUEUE_LARGE_DELAY_THRESHOLD) {
		if (time_cost < QUEUE_SLOW_DURATION_MAX_THRESHOLD) {
			info->app_recv_continuously_large_count +=
				get_increase_value(g_flow_list[hash_cnt].continue_slow_count);
			info->app_recv_large_delay_count++;
			info->app_recv_large_delay_total_delay += time_cost;
			if (g_flow_list[hash_cnt].continue_slow_count > info->app_recv_continuously_large_max_count) {
				info->app_recv_continuously_large_max_count = g_flow_list[hash_cnt].continue_slow_count;
			}
			set_flow_list_slow_data(hash_cnt, shinfo->born_stamp);
		}
    } else {
		spin_unlock_bh(&g_queue_out_lock);
		if (g_flow_list[hash_cnt].continue_slow_count >= CONTINUE_RECV_LARGEDELAY_COUNT_THRESHOLD &&
				g_flow_list[hash_cnt].continue_slow_first_time > 0) {
			int duration = get_time_cost(g_flow_list[hash_cnt].continue_slow_first_time);
			report_queue_conti_large_delay(g_flow_list[hash_cnt].continue_slow_count,
				duration, interface, get_uid_from_sock_new(sk), proto);
    	}
		spin_lock_bh(&g_queue_out_lock);
		g_flow_list[hash_cnt].continue_slow_count = 0;
		g_flow_list[hash_cnt].continue_slow_first_time = 0;
    }
	if (time_cost > info->app_longest_nurecv_delay) {
        info->app_longest_nurecv_delay = time_cost;
    }
	set_skb_shinfo(skb, PROC_BITMASK_APP_RECV);
	spin_unlock_bh(&g_queue_out_lock);
	chr_queue_stat_report();
}

static bool hw_pt_is_uplink_need_continue(struct sk_buff *skb, struct net_device *dev)
{
	if (!skb || !dev) {
		pr_err("%s ERR: input params is null\n", __func__);
		return false;
	}

	if (!is_socket_belong_fg_app(skb->sk))
		return false;

	if (strncmp(dev->name, DS_NET_PREFIX, strlen(DS_NET_PREFIX)))
		return false;

	if ((skb->protocol != ntohs(ETH_P_IP)) && (skb->protocol != ntohs(ETH_P_IPV6))) {
		pr_err("%s ERR: skb->protocol = %d \n", __func__, skb->protocol);
		return false;
	}
	return true;
}

static int get_layer4_protocl(struct sk_buff *skb)
{
	struct iphdr *iph = NULL;
	struct ipv6hdr *ip6h = NULL;
	__u8 l4_prot = 0;

	if (skb->protocol == ntohs(ETH_P_IP)) {
		iph = (struct iphdr *)(skb_network_header(skb));
		if (!iph || (iph->saddr == iph->daddr))
			return 0;
		l4_prot = iph->protocol;
	} else if (skb->protocol == ntohs(ETH_P_IPV6)) {
		ip6h = (struct ipv6hdr *)skb_network_header(skb);
		if (!ip6h)
			return 0;
		l4_prot = ip6h->nexthdr;
	}
	return l4_prot;
}

void hw_pt_dev_uplink(struct sk_buff *skb, struct net_device *dev)
{
	struct skb_shared_info *shinfo = NULL;
	struct tcphdr *th = NULL;
	struct iphdr *iph = NULL;
	int len; // tcp header length + data length
	int time_cost;
	int package_len; // tcp header length
	struct ipv6hdr *ip6h = NULL;

	if (!hw_pt_is_uplink_need_continue(skb, dev))
		return;

	shinfo = skb_shinfo(skb);
	if (!shinfo || (shinfo->proc_bitmask & PROC_BITMASK_RNIC_UL_OUT))
		return;

	time_cost = jiffies - shinfo->born_stamp;
	if (time_cost < 0)
		return;

	switch (get_layer4_protocl(skb)) {
	case IPPROTO_TCP:
		th = tcp_hdr(skb);
		if (th != NULL) {
			package_len = th->doff * TCP_HDR_DOFF_QUAD;
			if (skb->protocol == ntohs(ETH_P_IPV6)) {
				ip6h = (struct ipv6hdr *)skb_network_header(skb);
				len = ntohs(ip6h->payload_len);
			} else if (skb->protocol == ntohs(ETH_P_IP)) {
				iph = (struct iphdr *)(skb_network_header(skb));
				len = ntohs(iph->tot_len) - iph->ihl * TCP_HDR_IHL_QUAD;
			} else {
				return;
			}

			if (len > package_len) {
				shinfo->proc_bitmask |= PROC_BITMASK_RNIC_UL_OUT;
				return;
			}
		}
		hw_pt_log_exception(NULL, skb, IPPROTO_TCP, RNIC_UL_DELAY_LIMIT, __func__, UPLINK);
		break;
	case IPPROTO_UDP:
		hw_pt_log_exception(NULL, skb, IPPROTO_UDP, RNIC_UL_DELAY_LIMIT, __func__, UPLINK);
		break;
	default:
		return;
	}

	shinfo->proc_bitmask |= PROC_BITMASK_RNIC_UL_OUT; // record this skb is sent
}
EXPORT_SYMBOL(hw_pt_dev_uplink);

void hw_pt_l4_downlink_in(struct sock *skt, struct sk_buff *skb, int eaten)
{
	struct skb_shared_info *shinfo = NULL;
	struct sock *sk = NULL;

	if (!skb)
		return;
	if (skt)
		sk = sk_to_full_sk((struct sock *)skt);
	else
		sk = sk_to_full_sk(skb->sk);

	if (!is_socket_belong_fg_app(sk))
		return;

	switch (sk->sk_protocol) {
	case IPPROTO_TCP:
		hw_pt_log_exception(sk, skb, IPPROTO_TCP, RNIC_DL_DELAY_LIMIT, __func__, DOWNLINK_IN);
		if (!eaten) {
			shinfo = skb_shinfo(skb);
			if (shinfo == NULL)
				break;
			shinfo->born_stamp = jiffies;
		}
		break;
	case IPPROTO_UDP:
		hw_pt_log_exception(sk, skb, IPPROTO_UDP, RNIC_DL_DELAY_LIMIT, __func__, DOWNLINK_IN);
		shinfo = skb_shinfo(skb);
		if (shinfo == NULL)
			break;
		shinfo->born_stamp = jiffies;
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(hw_pt_l4_downlink_in);

void hw_pt_queue_recv(struct sock *skt, struct sk_buff *skb, int eaten)
{
	if (eaten) {
		return;
	}
	struct skb_shared_info *shinfo = NULL;
	struct sock *sk = NULL;
	if (!skb)
		return;
	if (skt)
		sk = sk_to_full_sk((struct sock *)skt);
	else
		sk = sk_to_full_sk(skb->sk);
	if (!is_fore_app_socket(sk)) {
        return;
    }
	if (!hw_pt_log_check_recv_skb(skb)) {
		return;
	}
	int app_index = get_top_app_index(get_uid_from_sock(sk));
	if (app_index == APP_UID_INVALID) {
		return;
	}
	int cur_interface = get_skb_interface(sk, skb);
    if (cur_interface == INVALID_INTERFACE) {
		return;
    }
	shinfo = skb_shinfo(skb);
	int time_cost = 0;
	if (shinfo != NULL) {
		spin_lock_bh(&g_queue_in_lock);
		time_cost = jiffies_to_msecs(jiffies - shinfo->born_stamp);
		set_skb_shinfo(skb, PROC_BITMASK_QUEUE_RECV);
		spin_unlock_bh(&g_queue_in_lock);
	}
	if (sk->sk_protocol == IPPROTO_TCP || sk->sk_protocol == IPPROTO_UDP) {
		hw_pt_queue_recv_calc(app_index, cur_interface, time_cost);
	}
}
EXPORT_SYMBOL(hw_pt_queue_recv);

void hw_pt_app_recv(struct sock *skt, struct sk_buff *skb)
{
	struct sock *sk = NULL;
	struct skb_shared_info *shinfo = NULL;
	if (!skb)
		return;
	if (skt)
		sk = sk_to_full_sk((struct sock *)skt);
	else
		sk = sk_to_full_sk(skb->sk);
	if (!is_fore_app_socket(sk)) {
        return;
    }
    int cur_interface = get_skb_interface(sk, skb);
    if (cur_interface == INVALID_INTERFACE) {
		return;
    }
	spin_lock_bh(&g_queue_out_lock);
	if (get_app_top_uid() != g_last_app_top_uid) {
		int old_uid = g_last_app_top_uid;
        g_last_app_top_uid = get_app_top_uid();
		spin_unlock_bh(&g_queue_out_lock);
        hw_pt_reset_app_continue_info(old_uid, cur_interface);
    } else {
		spin_unlock_bh(&g_queue_out_lock);
	}
	if (!hw_pt_log_check_recv_skb(skb)) {
		return;
	}
	int app_index = get_top_app_index(get_uid_from_sock(sk));
	if (app_index == APP_UID_INVALID) {
		return;
	}
	shinfo = skb_shinfo(skb);
	if (shinfo == NULL || shinfo->proc_bitmask != PROC_BITMASK_QUEUE_RECV) {
		return;
	}
	if (sk->sk_protocol == IPPROTO_TCP || sk->sk_protocol == IPPROTO_UDP) {
		hw_pt_app_recv_calc(sk, skb, app_index, cur_interface, shinfo);
	}
}
EXPORT_SYMBOL(hw_pt_app_recv);

void hw_pt_l4_downlink_out(struct sock *skt, struct sk_buff *skb)
{
	struct sock *sk = NULL;

	if (!skb)
		return;

	if (skt)
		sk = sk_to_full_sk((struct sock *)skt);
	else
		sk = sk_to_full_sk(skb->sk);

	if (!is_socket_belong_fg_app(sk))
		return;

	switch (sk->sk_protocol) {
	case IPPROTO_TCP:
		hw_pt_log_exception(sk, skb, IPPROTO_TCP, L4_DL_DELAY_LIMIT, __func__, DOWNLINK_OUT);
		break;
	case IPPROTO_UDP:
		hw_pt_log_exception(sk, skb, IPPROTO_UDP, L4_DL_DELAY_LIMIT, __func__, DOWNLINK_OUT);
		break;
	default:
		break;
	}
}
EXPORT_SYMBOL(hw_pt_l4_downlink_out);

static void variable_initialization(void)
{
	pr_info("%s, init", __func__);
	g_app_queue_stats_rpt_time = jiffies;
    memset_s(&g_buffer_queue_stat, sizeof(g_buffer_queue_stat), 0, sizeof(g_buffer_queue_stat));
	create_flow_list();
}
 
static void spinlock_initialization(void)
{
    spin_lock_init(&g_queue_out_lock);
    spin_lock_init(&g_queue_in_lock);
	spin_lock_init(&g_tcp_zero_window_lock);
}

static bool is_report_zero_window_event(struct sock *sk)
{
	if (sk == NULL || !is_fore_app_socket(sk)) {
		pr_debug("%s: tcp zero window report is not fore app", __func__);
		return false;
	}
	spin_lock_bh(&g_tcp_zero_window_lock);
	if (g_tcp_zero_window_rpt_time > 0 && get_time_cost(g_tcp_zero_window_rpt_time) < g_tcp_zero_window_interval) {
		pr_debug("%s: tcp zero window report interval is not enough", __func__);
		spin_unlock_bh(&g_tcp_zero_window_lock);
		return false;
	}
	g_tcp_zero_window_rpt_time = jiffies;
	spin_unlock_bh(&g_tcp_zero_window_lock);
	return true;
}

static void report_zero_window_info(struct sk_buff *skb)
{
	if (notifier == NULL || skb == NULL || skb->sk == NULL) {
		pr_err("%s, notifier or sock is NULL", __func__);
		return;
	}
	struct sock *sk = sk_to_full_sk(skb->sk);
	if (!is_report_zero_window_event(sk)) {
		return;
	}
	u16 data_len = sizeof(struct tcp_zero_window_info);
	struct tcp_zero_window_info info;
	memset_s(&info, data_len, 0, data_len);
	info.msg_head.type = (u16)TCP_ZERO_WINDOW_RPT;
	info.msg_head.len = data_len;
	struct inet_sock *inet = inet_sk(sk);
	if (sk->sk_family == AF_INET6) {
		info.ip_type = 1;
		if(sprintf_s(info.src_ip, IP_ADDR_STR_MAX_LEN, "%pI6", &(sk->sk_v6_rcv_saddr)) == -1) {
			pr_err("%s, source IPv6 addr sprintf_s error", __func__);
		}
		if (sprintf_s(info.dest_ip, IP_ADDR_STR_MAX_LEN, "%pI6", &(sk->sk_v6_daddr)) == -1) {
			pr_err("%s, dest IPv6 addr sprintf_s error", __func__);
		}
	} else {
		info.ip_type = 0;
		if (sprintf_s(info.src_ip, IP_ADDR_STR_MAX_LEN, "%pI4", &(inet->inet_saddr)) == -1) {
			pr_err("%s, source IPv4 addr sprintf_s error", __func__);
		}
		if (sprintf_s(info.dest_ip, IP_ADDR_STR_MAX_LEN, "%pI4", &(inet->inet_daddr)) == -1) {
			pr_err("%s, dest IPv4 addr sprintf_s error", __func__);
		}
	}
	info.src_port = ntohs(inet->inet_sport);
	info.dest_port = ntohs(inet->inet_dport);
	info.uid = get_uid_from_sock(sk);
	info.interface = get_skb_interface(sk, skb);
	notifier((struct res_msg_head *)(&info));
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
static void chr_kernel_data_process_timer(unsigned long data)
#else
static void chr_kernel_data_process_timer(struct timer_list* data)
#endif
{
	struct hw_kernel_delay_info info_rpt = {0};

	g_chr_data_process_timer_state = CHR_TIMER_NOT_START;

	if (is_chr_report_needed()) {
		prepare_report_data(&info_rpt);
		notify_packet_delay_event(g_last_app_top_uid, &info_rpt);
		set_chr_report_timer(CHR_TIMER_REPORT_WAIT, TIMER_REPORT_BACKOFF_PEROID);
		chr_kernel_clear_data_record();
	}
}

msg_process* __init hw_pt_init(notify_event *notify)
{
	pr_debug("%s: ENTER \n", __func__);
    variable_initialization();
    spinlock_initialization();
    set_tcp_zero_window_handler(report_zero_window_info);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,0)
	init_timer(&g_kernel_data_process_timer);
	g_kernel_data_process_timer.data = 0;
	g_kernel_data_process_timer.function = chr_kernel_data_process_timer;
#else
	timer_setup(&g_kernel_data_process_timer, chr_kernel_data_process_timer, 0);
#endif

	if (notify == NULL) {
		pr_err("%s: notify parameter is null\n", __func__);
		return NULL;
	}
	notifier = notify;
	return do_commands;
}

MODULE_LICENSE("GPL");
