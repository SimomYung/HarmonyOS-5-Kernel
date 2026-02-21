/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: This module is to collect model parameters
 * Author: hongyuping1@huawei.com
 * Create: 2024-02-26
 */

#include "model_para_collec.h"
#include <linux/net.h>
#include <linux/list.h>
#include <linux/inet.h>
#include <linux/sort.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <securec.h>
#include "netlink_handle.h"
#include "ip_para_collec.h"

static DEFINE_SPINLOCK(g_model_para_collec_lock);
struct model_ctx_list *g_ctx_list = NULL;
bool g_is_model_collected = false;
u16 g_parallels_flow_num = 0;

// linktype、lenOffset、fiTypeOffset、snOffsetHigh、snOffsetLow
const u16 payload_offset_info[PAYLOAD_HEAD_TYPE_MAX_NUM][OFFSET_MAX_NUM] = {
	{PAYLOAD_HEAD_TYPE_9711, PKTTYPE_OFFSET_9711, SN_HIGH_OFFSET_9711, SN_LOW_OFFSET_9711},
	{PAYLOAD_HEAD_TYPE_A401, PKTTYPE_OFFSET_A401, SN_HIGH_OFFSET_A401, SN_LOW_OFFSET_A401},
	{PAYLOAD_HEAD_TYPE_9613, PKTTYPE_OFFSET_9613, SN_HIGH_OFFSET_9613, SN_LOW_OFFSET_9613},
	{PAYLOAD_HEAD_TYPE_9815, PKTTYPE_OFFSET_9815, SN_HIGH_OFFSET_9815, SN_LOW_OFFSET_9815},
};

static bool match_key_flow_udp(struct sock *sk, struct sk_buff *skb, struct ip_info_tuple *info)
{
	if (sk == NULL || skb == NULL || info == NULL) {
		pr_err("[MODEL_PARA] %s:udph or info == NULL\n", __func__);
		return false;
	}
	u32 uid;
	struct  udphdr *uh = NULL;
	struct iphdr *iph = NULL;
	struct ipv6hdr *ipv6h = NULL;

	uid = (u32)sk->sk_uid.val;
	if (uid != info->uid)
		return false;

	uh = udp_hdr(skb);
	if (uh == NULL)
		return false;

	if (htons(uh->source) != info->dest_port)
		return false;
	if (skb->protocol == ntohs(ETH_P_IP)) {
		iph = ip_hdr(skb);
		if (iph == NULL)
			return false;
		if (iph->saddr == info->dest_addr.ip4_addr)
			return true;
	} else if (skb->protocol == ntohs(ETH_P_IPV6)) {
		ipv6h = ipv6_hdr(skb);
		if (ipv6h == NULL)
			return false;
		if (ipv6_addr_equal(&ipv6h->saddr, &info->dest_addr.ip6_addr))
			return true;
	} else {
		return false;
	}

	return false;
}

static void report_data_process_bitmap(struct seqnum_bit_map *bit_map,
	struct udp_para_rpt *udp_para, struct temp_calc_para *temp_para)
{
	if (bit_map == NULL || udp_para == NULL || temp_para == NULL)
		return;
	if (!temp_para->is_record_start_sn)
		return;

	int curt = -1;
	int minseq = 0;
	int sn_index = temp_para->sn_start;
	int sn_end = temp_para->sn_end;
	if (sn_index > sn_end)
		sn_end = sn_end + SHORT_MAX_HEX + 1;
	udp_para->pktnum = 0;
	while (sn_index <= sn_end) {
		int offset = (sn_index >= SHORT_MAX_HEX + 1) ? (SHORT_MAX_HEX + 1) : 0;
		int high = (sn_index - offset) >> BYTE_LEN;
		int low = (sn_index - offset) & BYTE_MAX_HEX;
		if (((bit_map[high % BITMAP_MAX_NUM].map[low >> BITMAP_INDEX_LEN]) & (1 << (low & FOUR_BIT_MAX_HEX))) <= 0) {
			sn_index++;
			continue;
		}
		udp_para->pktnum += 1;
		if (curt == -1) {
			curt = sn_index;
			minseq = curt;
		} else {
			int loss = sn_index - curt - 1;
			udp_para->loss_num += loss;
			udp_para->max_con_loss_num = (loss > udp_para->max_con_loss_num) ?
				loss : udp_para->max_con_loss_num;
			curt = sn_index;
		}
		sn_index++;
	}
	temp_para->sn_start = temp_para->sn_end;
}

