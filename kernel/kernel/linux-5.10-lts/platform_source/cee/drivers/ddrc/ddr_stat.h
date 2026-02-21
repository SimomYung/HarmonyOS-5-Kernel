/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#ifndef __DDR_STAT_H__
#define __DDR_STAT_H__

#include <linux/types.h>
#include <ddr/ddr_define_kernel.h>
#include <ddr/ddr_release_message.h>

#define DDR_STAT_SEND_DATA_SIZE 2
#define DDR_STAT_WRITE_DATA_SIZE 64
#define DDR_STAT_BANDWIDTH_INTERVAL_MIN 20
#define DDR_STAT_BANDWIDTH_INTERVAL_10S 10000
#define DDR_STAT_BANDWIDTH_INTERVAL_MAX 600000
#define DDR_STAT_BANDWIDTH_DURATION_MAX 10
#define DDR_STAT_BANDWIDTH_THRESHOLD_MAX	99
#define DDR_STAT_BANDWIDTH_DEFAULT_DURATION 1
#define DDR_STAT_BANDWIDTH_DEFAULT_THRESHOLD 90
#define DDR_STAT_ONE_HUNDRED	100
#define DDR_STAT_BANDWIDTH_MASTER_MAX	10
#define DDR_STAT_LP_INTERVAL_MIN 10
#define DDR_STAT_LP_INTERVAL_MAX 10000
#define DDR_STAT_LAT_INTERVAL_MIN 10
#define DDR_STAT_LAT_INTERVAL_MAX 10000
#define DDR_STAT_LAT_MASTER_MAX	5
#define DDR_STAT_TEN_THOUSAND 10000
#define DATA_BIT0_MASK	0x01
#define LOG_BUFFER_SIZE 256
#define LOG_BUFFER_TEST_SIZE 20
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
/* for big model */
int ddr_stat_get_info(ddr_release_message_str *info);
int ddr_bandwidth_set_enable_force(unsigned int enable);
int ddr_bandwidth_set_interval_force(unsigned int interval);
int ddr_bandwidth_set_threshold_force(unsigned int threshold);
int ddr_bandwidth_set_duration_force(unsigned int duration);
int ddr_bandwidth_trigger_once_force(void);
int ddr_bandwidth_set_master_id_force(unsigned int master_value, unsigned int enable);
/* for modem */
void ddr_stat_bandwidth_mntn(void);
int ddr_stat_bandwidth_record_for_modem(unsigned int* value, unsigned int length);
void ddr_stat_bandwidth_record_for_modem_test(void);

#else
static inline int ddr_stat_get_info(ddr_release_message_str *info)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_enable_force(unsigned int enable)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_interval_force(unsigned int interval)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_duration_force(unsigned int duration)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_threshold_force(unsigned int threshold)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_master_id_force(unsigned int master_value, unsigned int enable)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_trigger_once_force(void)
{
	return -EFAULT;
}

static inline int ddr_bandwidth_set_duration_force(unsigned int duration)
{
	return -EFAULT;
}
#endif

#endif