/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef _SMARTCOM_UTILS_H
#define _SMARTCOM_UTILS_H

#include <net/sock.h>

#include <huawei_platform/log/hw_log.h>

#undef HWLOG_TAG
#define HWLOG_TAG SMARTCOMK

HWLOG_REGIST();

#define SMARTCOM_DEBUG    1
#define SMARTCOM_INFO     1

#define SMARTCOM_DEFAULT_NET_ID 0
#define SMARTCOM_NET_ID_MASK 0xffff

#define SMARTCOM_PORT_80 80
#define SMARTCOM_PORT_443 443
#define HTTP_REQUEST_GET_HOST_STR "Host: "
#define HTTP_REQUEST_ENTER_STR "\r\n"

#define IPV6_ADDR_STR_SEP  ":"
#define IPV4_ADDR_STR_SEP  "."
#define MAX_IPV4_ADDR_STR_LEN 15
#define MIN_IPV4_ADDR_STR_LEN 7
#define IPV4_ADDR_STR_SEG_NUM 3
#define IPV6_ADDR32_SEG_NUM 4
#define SMARTCOM_CLAT_IPV4_INDEX 3
#define SMARTCOM_CLAT_IPV6 4

#define IFNAMSIZ 16
#define INVALID_IFACE_NAME  "none"

#define UID_APP (100 * 20000) // normal apps are intalled in user100, uid base is 20000 on OH
#define UID_INVALID_APP 0

#define INDEX_INVALID (-1)
#define SMARTCOM_TRUE_VALUE  1

typedef enum {
	SMARTCOM_NET_INVALID = -1,
	SMARTCOM_NET_WLAN0,
	SMARTCOM_NET_WLAN1,
	SMARTCOM_NET_CELL0,
	SMARTCOM_NET_CELL1,
	SMARTCOM_NET_MAX_NUM,
	SMARTCOM_NET_DEFAULT = 15,
} smartcom_net_no;

#define smartcom_is_net_type_valid(net_type) \
	((net_type) > SMARTCOM_NET_INVALID && (net_type) < SMARTCOM_NET_MAX_NUM)

#define smartcom_logd(fmt, ...) \
	do { \
		if (SMARTCOM_DEBUG) { \
			hwlog_info("%s "fmt"\n", __func__, ##__VA_ARGS__); \
		} \
	} while (0)

#define smartcom_logi(fmt, ...) \
	do { \
		if (SMARTCOM_INFO) { \
			hwlog_info("%s "fmt"\n", __func__, ##__VA_ARGS__); \
		} \
	} while (0)

#define smartcom_loge(fmt, ...) \
	do { \
		hwlog_err("%s "fmt"\n", __func__, ##__VA_ARGS__); \
	} while (0)

#define IP_DEBUG 0

#define ip6_addr_block1(ip6_addr) (ntohs((ip6_addr).s6_addr16[0]) & 0xffff)
#define ip6_addr_block2(ip6_addr) (ntohs((ip6_addr).s6_addr16[1]) & 0xffff)
#define ip6_addr_block3(ip6_addr) (ntohs((ip6_addr).s6_addr16[2]) & 0xffff)
#define ip6_addr_block4(ip6_addr) (ntohs((ip6_addr).s6_addr16[3]) & 0xffff)
#define ip6_addr_block5(ip6_addr) (ntohs((ip6_addr).s6_addr16[4]) & 0xffff)
#define ip6_addr_block6(ip6_addr) (ntohs((ip6_addr).s6_addr16[5]) & 0xffff)
#define ip6_addr_block7(ip6_addr) (ntohs((ip6_addr).s6_addr16[6]) & 0xffff)
#define ip6_addr_block8(ip6_addr) (ntohs((ip6_addr).s6_addr16[7]) & 0xffff)

#if IP_DEBUG
#define IPV4_FMT "%u.%u.%u.%u"
#define ipv4_info(addr) \
	((unsigned char *)&(addr))[0], \
	((unsigned char *)&(addr))[1], \
	((unsigned char *)&(addr))[2], \
	((unsigned char *)&(addr))[3]

#define IPV6_FMT "%x:%x:%x:%x:%x:%x:%x:%x"
#define ipv6_info(addr) \
	ip6_addr_block1(addr), \
	ip6_addr_block2(addr), \
	ip6_addr_block3(addr), \
	ip6_addr_block4(addr), \
	ip6_addr_block5(addr), \
	ip6_addr_block6(addr), \
	ip6_addr_block7(addr), \
	ip6_addr_block8(addr)

#else
#define IPV4_FMT "%u.%u.*.*"
#define ipv4_info(addr) \
	((unsigned char *)&(addr))[0], \
	((unsigned char *)&(addr))[1]

#define IPV6_FMT "%x:***:%x"
#define ipv6_info(addr) \
	ip6_addr_block1(addr), \
	ip6_addr_block8(addr)
#endif

bool smartcom_check_ip_is_private(struct sockaddr *addr);
bool smartcom_check_ip_addrss(struct sockaddr *addr);
bool smartcom_get_network_iface_name(int net_type, char *ifname, int len);
void smartcom_reset_tcp_skerror(struct sock *sk, int32_t err_num);
void smartcom_reset_udp_skerror(struct sock *sk, int32_t err_num);
void smartcom_enable_selected_path(struct sock *sk, const char *selected_path_iface);

#endif  /* _SMARTCOM_UTILS_H */
