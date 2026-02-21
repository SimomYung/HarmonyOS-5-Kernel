/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include "fi.h"
#include <linux/highmem.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/netdevice.h>
#include <linux/tcp.h>
#include <linux/timer.h>
#include <linux/udp.h>
#include <linux/version.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/udp.h>
#include "fi_utils.h"

struct fi_ctx g_fi_context;
static struct fi_app_info_table g_fi_app_info;
static uint64_t g_fi_iface_bytes[FI_DEV_NUM];

void fi_qos_pre_update(const struct sk_buff *skb, struct fi_flow_node *flow);
void fi_filesize_pre_update(const struct sk_buff *skb, struct fi_flow_node *node);
void fi_iface_qos_update_report(void);
static int fi_get_http_rsp_code(const char *buffer, unsigned int buf_len,
	unsigned char *out_buf, unsigned int out_len);

static unsigned int fi_ipv4_nf_hook_out(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state);

static unsigned int fi_ipv4_nf_hook_in(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state);

#if IS_ENABLED(CONFIG_IPV6)
static unsigned int fi_ipv6_nf_hook_out(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state);

static unsigned int fi_ipv6_nf_hook_in(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
	static const struct nf_hook_ops fi_nf_hooks[] = {
#else
	static struct nf_hook_ops fi_nf_hooks[] = {
#endif
	{
		.hook = fi_ipv4_nf_hook_out,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_OUT,
		.priority = NF_IP_PRI_FILTER + 1,
	},
	{
		.hook = fi_ipv4_nf_hook_in,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_IN,
		.priority = NF_IP_PRI_FILTER + 1,
	},
#if IS_ENABLED(CONFIG_IPV6)
	{
		.hook = fi_ipv6_nf_hook_out,
		.pf = PF_INET6,
		.hooknum = NF_INET_LOCAL_OUT,
		.priority = NF_IP_PRI_FILTER + 1,
	},
	{
		.hook = fi_ipv6_nf_hook_in,
		.pf = PF_INET6,
		.hooknum = NF_INET_LOCAL_IN,
		.priority = NF_IP_PRI_FILTER + 1,
	},
#endif
};

void fi_start_timer(void)
{
	if (!g_fi_context.is_tm_start && !timer_pending(&(g_fi_context.tm))) {
		g_fi_context.tm.expires = jiffies + HZ / FI_TIMER_INTERVAL;
		add_timer(&(g_fi_context.tm));
		g_fi_context.is_tm_start = true;
	} else {
		fi_loge("timer_pending, not need to add again");
	}
}

bool fi_check_periodic_rpt_enable(void)
{
	struct fi_app_info_node *app_info = NULL;
	unsigned int i, j;
	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		app_info = &(g_fi_app_info.app[i]);
		if (app_info->msg.status == FI_INVALID_APP_STATUS) {
			spin_unlock_bh(&(g_fi_app_info.app[i].lock));
			continue;
		}

		for (j = 0; j < FI_FLOW_IDENTIFY_MAX; j++) {
			if (app_info->msg.flow_identify_cfg[j].periodic[FI_DIR_UP].report_type != FI_RPT_OFF ||
				app_info->msg.flow_identify_cfg[j].periodic[FI_DIR_DOWN].report_type != FI_RPT_OFF) {
				spin_unlock_bh(&(g_fi_app_info.app[i].lock));
				return true;
			}
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
	return false;
}

void fi_del_timer_sync_if_no_periodic_rpt(bool is_periodic_rpt_enabled)
{
	if (g_fi_context.is_tm_start && !is_periodic_rpt_enabled) {
		del_timer_sync(&(g_fi_context.tm));
		g_fi_context.is_tm_start = false;
	}
}

void fi_register_nfhook(bool periodic_rpt)
{
	int ret;
	int i;
	struct net_device *dev = NULL;
	char iface[IFNAMSIZ] = {0};
	struct rtnl_link_stats64 stats;

	mutex_lock(&(g_fi_context.nf_mutex));
	if (g_fi_context.nf_exist) {
		if (periodic_rpt)
			fi_start_timer();
		else
			fi_del_timer_sync_if_no_periodic_rpt(false);
		mutex_unlock(&(g_fi_context.nf_mutex));
		return;
	}
	local_irq_disable();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
	ret = nf_register_net_hooks(&init_net, fi_nf_hooks, ARRAY_SIZE(fi_nf_hooks));
#else
	ret = nf_register_hooks(fi_nf_hooks, ARRAY_SIZE(fi_nf_hooks));
#endif
	local_irq_enable();
	if (ret == 0) {
		g_fi_context.nf_exist = true;
		if (periodic_rpt)
			fi_start_timer();
	}
	mutex_unlock(&(g_fi_context.nf_mutex));
	for (i = 0; i < FI_DEV_NUM; i++) {
		(void)memset_s(iface, sizeof(iface), 0, IFNAMSIZ);
		smartcom_get_network_iface_name(i, iface, IFNAMSIZ - 1);
		dev = dev_get_by_name(&init_net, iface);
		if (dev == NULL) {
			/* clear g_fi_iface_bytes if dev is off */
			g_fi_iface_bytes[i] = 0;
			fi_logi("can't find dev for net_type:%d ifname:%s", i, iface);
			continue;
		}
		read_lock(&dev_base_lock);
		dev_get_stats(dev, &stats);
		read_unlock(&dev_base_lock);
		dev_put(dev);
		g_fi_iface_bytes[i] = stats.rx_bytes;
		fi_logi("init ifname: %s, total_rx_byes: %lu", iface, stats.rx_bytes);
	}
	if (ret == 0)
		fi_logi("register nf hooks successfully");
	else
		fi_loge("register nf hooks failed, ret:%d", ret);
}

void fi_unregister_nfhook(void)
{
	mutex_lock(&(g_fi_context.nf_mutex));
	if (!g_fi_context.nf_exist) {
		mutex_unlock(&(g_fi_context.nf_mutex));
		return;
	}

	local_irq_disable();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
	nf_unregister_net_hooks(&init_net, fi_nf_hooks, ARRAY_SIZE(fi_nf_hooks));
#else
	nf_unregister_hooks(fi_nfhooks, ARRAY_SIZE(fi_nf_hooks));
#endif
	local_irq_enable();

	g_fi_context.nf_exist = false;
	fi_ipv4_flow_clear();
#if IS_ENABLED(CONFIG_IPV6)
	fi_ipv6_flow_clear();
#endif
	mutex_unlock(&(g_fi_context.nf_mutex));
	fi_logi("FI unregister nf hooks successfully");
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)
void fi_mod_timer(unsigned long arg)
#else
void fi_mod_timer(struct timer_list *t)
#endif
{
	fi_flow_stat_update_report();
	fi_flow_qos_update_report();
	fi_iface_qos_update_report();
	fi_ipv4_flow_age();
#if IS_ENABLED(CONFIG_IPV6)
	fi_ipv6_flow_age();
#endif
	mod_timer(&(g_fi_context.tm), jiffies + HZ / FI_TIMER_INTERVAL);
}

static inline bool fi_payload_len_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_tcp_seq_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_tcp_ack_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_tcp_flags_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_tcp_window_size_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_tcp_options_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);
static inline bool fi_payload_segment_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info);

static struct fi_pkt_parse_f g_fi_pkt_parse_f[] = {
	{FI_PAYLOAD_LEN_PARSE,		fi_payload_len_parse},
	{FI_PAYLOAD_SEGMENT_PARSE,	fi_payload_segment_parse},
	{FI_ECHO_DELAY,				NULL},
	{FI_TCP_SEQ_PARSE,			fi_tcp_seq_parse},
	{FI_TCP_ACK_PARSE,			fi_tcp_ack_parse},
	{FI_TCP_FLAGS_PARSE,			fi_tcp_flags_parse},
	{FI_TCP_WINDOW_SIZE_PARSE,	fi_tcp_window_size_parse},
	{FI_TCP_OPTIONS_PARSE,		fi_tcp_options_parse},
	{FI_END_PARSE,				NULL},
};

static uint fi_get_hdr_len(const struct sk_buff *skb, uint8_t protocol)
{
	if (protocol == IPPROTO_UDP)
		return sizeof(struct udphdr);

	return tcp_hdrlen(skb);
}

static uint32_t fi_pre_get_payload_parse_len(fi_flow_identify_cfg *flow_cfg,
	uint32_t stat_stop, enum fi_dir dir)
{
	uint32_t pkt_report_len = 0;
	uint32_t period_report_len = 0;

	if (!(stat_stop & (dir == FI_DIR_UP ? FI_FLOW_PKT_REPORT_STOP_UP : FI_FLOW_PKT_REPORT_STOP_DOWN)))
		pkt_report_len = flow_cfg->pkt[dir].seg_end + 1;

	if (!(stat_stop & (dir == FI_DIR_UP ? FI_FLOW_PERIODIC_REPORT_STOP_UP : FI_FLOW_PERIODIC_REPORT_STOP_DOWN)))
		period_report_len = flow_cfg->periodic[dir].seg_end + 1;

	return max(pkt_report_len, period_report_len);
}

static uint32_t fi_copy_payload_by_offset(char *dst, uint32_t dst_len, char *src, uint32_t copy_len)
{
	if (unlikely(src == NULL)) {
		fi_loge("source address is null");
		return 0;
	}

	if (dst_len < copy_len)
		copy_len = dst_len;

	if (memcpy_s(dst, dst_len, src, copy_len) != EOK) {
		fi_loge("copy payload failed");
		return 0;
	}
	return copy_len;
}

static uint16_t fi_get_pkt_report_len(fi_flow_identify_cfg *fi_cfg, uint32_t parse_len, enum fi_dir dir)
{
	if (parse_len == 0 ||
		fi_cfg->pkt[dir].seg_end <= fi_cfg->pkt[dir].seg_begin ||
		fi_cfg->pkt[dir].seg_begin >= parse_len)
		return 0;

	return min(fi_cfg->pkt[dir].seg_end - fi_cfg->pkt[dir].seg_begin + 1, parse_len);
}

static bool fi_payload_len_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP) {
		if (skb->len > skb_transport_offset(skb) + sizeof(struct udphdr))
			pkt_info->msg.payload_len = skb->len -
				skb_transport_offset(skb) - sizeof(struct udphdr);
		else
			pkt_info->msg.payload_len = 0;
	} else {
		if (skb->len > skb_transport_offset(skb) + tcp_hdrlen(skb))
			pkt_info->msg.payload_len = skb->len - skb_transport_offset(skb) - tcp_hdrlen(skb);
		else
			pkt_info->msg.payload_len = 0;
	}

	pkt_info->msg.total_len = skb->len;
	return true;
}