static void report_data_process_bytype(struct model_ctx *ctx)
{
	struct calc_para *calc_para = NULL;
	if (ctx == NULL)
		return;

	calc_para = &(ctx->calc_para[VOICE_PARA_INDEX]);
	report_data_process_bitmap(&(calc_para->bit_map[0]), &(calc_para->calc_udp_para), &(calc_para->temp_para));
	if (ctx->fi_type == VIDEO_FI_TYPE) {
		calc_para = &(ctx->calc_para[VIDEO_PARA_INDEX]);
		report_data_process_bitmap(&(calc_para->bit_map[0]), &(calc_para->calc_udp_para), &(calc_para->temp_para));
	}
	if (ctx->payload_type == PAYLOAD_HEAD_TYPE_A401) {
		calc_para = &(ctx->ptp_extra);
		report_data_process_bitmap(&(calc_para->bit_map[0]), &(calc_para->calc_udp_para), &(calc_para->temp_para));
		ctx->calc_para[VOICE_PARA_INDEX].calc_udp_para.loss_num = calc_para->calc_udp_para.loss_num;
		ctx->calc_para[VOICE_PARA_INDEX].calc_udp_para.max_con_loss_num = calc_para->calc_udp_para.max_con_loss_num;
		if (ctx->fi_type == VIDEO_FI_TYPE) {
			ctx->calc_para[VIDEO_PARA_INDEX].calc_udp_para.loss_num = calc_para->calc_udp_para.loss_num;
			ctx->calc_para[VIDEO_PARA_INDEX].calc_udp_para.max_con_loss_num = calc_para->calc_udp_para.max_con_loss_num;
		}
	}
}

static int report_data_process(void)
{
	if (g_ctx_list == NULL) {
		pr_err("[MODEL_PARA] %s:report_data_process failed", __func__);
		return MODEL_FAIL;
	}

	struct process_para *para = NULL;
	struct seqnum_bit_map *bit_map = NULL;
	struct model_ctx *ctx = NULL;

	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; ++i) {
		if (g_ctx_list[i].ip_info.uid == 0) {
			continue;
		}
		ctx = &(g_ctx_list[i].ctx);
		if (ctx->payload_type == 0) {
			continue;
		}
		report_data_process_bytype(ctx);
	}

	return MODEL_OK;
}

static void stream_process_base_data(struct extracted_payload_para *payload_para,
	struct udp_para_rpt *udp_para, struct temp_calc_para *temp_para)
{
	if (payload_para == NULL || udp_para == NULL || temp_para == NULL)
		return;
	if (temp_para->header_number >= BITMAP_MAX_NUM) {
		pr_err("[MODEL_PARA] %s:too much pkt,dicard following pkt", __func__);
		return;
	}

	udp_para->total_pkt_len += payload_para->pkt_len;
	udp_para->pktnum += 1;

	if (temp_para->ts_last == -1) {
		temp_para->ts_last = payload_para->timestamp;
	} else {
		int tmp = payload_para->timestamp - temp_para->ts_last;
		tmp = (tmp >= 0) ? tmp : (tmp + TIME_ONESEC_TO_MS);
		temp_para->ts_last = payload_para->timestamp;
		udp_para->total_time_interval += tmp;
		udp_para->max_time_interval = ((tmp > udp_para->max_time_interval) ? tmp : udp_para->max_time_interval);
		udp_para->square_sum_time_interval += (int)(tmp * tmp);
	}
}

