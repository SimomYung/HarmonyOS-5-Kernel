/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "smartcom_mpflow.h"

#include <linux/in.h>
#include <net/inet_sock.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/ipv6.h>
#include <net/inet_hashtables.h>
#include <net/inet6_hashtables.h>
#include "securec.h"

#include "../smartcom_netlink.h"
#include "../smartcom_utils.h"

#undef HWLOG_TAG
#define HWLOG_TAG smartcom_mpflow
HWLOG_REGIST();

#ifdef CONFIG_HUAWEI_FEATURE_PRINT_PID_NAME
extern void print_process_pid_name(struct inet_sock *inet);
#endif

static struct smarmcom_mpflow_network_info g_mpflow_network_infos[SMARTCOM_NET_MAX_NUM];
static spinlock_t g_mpflow_network_infos_cache_lock;
static volatile int32_t g_last_default_net_type = SMARTCOM_NET_INVALID;

static spinlock_t g_mpflow_ai_lock;
static struct smartcom_mpflow_ai_info g_mpflow_ai_uids[SMARTCOM_MPFLOW_AI_MAX_APP];
static struct smartcom_mpflow_stat g_mpflow_fail_stat_list[SMARTCOM_MPFLOW_AI_MAX_LIST_NUM];

static inline bool invalid_uid(uid_t uid)
{
	/* if uid less than 100 * 20000, it is not a normal app */
	return (uid < UID_APP);
}

static bool isvalid_mpflow_bind_flag(struct sock *sk)
{
	return (sk->smarmcom_bind_bitmap & 0x01) != 0;
}

static void add_mpflow_bind_flag(struct sock *sk)
{
	sk->smarmcom_bind_bitmap = sk->smarmcom_bind_bitmap | 0x01;
}

static void clear_mpflow_bind_flag(struct sock *sk)
{
	sk->smarmcom_bind_bitmap = sk->smarmcom_bind_bitmap & 0xFFFFFFFE;
}

static bool smartcom_mpflow_is_bind_dev_valid(int bind_dev)
{
	return (bind_dev > SMARTCOM_MPFLOW_BIND_NONE) && (bind_dev < SMARTCOM_MPFLOW_BIND_MAX);
}

static int smartcom_mpflow_ai_get_reset_bind_device(uint16_t reset_bind_dev)
{
	uint16_t bind_device;
	if (smartcom_mpflow_is_bind_dev_valid(reset_bind_dev)) {
		bind_device = reset_bind_dev;
	} else {
		bind_device = SMARTCOM_MPFLOW_BIND_NONE;
	}
	smartcom_logi("using reset_bind_dev: %u get bind_device: %u", reset_bind_dev, bind_device);
	return (int)bind_device;
}

static bool smartcom_mpflow_ai_get_addr(uint16_t sa_family,
	struct in_addr *addr, __be32 *s_addr, uint8_t s_addr_len)
{
	uint8_t i;

	if (s_addr_len > SMARTCOM_CLAT_IPV6)
		return false;

	if (sa_family == AF_INET) {
		struct in_addr *usin4 = (struct in_addr *)addr;
		*(s_addr + s_addr_len - 1) = usin4->s_addr;
		return true;
#if IS_ENABLED(CONFIG_IPV6)
	} else if (sa_family == AF_INET6) {
		struct in6_addr *usin6_tmp = (struct in6_addr *)addr;
		for (i = 0; i < s_addr_len; i++)
			*(s_addr + i) = usin6_tmp->s6_addr32[i];
		return true;
#endif
	} else {
		return false;
	}
}

static bool smartcom_mpflow_ai_rehash_sk(struct sock *sk)
{
	if (sk->sk_protocol == IPPROTO_UDP &&
#if IS_ENABLED(CONFIG_IPV6)
		(inet_sk(sk)->inet_rcv_saddr || (!ipv6_addr_any(&sk->sk_v6_rcv_saddr)))
#else
		(inet_sk(sk)->inet_rcv_saddr)
#endif
		) {
		if (sk->sk_prot && sk->sk_prot->rehash) {
			/*lint -e666*/
			smartcom_logd("UDP sock is already binded by user, rehash sk sk[%pK] sk_userlocks[%#x] "
				"dev_if[%d] inet_num[%d] hash[%d] inet_rcv_saddr: "IPV4_FMT" inet_saddr: "IPV4_FMT,
				sk, sk->sk_userlocks, sk->sk_bound_dev_if, inet_sk(sk)->inet_num, udp_sk(sk)->udp_portaddr_hash,
				ipv4_info(inet_sk(sk)->inet_rcv_saddr), ipv4_info(inet_sk(sk)->inet_saddr));
			inet_sk(sk)->inet_rcv_saddr = 0;
			inet_sk(sk)->inet_saddr = 0;
			/*lint +e666*/

#if IS_ENABLED(CONFIG_IPV6)
			smartcom_logd("sk_v6_rcv_saddr: "IPV6_FMT, ipv6_info(sk->sk_v6_rcv_saddr));
			sk->sk_v6_rcv_saddr = in6addr_any;
#endif
			sk->sk_userlocks &= ~SOCK_BINDADDR_LOCK;
			sk->sk_prot->rehash(sk);
		} else {
			smartcom_logd("rehash error");
			return false;
		}
	}
	return true;
}

static int8_t smartcom_mpflow_ai_finduid(uid_t uid)
{
	int8_t index;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_APP; index++) {
		if (g_mpflow_ai_uids[index].uid == uid)
			return index;
	}
	return INDEX_INVALID;
}

static int8_t smartcom_mpflow_ai_get_free_uid_index(void)
{
	int8_t index;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_APP; index++) {
		if (g_mpflow_ai_uids[index].uid == UID_INVALID_APP)
			return index;
	}
	return INDEX_INVALID;
}

static uint16_t smartcom_transfer_bindmode_to_binddev(uint16_t bind_mode)
{
	uint16_t bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
	uint16_t bind_index;
	for (bind_index = SMARTCOM_MPFLOW_BIND_WIFI0; bind_index < SMARTCOM_MPFLOW_BIND_MAX; ++bind_index) {
		if (bind_mode == (1 << (bind_index - 1))) {
			bind_dev = bind_index;
			break;
		}
	}

	return bind_dev;
}

static bool smartcom_mpflow_ai_is_excluded_port(const struct smartcom_mpflow_ai_info *app,
	uint16_t dport, uint8_t proto)
{
	unsigned int i;
	if (proto == IPPROTO_TCP) {
		for (i = 0; i < SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE; i++) {
			if (app->excluded_tcp_ports[i] != 0 &&
				app->excluded_tcp_ports[i] == (int32_t)dport)
				return true;
		}
	}
	return false;
}

static bool smartcom_mpflow_ai_check_excluded_port(
				const struct smartcom_mpflow_ai_info *app, struct sock *sk)
{
	if (!isvalid_mpflow_bind_flag(sk) && sk->sk_bound_dev_if)
		return true;
	return smartcom_mpflow_ai_is_excluded_port(app, ntohs(sk->sk_dport), IPPROTO_TCP);
}

static bool smartcom_is_v6_sock(struct sock *sk)
{
	if (sk->sk_family == AF_INET6 &&
		!(ipv6_addr_type(&sk->sk_v6_daddr) & IPV6_ADDR_MAPPED))
		return true;
	return false;
}

static bool smartcom_transfer_sk_to_addr(struct sock *sk, struct sockaddr *addr)
{
	struct sockaddr_in *usin_tmp = (struct sockaddr_in *)addr;
	if (sk->sk_family == AF_INET6) {
		struct sockaddr_in6 *usin6_tmp = (struct sockaddr_in6 *)addr;
		usin_tmp->sin_family = AF_INET6;
		usin6_tmp->sin6_addr = sk->sk_v6_daddr;
		return true;
	} else if (sk->sk_family == AF_INET) {
		usin_tmp->sin_family = AF_INET;
		usin_tmp->sin_addr.s_addr = sk->sk_daddr;
		return true;
	}

	return false;
}

static int8_t smartcom_mpflow_ai_get_port_index_in_range(struct smartcom_mpflow_ai_info *app_info,
	uint16_t dport, uint8_t proto)
{
	struct smartcom_mpflow_dport_range *exist = NULL;
	int8_t i;
	int8_t port_index = SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;

	if (app_info->port_num == 0)
		return SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;

	for (i = 0; i < app_info->port_num; i++) {
		exist = &app_info->port_bind_policies[i].range;
		if ((exist->start_port <= dport) && (dport <= exist->end_port) &&
			(app_info->port_bind_policies[i].protocol == proto)) {
			port_index = i;
			break;
		}
	}
	return port_index;
}

static void smartcom_mpflow_clear_blocked(uid_t uid)
{
	int8_t index;
	struct smartcom_mpflow_stat *tmp = NULL;
	errno_t err;
	struct smartcom_mpflow_stat *mpflow_list = g_mpflow_fail_stat_list;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_LIST_NUM; index++) {
		tmp = &mpflow_list[index];
		if (tmp->uid == uid) {
			tmp->mpflow_fallback = SMARTCOM_MPFLOW_FALLBACK_CLR;
			tmp->mpflow_fail_nopayload = 0;
			tmp->mpflow_fail_syn_rst = 0;
			tmp->mpflow_fail_syn_timeout = 0;
			tmp->start_jiffies = 0;
			err = memset_s(tmp->retrans_count, sizeof(tmp->retrans_count),
						   0, sizeof(tmp->retrans_count));
			if (err != EOK)
				smartcom_logd("smartcom_mpflow_clear_blocked memset failed");
		}
	}
}

bool smartcom_get_network_iface_name(int net_type, char *ifname, int len)
{
	unsigned int iface_len;
	int ret = 0;

	if (net_type < 0 || net_type >= SMARTCOM_NET_MAX_NUM) {
		smartcom_loge("net_type is invalid: %d", net_type);
		return false;
	}
	spin_lock_bh(&g_mpflow_network_infos_cache_lock);
	iface_len = strlen(g_mpflow_network_infos[net_type].iface_name);
	if (iface_len == 0 || iface_len >= IFNAMSIZ) {
		spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
		return false;
	}
	ret = memcpy_s(ifname, len, g_mpflow_network_infos[net_type].iface_name, iface_len);
	spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
	return ret == EOK;
}