static bool fi_tcp_seq_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	struct tcphdr *tcph = NULL;

	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		pkt_info->msg.tcp_seq = ntohl(tcph->seq);
		return true;
	}
	return true;
}

static bool fi_tcp_ack_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	struct tcphdr *tcph = NULL;

	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		pkt_info->msg.tcp_ack = ntohl(tcph->ack);
		return true;
	}
	return true;
}

static bool fi_tcp_flags_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	struct tcphdr *tcph = NULL;

	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		pkt_info->msg.tcp_flags |= tcph->fin;
		pkt_info->msg.tcp_flags |= tcph->syn << TCPHDR_OFFSET_SYN;
		pkt_info->msg.tcp_flags |= tcph->rst << TCPHDR_OFFSET_RST;
		pkt_info->msg.tcp_flags |= tcph->psh << TCPHDR_OFFSET_PSH;
		pkt_info->msg.tcp_flags |= tcph->ack << TCPHDR_OFFSET_ACK;
		pkt_info->msg.tcp_flags |= tcph->urg << TCPHDR_OFFSET_URG;
		pkt_info->msg.tcp_flags |= tcph->ece << TCPHDR_OFFSET_ECE;
		pkt_info->msg.tcp_flags |= tcph->cwr << TCPHDR_OFFSET_CWR;
		return true;
	}
	return true;
}

static bool fi_tcp_window_size_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	struct tcphdr *tcph = NULL;

	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		pkt_info->msg.tcp_window = ntohs(tcph->window);
		return true;
	}
	return true;
}

static bool fi_tcp_options_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	unsigned int optionslen;
	int ret;

	if (pkt_info->msg.flow_msg.l4proto != IPPROTO_TCP)
		return true;
	if (tcp_hdrlen(skb) <= sizeof(struct tcphdr))
		return true;
	optionslen = tcp_hdrlen(skb) - sizeof(struct tcphdr);
	if (optionslen > FI_TCP_OPTIONS_LEN)
		return false;
	ret = memcpy_s(pkt_info->msg.tcp_options, FI_TCP_OPTIONS_LEN,
		(char *)(skb_transport_header(skb) + sizeof(struct tcphdr)), optionslen);
	if (ret) {
		fi_loge("memcpy_s failed ret=%d", ret);
		return false;
	}

	return true;
}

static bool fi_payload_segment_parse_nonlinear(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	const skb_frag_t *frag = NULL;
	char *payload = NULL;
	uint transport_len = 0;
	uint payload_len = 0;
	uint i = 0;
	uint32_t offset = 0;

	transport_len = fi_get_hdr_len(skb, pkt_info->msg.flow_msg.l4proto);
	/* copy from the linear region */
	if (skb_headlen(skb) > skb_transport_offset(skb) + transport_len) {
		payload_len = skb_headlen(skb) - skb_transport_offset(skb) - transport_len;
		payload = skb_transport_header(skb) + transport_len;
		if (payload_len >= pkt_info->parse_len) {
			pkt_info->payload = payload;
			return true;
		}
		pkt_info->payload = kzalloc(pkt_info->parse_len, GFP_ATOMIC);
		if (pkt_info->payload == NULL) {
			fi_loge("kzalloc failed");
			return false;
		}
		pkt_info->is_malloc_buf = true;
		offset += fi_copy_payload_by_offset(pkt_info->payload + offset,
			pkt_info->parse_len - offset, payload, payload_len);
	}

	/* copy from the frags */
	for (i = 0; i < skb_shinfo(skb)->nr_frags && i < MAX_SKB_FRAGS; i++) {
		frag = (const skb_frag_t *)(&skb_shinfo(skb)->frags[i]);
		payload_len = skb_frag_size(frag);
		payload = skb_frag_address_safe(frag);
		if (i == 0 && !pkt_info->is_malloc_buf) {
			if (payload_len >= pkt_info->parse_len) {
				pkt_info->payload = payload;
				return true;
			}
			pkt_info->payload = kzalloc(pkt_info->parse_len, GFP_ATOMIC);
			if (pkt_info->payload == NULL) {
				fi_loge("kzalloc failed");
				return false;
			}
			pkt_info->is_malloc_buf = true;
		}
		offset += fi_copy_payload_by_offset(pkt_info->payload + offset,
			pkt_info->parse_len - offset, payload, payload_len);
		if (offset >= pkt_info->parse_len)
			return true;
	}

	pkt_info->parse_len = offset;
	/* now we do not copy from frag_list, because linear area and frags is enough */
	if (pkt_info->payload != NULL)
		return true;
	return false;
}

static bool fi_payload_segment_parse(struct sk_buff *skb, struct fi_pkt_parse *pkt_info)
{
	uint transport_len = 0;

	pkt_info->parse_len = min((uint32_t)pkt_info->msg.payload_len, pkt_info->parse_len);
	if (pkt_info->parse_len == 0)
		return false;

	transport_len = fi_get_hdr_len(skb, pkt_info->msg.flow_msg.l4proto);
	/* skb is linear */
	if (!skb_is_nonlinear(skb)) {
		pkt_info->payload = skb_transport_header(skb) + transport_len;
		return true;
	}

	/* skb is nonlinear */
	return fi_payload_segment_parse_nonlinear(skb, pkt_info);
}

static void fi_payload_update_frags(struct fi_pkt_parse *pkt_info)
{
	fi_rpt_cfg *periodic_cfg = NULL;

	pkt_info->raw_frag_num++;
	periodic_cfg = &pkt_info->flow_cb.flow_identify_cfg->periodic[pkt_info->dir]; // only use periodic_cfg!!
	if (fi_payload_len_filter_check(pkt_info->msg.payload_len,
			periodic_cfg->filter_pkt_size_start, periodic_cfg->filter_pkt_size_stop) &&
		fi_payload_str_filter_check(pkt_info, periodic_cfg)) {
		pkt_info->filter_frag_num++;
		pkt_info->filter_frag_payload_len = pkt_info->frag_payload_len;
		pkt_info->filter_frag_payload = pkt_info->frag_payload;
	}
}

static void fi_payload_frag_meta_frags_parse(struct fi_pkt_parse *pkt_info, const struct sk_buff *skb)
{
	int tmp_num = 1;
	int i = 0;
	int j = 0;
	unsigned short gso_size = 0;
	const skb_frag_t *skb_frag = NULL;
	char *frag_addr = NULL;
	if (skb_shinfo(skb)->nr_frags == 0)
		return;

	gso_size = skb_shinfo(skb)->gso_size;
	for (i = 0; i < skb_shinfo(skb)->nr_frags && i < MAX_SKB_FRAGS; i++) {
		skb_frag = (const skb_frag_t *)(&skb_shinfo(skb)->frags[i]);
		frag_addr = skb_frag_address_safe(skb_frag);
		if (!skb_is_gso(skb)) {
			pkt_info->is_multi_frag = true;
			pkt_info->frag_payload_len = skb_frag_size(skb_frag);
			pkt_info->frag_payload = frag_addr;
			fi_payload_update_frags(pkt_info);
		} else if (skb_frag_size(skb_frag) >= gso_size) {
			pkt_info->is_multi_frag = true;
			tmp_num = skb_frag_size(skb_frag) / gso_size;
			for (j = 0; j < tmp_num; j++) {
				pkt_info->frag_payload_len = gso_size;
				pkt_info->frag_payload = (frag_addr != NULL ? frag_addr + j * gso_size : NULL);
				fi_payload_update_frags(pkt_info);
			}
			if (skb_frag_size(skb_frag) % gso_size == 0)
				continue;
			pkt_info->frag_payload_len = skb_frag_size(skb_frag) % gso_size;
			pkt_info->frag_payload = (frag_addr != NULL ? frag_addr + tmp_num * gso_size : NULL);
			if (frag_addr != NULL && pkt_info->frag_payload - frag_addr + pkt_info->frag_payload_len >
				skb_frag_size(skb_frag)) {
				pkt_info->frag_payload_len = skb_frag_size(skb_frag);
				pkt_info->frag_payload = frag_addr;
			}
			fi_payload_update_frags(pkt_info);
		}
	}
}

static void fi_payload_frag_meta_parse(struct fi_pkt_parse *pkt_info, const struct sk_buff *skb)
{
	int transport_hdr_len = 0;
	uint16_t linear_payload_len = 0;

	/* step1: parse the linear area. linear area may have no payload or have only payload! */
	if (skb_transport_header_was_set(skb)) {
		transport_hdr_len = fi_get_hdr_len(skb, pkt_info->msg.flow_msg.l4proto);
		if (skb_headlen(skb) > skb_transport_offset(skb) + transport_hdr_len)
			linear_payload_len = skb_headlen(skb) - skb_transport_offset(skb) - transport_hdr_len;
	} else {
		linear_payload_len = skb_headlen(skb);
	}
	if (linear_payload_len > 0) { // linear area has payload
		pkt_info->frag_payload_len = linear_payload_len;
		pkt_info->frag_payload = skb_transport_header_was_set(skb) ?
			skb_transport_header(skb) + transport_hdr_len : skb->data;
		fi_payload_update_frags(pkt_info);
	}

	/* step2: parse the frags area */
	fi_payload_frag_meta_frags_parse(pkt_info, skb);
}

static void fi_payload_frag_parse(struct fi_pkt_parse *pkt_info, const struct sk_buff *skb)
{
	struct sk_buff *frag_list = NULL;
	pkt_info->is_multi_frag = false;
	if (skb_is_gso(skb))
		pkt_info->is_multi_frag = true;

	/* step1: parse the linear and frags area */
	fi_payload_frag_meta_parse(pkt_info, skb);

	/* step2: parse the frag_list area, no recursive */
	if (skb_shinfo(skb)->frag_list != NULL) {
		frag_list = skb_shinfo(skb)->frag_list;
		pkt_info->is_multi_frag = true;
		while (frag_list && pkt_info->raw_frag_num < FI_SKB_MAX_FRAG_LIST_NUM) {
			fi_payload_frag_meta_parse(pkt_info, frag_list);
			frag_list = frag_list->next;
		}
	}

	if (pkt_info->is_multi_frag &&
		pkt_info->filter_frag_payload != NULL && pkt_info->filter_frag_payload_len != 0) {
		pkt_info->frag_payload = pkt_info->filter_frag_payload;
		pkt_info->frag_payload_len = pkt_info->filter_frag_payload_len;
	}
}