static void stream_process_bit_map(u16 seq_num, struct seqnum_bit_map *bit_map, struct temp_calc_para *temp_para)
{
	if (bit_map == NULL || temp_para == NULL)
		return;

	u8 cur_sn_high = seq_num >> BYTE_LEN;
	u8 cur_sn_low = seq_num & BYTE_MAX_HEX;
	u8 last_sn_start_high = temp_para->sn_start >> BYTE_LEN;
	u8 last_sn_end_high = temp_para->sn_end >> BYTE_LEN;
	u16 last_bitmap_index = last_sn_start_high % BITMAP_MAX_NUM + (temp_para->sn_start & BYTE_MAX_HEX);
	u16 cur_bitmap_index = cur_sn_high % BITMAP_MAX_NUM + cur_sn_low;

	bit_map[cur_sn_high % BITMAP_MAX_NUM].header = cur_sn_high;
	bit_map[cur_sn_high % BITMAP_MAX_NUM].map[cur_sn_low >> BITMAP_INDEX_LEN] |= 1 << (cur_sn_low & FOUR_BIT_MAX_HEX);
	if (!temp_para->is_record_start_sn) {
		temp_para->sn_start = seq_num;
		temp_para->sn_end = seq_num;
		temp_para->is_record_start_sn = true;
	} else {
		if (cur_sn_high >= BYTE_MAX_HEX - 1 && last_sn_start_high <= 1) {
			temp_para->sn_start = seq_num;
		} else if (cur_sn_high <= 1 && last_sn_start_high >= BYTE_MAX_HEX - 1) {
			if (last_bitmap_index < cur_bitmap_index)
				temp_para->sn_start = seq_num + 1;
		} else {
			temp_para->sn_start = temp_para->sn_start > seq_num ? seq_num : temp_para->sn_start;
		}
		if (last_sn_end_high >= BYTE_MAX_HEX - 1 && cur_sn_high <= 1) {
			temp_para->sn_end = seq_num;
		} else if (!(last_sn_end_high <= 1 && cur_sn_high >= BYTE_MAX_HEX - 1)) {
			temp_para->sn_end = temp_para->sn_end > seq_num ? temp_para->sn_end : seq_num;
		}
	}
}

static void stream_data_process(struct extracted_payload_para *payload_para, struct model_ctx *ctx)
{
	if (ctx == NULL || payload_para == NULL)
		return;

	struct udp_para_rpt *udp_para = NULL;
	struct temp_calc_para *temp_para = NULL;
	struct seqnum_bit_map *bit_map = NULL;
	int index = 0;

	spin_lock_bh(&g_model_para_collec_lock);
	index = (payload_para->pkt_type == 1) ? VIDEO_PARA_INDEX : VOICE_PARA_INDEX;
	udp_para = &(ctx->calc_para[index].calc_udp_para);
	temp_para = &(ctx->calc_para[index].temp_para);
	bit_map = &(ctx->calc_para[index].bit_map[0]);
	stream_process_base_data(payload_para, udp_para, temp_para);
	stream_process_bit_map(payload_para->seq_num, bit_map, temp_para);
	if (ctx->payload_type == PAYLOAD_HEAD_TYPE_A401) {
		udp_para = &(ctx->ptp_extra.calc_udp_para);
		bit_map = &(ctx->ptp_extra.bit_map[0]);
		temp_para = &(ctx->ptp_extra.temp_para);
		stream_process_base_data(payload_para, udp_para, temp_para);
		stream_process_bit_map(payload_para->seq_num, bit_map, temp_para);
	}
	spin_unlock_bh(&g_model_para_collec_lock);
}

static bool is_cared_linktype(struct model_ctx *ctx, u16 payload_type)
{
	if (ctx == NULL)
		return false;
	if (ctx->payload_type == 0) {
		for (int i = 0; i < PAYLOAD_HEAD_TYPE_MAX_NUM; i++) {
			if (payload_offset_info[i][0] != payload_type) {
				continue;
			}
			ctx->payload_type = payload_type;
			ctx->offset.pkt_type_offset = payload_offset_info[i][PKT_TYPE_OFFSET_INDEX];
			ctx->offset.sn_high_offset = payload_offset_info[i][SN_HIGH_OFFSET_INDEX];
			ctx->offset.sn_low_offset = payload_offset_info[i][SN_LOW_OFFSET_INDEX];
			return true;
		}
	} else {
		if (payload_type == ctx->payload_type)
			return true;
		if ((payload_type == PAYLOAD_HEAD_TYPE_9613 || payload_type == PAYLOAD_HEAD_TYPE_9815) &&
				((ctx->payload_type == PAYLOAD_HEAD_TYPE_9613) || (ctx->payload_type == PAYLOAD_HEAD_TYPE_9815)))
			return true;
	}
	return false;
}