static uint8_t smartcom_transfer_ifname_to_binddev(const char* ifname)
{
	uint8_t index;
	for (index = SMARTCOM_NET_WLAN0; index < SMARTCOM_NET_MAX_NUM; ++index) {
		char cur_ifname[IFNAMSIZ] = {0};
		if (!smartcom_get_network_iface_name(index, cur_ifname, IFNAMSIZ - 1))
			continue;

		if (strncmp(ifname, cur_ifname, strlen(cur_ifname)) == 0)
			return index + 1;
	}

	smartcom_logd("ifname:%s is wrong!", ifname);
	return SMARTCOM_MPFLOW_BIND_NONE;
}

static void smartcom_mpflow_ai_clear_blocked(uid_t uid, uint16_t net_bit)
{
	int8_t index;
	struct smartcom_mpflow_stat *tmp = NULL;
	errno_t err;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_LIST_NUM; index++) {
		tmp = &g_mpflow_fail_stat_list[index];
		if (tmp->uid == uid && tmp->mpflow_fallback != SMARTCOM_MPFLOW_FALLBACK_CLR) {
			uint8_t bind_device = smartcom_transfer_ifname_to_binddev(tmp->name);
			if (bind_device == SMARTCOM_MPFLOW_BIND_NONE) {
				continue;
			}
			if (((net_bit >> bind_device) & 0x1) != 0x1) {
				continue;
			}
			if (time_before_eq(jiffies, tmp->report_jiffies + SMARTCOM_MPFLOW_FALLBACK_WAIT_TIME * HZ)) {
				continue;
			}
			tmp->mpflow_fallback = SMARTCOM_MPFLOW_FALLBACK_CLR;
			tmp->mpflow_fail_nopayload = 0;
			tmp->mpflow_fail_syn_rst = 0;
			tmp->mpflow_fail_syn_timeout = 0;
			tmp->start_jiffies = 0;
			err = memset_s(tmp->retrans_count, sizeof(tmp->retrans_count), 0, sizeof(tmp->retrans_count));
			if (err != EOK)
				smartcom_logd("smartcom_mpflow_clear_blocked memset failed");
		}
	}
}

static void smartcom_mpflow_delete(uid_t uid)
{
	int8_t index;
	struct smartcom_mpflow_stat *node = NULL;
	struct smartcom_mpflow_stat *mpflow_list = g_mpflow_fail_stat_list;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_LIST_NUM; index++) {
		node = &mpflow_list[index];
		if (node->uid == uid)
			node->uid = UID_INVALID_APP;
	}
}

static struct smartcom_mpflow_ai_info* smartcom_mpflow_ai_start(uid_t uid)
{
	struct smartcom_mpflow_ai_info *app = NULL;
	int8_t index;
	int8_t newindex;
	bool is_new_uid = false;

	smartcom_logd("uid: %u", uid);

	index = smartcom_mpflow_ai_finduid(uid);
	if (index == INDEX_INVALID) {
		smartcom_logd("new app uid: %d", uid);
		newindex = smartcom_mpflow_ai_get_free_uid_index();
		if (newindex == INDEX_INVALID) {
			smartcom_loge("get free index exceed. uid: %d", uid);
			return NULL;
		}
		index = newindex;
		is_new_uid = true;
	}
	app = &g_mpflow_ai_uids[index]; /*lint !e676*/

	if (is_new_uid) {
		smartcom_mpflow_clear_blocked(uid);
		app->uid = uid;
		app->port_num = 0;
		app->rst_bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
		app->rst_duration = SMARTCOM_MPFLOW_AI_RESET_DURATION;
		app->rst_jiffies = 0;
		app->rst_all_flow_bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
		app->rst_all_flow_duration = SMARTCOM_MPFLOW_AI_RESET_DURATION;
		app->rst_all_flow_jiffies = 0;
		app->rst_all_flow = false;
	}

	return app;
}

static void smartcom_mpflow_ai_app_clear(int8_t index, uid_t uid)
{
	g_mpflow_ai_uids[index].uid = 0;
	g_mpflow_ai_uids[index].port_num = 0;
	g_mpflow_ai_uids[index].rst_bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
	g_mpflow_ai_uids[index].rst_jiffies = 0;
}

static bool smartcom_mpflow_ai_check_bind_config_valid(struct smartcom_mpflow_ai_init_bind_cfg *config)
{
	if (config->policy_num > SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE) {
		smartcom_loge("too many policy, %u", config->policy_num);
		return false;
	}

	struct smartcom_mpflow_ai_init_bind_policy *burst_policy = &config->burst_bind;
	if (burst_policy->port_num > SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE) {
		smartcom_loge("too many port: %d", burst_policy->port_num);
		return false;
	}

	if ((burst_policy->l4_protocol != IPPROTO_TCP) && (burst_policy->l4_protocol != IPPROTO_UDP)) {
		smartcom_loge("burst: invalid protocol: %d", burst_policy->l4_protocol);
		return false;
	}

	if (burst_policy->divide_mode > SMARTCOM_MPFLOW_IP_BURST_FIX) {
		smartcom_loge("burst: invalid divide_mode: %d", burst_policy->divide_mode);
		return false;
	}

	smartcom_logi("policy num: %u, burst: proto[%u] divide mode[%u] portnum[%u]",
		config->policy_num, burst_policy->l4_protocol, burst_policy->divide_mode, burst_policy->port_num);
	return true;
}

static void smartcom_mpflow_ai_cpy_ratio(const uint8_t *src_ratio, uint8_t *dst_ratio)
{
	uint32_t ratio_index;
	for (ratio_index = SMARTCOM_NET_WLAN0; ratio_index < SMARTCOM_NET_MAX_NUM; ++ratio_index)
		dst_ratio[ratio_index] = src_ratio[ratio_index];
}

static void smartcom_mpflow_ai_set_burst_info_from_burst_policy(struct smartcom_mpflow_ai_info *app,
				struct smartcom_mpflow_ai_init_bind_policy *burst_policy)
{
	uint32_t i, j;

	app->burst_info.burst_port_num = 0;
	app->burst_info.burst_protocol = burst_policy->l4_protocol;
	app->burst_info.burst_divide_mode = burst_policy->divide_mode;
	smartcom_mpflow_ai_cpy_ratio(burst_policy->ratio, app->burst_info.burst_ratio);
	j = 0;
	for (i = 0; i < burst_policy->port_num; i++) {
		smartcom_logi("burst: port range[%d, %d]", burst_policy->port_range[i].start_port,
			burst_policy->port_range[i].end_port);

		if (burst_policy->port_range[i].start_port > burst_policy->port_range[i].end_port) {
			smartcom_loge("burst: invalid port range");
			continue;
		}
		app->burst_info.burst_ports[j].start_port = burst_policy->port_range[i].start_port;
		app->burst_info.burst_ports[j].end_port = burst_policy->port_range[i].end_port;
		j++;
	}

	app->burst_info.burst_port_num = j;
	app->burst_info.launch_bind_device = SMARTCOM_MPFLOW_BIND_WIFI0;
	app->burst_info.burst_cnt = 0;
	app->burst_info.jiffies = 0;
}

static void smartcom_mpflow_ai_policy_init(
	const struct smartcom_mpflow_ai_init_bind_cfg *config,
	struct smartcom_mpflow_ai_info *app)
{
	const struct smartcom_mpflow_ai_init_bind_policy *policy = NULL;
	uint32_t i;
	uint32_t index;

	for (index = 0; index < config->policy_num; index++) {
		policy = &config->scatter_bind[index];

		if ((policy->l4_protocol != IPPROTO_TCP) && (policy->l4_protocol != IPPROTO_UDP)) {
			smartcom_loge("invalid protocol: %d", policy->l4_protocol);
			continue;
		}
		if (policy->divide_mode > SMARTCOM_MPFLOW_IP_BURST_FIX) {
			smartcom_loge("invalid divide_mode: %d", policy->divide_mode);
			continue;
		}

		for (i = 0; (i < policy->port_num) && (i < SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE); i++) {
			smartcom_logi("port[%d, %d]", policy->port_range[i].start_port,
				policy->port_range[i].end_port);

			if (policy->port_range[i].start_port > policy->port_range[i].end_port) {
				smartcom_loge("error port");
				continue;
			}
			app->port_bind_policies[app->port_num].protocol = policy->l4_protocol;
			app->port_bind_policies[app->port_num].range.start_port = policy->port_range[i].start_port;
			app->port_bind_policies[app->port_num].range.end_port = policy->port_range[i].end_port;

			/* reserved for future logic */
			app->port_bind_policies[app->port_num].pattern.select_mode = policy->divide_mode;
			smartcom_mpflow_ai_cpy_ratio(policy->ratio, app->port_bind_policies[app->port_num].pattern.ratio);
			/* reserved for future logic end */

			app->port_bind_policies[app->port_num].bind_mode = SMARTCOM_MPFLOW_BIND_NONE;
			app->port_num++;
			if (app->port_num >= SMARTCOM_MPFLOW_BIND_PORT_SIZE) {
				smartcom_loge("too many port: %d", app->port_num);
				return;
			}
		}
	}
}

static void smartcom_mpflow_ai_reset_app_port_bind_mode(void)
{
	int app_index;
	int port_index;

	spin_lock_bh(&g_mpflow_ai_lock);
	for (app_index = 0; app_index < SMARTCOM_MPFLOW_AI_MAX_APP; app_index++) {
		if (g_mpflow_ai_uids[app_index].uid != UID_INVALID_APP) {
			for (port_index = 0; port_index < SMARTCOM_MPFLOW_BIND_PORT_SIZE; port_index++)
				g_mpflow_ai_uids[app_index].port_bind_policies[port_index].bind_mode = SMARTCOM_MPFLOW_BIND_NONE;
		}
	}
	spin_unlock_bh(&g_mpflow_ai_lock);
}

