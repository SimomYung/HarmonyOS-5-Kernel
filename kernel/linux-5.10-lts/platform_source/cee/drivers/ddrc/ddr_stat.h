/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#ifndef __DDR_STAT_H__
#define __DDR_STAT_H__

#include <linux/types.h>
#include <ddr/ddr_release_message.h>

#define DDR_STAT_SEND_DATA_SIZE 2
#define DDR_STAT_WRITE_DATA_SIZE 64
#define DDR_STAT_BANDWIDTH_INTERVAL_MIN 4
#define DDR_STAT_BANDWIDTH_INTERVAL_10S 10000
#define DDR_STAT_BANDWIDTH_INTERVAL_MAX 600000
#define DDR_STAT_BANDWIDTH_DURATION_MAX 10
#define DDR_STAT_BANDWIDTH_THRESHOLD_MAX	99
#define DDR_STAT_BANDWIDTH_DEFAULT_INTERVAL 60000
#define DDR_STAT_BANDWIDTH_DEFAULT_DURATION 1
#define DDR_STAT_BANDWIDTH_DEFAULT_THRESHOLD 90
#define DDR_STAT_ONE_HUNDRED	100
#define DDR_STAT_BANDWIDTH_MASTER_MAX	10
#define DDR_STAT_LP_INTERVAL_MIN 10
#define DDR_STAT_LP_INTERVAL_MAX 1000

enum ddr_stat_master_id {
	DDR_STAT_MASTER_CPU,
	DDR_STAT_MASTER_GPU,
	DDR_STAT_MASTER_DSS,
	DDR_STAT_MASTER_ISP1,
	DDR_STAT_MASTER_ISP2,
	DDR_STAT_MASTER_SC,
	DDR_STAT_MASTER_VDEC,
	DDR_STAT_MASTER_NPU,
	DDR_STAT_MASTER_HIFI,
	DDR_STAT_MASTER_MODEM,
	DDR_STAT_MASTER_MAX,
};

#ifdef CONFIG_DDR_DDRC_STAT
int ddr_stat_get_info(ddr_release_message_str *info);
int ddr_bandwidth_set_enable(unsigned int enable);
int ddr_bandwidth_set_interval(unsigned int interval);
int ddr_bandwidth_set_duration(unsigned int duration);
int ddr_bandwidth_set_threshold(unsigned int threshold);
int ddr_bandwidth_trigger_once(unsigned int trigger_once);
int ddr_bandwidth_set_master_id(unsigned int master_value);

int ddr_lp_set_enable(unsigned int lp_enable);

static inline int ddr_bandwidth_trigger(void)
{
	return ddr_bandwidth_trigger_once(true);
}

static inline int ddr_bandwidth_set_master(unsigned int master_id, bool enable)
{
	return ddr_bandwidth_set_master_id(master_id * DDR_STAT_ONE_HUNDRED + enable);
}
#else
static inline int ddr_stat_get_info(ddr_release_message_str *info)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_enable(unsigned int enable)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_interval(unsigned int interval)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_duration(unsigned int duration)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_threshold(unsigned int threshold)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_trigger_once(unsigned int trigger_once)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_master_id(unsigned int master_value)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_trigger(void)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_master(unsigned int master_id, bool enable)
{
	return -EFAULT;
}

static inline int ddr_lp_set_enable(unsigned int lp_enable)
{
	return -EFAULT;
}
#endif

#endif