static bool is_care_pkttype(struct extracted_payload_para *payload_para, u16 payload_type, u16 pkt_type)
{
	if (payload_para == NULL)
		return false;
	if (payload_type == PAYLOAD_HEAD_TYPE_9711 || payload_type == PAYLOAD_HEAD_TYPE_A401) {
		if (pkt_type == WECHAT_VIDEO_LAG) {
			payload_para->pkt_type = 1;
		} else if (pkt_type == WECHAT_VOICE_LAG) {
			payload_para->pkt_type = 0;
		} else {
			return false;
		}
	}
	if (payload_type == PAYLOAD_HEAD_TYPE_9613) {
		if (pkt_type == VIDEO_TYPE_VALUE_9613) {
			payload_para->pkt_type = 1;
		} else {
			return  false;
		}
	}
	if (payload_type == PAYLOAD_HEAD_TYPE_9815) {
		if (pkt_type == VOICE_TYPE_VALUE_9815) {
			payload_para->pkt_type = 0;
		} else {
			return  false;
		}
	}
	return true;
}

static void payload_process(struct model_pkt_parse *pktinfo, struct model_ctx *ctx)
{
	if (pktinfo == NULL || ctx == NULL)
		return;

	u16 payload_type;
	u16 pkt_type;
	u16 seq_num;
	struct extracted_payload_para payload_para;
	unsigned char *payload = pktinfo->frag_payload;
	if (payload == NULL)
		return;

	pktinfo->raw_frag_num += 1;
	payload_type = ((*payload) << BYTE_LEN) + (*(payload + 1));
	if (!is_cared_linktype(ctx, payload_type))
		return;
	// 9815的第20为0x0b时抛弃
	if ((payload_type == PAYLOAD_HEAD_TYPE_9815) && ((*(payload + EXCEPTION_OFFSET_9815)) == EXCEPTION_VALUE_9815))
		return;
	pkt_type = *(payload + ctx->offset.pkt_type_offset);
	if (!is_care_pkttype(&payload_para, payload_type, pkt_type))
		return;

	seq_num = ((*(payload + ctx->offset.sn_high_offset)) << BYTE_LEN) + (*(payload + ctx->offset.sn_low_offset));
	payload_para.seq_num = seq_num;
	payload_para.timestamp = pktinfo->time_stamp;
	payload_para.pkt_len = pktinfo->frag_payload_len;
	stream_data_process(&payload_para, ctx);
}


static void payload_frag_meta_frags_parse(struct model_pkt_parse *pktinfo,
	const struct sk_buff *skb, struct model_ctx *ctx)
{
	int tmp_num = 1;
	unsigned short gso_size = 0;
	const skb_frag_t *frag = NULL;
	unsigned char *frag_addr = NULL;

	if (skb_shinfo(skb)->nr_frags == 0)
		return;

	gso_size = skb_shinfo(skb)->gso_size;
	for (int i = 0; i < skb_shinfo(skb)->nr_frags && i < MAX_SKB_FRAGS; i++) {
		frag = (const skb_frag_t *)(&skb_shinfo(skb)->frags[i]);
		frag_addr = skb_frag_address_safe(frag);
		if (!skb_is_gso(skb)) {
			pktinfo->frag_payload_len = skb_frag_size(frag);
			pktinfo->frag_payload = frag_addr;
			payload_process(pktinfo, ctx);
			continue;
		}
		if (gso_size <= 0) {
			pr_err("[MODEL_PARA] %s:gso_size is 0 ", __func__);
			continue;
		}
		if (skb_frag_size(frag) >= gso_size) {
			tmp_num = skb_frag_size(frag) / gso_size;
			for (int j = 0; j < tmp_num; j++) {
				pktinfo->frag_payload_len = gso_size;
				pktinfo->frag_payload = (frag_addr != NULL ? frag_addr + j * gso_size : NULL);
				payload_process(pktinfo, ctx);
			}
			if (skb_frag_size(frag) % gso_size == 0)
				continue;
			pktinfo->frag_payload_len = skb_frag_size(frag) % gso_size;
			pktinfo->frag_payload = (frag_addr != NULL ? frag_addr + tmp_num * gso_size : NULL);
			if (frag_addr != NULL && pktinfo->frag_payload - frag_addr + pktinfo->frag_payload_len >
									 skb_frag_size(frag)) {
				pktinfo->frag_payload_len = skb_frag_size(frag);
				pktinfo->frag_payload = frag_addr;
			}
			payload_process(pktinfo, ctx);
		}
	}
}