static void smartcom_mpflow_update_network_info(const uint8_t *data, uint16_t len)
{
	struct smarmcom_mpflow_network_info *net_info = NULL;
	errno_t err;
	bool is_default_type_changed = false;

	if (data == NULL || (len != sizeof(struct smarmcom_mpflow_network_info))) {
		smartcom_loge("data or length %u is invalid", len);
		return;
	}

	net_info = (struct smarmcom_mpflow_network_info *)data;
	smartcom_logi("net type:%d, net id:%d, validated:%d, default:%d, iface: %s", net_info->net_type, net_info->net_id,
		net_info->is_validated, net_info->is_default, net_info->iface_name);
	if (!smartcom_is_net_type_valid(net_info->net_type))
		return;

	spin_lock_bh(&g_mpflow_network_infos_cache_lock);
	err = memcpy_s(&(g_mpflow_network_infos[net_info->net_type]),
		sizeof(struct smarmcom_mpflow_network_info), net_info, len);
	if (net_info->is_default && net_info->net_type != g_last_default_net_type) {
		smartcom_logi("default type changed, last default type: %d", g_last_default_net_type);
		g_last_default_net_type = net_info->net_type;
		is_default_type_changed = true;
	}
	spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
	if (is_default_type_changed)
		smartcom_mpflow_ai_reset_app_port_bind_mode();
	if (err != EOK)
		smartcom_loge("copy network info fail");
}

static void smartcom_mpflow_start(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_ai_init_bind_cfg *config = NULL;
	struct smartcom_mpflow_ai_info *app = NULL;
	uint32_t i;

	if (!data || (len != sizeof(struct smartcom_mpflow_ai_init_bind_cfg))) {
		smartcom_loge("input len error expect: %zu, real: %u", sizeof(struct smartcom_mpflow_ai_init_bind_cfg), len);
		return;
	}

	config = (struct smartcom_mpflow_ai_init_bind_cfg *)data;
	if (!smartcom_mpflow_ai_check_bind_config_valid(config))
		return;

	spin_lock_bh(&g_mpflow_ai_lock);
	app = smartcom_mpflow_ai_start(config->uid);
	if (app == NULL) {
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}

	smartcom_mpflow_ai_set_burst_info_from_burst_policy(app, &config->burst_bind);
	app->port_num = 0;
	smartcom_mpflow_ai_policy_init(config, app);
	for (i = 0; i < SMARTCOM_MPFLOW_BIND_PORT_CFG_SIZE; i++)
		app->excluded_tcp_ports[i] = config->excluded_tcp_ports[i];
	spin_unlock_bh(&g_mpflow_ai_lock);
}

static void smartcom_mpflow_stop(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_parse_stop_info *stop = NULL;
	int8_t index;
	int32_t stop_reason;

	if (!data || (len != sizeof(struct smartcom_mpflow_parse_stop_info))) {
		smartcom_loge("data or length %d is error", len);
		return;
	}

	stop = (struct smartcom_mpflow_parse_stop_info *)data;
	stop_reason = stop->stop_reason;
	smartcom_logd("uid: %u, stop reason: %u", stop->uid, stop_reason);

	spin_lock_bh(&g_mpflow_ai_lock);
	index = smartcom_mpflow_ai_finduid(stop->uid);
	if (index != INDEX_INVALID)
		smartcom_mpflow_ai_app_clear(index, stop->uid);

	smartcom_mpflow_delete(stop->uid);
	spin_unlock_bh(&g_mpflow_ai_lock);
}

static int8_t smartcom_mpflow_ai_get_port_index(int8_t index,
	const struct smartcom_mpflow_dport_range *range, uint8_t proto)
{
	struct smartcom_mpflow_ai_info *app_info = &g_mpflow_ai_uids[index];
	struct smartcom_mpflow_dport_range *exist = NULL;
	int8_t i;
	int8_t port_index = SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;

	if (app_info->port_num == 0)
		return SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;

	for (i = 0; i < app_info->port_num; i++) {
		exist = &app_info->port_bind_policies[i].range;
		if ((range->start_port == exist->start_port) && (range->end_port == exist->end_port)
			&& (proto == app_info->port_bind_policies[i].protocol)) {
			port_index = i;
			break;
		}
	}
	return port_index;
}

static void smartcom_mpflow_update_port_bind_config(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_port_bind_cfg *config = NULL;
	struct smartcom_mpflow_ai_info *app = NULL;
	int8_t app_index;
	int8_t port_index;
	uint16_t config_high_bit;
	uint16_t app_high_bit;

	if (!data || (len != sizeof(struct smartcom_mpflow_port_bind_cfg))) {
		smartcom_loge("invalid data length %u expect: %zu", len,
			sizeof(struct smartcom_mpflow_port_bind_cfg));
		return;
	}

	config = (struct smartcom_mpflow_port_bind_cfg *)data;
	if (config->port_range.start_port > config->port_range.end_port) {
		smartcom_loge("invalid port range[%d, %d]",
			config->port_range.start_port, config->port_range.end_port);
		return;
	}

	spin_lock_bh(&g_mpflow_ai_lock);

	app_index = smartcom_mpflow_ai_finduid(config->uid);
	if (app_index == INDEX_INVALID) {
		smartcom_loge("get app fail, uid: %d", config->uid);
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}
	app = &g_mpflow_ai_uids[app_index];

	port_index = smartcom_mpflow_ai_get_port_index(app_index, &config->port_range, config->l4protocol);
	if (port_index == SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX) {
		smartcom_loge("port range[%d, %d] not exist",
			config->port_range.start_port, config->port_range.end_port);
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}

	config_high_bit = (config->bind_mode >> SMARTCOM_MPFLOW_BINDMODE_SHIFT) & SMARTCOM_MPFLOW_BINDMODE_MASK;
	app_high_bit = (app->port_bind_policies[port_index].bind_mode >> SMARTCOM_MPFLOW_BINDMODE_SHIFT) &
		SMARTCOM_MPFLOW_BINDMODE_MASK;
	if (config->bind_mode != app->port_bind_policies[port_index].bind_mode) {
		if (config_high_bit != app_high_bit)
			smartcom_mpflow_ai_clear_blocked(config->uid, (config_high_bit ^ app_high_bit) & config_high_bit);
		smartcom_logi("uid:%u policy port[%d, %d] bind mode: %#x",
			config->uid, config->port_range.start_port, config->port_range.end_port, config->bind_mode);
	}
	app->port_bind_policies[port_index].bind_mode = config->bind_mode;

	spin_unlock_bh(&g_mpflow_ai_lock);
}

static void smartcom_mpflow_change_burst_ratio(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_burst_ratio *new_burst_ratio = NULL;
	uint32_t uid;
	int8_t app_index;
	struct smartcom_mpflow_ai_info *app = NULL;

	if ((data == NULL) || (len < sizeof(struct smartcom_mpflow_burst_ratio))) {
		smartcom_loge("change burst ratio pointer null or length %u error", len);
		return;
	}
	new_burst_ratio = (struct smartcom_mpflow_burst_ratio *)data;
	uid = new_burst_ratio->uid;

	smartcom_logd("change burst ratio, uid:%u", uid);

	spin_lock_bh(&g_mpflow_ai_lock);
	app_index = smartcom_mpflow_ai_finduid(uid);
	if (app_index == INDEX_INVALID) {
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}
	app = &g_mpflow_ai_uids[app_index];
	smartcom_mpflow_ai_cpy_ratio(new_burst_ratio->ratio, app->burst_info.burst_ratio);

	spin_unlock_bh(&g_mpflow_ai_lock);
}

static bool smartcom_mpflow_ai_get_addr_port(struct sockaddr *addr, __be32 *s_addr, uint16_t *port)
{
	uint8_t i;

	if (addr->sa_family == AF_INET) {
		struct sockaddr_in *usin = (struct sockaddr_in *)addr;
		*(s_addr + SMARTCOM_CLAT_IPV6 - 1) = usin->sin_addr.s_addr;
		*port = ntohs(usin->sin_port);
		return true;
#if IS_ENABLED(CONFIG_IPV6)
	} else if (addr->sa_family == AF_INET6) {
		struct sockaddr_in6 *usin6 = (struct sockaddr_in6 *)addr;

		for (i = 0; i < SMARTCOM_CLAT_IPV6; i++)
			*(s_addr + i) = usin6->sin6_addr.s6_addr32[i];

		*port = ntohs(usin6->sin6_port);
		return true;
#endif
	} else {
		smartcom_loge("sa_family error, sa_family: %hu", addr->sa_family);
		return false;
	}
}

static bool smartcom_mpflow_ai_checkvalid(struct sock *sk, struct sockaddr *uaddr,
	struct smartcom_mpflow_ai_info *app, uint16_t dport, uint8_t *output_port_index)
{
	struct sockaddr_in *usin = (struct sockaddr_in *)uaddr;
	bool isvalidaddr = false;
	uint8_t port_index = SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;

	if (!sk || !uaddr)
		return false;

	if (smartcom_mpflow_ai_is_excluded_port(app, dport, sk->sk_protocol))
		return false;

	isvalidaddr = smartcom_check_ip_addrss(uaddr) && (!smartcom_check_ip_is_private(uaddr));
	if (isvalidaddr == false) {
		smartcom_logd("invalid addr. uid:%u, protocol:%u, dev_if:%u", app->uid, sk->sk_protocol, sk->sk_bound_dev_if);
		return false;
	}

	if (usin->sin_family != AF_INET && usin->sin_family != AF_INET6) {
		smartcom_logd("unsupport family uid: %u, sin_family: %d",
					app->uid, usin->sin_family);
		return false;
	}

	if (time_after(jiffies, app->rst_all_flow_jiffies + app->rst_all_flow_duration)) {
		app->rst_all_flow_bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
		app->rst_all_flow = false;
	} else if (app->rst_all_flow) {
		smartcom_logd("reset all flow");
		return true;
	}

	port_index = smartcom_mpflow_ai_get_port_index_in_range(app, dport, sk->sk_protocol);
	if (port_index == SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX) {
		smartcom_logd("port not in range uid: %u, dport: %d",
				app->uid, dport);
		return false;
	}

	if (sk->sk_protocol != app->port_bind_policies[port_index].protocol) {
		smartcom_logd("protocol not match uid: %u, sk: %pK", app->uid, sk);
		return false;
	}

	smartcom_logd("valid uid: %u sk: %pK, famliy: %d, proto: %d, dport: %d. "
		"policy_proto: %d, bindmode: %#x",
		app->uid, sk, sk->sk_family, sk->sk_protocol, dport,
		app->port_bind_policies[port_index].protocol, app->port_bind_policies[port_index].bind_mode);