static bool is_tcp_protocol(const struct sk_buff *skb)
{
	struct iphdr *iphdr_ptr = NULL;
	struct ipv6hdr *ipv6hdr_ptr = NULL;
	int thoff = 0;

	if (ntohs(skb->protocol) == ETH_P_IP) {
		iphdr_ptr = ip_hdr(skb);
		return iphdr_ptr != NULL && iphdr_ptr->protocol == IPPROTO_TCP;
	} else {
		ipv6hdr_ptr = ipv6_hdr(skb);
		return ipv6hdr_ptr != NULL && ipv6_find_hdr(skb, &thoff, -1, NULL, NULL) == IPPROTO_TCP;
	}
}

/**
 * parse http dl packet and return the http response code
 * return 0: may try parse next time; -1: no need parse next time; other: the http real rsp code
 */
int fi_parse_http_rsp_code(const struct sk_buff *skb, char *payload, uint32_t parse_len)
{
	uint16_t port;
	unsigned char http_rsp_code[FI_HTTP_RSP_CODE_MAX_LEN + 1] = {0};
	int code = -1;
	struct tcphdr *tcphdr_ptr = NULL;

	if (skb == NULL)
		return 0;

	if (!is_tcp_protocol(skb))
		return -1;

	tcphdr_ptr = tcp_hdr(skb);
	port = ntohs(tcphdr_ptr->source);
	if (port != SMARTCOM_PORT_80 || tcphdr_ptr->rst || tcphdr_ptr->fin)
		return -1;

	if (tcphdr_ptr->syn)
		return 0; /* deal next time */

	if (parse_len == 0 || payload == NULL) /* may be an ack */
		return 0; // deal next time

	if (parse_len < FI_MIN_HTTP_LEN) /* first dl pkt lenth samll */
		return -1;

	if (fi_get_http_rsp_code(payload, FI_MIN_HTTP_LEN, http_rsp_code, FI_HTTP_RSP_CODE_MAX_LEN) != 0)
		return -1;

	if (kstrtoint(http_rsp_code, FI_DECIMAL_BASE, &code) != 0)
		return -1;
	fi_logd("src port:%u, dst port:%u, rsp_code:%d", port, ntohs(tcphdr_ptr->dest), code);
	return (code <= 0 ? -1 : code);
}

static int fi_get_http_rsp_code(const char *buffer, unsigned int buf_len,
	unsigned char *out_buf, unsigned int out_len)
{
	unsigned int i;
	unsigned int code_begin;
	unsigned int code_end;
	unsigned int code_len;
	bool is_end = false;
	const unsigned char *http_rsp_hdr = FI_HTTP_VERSION_STR;
	const unsigned int http_rsp_hdr_len = strlen(FI_HTTP_VERSION_STR);

	for (i = 0; i < http_rsp_hdr_len; i++) {
		if (buffer[i] != http_rsp_hdr[i])
			return -1;
	}
	code_begin = i;
	for (i = 0; i < FI_HTTP_RSP_CODE_MAX_LEN &&
		*(buffer + code_begin + i) != 0 && *(buffer + code_begin + i) != '\t' &&
		*(buffer + code_begin + i) != '\r' && *(buffer + code_begin + i) != '\n'; i++) {
		if (*(buffer + code_begin + i) == ' ') {
			code_end = code_begin + i - 1;
			is_end = true;
			break;
		}
	}
	if (!is_end)
		return -1;

	code_len = code_end - code_begin + 1;
	if (code_len == 0 || code_len > out_len)
		return -1;

	if (memcpy_s(out_buf, FI_HTTP_RSP_CODE_MAX_LEN, buffer + code_begin, code_len) != EOK)
		return -1;
	return 0;
}

