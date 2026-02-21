/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/wireless.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 24 13:48:57 2022
 */

#ifndef LINUX_WIRELESS_H
#define LINUX_WIRELESS_H

#include <linux/types.h>
#include <linux/socket.h>
#include <net/if.h>

#define SIOCSIWFREQ		0x8B04
#define SIOCGIWRANGE		0x8B0B
#define SIOCSIWESSID		0x8B1A
#define SIOCIWFIRSTPRIV		0x8BE0
#define IWEVCUSTOM		0x8C02
#define IWEVMICHAELMICFAILURE	0x8C06

#define IW_MAX_FREQUENCIES	32
#define IW_MAX_BITRATES		32
#define IW_MAX_ENCODING_SIZES	8
#define IW_MAX_TXPOWER		8


struct iw_param {
	__s32		value;
	__u8		fixed;
	__u8		disabled;
	__u16		flags;
};

struct iw_point {
	void __user	*pointer;
	__u16		length;
	__u16		flags;
};

struct iw_freq {
	__s32		m;
	__s16		e;
	__u8		i;
	__u8		flags;
};

struct iw_quality {
	__u8		qual;
	__u8		level;
	__u8		noise;
	__u8		updated;
};

union iwreq_data {
	char		name[IFNAMSIZ];
	struct iw_point	essid;
	struct iw_param	nwid;
	struct iw_freq	freq;

	struct iw_param	sens;
	struct iw_param	bitrate;
	struct iw_param	txpower;
	struct iw_param	rts;
	struct iw_param	frag;
	__u32		mode;
	struct iw_param	retry;
	struct iw_point	encoding;
	struct iw_param	power;
	struct iw_quality qual;
	struct sockaddr	ap_addr;
	struct sockaddr	addr;
	struct iw_param	param;
	struct iw_point	data;
};

struct iwreq {
	union
	{
		char	ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	union iwreq_data	u;
};

struct iw_range {
	__u32		throughput;
	__u32		min_nwid;
	__u32		max_nwid;
	__u16		old_num_channels;
	__u8		old_num_frequency;
	__u8		scan_capa;
	__u32		event_capa[6];
	__s32		sensitivity;

	struct iw_quality	max_qual;
	struct iw_quality	avg_qual;
	__u8		num_bitrates;
	__s32		bitrate[IW_MAX_BITRATES];
	__s32		min_rts;
	__s32		max_rts;
	__s32		min_frag;
	__s32		max_frag;

	__s32		min_pmp;
	__s32		max_pmp;
	__s32		min_pmt;
	__s32		max_pmt;
	__u16		pmp_flags;
	__u16		pmt_flags;
	__u16		pm_capa;
	__u16		encoding_size[IW_MAX_ENCODING_SIZES];
	__u8		num_encoding_sizes;
	__u8		max_encoding_tokens;
	__u8		encoding_login_index;
	__u16		txpower_capa;
	__u8		num_txpower;
	__s32		txpower[IW_MAX_TXPOWER];
	__u8		we_version_compiled;
	__u8		we_version_source;
	__u16		retry_capa;
	__u16		retry_flags;
	__u16		r_time_flags;
	__s32		min_retry;
	__s32		max_retry;
	__s32		min_r_time;
	__s32		max_r_time;
	__u16		num_channels;
	__u8		num_frequency;
	struct iw_freq	freq[IW_MAX_FREQUENCIES];
	__u32		enc_capa;
};

struct iw_event {
	__u16			len;
	__u16			cmd;
	union iwreq_data	u;
};

#define IW_EV_LCP_LEN		(sizeof(struct iw_event) - sizeof(union iwreq_data))
#define IW_EV_POINT_OFF		offsetof(struct iw_point, length)
#define IW_EV_POINT_LEN		(IW_EV_LCP_LEN + sizeof(struct iw_point) - IW_EV_POINT_OFF)

#endif