	*output_port_index = port_index;
	return true;
}

static bool smartcom_mpflow_blocked(uid_t uid, const char *name)
{
	int8_t index;
	struct smartcom_mpflow_stat *node = NULL;
	struct smartcom_mpflow_stat *mpflow_list = g_mpflow_fail_stat_list;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_LIST_NUM; index++) {
		node = &mpflow_list[index];
		if ((node->uid == uid) && (!strncmp(node->name, name, strlen(name))))
			return (node->mpflow_fallback == SMARTCOM_MPFLOW_FALLBACK_SET);
	}

	return false;
}

static void smartcom_mpflow_init_blocked_array(uid_t uid, bool *blocked_array)
{
	uint32_t net_type;
	for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; net_type++) {
		char cur_ifname[IFNAMSIZ] = {0};
		if (!smartcom_get_network_iface_name(net_type, cur_ifname, IFNAMSIZ - 1))
			continue;

		blocked_array[net_type] = smartcom_mpflow_blocked(uid, cur_ifname);
	}
}

static bool smartcom_mpflow_check_all_blocked(bool *blocked_array)
{
	uint32_t net_type;
	for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; net_type++) {
		if (!blocked_array[net_type])
			return false;
	}
	return true;
}

static bool smartcom_mpflow_is_bind_dev_block(const int bind_device, const bool *blocked_array)
{
	bool res = true;

	if (smartcom_mpflow_is_bind_dev_valid(bind_device))
		res = blocked_array[bind_device - 1];

	return res;
}

static int smartcom_mpflow_ai_bind_random(struct smartcom_mpflow_ai_bind_pattern *calc,
	uid_t uid, uint16_t high_bit)
{
	uint16_t bind_device = calc->last_bind_device;
	int cnt = 0;
	bool blocked_array[SMARTCOM_NET_MAX_NUM] = {0};
	smartcom_mpflow_init_blocked_array(uid, blocked_array);
	if (smartcom_mpflow_check_all_blocked(blocked_array)) {
		calc->last_bind_device = SMARTCOM_MPFLOW_BIND_NONE;
		return SMARTCOM_MPFLOW_BIND_NONE;
	}

	do {
		// find no suitable device, then break
		if (++cnt > SMARTCOM_MPFLOW_DEV_NUM) {
			smartcom_loge("can't find bind_device");
			break;
		}
		if ((bind_device + 1) >= SMARTCOM_MPFLOW_BIND_MAX) {
			bind_device = SMARTCOM_MPFLOW_BIND_WIFI0;
		} else {
			bind_device++;
		}
	} while ((((high_bit >> (bind_device - 1)) & 0x1) == 0x0) ||
	smartcom_mpflow_is_bind_dev_block(bind_device, blocked_array));

	smartcom_logi("bind random to bind_device: %u", bind_device);
	calc->last_bind_device = bind_device;

	return bind_device;
}

static bool smartcom_mpflow_ai_bind_block(uid_t uid, uint16_t bind_device)
{
	uint16_t net_type;
	for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; ++net_type) {
		char cur_ifname[IFNAMSIZ] = {0};
		if (!smartcom_get_network_iface_name(net_type, cur_ifname, IFNAMSIZ - 1))
			continue;

		if (!smartcom_mpflow_blocked(uid, cur_ifname))
			return false;

		if (bind_device == (net_type + 1)) {
			smartcom_logi("mpflow bind blocked uid: %u, bind_device: %u", uid, bind_device);
			return true;
		}
	}
	smartcom_logi("mpflow bind all blocked uid: %u, bind_device: %u", uid, bind_device);

	return true;
}

static bool smartcom_mpflow_ai_in_busrt_range(struct smartcom_mpflow_ai_info *app_info,
	uint16_t dport, uint8_t proto)
{
	struct smartcom_mpflow_dport_range *tmp = NULL;
	int8_t i;

	if (app_info->burst_info.burst_protocol == 0 || app_info->burst_info.burst_port_num == 0)
		return true;

	if (app_info->burst_info.burst_protocol != proto)
		return false;

	for (i = 0; i < app_info->burst_info.burst_port_num; i++) {
		tmp = &app_info->burst_info.burst_ports[i];
		if ((tmp->start_port <= dport) && (dport <= tmp->end_port))
			return true;
	}
	return false;
}

static void smartcom_mpflow_ai_get_ratio(struct smartcom_mpflow_ai_burst_port *burst_info,
	uint8_t* cur_burst_ratio, uint16_t high_bit)
{
	uint32_t ratio_index;
	for (ratio_index = SMARTCOM_NET_WLAN0; ratio_index < SMARTCOM_NET_MAX_NUM; ++ratio_index) {
		if (((high_bit >> ratio_index) & 0x1) == 0x0) {
			cur_burst_ratio[ratio_index] = 0;
		} else {
			cur_burst_ratio[ratio_index] = burst_info->burst_ratio[ratio_index];
		}
	}
}

static bool smartcom_mpflow_ai_is_ratio_on(uint8_t* cur_burst_ratio)
{
	uint32_t ratio_index;
	for (ratio_index = SMARTCOM_NET_WLAN0; ratio_index < SMARTCOM_NET_MAX_NUM; ++ratio_index) {
		if (cur_burst_ratio[ratio_index] != 0)
			return true;
	}
	return false;
}

static bool smartcom_mpflow_ai_check_burst_info_valid(struct smartcom_mpflow_ai_burst_port *burst_info,
				uint8_t *cur_burst_ratio, uint16_t bind_device)
{
	if (burst_info->burst_divide_mode != SMARTCOM_MPFLOW_IP_BURST_FIX)
		return false;

	if (!smartcom_mpflow_ai_is_ratio_on(cur_burst_ratio))
		return false;

	if (burst_info->launch_bind_device != bind_device) {
		burst_info->burst_cnt = 0;
		burst_info->jiffies = jiffies;
		burst_info->launch_bind_device = bind_device;
		return false;
	}
	return true;
}

static uint8_t smartcom_mpflow_ai_get_total_ratio(uint8_t *cur_burst_ratio)
{
	uint8_t total = 0;
	uint32_t ratio_index;
	for (ratio_index = SMARTCOM_NET_WLAN0; ratio_index < SMARTCOM_NET_MAX_NUM; ++ratio_index)
		total += cur_burst_ratio[ratio_index];

	return total;
}

static int smartcom_mpflow_ai_bind_burst(struct smartcom_mpflow_ai_info *app, uint16_t dport, uint8_t proto,
	uint16_t bind_device, uint16_t device_list_bits)
{
	uint16_t launch_bind_device;
	uint8_t launch_cnt;
	uint8_t cur_pos;

	if (bind_device == SMARTCOM_MPFLOW_BIND_NONE || smartcom_mpflow_ai_bind_block(app->uid, bind_device))
		return SMARTCOM_MPFLOW_BIND_NONE;

	if (!smartcom_mpflow_ai_in_busrt_range(app, dport, proto))
		return bind_device;

	int burst_bind_device;
	uint8_t tar_burst_ratio[SMARTCOM_NET_MAX_NUM];
	struct smartcom_mpflow_ai_burst_port *burst_info = &app->burst_info;
	smartcom_mpflow_ai_get_ratio(burst_info, tar_burst_ratio, device_list_bits);
	smartcom_logi("bind_device: %u, last launch_bind_device: %u, last burst cnt: %lu, burst_divide_mode: %u",
		bind_device, burst_info->launch_bind_device, burst_info->burst_cnt, burst_info->burst_divide_mode);

	if (!smartcom_mpflow_ai_check_burst_info_valid(burst_info, tar_burst_ratio, bind_device)) {
		smartcom_logi("check burst info invalid, (new) launch_bind_device: %u, return bind_device:%u",
			burst_info->launch_bind_device, bind_device);
		return bind_device;
	}

	/*lint -e666*/
	if (time_after(jiffies, (burst_info->jiffies + SMARTCOM_MPFLOW_FLOW_BIND_BURST_TIME)) ||
		(burst_info->jiffies == 0)) {
		burst_bind_device = bind_device;
		burst_info->burst_cnt = 0;
		burst_info->jiffies = jiffies;
		burst_info->launch_bind_device = burst_bind_device; // update launch device every burst period begining
		smartcom_logi("burst period elap, new burst_bind_device: %u, new launch_bind_device: %u, reset burst cnt: 0",
			burst_bind_device, burst_info->launch_bind_device);
	} else {
		burst_info->burst_cnt++;
		// base device
		launch_bind_device = burst_info->launch_bind_device;
		launch_cnt = burst_info->burst_cnt % (smartcom_mpflow_ai_get_total_ratio(tar_burst_ratio));
		// last device postion, bindmode - 1
		cur_pos = launch_bind_device - 1;
		// find this burst device
		while (launch_cnt >= tar_burst_ratio[cur_pos]) {
			launch_cnt = launch_cnt - tar_burst_ratio[cur_pos];
			if (cur_pos < (SMARTCOM_NET_MAX_NUM - 1)) {
				// jump to next device
				cur_pos++;
			} else {
				// need restart
				cur_pos = SMARTCOM_NET_WLAN0;
			}
		}
		burst_bind_device = cur_pos + 1;
		smartcom_logi("re calc, new burst_bind_device: %u, launch_bind_device: %u, burst cnt:%lu",
			burst_bind_device, burst_info->launch_bind_device, burst_info->burst_cnt);
	}
	/*lint +e666*/

	return burst_bind_device;
}

static uint32_t smartcom_mpflow_get_bind_dev(struct smartcom_mpflow_ai_info *app, struct sock *sk,
	struct sockaddr *uaddr)
{
	struct smartcom_mpflow_ai_bind_pattern *pattern_calc = NULL;
	__be32 daddr[SMARTCOM_CLAT_IPV6] = {0};
	uint16_t dport = 0;
	uint32_t bind_mode = 0;
	uint16_t init_bind_dev;
	uint32_t bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
	uint8_t port_index = SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX;
	uint16_t low_bit;
	uint16_t high_bit;

	if (!smartcom_mpflow_ai_get_addr_port(uaddr, daddr, &dport))
		return SMARTCOM_MPFLOW_BIND_NONE;
 