static void payload_frag_meta_parse(struct model_pkt_parse *pktinfo, const struct sk_buff *skb, struct model_ctx *ctx)
{
	int transport_hdr_len = 0;
	u16 linear_payload_len = 0;

	/* step1: parse the linear area. linear area may have no payload or have only payload! */
	if (skb_transport_header_was_set(skb)) {
		transport_hdr_len = sizeof(struct udphdr);
		if (skb_headlen(skb) > skb_transport_offset(skb) + transport_hdr_len)
			linear_payload_len = skb_headlen(skb) - skb_transport_offset(skb) - transport_hdr_len;
	} else {
		linear_payload_len = skb_headlen(skb);
	}
	if (linear_payload_len > 0) { // linear area has payload
		pktinfo->frag_payload_len = linear_payload_len;
		pktinfo->frag_payload = skb_transport_header_was_set(skb) ?
								skb_transport_header(skb) + transport_hdr_len : skb->data;
		payload_process(pktinfo, ctx);
	}

	/* step2: parse the frags area */
	payload_frag_meta_frags_parse(pktinfo, skb, ctx);
}


static void payload_frag_parse(struct model_pkt_parse *pktinfo, const struct sk_buff *skb, struct model_ctx *ctx)
{
	if (pktinfo == NULL || ctx == NULL)
		return;
	struct sk_buff *frag_list = NULL;
	if (!skb_is_gso(skb)) {
		payload_process(pktinfo, ctx);
		return;
	}

	/* step1: parse the linear and frags area */
	payload_frag_meta_parse(pktinfo, skb, ctx);

	/* step2: parse the frag_list area, no recursive */
	if (skb_shinfo(skb)->frag_list != NULL) {
		frag_list = skb_shinfo(skb)->frag_list;
		while (frag_list && pktinfo->raw_frag_num < SKB_MAX_FRAG_LIST_NUM) {
			payload_frag_meta_parse(pktinfo, frag_list, ctx);
			frag_list = frag_list->next;
		}
	}
}

void model_get_udp_para(struct sock *sk, struct sk_buff *skb, u8 direction)
{
	if (!g_is_model_collected || direction != NF_INET_UDP_IN_HOOK)
		return;
	if (skb == NULL || g_ctx_list == NULL)
		return;

	u16 time;
	struct model_pkt_parse pktinfo;
	unsigned char *para = skb->data;
	if (para == NULL)
		return;

	long time_ms = ktime_to_ms(skb->tstamp);
	time = (u16)(time_ms % TIME_ONESEC_TO_MS);
	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; i++) {
		if (g_ctx_list[i].ip_info.uid == 0)
			continue;
		if (match_key_flow_udp(sk, skb, &(g_ctx_list[i].ip_info))) {
			pktinfo.frag_payload_len = skb->len;
			pktinfo.raw_frag_num = 0;
			pktinfo.time_stamp = time;
			pktinfo.frag_payload = para + PAYLOAD_HEADER_ADDR;
			payload_frag_parse(&pktinfo, skb, &(g_ctx_list[i].ctx));
		}
	}
}