static void fi_reset(void)
{
	int i;
	fi_logi("fi need reset when netlink pid change]free_cnt = %d", atomic_read(&(g_fi_app_info.free_cnt)));
	if (atomic_read(&(g_fi_app_info.free_cnt)) >= FI_LAUNCH_APP_MAX)
		return;
	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		if (g_fi_app_info.app[i].msg.status != FI_INVALID_APP_STATUS) {
			struct fi_app_info_node *app_node = &(g_fi_app_info.app[i]);
			app_node->rand_num = 0;
			app_node->msg.status = FI_INVALID_APP_STATUS;
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
	atomic_set(&(g_fi_app_info.free_cnt), FI_LAUNCH_APP_MAX);
	fi_unregister_nfhook();
	mutex_lock(&(g_fi_context.nf_mutex));
	fi_del_timer_sync_if_no_periodic_rpt(false);
	mutex_unlock(&(g_fi_context.nf_mutex));
}

static void fi_set_nl_pid(const struct fi_cfg_head *cfg_head)
{
	uint8_t service = FI_MPFLOW_SERVICE;
	if (cfg_head->len < sizeof(struct fi_cfg_head) + sizeof(uint8_t)+ sizeof(uint32_t)) {
		fi_loge(" received a message without enough FI app info, len %d, min_len %zu\n",
			cfg_head->len, sizeof(struct fi_cfg_head) + sizeof(uint32_t));
		return;
	}
	/* fi need reset when netlink pid change, in case of daemon process restart */
	fi_reset();

	service = *((char *)cfg_head + sizeof(struct fi_cfg_head));
	if (service == FI_MPROUTE_SERVICE)
		g_fi_context.mproute_pid = *(uint32_t *)((char *)cfg_head + sizeof(uint8_t) + sizeof(struct fi_cfg_head));
	else
		g_fi_context.nl_pid = *(uint32_t *)((char *)cfg_head + sizeof(uint8_t) + sizeof(struct fi_cfg_head));

	fi_logi("set netlink pid from daemon, service:%u, netlink pid:%u", service,
		(service == FI_MPROUTE_SERVICE) ? g_fi_context.mproute_pid : g_fi_context.nl_pid);
	return;
}

static void fi_iface_qos_rpt_update(void)
{
	int i;

	g_fi_context.iface_qos_rpt_enable = 0;
	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		if (g_fi_app_info.app[i].msg.uid != 0 &&
			g_fi_app_info.app[i].msg.status == FI_COLLECT_RUN &&
			g_fi_app_info.app[i].msg.qos_rpt_enable) {
			g_fi_context.iface_qos_rpt_enable = 1;
			spin_unlock_bh(&(g_fi_app_info.app[i].lock));
			break;
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
}

static bool fi_app_info_update_status_check_invalid(const enum fi_app_status app_status)
{
	if ((app_status <= FI_INVALID_APP_STATUS) || (app_status >= FI_MAX_APP_STATUS)) {
		fi_loge("received an app with illegal status %d", app_status);
		return true;
	} else {
		return false;
	}
}

static bool fi_app_info_update_same_app_check(struct fi_app_update_msg *update_msg,
	struct fi_app_info_node *app_node)
{
	if (app_node->msg.uid == update_msg->uid)
		return true;

	return false;
}

static int protocol_trans(int protocol)
{
	switch (protocol) {
	case FI_TCP:
		return IPPROTO_TCP;
	case FI_UDP:
		return IPPROTO_UDP;
	default:
		break;
	}
	return protocol;
}

static struct fi_app_info_node *fi_app_info_update_get_app(struct fi_app_update_msg *update_msg)
{
	struct fi_app_info_node *app_node = NULL;
	struct fi_app_info_node *new_app_node = NULL;
	int i, ret;

	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		if (fi_app_info_update_same_app_check(update_msg, &g_fi_app_info.app[i]) &&
			g_fi_app_info.app[i].msg.status != FI_INVALID_APP_STATUS) {
			app_node = &(g_fi_app_info.app[i]);
			break;
		}
		if (new_app_node == NULL && g_fi_app_info.app[i].msg.status ==
			FI_INVALID_APP_STATUS) {
			new_app_node = &(g_fi_app_info.app[i]);
			continue;
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
	if (app_node == NULL && new_app_node == NULL) {
		fi_loge("reach max app launch size %d", FI_LAUNCH_APP_MAX);
		return NULL;
	} else if (app_node == NULL) {
		app_node = new_app_node;
		atomic_dec(&(g_fi_app_info.free_cnt));
		if (update_msg->service == FI_MPFLOW_SERVICE)
			g_fi_context.mpflow_app_num++;
		else
			g_fi_context.mproute_app_num++;
	} else if (new_app_node != NULL) {
		spin_unlock_bh(&(new_app_node->lock));
	}

	ret = memcpy_s(&(app_node->msg), sizeof(struct fi_app_update_msg),
		update_msg, sizeof(struct fi_app_update_msg));
	if (ret) {
		fi_loge("memcpy_s failed ret=%d", ret);
		spin_unlock_bh(&(app_node->lock));
		return NULL;
	}

	for (i = 0; i < FI_FLOW_IDENTIFY_MAX; i++)
		app_node->msg.flow_identify_cfg[i].proto = protocol_trans(app_node->msg.flow_identify_cfg[i].proto);

	app_node->rand_num++;
	spin_unlock_bh(&(app_node->lock));

	return app_node;
}

static void fi_app_info_update_proc(struct fi_app_info_node *app_node)
{
	fi_rpt_cfg *periodic = NULL;
	fi_rpt_cfg *pkt = NULL;
	int i;

	for (i = 0; i < FI_DIR_BOTH; i++) {
		periodic = &(app_node->msg.flow_identify_cfg[0].periodic[i]);
		pkt = &(app_node->msg.flow_identify_cfg[0].pkt[i]);
		fi_logi("Config received.app %s[%d], service:%u, status=%d,proto=%d,dport=%d,"
			"periodic[dir=%d roiType=%d roiStart=%d roiStart=%d],"
			"immediate[dir=%d roiType=%d roiStart=%d roiStart=%d] version:%s",
			app_node->msg.app_name, app_node->msg.uid, app_node->msg.service, app_node->msg.status,
			app_node->msg.flow_identify_cfg[0].proto, app_node->msg.flow_identify_cfg[0].dport[0],
			i, periodic->report_type, periodic->roi_start, periodic->roi_stop,
			i, pkt->report_type, pkt->roi_start, pkt->roi_stop, FI_VER);
	}
	fi_register_nfhook(fi_check_periodic_rpt_enable());
	fi_iface_qos_rpt_update();
}

static void fi_app_info_update(const struct fi_cfg_head *cfg_head)
{
	struct fi_app_update_msg *update_msg = NULL;
	struct fi_app_info_node *app_node = NULL;

	if (cfg_head->len != sizeof(struct fi_cfg_head) + sizeof(struct fi_app_update_msg)) {
		fi_loge("received message without enough FI app info, len %d, min_len %zu",
			cfg_head->len, sizeof(struct fi_cfg_head) + sizeof(struct fi_app_update_msg));
		return;
	}
	update_msg = (struct fi_app_update_msg *)((char *)cfg_head + sizeof(struct fi_cfg_head));
	if (fi_app_info_update_status_check_invalid(update_msg->status) ||
		(update_msg->status == FI_COLLECT_STOP)) {
		fi_loge("received an app with illegal status %d",
			update_msg->status);
		return;
	}

	app_node = fi_app_info_update_get_app(update_msg);
	if (app_node == NULL)
		return;

	fi_app_info_update_proc(app_node);
	return;
}

static struct fi_app_info_node *fi_app_status_update_and_get_app(struct fi_app_update_msg *update_msg)
{
	struct fi_app_info_node *app_node = NULL;
	int i;

	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		if (g_fi_app_info.app[i].msg.uid == update_msg->uid &&
			g_fi_app_info.app[i].msg.status != FI_INVALID_APP_STATUS) {
			app_node = &(g_fi_app_info.app[i]);
			break;
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
	if (app_node == NULL) {
		fi_loge("app %s, uid %d not exist", update_msg->app_name, update_msg->uid);
		return NULL;
	}

	if (update_msg->status == FI_COLLECT_STOP) {
		app_node->rand_num = 0;
		app_node->msg.status = FI_INVALID_APP_STATUS;
		spin_unlock_bh(&(app_node->lock));
		fi_ipv4_flow_clear_by_uid(update_msg->uid);
#if IS_ENABLED(CONFIG_IPV6)
		fi_ipv6_flow_clear_by_uid(update_msg->uid);
#endif
		if (update_msg->service == FI_MPFLOW_SERVICE) {
			if (g_fi_context.mpflow_app_num > 0)
				g_fi_context.mpflow_app_num--;
		} else {
			if (g_fi_context.mproute_app_num > 0)
				g_fi_context.mproute_app_num--;
		}
		atomic_inc(&(g_fi_app_info.free_cnt));
	} else {
		app_node->msg.status = update_msg->status;
		app_node->rand_num++;
		spin_unlock_bh(&(app_node->lock));
	}

	return app_node;
}

static void fi_app_status_update(const struct fi_cfg_head *cfg_head)
{
	struct fi_app_update_msg *update_msg = NULL;
	struct fi_app_info_node *app_node = NULL;

	if (cfg_head->len != sizeof(struct fi_cfg_head) + sizeof(struct fi_app_update_msg)) {
		fi_loge("receive a message without enough FI app info, len %d, min_len %zu",
			cfg_head->len, sizeof(struct fi_cfg_head) + sizeof(struct fi_app_update_msg));
		return;
	}
	update_msg = (struct fi_app_update_msg *)((char *)cfg_head +
		sizeof(struct fi_cfg_head));
	if (fi_app_info_update_status_check_invalid(update_msg->status))
		return;

	app_node = fi_app_status_update_and_get_app(update_msg);
	if (app_node == NULL)
		return;
	if (atomic_read(&(g_fi_app_info.free_cnt)) >= FI_LAUNCH_APP_MAX) {
		fi_unregister_nfhook();
		mutex_lock(&(g_fi_context.nf_mutex));
		fi_del_timer_sync_if_no_periodic_rpt(false);
		mutex_unlock(&(g_fi_context.nf_mutex));
	} else {
		mutex_lock(&(g_fi_context.nf_mutex));
		fi_del_timer_sync_if_no_periodic_rpt(fi_check_periodic_rpt_enable());
		mutex_unlock(&(g_fi_context.nf_mutex));
	}

	fi_iface_qos_rpt_update();
	fi_logi("app %s status update, uid:%d, service:%d, status:%d, rand_num:%lu, free_cnt:%d",
		app_node->msg.app_name, app_node->msg.uid, app_node->msg.service, app_node->msg.status,
		app_node->rand_num, atomic_read(&(g_fi_app_info.free_cnt)));

	return;
}

static void fi_ipv4_stop_flow_pkt_report(const struct fi_flow_msg *flow_msg)
{
	struct fi_pkt_parse pkt_info;
	uint32_t hash;
	struct fi_flow_head *head = NULL;
	struct fi_flow_node *node = NULL;

	if (memset_s(&pkt_info, sizeof(pkt_info), 0, sizeof(struct fi_pkt_parse))) {
		fi_loge("memset_s failed");
		return;
	}

	pkt_info.msg.flow_msg.ipv4_sip = flow_msg->ipv4_sip;
	pkt_info.msg.flow_msg.ipv4_dip = flow_msg->ipv4_dip;
	pkt_info.msg.flow_msg.sport = flow_msg->sport;
	pkt_info.msg.flow_msg.dport = flow_msg->dport;

	hash = fi_ipv4_flow_hash(&pkt_info, FI_FLOW_LEN + 1, false);
	if (strcmp(pkt_info.msg.flow_msg.flow, flow_msg->flow) != 0) {
		fi_loge("flow:%s not equal input param:%s", pkt_info.msg.flow_msg.flow, flow_msg->flow);
		return;
	}

	head = fi_ipv4_flow_header(hash);
	if (head == NULL) {
		fi_loge("flow:%s not find, hash head is null", pkt_info.msg.flow_msg.flow);
		return;
	}

	fi_ipv4_flow_lock(hash);
	node = fi_ipv4_flow_find(head, flow_msg->ipv4_sip, flow_msg->sport, flow_msg->ipv4_dip, flow_msg->dport);
	if (node != NULL) {
		node->flow_ctx.stat_stop |= FI_FLOW_PKT_REPORT_STOP;
		fi_logi("flow:%s stop pkt report", pkt_info.msg.flow_msg.flow);
	} else {
		fi_loge("flow:%s not find", pkt_info.msg.flow_msg.flow);
	}
	fi_ipv4_flow_unlock(hash);

	return;
}

static void fi_ipv6_stop_flow_pkt_report(const struct fi_flow_msg *flow_msg);
static void fi_stop_flow_pkt_report(const struct fi_cfg_head *cfg_head)
{
	struct fi_flow_msg *flow_msg = NULL;
	if (cfg_head->len != sizeof(struct fi_cfg_head) + sizeof(struct fi_flow_msg)) {
		fi_loge("receive a message without enough flow keyinfo, len %d, min_len %zu",
			cfg_head->len, sizeof(struct fi_cfg_head) + sizeof(struct fi_flow_msg));
		return;
	}

	flow_msg = (struct fi_flow_msg *)((char *)cfg_head + sizeof(struct fi_cfg_head));
	if (flow_msg->l3proto == ETH_P_IP)
		fi_ipv4_stop_flow_pkt_report(flow_msg);
#if IS_ENABLED(CONFIG_IPV6)
	else
		fi_ipv6_stop_flow_pkt_report(flow_msg);
#endif
}

void fi_cfg_process(const struct fi_cfg_head *cfg_head)
{
	if (!g_fi_context.running) {
		fi_loge("FI not initialized yet, cfg type:%d", cfg_head->type);
		return;
	}
	if (cfg_head == NULL || cfg_head->len == 0)
		return;

	switch (cfg_head->type) {
	case FI_SET_NL_PID_CMD:
		fi_set_nl_pid(cfg_head);
		break;
	case FI_COLLECT_START_CMD:
	case FI_COLLECT_INFO_UPDATE_CMD:
		fi_app_info_update(cfg_head);
		break;
	case FI_COLLECT_STATUS_UPDATE_CMD:
		fi_app_status_update(cfg_head);
		break;
	case FI_STOP_FLOW_PKT_REPORT_CMD:
		fi_stop_flow_pkt_report(cfg_head);
		break;
	default:
		fi_loge("receive a not FI message, type:%d", cfg_head->type);
		break;
	}
	return;
}
EXPORT_SYMBOL(fi_cfg_process); /*lint !e580*/

static bool fi_copy_pkt_report_payload(const struct fi_pkt_parse *pkt_info,
	char *dst_buff, uint32_t dst_buff_len)
{
	int ret;
	bool is_udp_multi_frag = (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP && pkt_info->is_multi_frag);
	fi_flow_identify_cfg *fi_cfg = pkt_info->flow_cb.flow_identify_cfg;
	if (pkt_info->msg.report_len == 0)
		return true;

	if (pkt_info->payload != NULL) {
		ret = memcpy_s(dst_buff, dst_buff_len,
			pkt_info->payload + fi_cfg->pkt[pkt_info->dir].seg_begin, pkt_info->msg.report_len);
	} else {
		fi_loge("payload null");
		return false;
	}

	if (ret) {
		fi_loge("memcpy_s failed ret=%d, parse_len=%u, report_len:%u",
			ret, pkt_info->parse_len, pkt_info->msg.report_len);
		return false;
	}

	fi_logd("flow:%s udp_multi_frag:%d, get payload_seg payload_len:%u, parse_len:%u, report_len:%u. "
		"pkt cfg begin=%u end=%u", pkt_info->msg.flow_msg.flow, is_udp_multi_frag,
		pkt_info->msg.payload_len, pkt_info->parse_len, pkt_info->msg.report_len,
		fi_cfg->pkt[pkt_info->dir].seg_begin, fi_cfg->pkt[pkt_info->dir].seg_end);
	return true;
}

static void fi_pkt_msg_report(struct fi_pkt_parse *pkt_info, int type)
{
	uint32_t size;
	char *data = NULL;
	int ret;
	struct sk_buff *pskb = NULL;
	spin_lock_bh(&(pkt_info->flow_cb.app_info->lock));
	pkt_info->msg.report_len = fi_get_pkt_report_len(pkt_info->flow_cb.flow_identify_cfg,
			pkt_info->parse_len, pkt_info->dir);
	if (pkt_info->rand_num == pkt_info->flow_cb.app_info->rand_num) {
		spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
		size = sizeof(struct fi_pkt_msg) + pkt_info->msg.report_len;
		pskb = fi_get_netlink_skb(type, size, &data);
		if (pskb == NULL) {
			fi_loge("failed to malloc memory for data, size:%u, report_len:%u",
				size, pkt_info->parse_len);
			return;
		}

		ret = memcpy_s(data, sizeof(struct fi_pkt_msg), &(pkt_info->msg), sizeof(struct fi_pkt_msg));
		if (ret) {
			fi_loge("memcpy_s failed ret:%d", ret);
			kfree_skb(pskb);
			return;
		}

		if (!fi_copy_pkt_report_payload(pkt_info, data + offsetof(struct fi_pkt_msg, payload_seg),
			size - offsetof(struct fi_pkt_msg, payload_seg))) {
			kfree_skb(pskb);
			return;
		}

		fi_enqueue_netlink_skb(pskb);
		fi_logd("finish to send a message to native, uid:%d, flow:%s, parse_len:%u, report_len:%u",
			pkt_info->msg.flow_msg.uid, pkt_info->msg.flow_msg.flow, pkt_info->parse_len, pkt_info->msg.report_len);
	} else {
		spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
	}
	return;
}

static bool fi_check_ip_addrss(struct fi_pkt_parse *pkt_info, int32_t uid)
{
	struct sockaddr_in6 addr = {0};
	if (pkt_info->msg.flow_msg.l3proto == ETH_P_IP) {
		struct sockaddr_in *usin = (struct sockaddr_in *)&addr;
		usin->sin_family = AF_INET;
		usin->sin_addr.s_addr = pkt_info->msg.flow_msg.ipv4_dip;
	} else if (pkt_info->msg.flow_msg.l3proto == ETH_P_IPV6) {
		struct sockaddr_in6 *usin6 = (struct sockaddr_in6 *)&addr;
		usin6->sin6_family = AF_INET6;
		usin6->sin6_addr = pkt_info->msg.flow_msg.ipv6_dip;
	} else {
		return false;
	}
	if (!smartcom_check_ip_addrss((struct sockaddr *)&addr) ||
		smartcom_check_ip_is_private((struct sockaddr *)&addr)) {
		fi_logd("invalid addr. uid: %d", uid);
		return false;
	}
	return true;
}

static bool fi_app_info_get(struct fi_pkt_parse *pkt_info,
	int32_t uid, enum fi_app_status status)
{
	struct fi_app_info_node *app_node = NULL;
	int i, j;
	bool match = false;

	if (pkt_info == NULL || !fi_check_ip_addrss(pkt_info, uid))
		return false;

	for (i = 0; i < FI_LAUNCH_APP_MAX; i++) {
		spin_lock_bh(&(g_fi_app_info.app[i].lock));
		if (g_fi_app_info.app[i].msg.uid == uid &&
			g_fi_app_info.app[i].msg.status == status) {
			app_node = &(g_fi_app_info.app[i]);
			break;
		}
		spin_unlock_bh(&(g_fi_app_info.app[i].lock));
	}
	if (app_node == NULL)
		return false;
	pkt_info->flow_cb.app_info = app_node;
	pkt_info->rand_num = pkt_info->flow_cb.app_info->rand_num;
	pkt_info->msg.flow_msg.uid = uid;
	for (i = 0; i < FI_FLOW_IDENTIFY_MAX; i++) {
		int proto = app_node->msg.flow_identify_cfg[i].proto;
		if (proto == FI_PROTO_ANY || proto == pkt_info->msg.flow_msg.l4proto) {
			for (j = 0; j < FI_DPORT_MAX; j++) {
				if (app_node->msg.flow_identify_cfg[i].dport[j] == FI_PORT_ANY ||
					pkt_info->msg.flow_msg.dport == app_node->msg.flow_identify_cfg[i].dport[j]) {
					match = true;
					break;
				}
			}
		}
		if (match)
			break;
	}
	spin_unlock_bh(&(app_node->lock));
	if (!match)
		return false;
	pkt_info->flow_cb.flow_identify_cfg = &app_node->msg.flow_identify_cfg[i];
	return true;
}

static bool fi_ipv4_pkt_check(struct sk_buff *skb)
{
	struct iphdr *iphdr_ptr = ip_hdr(skb);
	if (iphdr_ptr == NULL)
		return false;
	if (ipv4_is_loopback(iphdr_ptr->saddr) || ipv4_is_loopback(iphdr_ptr->daddr))
		return false;
	if (iphdr_ptr->protocol == IPPROTO_UDP) {
		if (skb->len >= skb_transport_offset(skb) + sizeof(struct udphdr))
			return true;
	} else if (iphdr_ptr->protocol == IPPROTO_TCP) {
		if (skb->len >= skb_transport_offset(skb) + tcp_hdrlen(skb))
			return true;
	}
	return false;
}

static void fi_ip_save_5tuple_info(struct fi_pkt_parse *pkt_info, uint16_t protocol, const struct sk_buff *skb)
{
	struct udphdr *udph = NULL;
	struct tcphdr *tcph = NULL;
	if (protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		pkt_info->msg.flow_msg.l4proto = IPPROTO_UDP;
		pkt_info->msg.flow_msg.sport = ntohs(udph->source);
		pkt_info->msg.flow_msg.dport = ntohs(udph->dest);
	} else {
		tcph = tcp_hdr(skb);
		pkt_info->msg.flow_msg.l4proto = IPPROTO_TCP;
		pkt_info->msg.flow_msg.sport = ntohs(tcph->source);
		pkt_info->msg.flow_msg.dport = ntohs(tcph->dest);
	}
}

static void fi_ipv4_5tuple_parse(struct fi_pkt_parse *pkt_info, const struct sk_buff *skb,
				enum fi_dir dir)
{
	struct iphdr *iph = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
	struct timeval ts;
	do_gettimeofday(&ts);
	pkt_info->msg.tstamp.tv_sec = ts.tv_sec;
	pkt_info->msg.tstamp.tv_usec = ts.tv_usec;
#else
	struct timespec64 ts;
	ktime_get_real_ts64(&ts);
	pkt_info->msg.tstamp.tv_sec = ts.tv_sec;
	pkt_info->msg.tstamp.tv_usec = ts.tv_nsec / 1000;
#endif
	pkt_info->msg.flow_msg.l3proto = ntohs(skb->protocol);
	pkt_info->msg.dir = dir;
	iph = ip_hdr(skb);
	pkt_info->msg.flow_msg.ipv4_sip = iph->saddr;
	pkt_info->msg.flow_msg.ipv4_dip = iph->daddr;
	fi_ip_save_5tuple_info(pkt_info, iph->protocol, skb);
	return;
}

static void fi_ipv4_pkt_parse(struct fi_pkt_parse *pkt_info, struct sk_buff *skb)
{
	int i;

	spin_lock_bh(&(pkt_info->flow_cb.app_info->lock));
	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP)
		fi_payload_frag_parse(pkt_info, skb);

	for (i = 0; i < FI_END_PARSE; i++) {
		if ((pkt_info->flow_cb.flow_identify_cfg->pkt[pkt_info->dir].opt_enable[i] ||
			pkt_info->flow_cb.flow_identify_cfg->periodic[pkt_info->dir].opt_enable[i]) &&
			(g_fi_pkt_parse_f[i].opt_val_parse != NULL)) {
			fi_logd("parse target:%d function", i);
			(void)(g_fi_pkt_parse_f[i].opt_val_parse(skb, pkt_info));
		}
	}
	spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
	return;
}

static void fi_flow_get_dev(struct fi_flow_msg *flow_msg, const struct net_device *dev)
{
	int result = memcpy_s(flow_msg->dev, IFNAMSIZ, dev->name, strlen(dev->name) + 1);
	if (result)
		fi_loge("memcpy_s failed ret=%d, dev %s", result, dev->name);
	return;
}

bool fi_is_blocked_rsp_code(int http_rsp_code)
{
	if (http_rsp_code == FI_HTTP_RESPONSE_MOVED || http_rsp_code == FI_HTTP_RESPONSE_REDIRECT ||
		http_rsp_code == FI_HTTP_RESPONSE_OTHER_LOCATION || http_rsp_code == FI_HTTP_RESPONSE_USE_GATEWAY ||
		http_rsp_code == FI_HTTP_RESPONSE_BAD_REQUEST || http_rsp_code == FI_HTTP_RESPONSE_SERVER_INTERNAL_ERROR ||
		http_rsp_code == F_HTTP_RESPONSE_GATEWAY_ERROR)
		return true;
	return false;
}

static struct sock *fi_get_sock_by_dir(struct sk_buff *skb, enum fi_dir direction)
{
	if (direction == FI_DIR_UP)
		return skb->sk;
	return NULL;
}

static bool fi_up_pkt_check(struct sk_buff *skb, struct fi_pkt_parse *pkt_info,
	kuid_t *kuid, enum fi_dir direction)
{
	struct sock *sk = NULL;

	if (direction != FI_DIR_UP)
		return true;

	sk = skb->sk;
	if (sk == NULL)
		return false;
	/* If sk state is SYN_RECV, sk is actually a request_sock, can not call sock_i_uid */
	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP &&
		(sk->sk_state == TCP_NEW_SYN_RECV || sk->sk_state == TCP_SYN_RECV)) {
		fi_logd("Ignore SYN_RECV sk, sk_state=%d l4proto=%d port[%hu--%hu]",
			sk->sk_state, pkt_info->msg.flow_msg.l4proto, pkt_info->msg.flow_msg.sport, pkt_info->msg.flow_msg.dport);
		return false;
	}
	*kuid = sock_i_uid(sk);
	if (!fi_app_info_get(pkt_info, kuid->val, FI_COLLECT_RUN))
		return false;
	pkt_info->msg.flow_msg.dev_if = sk->sk_bound_dev_if;
	return true;
}

static void fi_flow_get_mark(struct sock *sk, struct fi_flow_msg *flow_msg)
{
	if (flow_msg && sk)
		flow_msg->sk_mark = sk->sk_mark & 0xFFFF;
}

static void fi_update_fault_type(enum fi_dir direction, int last_tcp_state, struct fi_pkt_parse *pkt_info,
	const kuid_t *kuid)
{
	if (direction == FI_DIR_DOWN && pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP && last_tcp_state < TCP_FIN_WAIT1 &&
		(pkt_info->msg.tcp_flags & (TCPHDR_FIN | TCPHDR_RST))) {
		fi_logi("rcv active tcp fin/rst for sk port:%u, uid:%u", pkt_info->msg.flow_msg.dport, kuid->val);
		pkt_info->msg.flow_msg.flow_fault_type = FI_FLOW_FAULT_CLOSE_BY_NW;
	}
}

static void fi_update_last_tcp_state(int *last_tcp_state, struct fi_flow_node *node)
{
	*last_tcp_state = node->flow_ctx.tcp_state;
	if (node->sk)
		node->flow_ctx.tcp_state = node->sk->sk_state;
}

static bool fi_is_flow_already_closed(const struct fi_pkt_parse *pkt_info, const struct fi_flow_node *node,
	int last_tcp_state)
{
	return pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP && node->sk && node->sk->sk_state == TCP_CLOSE &&
		last_tcp_state == TCP_CLOSE;
}

static bool fi_is_all_stat_report_stopped(const struct fi_pkt_parse *pkt_info, const struct fi_flow_node *node)
{
	return (node->flow_ctx.stat_stop & FI_FLOW_ALL_REPORT_STOP) == FI_FLOW_ALL_REPORT_STOP &&
		!node->flow_cb.flow_identify_cfg->basic_rpt_en &&
		(pkt_info->msg.flow_msg.flow_fault_type == FI_FLOW_FAULT_NONE);
}

static bool fi_ipv4_pktinfo_update(struct sk_buff *skb, struct fi_pkt_parse *pkt_info, kuid_t *kuid, enum fi_dir dir)
{
	if (unlikely(skb == NULL))
		return false;
	if (!fi_ipv4_pkt_check(skb))
		return false;

	if (memset_s(pkt_info, sizeof(struct fi_pkt_parse), 0, sizeof(struct fi_pkt_parse))) {
		fi_loge("ipv4 hook memset_s failed");
		return false;
	}

	fi_ipv4_5tuple_parse(pkt_info, skb, dir);
	return fi_up_pkt_check(skb, pkt_info, kuid, dir);
}

static bool fi_check_flow_already_closed(int *last_tcp_state, struct fi_flow_node *node,
				struct fi_pkt_parse *pkt_info)
{
	fi_flow_get_mark(node->sk, &(node->flow_ctx.flow_msg));

	fi_update_last_tcp_state(last_tcp_state, node);

	if (fi_is_flow_already_closed(pkt_info, node, *last_tcp_state)) {
		fi_logd("Ignore flow:l4proto=%d,sk=%pK, sk_state=%d port[%hu--%hu]",
			pkt_info->msg.flow_msg.l4proto, node->sk, node->sk->sk_state,
			pkt_info->msg.flow_msg.sport, pkt_info->msg.flow_msg.dport);
		return true;
	}
	return false;
}

static bool fi_flow_qos_and_report_pre_check(struct fi_pkt_parse *pkt_info, const struct nf_hook_state *state,
				struct fi_flow_node *node, struct sk_buff *skb, enum fi_dir dir)
{
	if (pkt_info->msg.flow_msg.new_flow == true) {
		if (state->out) {
			fi_flow_get_dev(&pkt_info->msg.flow_msg, state->out);
			fi_flow_get_dev(&node->flow_ctx.flow_msg, state->out);
		} else {
			fi_logi("new flow dev is null");
		}
	}
	if (fi_flow_qos_rpt_enable(node) && pkt_info->msg.flow_msg.l4proto == IPPROTO_TCP) {
		if (dir == FI_DIR_UP)
			fi_filesize_pre_update(skb, node);
		else
			fi_qos_pre_update(skb, node);
	}

	/* if all stat report have stopped, bypass this packet */
	return fi_is_all_stat_report_stopped(pkt_info, node);
}

static void fi_set_pktinfo_from_flow(struct fi_pkt_parse *pktinfo, struct fi_flow_node *flow, enum fi_dir dir)
{
	if (dir == FI_DIR_DOWN) {
		pktinfo->flow_cb = flow->flow_cb;
		pktinfo->rand_num = pktinfo->flow_cb.app_info->rand_num;
		pktinfo->msg.flow_msg.uid = flow->uid;
		pktinfo->msg.flow_msg.dev_if = flow->flow_ctx.flow_msg.dev_if;
	}
	/* if the payload is segmented, copy payload to pktinfo->payload */
	pktinfo->dir = dir;
	pktinfo->is_malloc_buf = false;
	pktinfo->parse_len = fi_pre_get_payload_parse_len(flow->flow_cb.flow_identify_cfg, flow->flow_ctx.stat_stop, dir);
}

static void fi_ipv4_hook(struct sk_buff *ipv4_skb, const struct nf_hook_state *state, enum fi_dir dir)
{
	kuid_t kuid = {0};
	struct fi_pkt_parse pkt_info;
	uint32_t hash;
	bool enable = false;
	struct fi_flow_head *head = NULL;
	struct fi_flow_node *node = NULL;
	struct sk_buff *skb = ipv4_skb;
	int last_tcp_state = 0;

	if (!fi_ipv4_pktinfo_update(skb, &pkt_info, &kuid, dir))
		return;

	hash = fi_ipv4_flow_hash(&pkt_info, FI_FLOW_LEN + 1,
		((dir == FI_DIR_DOWN) ? true : false));
	head = fi_ipv4_flow_header(hash);
	fi_ipv4_flow_lock(hash);
	node = fi_ipv4_flow_get(head, kuid.val, &pkt_info,
		((dir == FI_DIR_UP) ? true : false), fi_get_sock_by_dir(skb, dir));
	if (node == NULL || fi_check_flow_already_closed(&last_tcp_state, node, &pkt_info) ||
		fi_flow_qos_and_report_pre_check(&pkt_info, state, node, skb, dir)) {
		fi_ipv4_flow_unlock(hash);
		return;
	}

	fi_set_pktinfo_from_flow(&pkt_info, node, dir);
	fi_ipv4_pkt_parse(&pkt_info, skb);

	if (dir == FI_DIR_DOWN && node->flow_ctx.key_info_check_rst == FI_RPT_NULL) {
		int http_rsp_code = fi_parse_http_rsp_code(skb, pkt_info.payload, pkt_info.parse_len);
		if (http_rsp_code > 0 && fi_is_blocked_rsp_code(http_rsp_code))
			node->flow_ctx.key_info_check_rst = FI_RPT_BLOCK;
		else if (http_rsp_code > 0 || http_rsp_code == -1)
			node->flow_ctx.key_info_check_rst = FI_RPT_GO;
	}

	fi_update_fault_type(dir, last_tcp_state, &pkt_info, &kuid);

	fi_flow_statistics(node, &pkt_info, dir);
	enable = (fi_pkt_report_check(node, &pkt_info, dir) &&
		node->flow_ctx.key_info_check_rst != FI_RPT_BLOCK) ||
		(pkt_info.msg.flow_msg.flow_fault_type > FI_FLOW_FAULT_NONE);
	if (enable) {
		int ret = strcpy_s(pkt_info.msg.flow_msg.dev, IFNAMSIZ, node->flow_ctx.flow_msg.dev);
		if (ret)
			fi_loge("strcpy_s failed ret=%d, dev=%s", ret, node->flow_ctx.flow_msg.dev);
	}
	fi_ipv4_flow_unlock(hash);

	if (enable)
		fi_pkt_msg_report(&pkt_info, FI_PKT_MSG_RPT);

	if (pkt_info.is_malloc_buf)
		kfree(pkt_info.payload);
}

static unsigned int fi_ipv4_nf_hook_out(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	fi_ipv4_hook(skb, state, FI_DIR_UP);
	return NF_ACCEPT;
}

static unsigned int fi_ipv4_nf_hook_in(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	fi_ipv4_hook(skb, state, FI_DIR_DOWN);
	return NF_ACCEPT;
}

#if IS_ENABLED(CONFIG_IPV6)
static void fi_ipv6_stop_flow_pkt_report(const struct fi_flow_msg *flow_msg)
{
	struct fi_pkt_parse pkt_info;
	uint32_t hash;
	struct fi_flow_head *head = NULL;
	struct fi_flow_node *node = NULL;

	if (memset_s(&pkt_info, sizeof(pkt_info), 0, sizeof(struct fi_pkt_parse))) {
		fi_loge("memset_s failed");
		return;
	}

	pkt_info.msg.flow_msg.ipv6_sip = flow_msg->ipv6_sip;
	pkt_info.msg.flow_msg.ipv6_dip = flow_msg->ipv6_dip;
	pkt_info.msg.flow_msg.sport = flow_msg->sport;
	pkt_info.msg.flow_msg.dport = flow_msg->dport;

	hash = fi_ipv6_flow_hash(&pkt_info, FI_FLOW_LEN + 1, false);
	if (strcmp(pkt_info.msg.flow_msg.flow, flow_msg->flow) != 0) {
		fi_loge("flow:%s not equal input param:%s", pkt_info.msg.flow_msg.flow, flow_msg->flow);
		return;
	}

	head = fi_ipv6_flow_header(hash);
	if (head == NULL) {
		fi_loge("flow:%s not find, hash head is null", pkt_info.msg.flow_msg.flow);
		return;
	}

	fi_ipv6_flow_lock(hash);
	node = fi_ipv6_flow_find(head, &flow_msg->ipv6_sip, flow_msg->sport, &flow_msg->ipv6_dip, flow_msg->dport);
	if (node != NULL) {
		node->flow_ctx.stat_stop |= FI_FLOW_PKT_REPORT_STOP;
		fi_logi("flow:%s stop pkt report", pkt_info.msg.flow_msg.flow);
	} else {
		fi_loge("flow:%s not find", pkt_info.msg.flow_msg.flow);
	}
	fi_ipv6_flow_unlock(hash);
}

static bool fi_ipv6_pkt_check(struct sk_buff *skb)
{
	int thoff = 0;
	int tprotocol;
	struct ipv6hdr *ip_hdr = ipv6_hdr(skb);
	if (ip_hdr == NULL)
		return false;
	if (ipv6_addr_loopback(&(ip_hdr->saddr)) ||
		ipv6_addr_loopback(&(ip_hdr->daddr)))
		return false;
	tprotocol = ipv6_find_hdr(skb, &thoff, -1, NULL, NULL);
	if (tprotocol == IPPROTO_UDP) {
		if (skb->len >= skb_transport_offset(skb) + sizeof(struct udphdr))
			return true;
	} else if (tprotocol == IPPROTO_TCP) {
		if (skb->len >= skb_transport_offset(skb) + tcp_hdrlen(skb))
			return true;
	}
	return false;
}

static void fi_ipv6_5tuple_parse(struct fi_pkt_parse *pkt_info, struct sk_buff *skb,
				enum fi_dir dir)
{
	struct ipv6hdr *ip_hdr = NULL;
	int thoff = 0;
	int tproto;
	int ret;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
	struct timeval ts;
	do_gettimeofday(&ts);
	pkt_info->msg.tstamp.tv_sec = ts.tv_sec;
	pkt_info->msg.tstamp.tv_usec = ts.tv_usec;
#else
	struct timespec64 ts;
	ktime_get_real_ts64(&ts);
	pkt_info->msg.tstamp.tv_sec = ts.tv_sec;
	pkt_info->msg.tstamp.tv_usec = ts.tv_nsec / 1000;
#endif
	pkt_info->msg.flow_msg.l3proto = ntohs(skb->protocol);
	pkt_info->msg.dir = dir;
	ip_hdr = ipv6_hdr(skb);
	ret = memcpy_s(&(pkt_info->msg.flow_msg.ipv6_sip), sizeof(pkt_info->msg.flow_msg.ipv6_sip),
		&(ip_hdr->saddr),
		sizeof(pkt_info->msg.flow_msg.ipv6_sip));
	if (ret) {
		fi_loge("memcpy_s failed ret=%d", ret);
		return;
	}

	ret = memcpy_s(&(pkt_info->msg.flow_msg.ipv6_dip), sizeof(pkt_info->msg.flow_msg.ipv6_dip),
		&(ip_hdr->daddr),
		sizeof(pkt_info->msg.flow_msg.ipv6_dip));
	if (ret) {
		fi_loge("memcpy_s failed ret=%d", ret);
		return;
	}
	tproto = ipv6_find_hdr(skb, &thoff, -1, NULL, NULL);
	fi_ip_save_5tuple_info(pkt_info, tproto, skb);
	return;
}

static void fi_ipv6_pkt_parse(struct fi_pkt_parse *pkt_info, struct sk_buff *skb)
{
	int i;
	spin_lock_bh(&(pkt_info->flow_cb.app_info->lock));
	if (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP)
		fi_payload_frag_parse(pkt_info, skb);

	for (i = 0; i < FI_END_PARSE; i++) {
		if ((pkt_info->flow_cb.flow_identify_cfg->pkt[pkt_info->dir].opt_enable[i] ||
			pkt_info->flow_cb.flow_identify_cfg->periodic[pkt_info->dir].opt_enable[i]) &&
			g_fi_pkt_parse_f[i].opt_val_parse != NULL) {
			fi_logd("parse target:%d function", i);
			(void)(g_fi_pkt_parse_f[i].opt_val_parse(skb, pkt_info));
		}
	}
	spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
	return;
}

static bool fi_ipv6_pktinfo_update(struct sk_buff *skb, struct fi_pkt_parse *pkt_info, kuid_t *kuid, enum fi_dir dir)
{
	if (unlikely(skb == NULL))
		return false;

	if (!fi_ipv6_pkt_check(skb))
		return false;

	if (memset_s(pkt_info, sizeof(struct fi_pkt_parse), 0, sizeof(struct fi_pkt_parse))) {
		fi_loge("ipv6 hook memset_s failed");
		return false;
	}

	fi_ipv6_5tuple_parse(pkt_info, skb, dir);

	return fi_up_pkt_check(skb, pkt_info, kuid, dir);
}

static void fi_ipv6_hook(struct sk_buff *ipv6_skb, const struct nf_hook_state *state, enum fi_dir dir)
{
	kuid_t kuid = {0};
	struct fi_pkt_parse pkt_info;
	uint32_t hash;
	bool enable = false;
	struct fi_flow_head *head = NULL;
	struct fi_flow_node *node = NULL;
	struct sk_buff *skb = ipv6_skb;
	int last_tcp_state = 0;

	if (!fi_ipv6_pktinfo_update(skb, &pkt_info, &kuid, dir))
		return;

	hash = fi_ipv6_flow_hash(&pkt_info, FI_FLOW_LEN + 1,
		((dir == FI_DIR_DOWN) ? true : false));
	head = fi_ipv6_flow_header(hash);
	fi_ipv6_flow_lock(hash);
	node = fi_ipv6_flow_get(head, kuid.val, &pkt_info,
		((dir == FI_DIR_UP) ? true : false), fi_get_sock_by_dir(skb, dir));
	if (node == NULL || fi_check_flow_already_closed(&last_tcp_state, node, &pkt_info) ||
		fi_flow_qos_and_report_pre_check(&pkt_info, state, node, skb, dir)) {
		fi_ipv6_flow_unlock(hash);
		return;
	}

	fi_set_pktinfo_from_flow(&pkt_info, node, dir);
	fi_ipv6_pkt_parse(&pkt_info, skb);

	if (dir == FI_DIR_DOWN && node->flow_ctx.key_info_check_rst == FI_RPT_NULL) {
		int http_rsp_code = fi_parse_http_rsp_code(skb, pkt_info.payload, pkt_info.parse_len);
		if (http_rsp_code > 0 && fi_is_blocked_rsp_code(http_rsp_code))
			node->flow_ctx.key_info_check_rst = FI_RPT_BLOCK;
		else if (http_rsp_code > 0 || http_rsp_code == -1)
			node->flow_ctx.key_info_check_rst = FI_RPT_GO;
	}

	fi_update_fault_type(dir, last_tcp_state, &pkt_info, &kuid);

	fi_flow_statistics(node, &pkt_info, dir);
	enable = (fi_pkt_report_check(node, &pkt_info, dir) && node->flow_ctx.key_info_check_rst != FI_RPT_BLOCK) ||
		(pkt_info.msg.flow_msg.flow_fault_type > FI_FLOW_FAULT_NONE);
	if (enable) {
		int ret = strcpy_s(pkt_info.msg.flow_msg.dev, IFNAMSIZ, node->flow_ctx.flow_msg.dev);
		if (ret)
			fi_loge("strcpy_s failed ret=%d, dev=%s", ret, node->flow_ctx.flow_msg.dev);
	}
	fi_ipv6_flow_unlock(hash);

	if (enable)
		fi_pkt_msg_report(&pkt_info, FI_PKT_MSG_RPT);

	if (pkt_info.is_malloc_buf)
		kfree(pkt_info.payload);
}

static unsigned int fi_ipv6_nf_hook_out(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	fi_ipv6_hook(skb, state, FI_DIR_UP);
	return NF_ACCEPT;
}

static unsigned int fi_ipv6_nf_hook_in(void *priv_data, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	fi_ipv6_hook(skb, state, FI_DIR_DOWN);
	return NF_ACCEPT;
}
#endif

void fi_init(struct sock *nlfd)
{
	int result;
	int i;

	result = memset_s(&g_fi_app_info, sizeof(g_fi_app_info), 0, sizeof(g_fi_app_info));
	if (result) {
		fi_loge("memset_s failed ret=%d", result);
		return;
	}

	for (i = 0; i < FI_LAUNCH_APP_MAX; i++)
		spin_lock_init(&(g_fi_app_info.app[i].lock));
	atomic_set(&(g_fi_app_info.free_cnt), FI_LAUNCH_APP_MAX);
	fi_logd("app_info init successfully");

	fi_ipv4_flow_init();
	fi_logd("IPV4 flow table init successfully");
#if IS_ENABLED(CONFIG_IPV6)
	fi_ipv6_flow_init();
	fi_logd("IPV6 flow table init successfully");
#endif

	result = memset_s(&g_fi_context, sizeof(g_fi_context), 0, sizeof(g_fi_context));
	if (result) {
		fi_loge("memset_s failed ret=%d", result);
		return;
	}

	mutex_init(&g_fi_context.nf_mutex);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)
	init_timer(&(g_fi_context.tm));
	g_fi_context.tm.function = fi_mod_timer;
#else
	timer_setup(&(g_fi_context.tm), fi_mod_timer, 0);
#endif
	g_fi_context.is_tm_start = false;
	g_fi_context.nlfd = nlfd;
	g_fi_context.running = true;
	skb_queue_head_init(&g_fi_context.skb_queue);
	sema_init(&g_fi_context.netlink_sync_sema, 0);
	g_fi_context.netlink_task = kthread_run(fi_netlink_thread, NULL, "fi_nl_thread");
	fi_logi("fi init success");
	return;
}
EXPORT_SYMBOL(fi_init); /*lint !e580*/

void fi_deinit(void)
{
	int result;

	fi_unregister_nfhook();
	mutex_lock(&(g_fi_context.nf_mutex));
	fi_del_timer_sync_if_no_periodic_rpt(false);
	mutex_unlock(&(g_fi_context.nf_mutex));

	result = memset_s(&g_fi_app_info, sizeof(g_fi_app_info), 0, sizeof(g_fi_app_info));
	if (result) {
		fi_loge("memset_s failed ret=%d", result);
		return;
	}
	fi_logd("app_info deinit successfully");

	fi_ipv4_flow_clear();
#if IS_ENABLED(CONFIG_IPV6)
	fi_ipv6_flow_clear();
#endif
	fi_logd("flow table deinit successfully");

	g_fi_context.running = false;
	if (g_fi_context.netlink_task != NULL) {
		kthread_stop(g_fi_context.netlink_task);
		g_fi_context.netlink_task = NULL;
	}
	fi_empty_netlink_skb_queue();

	fi_logi("deinit successfully");
	return;
}
EXPORT_SYMBOL(fi_deinit); /*lint !e580*/
/**
 * fi_mstamp_get - get current timestamp
 * @cl: place to store timestamps
 */
static void fi_mstamp_get(struct fi_mstamp *stamp)
{
	u64 time_now = local_clock();
	do_div(time_now, NSEC_PER_USEC);
	stamp->stamp_us = (u32)time_now;
	stamp->stamp_jiffies = (u32)jiffies;
}

/**
 * fi_mstamp_delta - compute the difference in usec between two fi_mstamp
 * @t1: pointer to newest sample
 * @t0: pointer to oldest sample
 */
static u32 fi_mstamp_us_delta(struct fi_mstamp *t1, struct fi_mstamp *t0)
{
	s32 delta_us = t1->stamp_us - t0->stamp_us;
	u32 delta_jiffies = t1->stamp_jiffies - t0->stamp_jiffies;

	/* If delta_us is negative, this might be because interval is too big,
	 * or local_clock() drift is too big : fallback using jiffies.
	 */
	if ((delta_us <= 0) || (delta_jiffies >= (INT_MAX / (USEC_PER_SEC / HZ))))
		delta_us = jiffies_to_usecs(delta_jiffies);

	return delta_us;
}

void fi_bw_calc(struct fi_flow_node *node)
{
	uint32_t delta_us;

	if ((node->rcv_bw.first_rcv_time.stamp_jiffies == 0) && (node->rcv_bw.first_rcv_time.stamp_us == 0))
		return;
	delta_us = fi_mstamp_us_delta(&(node->rcv_bw.last_rcv_time), &(node->rcv_bw.first_rcv_time));
	if (node->rcv_bw.total_bytes == 0) {
		if (node->rcv_bw.burst_bytes < FI_MIN_BURST_SIZE || delta_us * FI_TIMER_INTERVAL < USEC_PER_SEC)
			return;
	}
	if (node->rcv_bw.burst_bytes >= FI_MIN_BURST_SIZE) {
		node->rcv_bw.total_bytes += node->rcv_bw.burst_bytes;
		node->rcv_bw.total_time += delta_us;
	}
	if (node->rcv_bw.total_time < FI_MIN_BURST_DURATION)
		node->qos.bw_est = (uint64_t)node->rcv_bw.total_bytes * USEC_PER_SEC / FI_MIN_BURST_DURATION;
	else
		node->qos.bw_est = (uint64_t)node->rcv_bw.total_bytes * USEC_PER_SEC / node->rcv_bw.total_time;
	fi_logi("bw: uid %u, sport: %u, total_bytes: %u, total_time: %u, bw_est: %u",
			node->uid, node->sport, node->rcv_bw.total_bytes, node->rcv_bw.total_time, node->qos.bw_est);
	node->rcv_bw.total_bytes = 0;
	node->rcv_bw.total_time = 0;
}

void fi_bw_update(struct fi_flow_node *node)
{
	if (node->rcv_bw.burst_bytes > FI_MIN_BURST_SIZE) {
		uint32_t delta_us = fi_mstamp_us_delta(&(node->rcv_bw.last_rcv_time), &(node->rcv_bw.first_rcv_time));
		fi_logd("burst: uid %u, sport: %u, burst_bytes: %u, delta_us: %u",
			node->uid, node->sport, node->rcv_bw.burst_bytes, delta_us);
		delta_us = (delta_us > FI_MIN_BURST_DURATION) ? delta_us : FI_MIN_BURST_DURATION;
		node->rcv_bw.total_bytes += node->rcv_bw.burst_bytes;
		node->rcv_bw.total_time += delta_us;
	}
}

void fi_rtt_update(struct fi_flow_node *flow)
{
	unsigned long now_jiffies = jiffies;
	u64 now_mstamp = tcp_clock_us();
	struct tcp_sock *tp = NULL;
	uint32_t srtt;
	uint32_t rcv_rtt;
	unsigned long srtt_time;
	u64 rcv_rtt_time;
	u32 delta_us;
	char cell0_ifname[IFNAMSIZ] = {0};
	char cell1_ifname[IFNAMSIZ] = {0};

	if ((flow->sk == NULL) || (flow->sk->sk_protocol != IPPROTO_TCP)
		|| (flow->sk->sk_state != TCP_ESTABLISHED && flow->sk->sk_state != TCP_SYN_SENT))
		return;

	tp = tcp_sk(flow->sk);
	srtt = tp->srtt_us >> FI_RTT_SHIFT;
	srtt_time = tp->rcv_tstamp;
	rcv_rtt = tp->rcv_rtt_est.rtt_us >> FI_RTT_SHIFT;
	rcv_rtt_time = tp->rcv_rtt_est.time;
	fi_logd("sport %u now %lu now_mstamp %llu srtt_time %lu srtt %u rcv_rtt_time %llu rcv_rtt %u",
			flow->sk->sk_num, now_jiffies, now_mstamp, srtt_time, srtt, rcv_rtt_time, rcv_rtt);
	fi_logd("flow updatetime %u tcp_mstamp %llu mdev_us %u rttvar_us %u total_retrans %u retrans_stamp %u",
			flow->updatetime, tp->tcp_mstamp, tp->mdev_us, tp->rttvar_us, tp->total_retrans, tp->retrans_stamp);
	/* SYN TIMEOUT */
	if (flow->sk->sk_state == TCP_SYN_SENT && tp->total_retrans > 0) {
		if ((smartcom_get_network_iface_name(SMARTCOM_NET_CELL0, cell0_ifname, IFNAMSIZ - 1) &&
				strncmp(cell0_ifname, flow->flow_ctx.flow_msg.dev, strlen(flow->flow_ctx.flow_msg.dev)) == 0) ||
			(smartcom_get_network_iface_name(SMARTCOM_NET_CELL1, cell1_ifname, IFNAMSIZ - 1) &&
				strncmp(cell1_ifname, flow->flow_ctx.flow_msg.dev, strlen(flow->flow_ctx.flow_msg.dev)) == 0))
			flow->qos.rtt = tp->total_retrans * USEC_PER_SEC;
		else
			flow->qos.rtt = 0;
		fi_logi("syn time out, set rtt = %u on iface:%s", flow->qos.rtt, flow->flow_ctx.flow_msg.dev);
		return;
	}
	if (rcv_rtt == 0) {
		flow->qos.rtt = srtt;
		return;
	}
	delta_us = tcp_stamp_us_delta(now_mstamp, rcv_rtt_time);
	/*lint -e666*/
	if (time_before(now_jiffies, srtt_time + msecs_to_jiffies(MSEC_PER_SEC)) ||
			time_before(now_jiffies, srtt_time + usecs_to_jiffies(delta_us)) || srtt < tp->rcv_rtt_est.rtt_us)
		flow->qos.rtt = srtt;
	else
		flow->qos.rtt = rcv_rtt;
	/*lint +e666*/
}

void fi_qos_pre_update(const struct sk_buff *skb, struct fi_flow_node *flow)
{
	uint32_t max_delta;
	uint32_t delta_time_last;
	uint32_t delta_time_first;
	struct fi_mstamp now;
	fi_mstamp_get(&now);
	if (unlikely(flow->qos.rtt == 0))
		fi_rtt_update(flow);
	if (flow->sk == NULL || flow->sk->sk_protocol != IPPROTO_TCP)
		return;
	max_delta = flow->qos.rtt >> 1;
	delta_time_last = fi_mstamp_us_delta(&now, &(flow->rcv_bw.last_rcv_time));
	delta_time_first = fi_mstamp_us_delta(&now, &(flow->rcv_bw.first_rcv_time));
	fi_logd("dev %s, sport %u, jiffies %u us %u len %u", flow->flow_ctx.flow_msg.dev, flow->sk->sk_num,
		now.stamp_jiffies, now.stamp_us, skb->len);
	if (delta_time_last <= max_delta || delta_time_last <= FI_MIN_DELTA_US || delta_time_first <= flow->qos.rtt) {
		flow->rcv_bw.burst_bytes += skb->len;
	} else {
		fi_bw_update(flow);
		flow->rcv_bw.first_rcv_time = now;
		flow->rcv_bw.burst_bytes = 0;
	}
	flow->rcv_bw.last_rcv_time = now;
	flow->qos.rcv_bytes += skb->len;
}

void fi_filesize_pre_update(const struct sk_buff *skb, struct fi_flow_node *node)
{
	uint32_t download_bytes;
	node->flow_ctx.up_pkt_num++;
	if (skb->len <= skb_transport_offset(skb) + tcp_hdrlen(skb) ||
		(node->sk == NULL || node->sk->sk_protocol != IPPROTO_TCP)) {
		return;
	} else {
		struct tcp_sock *tp = tcp_sk(node->sk);
		if (unlikely(node->file_size.last_rcv_bytes == 0)) {
			node->file_size.last_rcv_bytes = tp->bytes_received;
			return;
		}

		if (tp->bytes_received < node->file_size.last_rcv_bytes)
			return;

		download_bytes = tp->bytes_received - node->file_size.last_rcv_bytes;
		node->file_size.last_rcv_bytes = tp->bytes_received;
		if (download_bytes > node->file_size.size) {
			node->file_size.size = download_bytes;
			fi_logd("sport %u filesize %u", node->sk->sk_num, node->file_size.size);
		}
	}
}

int fi_iface_qos_update_by_dev(char *data, int net_type, uint32_t size)
{
	struct fi_iface_msg iface_msg;
	struct net_device *dev = NULL;
	struct rtnl_link_stats64 stats;
	char iface[IFNAMSIZ] = {0};
	int ret;
	static int report_cnt = FI_IFACE_PRINT_LOG_INTERVAL;
	if (!smartcom_get_network_iface_name(net_type, iface, IFNAMSIZ - 1))
		return -1;
	ret = strncpy_s(iface_msg.dev, IFNAMSIZ, iface, strlen(iface));
	if (ret != EOK) {
		fi_loge("strncpy_s failed ret: %d, iface: %s", ret, iface);
		return -1;
	}

	dev = dev_get_by_name(&init_net, iface);
	if (dev) {
		read_lock(&dev_base_lock);
		dev_get_stats(dev, &stats);
		read_unlock(&dev_base_lock);
		dev_put(dev);
		if (stats.rx_bytes > g_fi_iface_bytes[net_type])
			iface_msg.rcv_bytes = stats.rx_bytes - g_fi_iface_bytes[net_type];
		else
			iface_msg.rcv_bytes = 0;
		g_fi_iface_bytes[net_type] = stats.rx_bytes;
	} else {
		iface_msg.rcv_bytes = 0;
		/* clear g_fi_iface_bytes if dev is off */
		g_fi_iface_bytes[net_type] = 0;
		iface_msg.dev[0] = '\0';
	}

	if (dev && report_cnt == FI_IFACE_PRINT_LOG_INTERVAL)
		fi_logi("Periodic report. iface: %s total rcv_byes: %lu", iface, g_fi_iface_bytes[net_type]);

	ret = memcpy_s(data + net_type * sizeof(struct fi_iface_msg), size - net_type * sizeof(struct fi_iface_msg),
		&iface_msg, sizeof(struct fi_iface_msg));
	if (ret != EOK) {
		fi_loge("memcpy_s failed ret: %d, size: %u", ret, size);
		return -1;
	}

	if (dev && report_cnt == FI_IFACE_PRINT_LOG_INTERVAL)
		fi_logi("Periodic report. iface %s inc rcv_bytes: %u", iface_msg.dev, iface_msg.rcv_bytes);

	report_cnt--;
	if (report_cnt <= 0)
		report_cnt = FI_IFACE_PRINT_LOG_INTERVAL;

	return 0;
}

void fi_iface_qos_update_report(void)
{
	int i;
	uint32_t size = sizeof(struct fi_iface_msg) * FI_DEV_NUM;
	char *data = NULL;
	struct sk_buff *pskb = NULL;

	if (!g_fi_context.iface_qos_rpt_enable)
		return;

	pskb = fi_get_netlink_skb(FI_IFACE_MSG_RPT, size, &data);
	if (pskb == NULL) {
		fi_loge("failed to malloc memory for data");
		return;
	}
	for (i = 0; i < FI_DEV_NUM; i++) {
		if (fi_iface_qos_update_by_dev(data, i, size) != 0) {
			kfree_skb(pskb);
			return;
		}
	}
	fi_enqueue_netlink_skb(pskb);

	return;
}