	if (!smartcom_mpflow_ai_checkvalid(sk, uaddr, app, dport, &port_index))
		return SMARTCOM_MPFLOW_BIND_NONE;
 
	/* apply all flow reset first */
	if (app->rst_all_flow)
		return smartcom_mpflow_ai_get_reset_bind_device(app->rst_all_flow_bind_dev);

	/* apply reset mode second */
	if (time_after(jiffies, app->rst_jiffies + app->rst_duration))
		app->rst_bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
	else
		return smartcom_mpflow_ai_get_reset_bind_device(app->rst_bind_dev);

	if (port_index >= SMARTCOM_MPFLOW_BIND_INVALID_PORT_INDEX)
		return SMARTCOM_MPFLOW_BIND_NONE;
	
	bind_mode = app->port_bind_policies[port_index].bind_mode;
	pattern_calc = &app->port_bind_policies[port_index].pattern;

	low_bit = bind_mode & SMARTCOM_MPFLOW_BINDMODE_MASK;
	high_bit = (bind_mode >> SMARTCOM_MPFLOW_BINDMODE_SHIFT) & SMARTCOM_MPFLOW_BINDMODE_MASK;
	init_bind_dev = smartcom_transfer_bindmode_to_binddev(low_bit);

	if (low_bit == SMARTCOM_MPFLOW_BINDMODE_MASK) {
		bind_dev = smartcom_mpflow_ai_bind_random(pattern_calc, app->uid, high_bit);
	} else if (low_bit == 0) {
		bind_dev = SMARTCOM_MPFLOW_BIND_NONE;
	} else {
		bind_dev = smartcom_mpflow_ai_bind_burst(app, dport, sk->sk_protocol, init_bind_dev, high_bit);
	}
	smartcom_logi("final bind_dev: %u, low bits: %#x, high bits: %#x, init_bind_dev:%u",
		bind_dev, low_bit, high_bit, init_bind_dev);
	return bind_dev;
}

static bool smartcom_mpflow_ai_get_update_iface_name_result(int bind_device, struct sock *sk, char *ifname, int len)
{
	struct inet_sock *inet_tmp = NULL;

	if (bind_device == SMARTCOM_MPFLOW_BIND_NONE) {
		/* 1.wzry sgame stuck one min when shutdown LTE, because UDP not create a new sock when receive a reset,
		the old sock sk_bound_dev_if is LTE, so the UDP sock will continue use LTE to send packet
		2.as a wifi hotpoint, can not bind wlan, need reset sk_bound_dev_if to 0 */
		if (sk->sk_bound_dev_if && sk->sk_protocol == IPPROTO_UDP) {
			/* Fix the bug for weixin app bind wifi ip, then reset to lte, cannot find the sk */
			if (!smartcom_mpflow_ai_rehash_sk(sk))
				return false;
			sk->sk_bound_dev_if = 0;
			sk_dst_reset(sk);
			inet_tmp = inet_sk(sk);
			if (inet_tmp->inet_saddr)
				inet_tmp->inet_saddr = 0;
		} else {
			sk->sk_bound_dev_if = 0;
		}
		return false;
	} else {
		return smartcom_get_network_iface_name(bind_device - SMARTCOM_MPFLOW_BIND_WIFI0, ifname, len);
	}
}

static void smartcom_mpflow_ai_path_handover(struct sock *sk, uint16_t bind_dev)
{
	char selected_path_iface[IFNAMSIZ] = {0};
	bool is_mp_flow_bind = isvalid_mpflow_bind_flag(sk);

	if (!smartcom_mpflow_ai_rehash_sk(sk))
		return;

	if (bind_dev == SMARTCOM_MPFLOW_BIND_NONE) {
		smartcom_logi("path_handover to default route, sk:%pK sport[%u] dev_if[%d] is_mp_flow_bind[%d] family[%u]",
			sk, sk->sk_num, sk->sk_bound_dev_if, is_mp_flow_bind, sk->sk_family);
		if (sk->sk_bound_dev_if) {
			struct inet_sock *inet = inet_sk(sk);
			sk->sk_bound_dev_if = 0;
			sk_dst_reset(sk);
			if (inet->inet_saddr)
				inet->inet_saddr = 0;
		}
		return;
	}

	if (!smartcom_mpflow_is_bind_dev_valid(bind_dev))
		return;

	if (!smartcom_get_network_iface_name(bind_dev - 1, selected_path_iface, IFNAMSIZ - 1))
		return;

	smartcom_logi("path_handover sk:%pK sport[%u] dev_if[%d] is_mp_flow_bind[%d] family[%u] selected_path_iface[%s]",
		sk, sk->sk_num, sk->sk_bound_dev_if,
		is_mp_flow_bind, sk->sk_family, selected_path_iface);
	smartcom_enable_selected_path(sk, selected_path_iface);
}

static void smartcom_mpflow_tcp_flow_reset(const struct smartcom_mpflow_ai_info *app,
	const struct smartcom_mpflow_reset_flow_policy_info *reset,
	bool *need_reset, int32_t err_num)
{
	struct sock *sk = NULL;
	bool is_mp_flow_bind = false;

	rcu_read_lock();
	if (reset->flow.l3proto == ETH_P_IP) {
		sk = inet_lookup_established(&init_net, &tcp_hashinfo, reset->flow.v4_dip, htons(reset->flow.dst_port),
			reset->flow.v4_sip, htons(reset->flow.src_port), reset->flow.sk_dev_itf);
	} else {
		sk = __inet6_lookup_established(&init_net, &tcp_hashinfo, &reset->flow.v6_dip, htons(reset->flow.dst_port),
			&reset->flow.v6_sip, reset->flow.src_port, reset->flow.sk_dev_itf, 0);
	}
	rcu_read_unlock();
	if (sk == NULL) {
		smartcom_loge("tcp reset flow not found");
		return;
	}

	if (smartcom_mpflow_ai_check_excluded_port(app, sk)) {
		sock_gen_put(sk);
		return;
	}

	is_mp_flow_bind = isvalid_mpflow_bind_flag(sk);
	smartcom_logi("reset sk:%pK sport[%u] state[%u] is_mp_flow_bind[%d] family[%u] protocol[%u] method[%u]",
		sk, sk->sk_num, sk->sk_state, is_mp_flow_bind, sk->sk_family, sk->sk_protocol, reset->policy.method);
	if (reset->policy.method == SK_ERROR) {
		smartcom_reset_tcp_skerror(sk, err_num);
		*need_reset = true;
		smartcom_logi("tcp reset completed");
	} else {
		smartcom_loge("tcp reset method not support, method: %u", reset->policy.method);
	}
	sock_gen_put(sk);
}

static void smartcom_mpflow_udp_flow_reset(const struct smartcom_mpflow_reset_flow_policy_info *reset,
	bool *need_reset, int32_t err_num)
{
	struct sock *sk = NULL;
	bool is_mp_flow_bind = false;

	if (reset->policy.method != SK_ERROR && reset->policy.method != INTF_CHANGE) {
		smartcom_loge("udp reset method not support, method: %u", reset->policy.method);
		return;
	}

	rcu_read_lock();
	if (reset->flow.l3proto == ETH_P_IP) {
		sk = udp4_lib_lookup(&init_net, reset->flow.v4_dip, htons(reset->flow.dst_port),
			reset->flow.v4_sip, htons(reset->flow.src_port), reset->flow.sk_dev_itf);
	} else {
		sk = udp6_lib_lookup(&init_net, &reset->flow.v6_dip, htons(reset->flow.dst_port),
			&reset->flow.v6_sip, htons(reset->flow.src_port), reset->flow.sk_dev_itf);
	}
	rcu_read_unlock();

	if (sk) {
		is_mp_flow_bind = isvalid_mpflow_bind_flag(sk);
		smartcom_logi("reset sk:%pK sport[%u] state[%u] is_mp_flow_bind[%d] family[%u] protocol[%u] method[%u]",
			sk, sk->sk_num, sk->sk_state, is_mp_flow_bind, sk->sk_family, sk->sk_protocol, reset->policy.method);
		if (sk->sk_protocol == IPPROTO_UDP) {
			if (reset->policy.method == SK_ERROR) {
				clear_mpflow_bind_flag(sk);
				smartcom_reset_udp_skerror(sk, err_num);
				*need_reset = true;
			} else if (reset->policy.method == INTF_CHANGE) {
				smartcom_mpflow_ai_path_handover(sk, reset->policy.rst_bind_dev_or_mode);
			}
			smartcom_logi("udp reset completed");
		}
		sock_gen_put(sk);
	} else {
		smartcom_loge("udp flow not found");
	}
}

static void smartcom_mpflow_ai_print_reset_policy_info(struct smartcom_mpflow_reset_flow_policy_info *reset)
{
	if (reset->flow.l3proto == ETH_P_IP) {
		smartcom_logi("SrcIP["IPV4_FMT"] SrcPort[%u] "
			"DstIP["IPV4_FMT"] DstPort[%u] l4proto[%u] l3proto[%u] intf[%u]",
			ipv4_info(reset->flow.v4_sip), reset->flow.src_port,
			ipv4_info(reset->flow.v4_dip), reset->flow.dst_port,
			reset->flow.l4proto, reset->flow.l3proto, reset->flow.sk_dev_itf);
	} else {
		smartcom_logi("SrcIP["IPV6_FMT"] SrcPort[%u] "
			"DstIP["IPV6_FMT"] DstPort[%u] l4proto[%u] l3proto[%u] intf[%u]",
			ipv6_info(reset->flow.v6_sip), reset->flow.src_port,
			ipv6_info(reset->flow.v6_dip), reset->flow.dst_port,
			reset->flow.l4proto, reset->flow.l3proto, reset->flow.sk_dev_itf);
	}
}