static void ctx_init(void)
{
	if (g_ctx_list == NULL) {
		pr_err("[MODEL_PARA] %s:reset data failed due to ctx list is null", __func__);
		return;
	}

	struct model_ctx *ctx = NULL;
	int len = sizeof(struct calc_para);
	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; i++) {
		if (g_ctx_list[i].ip_info.uid == 0)
			continue;
		ctx = &(g_ctx_list[i].ctx);
		for (int j = 0; j < VOIP_TYPE_NUMS; j++) {
			(void)memset_s(&(ctx->calc_para[j]), len, 0, len);
			ctx->calc_para[j].temp_para.ts_last = -1;
		}
		if (ctx->payload_type == PAYLOAD_HEAD_TYPE_A401) {
			(void)memset_s(&(ctx->ptp_extra), len, 0, len);
			ctx->ptp_extra.temp_para.ts_last = -1;
		}
	}
}

static int single_flow_rpt_data_cpy(struct udp_para_rpt* rpt_data, struct model_ctx *ctx)
{
	if (rpt_data == NULL || ctx == NULL)
		return MODEL_FAIL;

	struct udp_para_rpt *para = NULL;
	int len = sizeof(struct udp_para_rpt);

	para = &(ctx->calc_para[VOICE_PARA_INDEX].calc_udp_para);
	if (memcpy_s(&(rpt_data[VOICE_PARA_INDEX]), len, para, len) != MODEL_OK)
		return MODEL_FAIL;
	if (ctx->fi_type == VIDEO_FI_TYPE) {
		para = &(ctx->calc_para[VIDEO_PARA_INDEX].calc_udp_para);
		if (memcpy_s(&(rpt_data[VIDEO_PARA_INDEX]), len, para, len) != MODEL_OK)
			return MODEL_FAIL;
	}
	return MODEL_OK;
}

static int report_data_cpy(struct udp_info_msg *res)
{
	if (res == NULL)
		return MODEL_FAIL;
	int len = sizeof(struct udp_para_rpt);
	bool has_pts = false;
	bool has_ptp = false;
	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; i++) {
		if (g_ctx_list[i].ip_info.uid == 0 || g_ctx_list[i].ctx.payload_type == 0)
			continue;
		if (g_ctx_list[i].ctx.payload_type == PAYLOAD_HEAD_TYPE_A401) {
			res->para_ext[i].is_ptp = 1;
			continue;
		} else {
			res->para_ext[i].is_ptp = 0;
			has_pts = true;
		}
		res->para_ext[i].is_avaliable = 1;
		res->para_ext[i].fi_type = g_ctx_list[i].ctx.fi_type;
		if (single_flow_rpt_data_cpy(&(res->para_rpt[i][0]), &(g_ctx_list[i].ctx)) != MODEL_OK) {
			ctx_init();
			pr_err("[MODEL_PARA] %s:memcpy failed, index:%d", __func__, i);
			return MODEL_FAIL;
		}
	}
	if (has_pts && has_ptp && g_parallels_flow_num == PTP_PTS_ALL_EXIST_NUM) {
		res->is_need_combination = 1;
	}
	return MODEL_OK;
}

void model_info_report()
{
	struct app_ctx *g_app_ctx = NULL;
	struct udp_info_msg *res = NULL;
	if (!g_is_model_collected)
		return;
	res = kmalloc(sizeof(struct udp_info_msg), GFP_ATOMIC);
	if (res == NULL)
		return;
	(void)memset_s(res, sizeof(struct udp_info_msg), 0, sizeof(struct udp_info_msg));
	if (report_data_process() == MODEL_FAIL) {
		pr_err("[MODEL_PARA] %s:report_data_process failed", __func__);
		kfree(res);
		return;
	}
	u16 len = sizeof(struct udp_info_msg);
	res->type = MS_MODEL_PARA_RPT;
	res->len = len;
	if (report_data_cpy(res) != MODEL_OK) {
		kfree(res);
		return;
	}
	g_app_ctx = get_ip_para_collec_app_ctx();
	if (g_app_ctx == NULL) {
		pr_err("[MODEL_PARA] %s:g_app_ctx null", __func__);
		kfree(res);
		return;
	}
	if (g_app_ctx->fn)
		g_app_ctx->fn((struct res_msg_head *) res);
	ctx_init();
	kfree(res);
}