static void smartcom_mpflow_reset_flow(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_reset_flow_policy_info *reset = NULL;
	bool need_reset = false;
	int8_t app_index;
	struct smartcom_mpflow_ai_info *app = NULL;

	if ((data == NULL) || (len < sizeof(struct smartcom_mpflow_reset_flow_policy_info))) {
		smartcom_loge("mod flow pointer null or length %d error", len);
		return;
	}

	reset = (struct smartcom_mpflow_reset_flow_policy_info *)data;
	smartcom_mpflow_ai_print_reset_policy_info(reset);
	smartcom_logi("receive reset bind dev[%u] blinktime[%u]", reset->policy.rst_bind_dev_or_mode,
		reset->policy.blink_time);

	spin_lock_bh(&g_mpflow_ai_lock);
	app_index = smartcom_mpflow_ai_finduid(reset->uid);
	if (app_index == INDEX_INVALID) {
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}
	app = &g_mpflow_ai_uids[app_index];
	spin_unlock_bh(&g_mpflow_ai_lock);

	switch (reset->flow.l4proto) {
	case IPPROTO_TCP:
		smartcom_mpflow_tcp_flow_reset(app, reset, &need_reset, reset->policy.err_num);
		break;

	case IPPROTO_UDP:
		smartcom_mpflow_udp_flow_reset(reset, &need_reset, reset->policy.err_num);
		break;
	default:
		smartcom_logi("unsupport protocol: %d.\n", reset->flow.l4proto);
		break;
	}

	if (need_reset) {
		spin_lock_bh(&g_mpflow_ai_lock);
		app->rst_jiffies = jiffies;
		app->rst_bind_dev = reset->policy.rst_bind_dev_or_mode;
		app->rst_duration = msecs_to_jiffies(reset->policy.blink_time);
		spin_unlock_bh(&g_mpflow_ai_lock);
		smartcom_logi("Reset Completed ");
	}
}

int smartcom_get_net_type(int net_id)
{
	int net_type;
	if (net_id <= SMARTCOM_DEFAULT_NET_ID) {
		spin_lock_bh(&g_mpflow_network_infos_cache_lock);
		for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; net_type++) {
			if (g_mpflow_network_infos[net_type].is_default) {
				spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
				return net_type;
			}
		}
		spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
		return SMARTCOM_NET_INVALID;
	}
	spin_lock_bh(&g_mpflow_network_infos_cache_lock);
	for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; net_type++) {
		if (g_mpflow_network_infos[net_type].net_id == net_id) {
			spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
			return net_type;
		}
	}
	spin_unlock_bh(&g_mpflow_network_infos_cache_lock);
	return SMARTCOM_NET_INVALID;
}

static bool smartcom_mpflow_ai_check_loop_sk(struct sock *sk)
{
	struct sockaddr_in6 addr;
	if (smartcom_is_v6_sock(sk)) {
		if (ipv6_addr_any(&sk->sk_v6_daddr)) {
			return false;
		} else if (ipv6_addr_equal(&sk->sk_v6_daddr, &sk->sk_v6_rcv_saddr)) {
			smartcom_logi("ipv6 loop sk_v6_daddr:"IPV6_FMT, ipv6_info(sk->sk_v6_daddr));
			return true;
		}
	} else {
		if (sk->sk_daddr == 0) {
			/* If the UDP sk does not call connect(), sk_daddr is 0, in this case, we just return false. */
			return false;
		} else if (sk->sk_daddr == sk->sk_rcv_saddr) {
			smartcom_logi("ipv4 loop sk_daddr:"IPV4_FMT, ipv4_info(sk->sk_daddr));
			return true;
		}
	}

	if (!smartcom_transfer_sk_to_addr(sk, (struct sockaddr *)&addr)) {
		smartcom_logi("transfer sk to addr fail");
		return true;
	}
	if (!smartcom_check_ip_addrss((struct sockaddr *)&addr) ||
		smartcom_check_ip_is_private((struct sockaddr *)&addr)) {
		if (sk->sk_family == AF_INET6)
			smartcom_logi("ip is illegal"IPV6_FMT, ipv6_info(sk->sk_v6_daddr));
		else
			smartcom_logi("ip is illegal"IPV4_FMT, ipv4_info(sk->sk_daddr));
		return true;
	}

	return false;
}

static bool smartcom_get_sk_ifname(struct sock *sk, char *ifname, int len)
{
	int net_type;

	if (netdev_get_name(sock_net(sk), ifname, sk->sk_bound_dev_if) != 0) {
		if (sk->sk_bound_dev_if == SMARTCOM_MPFLOW_BIND_NONE) {
			net_type = smartcom_get_net_type(sk->sk_mark & SMARTCOM_NET_ID_MASK);
			if (!smartcom_get_network_iface_name(net_type, ifname, len))
				return false;
			smartcom_logi("sk:%pK port: %u state: %u devif: %d ifname: %s mark: %#x",
				sk, sk->sk_num, sk->sk_state, sk->sk_bound_dev_if, ifname, sk->sk_mark);
		} else {
			return false;
		}
	}

	return true;
}

static bool smartcom_mpflow_ai_need_reset(struct sock *sk, uint16_t tar_bind_dev, uint16_t ori_bind_dev)
{
	char ifname[IFNAMSIZ] = {0};
	uint8_t cur_bind_dev;

	if (smartcom_mpflow_ai_check_loop_sk(sk))
		return false;

	if (!smartcom_get_sk_ifname(sk, ifname, IFNAMSIZ - 1))
		return false;

	cur_bind_dev = smartcom_transfer_ifname_to_binddev(ifname);
	if (tar_bind_dev == SMARTCOM_MPFLOW_BIND_NONE && ((sk->sk_bound_dev_if == SMARTCOM_MPFLOW_BIND_NONE) ||
		cur_bind_dev == smartcom_get_net_type(SMARTCOM_DEFAULT_NET_ID) +
		SMARTCOM_MPFLOW_BIND_WIFI0 - SMARTCOM_NET_WLAN0)) {
		smartcom_logi("ignore flow already in defult dev, port: %u state: %u devif: %d cur_bind_dev: %u",
			sk->sk_num, sk->sk_state, sk->sk_bound_dev_if, cur_bind_dev);
		return false;
	}
	if (cur_bind_dev == SMARTCOM_MPFLOW_BIND_NONE)
		goto NEED_RESET;

	if (cur_bind_dev == tar_bind_dev || (ori_bind_dev != SMARTCOM_MPFLOW_BIND_NONE && cur_bind_dev != ori_bind_dev))
		return false;

NEED_RESET:
		return true;
}

static void smartcom_mpflow_ai_close_all_tcp_flow(const struct smartcom_mpflow_ai_info *app,
	const uint16_t tar_bind_dev, const uint16_t ori_bind_dev, int32_t err_num)
{
	uid_t uid = app->uid;
	struct sock *sk = NULL;
	uint32_t bucket_index = 0;
	struct sock *sk_arr[SKARR_SZ];
	int valid_num = 0;
	int idx = 0;

	for (; bucket_index <= tcp_hashinfo.ehash_mask; bucket_index++) {
		struct inet_ehash_bucket *head = &tcp_hashinfo.ehash[bucket_index];
		spinlock_t *lock = inet_ehash_lockp(&tcp_hashinfo, bucket_index);
		struct hlist_nulls_node *node = NULL;

		if (hlist_nulls_empty(&head->chain))
			continue;

		spin_lock_bh(lock);
		sk_nulls_for_each(sk, node, &head->chain) {
			if (unlikely(!refcount_inc_not_zero(&sk->sk_refcnt)))
				continue;

			if (sk->sk_state != TCP_ESTABLISHED && sk->sk_state != TCP_SYN_SENT) {
				sock_gen_put(sk);
				continue;
			}

			if (uid != sock_i_uid(sk).val || smartcom_mpflow_ai_check_excluded_port(app, sk)) {
				sock_gen_put(sk);
				continue;
			}

			if (!smartcom_mpflow_ai_need_reset(sk, tar_bind_dev, ori_bind_dev)) {
				sock_gen_put(sk);
				continue;
			}
			sk_arr[valid_num] = sk;
			valid_num++;
			if (valid_num >= SKARR_SZ)
				break;
		}
		spin_unlock_bh(lock);
		if (valid_num >= SKARR_SZ)
			break;
	}
	smartcom_logi("reset all tcp flow, uid: %u tar_bind_dev: %hu ori_bind_dev: %hu flow_num: %d", uid, tar_bind_dev,
		ori_bind_dev, valid_num);

	for (; idx < valid_num; idx++) {
		smartcom_reset_tcp_skerror(sk_arr[idx], err_num);
		sock_gen_put(sk_arr[idx]);
	}
}

static void smartcom_mpflow_ai_close_all_udp_flow(const uint32_t uid, const uint16_t tar_bind_dev,
	const uint16_t ori_bind_dev, const smartcom_mpflow_reset_method method, int32_t err_num)
{
	struct sock *sk = NULL;
	uint32_t bucket = 0;
	struct sock *sk_arr[SKARR_SZ];
	int accum = 0;
	int idx = 0;

	if (method != SK_ERROR && method != INTF_CHANGE) {
		smartcom_loge("reset all udp flow method not support, method: %u", method);
		return;
	}

	for (; bucket <= udp_table.mask; bucket++) {
		struct udp_hslot *hslot = &udp_table.hash[bucket];

		if (hlist_empty(&hslot->head))
			continue;

		spin_lock_bh(&hslot->lock);
		sk_for_each(sk, &hslot->head) {
			if (unlikely(!refcount_inc_not_zero(&sk->sk_refcnt)))
				continue;

			if (!(sk->sk_state != TCP_SYN_RECV && sk->sk_state != TCP_TIME_WAIT && sk->sk_state != TCP_NEW_SYN_RECV)) {
				sock_gen_put(sk);
				continue;
			}

			if (uid != sock_i_uid(sk).val || !smartcom_mpflow_ai_need_reset(sk, tar_bind_dev, ori_bind_dev)) {
				sock_gen_put(sk);
				continue;
			}

			sk_arr[accum] = sk;
			accum++;
			if (accum >= SKARR_SZ)
				break;
		}
		spin_unlock_bh(&hslot->lock);
		if (accum >= SKARR_SZ)
			break;
	}
	smartcom_logi("reset all udp flow, uid: %u method: %u tar_bind_dev: %hu ori_bind_dev: %hu flow_num: %d",
		uid, method, tar_bind_dev, ori_bind_dev, accum);

	for (; idx < accum; idx++) {
		if (method == SK_ERROR) {
			clear_mpflow_bind_flag(sk_arr[idx]);
			smartcom_reset_udp_skerror(sk_arr[idx], err_num);
		} else if (method == INTF_CHANGE) {
			smartcom_mpflow_ai_path_handover(sk_arr[idx], tar_bind_dev);
		}
		sock_gen_put(sk_arr[idx]);
	}
}

static void smartcom_mpflow_reset_all_flow(const uint8_t *data, uint16_t len)
{
	struct smartcom_mpflow_rst_all_flow_info *rst_all_flow_info = NULL;
	uint32_t uid;
	uint16_t low_bit;
	uint16_t high_bit;
	uint16_t ori_bind_dev;
	uint16_t tar_bind_dev;
	int8_t app_index;
	smartcom_mpflow_reset_method method;
	struct smartcom_mpflow_ai_info *app = NULL;

	smartcom_logd("rcv reset all flow");

	if ((data == NULL) || (len < sizeof(struct smartcom_mpflow_rst_all_flow_info))) {
		smartcom_loge("mod flow pointer null or length %d error", len);
		return;
	}

	rst_all_flow_info = (struct smartcom_mpflow_rst_all_flow_info *)data;
	uid = rst_all_flow_info->uid;
	low_bit = rst_all_flow_info->policy.rst_bind_dev_or_mode & SMARTCOM_MPFLOW_BINDMODE_ORI_DEV_MASK;
	high_bit = rst_all_flow_info->policy.rst_bind_dev_or_mode >> SMARTCOM_MPFLOW_BINDMODE_ORI_DEV_OFFSET;
	ori_bind_dev = smartcom_transfer_bindmode_to_binddev(high_bit);
	tar_bind_dev = smartcom_transfer_bindmode_to_binddev(low_bit);
	method = rst_all_flow_info->policy.method;
	smartcom_logi("reset all flow:uid[%u] tar_bind_dev[%hu] ori_bind_dev[%hu] method[%d] blink_time_ms[%u]",
		uid, tar_bind_dev, ori_bind_dev, method, rst_all_flow_info->policy.blink_time);

	spin_lock_bh(&g_mpflow_ai_lock);
	app_index = smartcom_mpflow_ai_finduid(uid);
	if (app_index == INDEX_INVALID) {
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}
	app = &g_mpflow_ai_uids[app_index];
	if (!rst_all_flow_info->policy.blink_time) {
		app->rst_all_flow = false;
		app->rst_all_flow_duration = 0;
		smartcom_logi("clear reset all flow");
		spin_unlock_bh(&g_mpflow_ai_lock);
		return;
	}
	app->rst_all_flow_bind_dev = tar_bind_dev;
	app->rst_all_flow = true;
	app->rst_all_flow_jiffies = jiffies;
	app->rst_all_flow_duration = msecs_to_jiffies(rst_all_flow_info->policy.blink_time);
	spin_unlock_bh(&g_mpflow_ai_lock);

	smartcom_mpflow_ai_close_all_tcp_flow(app, tar_bind_dev, ori_bind_dev, rst_all_flow_info->policy.err_num);
	smartcom_mpflow_ai_close_all_udp_flow(uid, tar_bind_dev, ori_bind_dev, method, rst_all_flow_info->policy.err_num);
}

static void smartcom_mpflow_clear_all_data(void)
{
	memset_s(g_mpflow_ai_uids, sizeof(struct smartcom_mpflow_ai_info) * SMARTCOM_MPFLOW_AI_MAX_APP,
		0, sizeof(struct smartcom_mpflow_ai_info) * SMARTCOM_MPFLOW_AI_MAX_APP);
	memset_s(g_mpflow_fail_stat_list, sizeof(struct smartcom_mpflow_stat) * SMARTCOM_MPFLOW_AI_MAX_LIST_NUM,
		0, sizeof(struct smartcom_mpflow_stat) * SMARTCOM_MPFLOW_AI_MAX_LIST_NUM);
	memset_s(g_mpflow_network_infos, sizeof(struct smarmcom_mpflow_network_info) * SMARTCOM_NET_MAX_NUM,
		0, sizeof(struct smarmcom_mpflow_network_info) * SMARTCOM_NET_MAX_NUM);
	g_last_default_net_type = SMARTCOM_NET_INVALID;
}

void smartcom_mpflow_init(void)
{
	spin_lock_init(&g_mpflow_network_infos_cache_lock);
	spin_lock_init(&g_mpflow_ai_lock);
	smartcom_mpflow_clear_all_data();
}

void smartcom_mpflow_deinit(void)
{
	smartcom_mpflow_clear_all_data();
}

int smartcom_mpflow_get_best_device(uid_t uid, struct sock *sk, struct sockaddr *uaddr, char *ifname, int len)
{
	struct smartcom_mpflow_ai_info *app = NULL;
	int8_t index;
	uint32_t bind_dev = 0;

	if (isvalid_mpflow_bind_flag(sk) || (sk->sk_bound_dev_if && sk->sk_protocol == IPPROTO_TCP)) {
		return MPFLOW_ERROR;
	} else if (sk->sk_protocol == IPPROTO_UDP) {
#ifdef CONFIG_HUAWEI_FEATURE_PRINT_PID_NAME
		struct inet_sock *inet = inet_sk(sk);
		print_process_pid_name(inet);
#endif
		add_mpflow_bind_flag(sk);
	}

	if (invalid_uid(uid))
		return MPFLOW_ERROR;

	spin_lock_bh(&g_mpflow_ai_lock);
	index = smartcom_mpflow_ai_finduid(uid);
	if (index == INDEX_INVALID) {
		spin_unlock_bh(&g_mpflow_ai_lock);
		return MPFLOW_ERROR;
	}

	app = &g_mpflow_ai_uids[index];
	bind_dev = smartcom_mpflow_get_bind_dev(app, sk, uaddr);
	spin_unlock_bh(&g_mpflow_ai_lock);

	add_mpflow_bind_flag(sk);
	if (!smartcom_mpflow_ai_get_update_iface_name_result(bind_dev, sk, ifname, len))
		return MPFLOW_ERROR;

	/* Fix the bug for weixin app bind wifi ip, then reset to lte, cannot find the sk */
	if (!smartcom_mpflow_ai_rehash_sk(sk))
		return MPFLOW_ERROR;

	return MPFLOW_OK;
}