static void ctx_free(void)
{
	if (g_ctx_list == NULL) {
		pr_err("[MODEL_PARA] %s:g_app_ctx null", __func__);
		return;
	}
	kfree(g_ctx_list);
	g_ctx_list = NULL;
}

static int ctx_malloc(void)
{
	int len = MAX_PARALLEL_FLOWS_NUM * sizeof(struct model_ctx_list);
	g_ctx_list = kmalloc(len, GFP_ATOMIC);
	if (g_ctx_list == NULL)
		return MODEL_FAIL;
	(void)memset_s(g_ctx_list, len, 0, len);
	return MODEL_OK;
}

static int ip_info_cmp(struct ip_info_tuple *info_a, struct ip_info_tuple *info_b)
{
	if (info_a == NULL || info_b == NULL)
		return MODEL_FAIL;
	if (info_a->uid != info_b->uid || info_a->dest_port != info_b->dest_port
		|| info_a->l3proto != info_b->l3proto || info_a->fi_type != info_b->fi_type)
		return MODEL_FAIL;
	if (info_a->l3proto == ETH_P_IP) {
		if (info_a->dest_addr.ip4_addr != info_b->dest_addr.ip4_addr)
			return MODEL_FAIL;
	}
	if (info_a->l3proto == ETH_P_IPV6) {
		if (!ipv6_addr_equal(&(info_a->dest_addr.ip6_addr), &(info_b->dest_addr.ip6_addr)))
			return MODEL_FAIL;
	}
	return MODEL_OK;
}

void model_data_collect_start(struct model_key_flow_req *msg)
{
	if (g_ctx_list == NULL && ctx_malloc() != MODEL_OK) {
		pr_err("[MODEL_PARA] %s:ctx malloc failed", __func__);
		return;
	}
	if (!g_is_model_collected) {
		ctx_init();
	}
	if (g_parallels_flow_num > MAX_PARALLEL_FLOWS_NUM) {
		pr_err("[MODEL_PARA] %s:abnormal parallels flow num:%d", __func__, g_parallels_flow_num);
		return;
	}
	struct ip_info_tuple *ip_info = NULL;
	int len = sizeof(struct ip_info_tuple);
	int index = -1;
	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; i++) {
		ip_info = &(g_ctx_list[i].ip_info);
		if (ip_info->uid == 0) {
			index = i;
			break;
		}
	}
	if (index >= 0 && index < MAX_PARALLEL_FLOWS_NUM) {
		if (memcpy_s(&(g_ctx_list[index].ip_info), len, &(msg->ip_info), len) != MODEL_OK)
			return;
		g_ctx_list[index].ctx.fi_type = msg->ip_info.fi_type;
		g_parallels_flow_num++;
	} else {
		return;
	}
	g_is_model_collected = true;
	pr_info("[MODEL_PARA] %s:start collection successfully", __func__);
}

void model_data_collect_stop(struct model_key_flow_req *msg)
{
	if (!g_is_model_collected)
		return;
	struct ip_info_tuple *ip_info = NULL;
	int len = sizeof(struct model_ctx_list);
	for (int i = 0; i < MAX_PARALLEL_FLOWS_NUM; i++) {
		ip_info = &(g_ctx_list[i].ip_info);
		if (ip_info->uid == 0)
			continue;
		if (ip_info_cmp(ip_info, &(msg->ip_info)) == MODEL_OK) {
			(void)memset_s(&(g_ctx_list[i]), len, 0, len);
			if (g_parallels_flow_num == 0) {
				pr_err("[MODEL_PARA] %s:parallel flow is 0", __func__);
				break;
			}
			g_parallels_flow_num = g_parallels_flow_num - 1;
			break;
		}
	}
	if (g_parallels_flow_num <= 0) {
		g_is_model_collected = false;
	}
	pr_info("[MODEL_PARA] %s:stop collection, g_parallels_flow_num:%d", __func__, g_parallels_flow_num);
}