void smartcom_mpflow_event_process(int32_t event, uint8_t *pdata, uint16_t len)
{
	smartcom_logi("received message: %d", event);
	switch (event) {
	case NETLINK_SMARTCOM_DK_MPFLOW_START:
		smartcom_mpflow_start(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_STOP:
		smartcom_mpflow_stop(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_PORT_BIND_CONFIG:
		smartcom_mpflow_update_port_bind_config(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_CHANGE_BURST_RADIO:
		smartcom_mpflow_change_burst_ratio(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_UPDATE_NETWORK_INFO:
		smartcom_mpflow_update_network_info(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_RESET_FLOW:
		smartcom_mpflow_reset_flow(pdata, len);
		break;
	case NETLINK_SMARTCOM_DK_MPFLOW_RESET_ALL_FLOW:
		smartcom_mpflow_reset_all_flow(pdata, len);
		break;
	default:
		break;
	}
}

static bool smartcom_mpflow_get_fallback_ver(struct sock *sk,
	struct smartcom_mpflow_fallback_ver *uid_info)
{
	uid_info->uid = sock_i_uid(sk).val;
	if (invalid_uid(uid_info->uid))
		return false;

	uid_info->index = smartcom_mpflow_ai_finduid(uid_info->uid);
	if (uid_info->index != INDEX_INVALID)
		return true;

	smartcom_logi("get fallback ver failed sk[%pK] uid[%u]", sk, uid_info->uid);
	return false;
}

static struct smartcom_mpflow_stat *smartcom_mpflow_get_fail_stat(uid_t uid, const char *name, int ifindex)
{
	struct smartcom_mpflow_stat *node = NULL;
	int8_t index;
	errno_t err;
	struct smartcom_mpflow_stat *mpflow_list = g_mpflow_fail_stat_list;

	for (index = 0; index < SMARTCOM_MPFLOW_AI_MAX_LIST_NUM; index++) {
		if ((node == NULL) && (mpflow_list[index].uid == UID_INVALID_APP))
			node = &mpflow_list[index];
		if ((mpflow_list[index].uid == uid) && (!strncmp(mpflow_list[index].name, name, strlen(name))))
			return &mpflow_list[index];
	}

	if (!node) {
		smartcom_logd("smartcom_mpflow_get_fail_stat list full\n");
		return NULL;
	}

	node->uid = uid;
	node->ifindex = ifindex;
	node->name[IFNAMSIZ - 1] = '\0';
	err = strncpy_s(node->name, IFNAMSIZ, name, IFNAMSIZ - 1);
	if (err != EOK) {
		smartcom_loge("smartcom_mpflow_get_fail_stat strncpy_s failed, errcode: %d", err);
		node->uid = UID_INVALID_APP;
		return NULL;
	}
	node->mpflow_fallback = SMARTCOM_MPFLOW_FALLBACK_CLR;
	node->mpflow_fail_nopayload = 0;
	node->mpflow_fail_syn_rst = 0;
	node->mpflow_fail_syn_timeout = 0;
	node->mpflow_estlink = 0;
	node->start_jiffies = 0;
	err = memset_s(node->retrans_count, sizeof(node->retrans_count), 0, sizeof(node->retrans_count));
	if (err != EOK) {
		smartcom_loge("smartcom_mpflow_get_fail_stat memset failed, errcode: %d", err);
		node->uid = UID_INVALID_APP;
		return NULL;
	}

	return node;
}

static void smartcom_mpflow_report(struct smartcom_mpflow_fallback_info *fallback)
{
	smartcom_logi("MpFlow fallback report uid:%d reason: %d", fallback->uid, fallback->reason);
	smartcom_send_msg2daemon(NETLINK_SMARTCOM_KD_MPFLOW_FALLBACK,
		(const void *)fallback, sizeof(struct smartcom_mpflow_fallback_info));
}

static bool smartcom_mpflow_errlink(int reason, struct smartcom_mpflow_stat *node)
{
	if (reason == SMARTCOM_TCP_FALLBACK_NOPAYLOAD)
		node->mpflow_fail_nopayload++;
	else if (reason == SMARTCOM_TCP_FALLBACK_SYN_RST)
		node->mpflow_fail_syn_rst++;
	else if ((reason == SMARTCOM_TCP_FALLBACK_SYN_TOUT) ||
			(reason == SMARTCOM_TCP_FALLBACK_SYN_TOUT_FAST))
		node->mpflow_fail_syn_timeout++;
	else
		return false;

	if ((node->mpflow_fail_nopayload >= SMARTCOM_MPFLOW_FALLBACK_NOPAYLOAD_THRH)
		|| (node->mpflow_fail_syn_rst >= SMARTCOM_MPFLOW_FALLBACK_SYN_RST_THRH)
		|| (node->mpflow_fail_syn_timeout >= SMARTCOM_MPFLOW_FALLBACK_SYN_TOUT_THRH))
		return true;

	return false;
}

bool smartcom_mpflow_retrans(int reason, struct smartcom_mpflow_stat *node)
{
	int i;
	errno_t err;

	if ((reason != SMARTCOM_TCP_FALLBACK_RETRANS) &&
		(reason != SMARTCOM_TCP_FALLBACK_SYN_TOUT_FAST))
		return false;

	for (i = 1; (i <= node->retrans_count[0]) && (i <= SMARTCOM_MPFLOW_FALLBACK_RETRANS_TIME); i++) {
		if (time_before_eq(jiffies, node->start_jiffies + i * HZ)) {
			node->retrans_count[i]++;
			break;
		}
	}

	/* Time range matched */
	if (i <= node->retrans_count[0]) {
		/* expand time range */
		if (node->retrans_count[i] == SMARTCOM_MPFLOW_FALLBACK_RETRANS_THRH) {
			node->retrans_count[0]++;
			smartcom_logd("MpFlow fallback uid:%d inf:%d(%s) retrans count:%d, jiffies:%lu\n",
					node->uid, node->ifindex, node->name, node->retrans_count[0], node->start_jiffies);
		}
		/* retransmission fallback */
		if (node->retrans_count[0] > SMARTCOM_MPFLOW_FALLBACK_RETRANS_TIME) {
			err = memset_s(node->retrans_count, sizeof(node->retrans_count), 0, sizeof(node->retrans_count));
			if (err != EOK)
				smartcom_loge("smartcom_mpflow_retrans memset failed");
			return true;
		}
	} else {
		err = memset_s(node->retrans_count, sizeof(node->retrans_count), 0, sizeof(node->retrans_count));
		if (err != EOK)
			smartcom_loge("smartcom_mpflow_retrans memset failed");
		node->retrans_count[0] = 1;
		node->retrans_count[1] = 1;
		node->start_jiffies = jiffies;
	}

	return false;
}

int8_t smartcom_mpflow_fallback_checkstatus(const struct sock *sk, int *reason, int state,
	struct smartcom_mpflow_stat *node)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct inet_sock *inet = inet_sk(sk);
	int8_t result = SMARTCOM_MPFLOW_FALLBACK_NONE;
	uint8_t oldstate = sk->sk_state;

	if (*reason == SMARTCOM_TCP_FALLBACK_NOPAYLOAD) {
		/* EST->DOWN */
		if ((oldstate == TCP_ESTABLISHED) && (state != TCP_ESTABLISHED)) {
			if (node->mpflow_estlink > 0)
				node->mpflow_estlink--;
			if (state != TCP_FIN_WAIT1) {
				result = ((tp->bytes_received <= 1) && ((tp->bytes_acked > 1) ||
					(tp->snd_nxt - tp->snd_una > SMARTCOM_MPFLOW_SND_BYTE_THRESHOLD))) ?
					SMARTCOM_MPFLOW_FALLBACK_SET : SMARTCOM_MPFLOW_FALLBACK_CLR;
			} else {
				result = SMARTCOM_MPFLOW_FALLBACK_NONE;
			}
		/* DOWN->EST */
		} else if ((oldstate != TCP_ESTABLISHED) && (state == TCP_ESTABLISHED)) {
			result = SMARTCOM_MPFLOW_FALLBACK_SYNCLR;
			node->mpflow_estlink++;
		}
	} else if (*reason == SMARTCOM_TCP_FALLBACK_SYN_TOUT) {
		result = ((1 << oldstate) & (TCPF_SYN_SENT | TCPF_SYN_RECV)) ? SMARTCOM_MPFLOW_FALLBACK_SET :
			SMARTCOM_MPFLOW_FALLBACK_NONE;
	} else if (*reason == SMARTCOM_TCP_FALLBACK_RETRANS) {
		result = SMARTCOM_MPFLOW_FALLBACK_SET;
		if (((1 << oldstate) & (TCPF_SYN_SENT | TCPF_SYN_RECV)) &&
			(tp->segs_out == SMARTCOM_MPFLOW_FALLBACK_SYN_TOUT_TIME))
			*reason = SMARTCOM_TCP_FALLBACK_SYN_TOUT_FAST;
	} else {
		result = SMARTCOM_MPFLOW_FALLBACK_SET;
	}

	if (result != SMARTCOM_MPFLOW_FALLBACK_NONE)
		smartcom_logd("uid:%d sk:%pK src_addr:"IPV4_FMT":%d dst_addr:"IPV4_FMT":%d inf:%d[%s] "
					"R:%d ost:%d nst:%d P[%d->%d] ret:%d,snt,fly,ack,rcv:%u,%u,%llu,%llu",
					node->uid, sk, ipv4_info(sk->sk_rcv_saddr), sk->sk_num, ipv4_info(sk->sk_daddr),
					ntohs(sk->sk_dport), node->ifindex, node->name,
					*reason, oldstate, state, ntohs(inet->inet_sport), ntohs(inet->inet_dport),
					result, tp->segs_out, tp->snd_nxt - tp->snd_una, tp->bytes_acked, tp->bytes_received);

	return result;
}

static bool smartcom_mpflow_fallback_check_valid_reason(const char *name, int net_type, int reason)
{
	char cur_ifname[IFNAMSIZ] = {0};
	if (!smartcom_get_network_iface_name(net_type, cur_ifname, IFNAMSIZ - 1))
		return false;

	if (strncmp(cur_ifname, name, strlen(name)) != 0)
		return false;

	if (net_type != SMARTCOM_NET_CELL0 && net_type != SMARTCOM_NET_CELL1)
		return true;

	if (reason != SMARTCOM_TCP_FALLBACK_RETRANS)
		return true;

	return false;
}

static bool smartcom_mpflow_fallback_update_reason(int reason, const char *name,
	struct smartcom_mpflow_stat *node, struct smartcom_mpflow_fallback_info *fallback)
{
	int net_type;
	fallback->uid = node->uid;
	for (net_type = SMARTCOM_NET_WLAN0; net_type < SMARTCOM_NET_MAX_NUM; ++net_type) {
		if (smartcom_mpflow_fallback_check_valid_reason(name, net_type, reason)) {
			fallback->reason = reason;
			fallback->net_type = net_type;
			return true;
		}
	}

	smartcom_loge("invalid ifname:%s", name);
	return false;
}

static bool smartcom_mpflow_fallback_set_report(int reason, int state, const char *name,
	struct smartcom_mpflow_stat *node)
{
	if (smartcom_mpflow_errlink(reason, node) || smartcom_mpflow_retrans(reason, node)) {
		struct smartcom_mpflow_fallback_info fallback;
		smartcom_loge("MpFlow fallback uid:%d inf:%d(%s) estlink:%d nodata,rst,tout:%d,%d,%d\n",
				node->uid, node->ifindex, node->name, node->mpflow_estlink,
				node->mpflow_fail_nopayload, node->mpflow_fail_syn_rst, node->mpflow_fail_syn_timeout);

		if (!smartcom_mpflow_fallback_update_reason(reason, name, node, &fallback))
			return false;

		node->report_jiffies = jiffies;
		smartcom_mpflow_report(&fallback);
	}

	return true;
}

static void smartcom_mpflow_fallback_report(struct sock *sk, int reason, int state, const char *name,
	struct smartcom_mpflow_stat *node)
{
	int8_t result = smartcom_mpflow_fallback_checkstatus(sk, &reason, state, node);
	if (result == SMARTCOM_MPFLOW_FALLBACK_SET) {
		if (!smartcom_mpflow_fallback_set_report(reason, state, name, node))
			return;
	} else if (result == SMARTCOM_MPFLOW_FALLBACK_CLR) {
		node->mpflow_fallback = SMARTCOM_MPFLOW_FALLBACK_CLR;
		node->mpflow_fail_nopayload = 0;
		node->mpflow_fail_syn_rst = 0;
		node->mpflow_fail_syn_timeout = 0;
	} else if (result == SMARTCOM_MPFLOW_FALLBACK_SYNCLR) {
		node->mpflow_fail_syn_rst = 0;
		node->mpflow_fail_syn_timeout = 0;
	}
}

void smartcom_mpflow_tcp_fallback(struct sock *sk, int reason, int state)
{
	struct smartcom_mpflow_stat *node = NULL;
	struct net_device *dev = NULL;
	char name[IFNAMSIZ] = {0};
	errno_t err;
	struct smartcom_mpflow_fallback_ver uid_info;
	int devif;

	if (!sk)
		return;

	devif = sk->sk_bound_dev_if;
	/* If the sk not bind yet, not need fallback check */
	if (!devif || !isvalid_mpflow_bind_flag(sk))
		return;

	if (!smartcom_mpflow_get_fallback_ver(sk, &uid_info))
		return;

	dev = dev_get_by_index(sock_net(sk), devif);
	if (!dev || (dev->name[0] == '\0')) {
		smartcom_loge("get dev name failed, sk[%pK] dev_if[%d]", sk, devif);
		if (dev)
			dev_put(dev);
		return;
	}

	err = strncpy_s(name, IFNAMSIZ, dev->name, IFNAMSIZ - 1);
	dev_put(dev);
	if (err != 0) {
		smartcom_loge("strncpy_s failed, errcode: %d", err);
		return;
	}

	node = smartcom_mpflow_get_fail_stat(uid_info.uid, name, devif);
	if (!node)
		return;

	smartcom_mpflow_fallback_report(sk, reason, state, name, node);
	return;
}