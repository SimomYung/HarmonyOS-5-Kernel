/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: function declaration for sending smc cmd
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef SMC_SMP_H
#define SMC_SMP_H

#include <linux/device.h>
#include <linux/kthread.h>
#include "teek_client_constants.h"
#include "teek_ns_client.h"
#include "tc_linux_def.h"
#include "tc_bitdef.h"

#if (KERNEL_VERSION(5, 4, 0) <= LINUX_VERSION_CODE)
#define CURRENT_CPUS_ALLOWED (&current->cpus_mask)
#else
#define CURRENT_CPUS_ALLOWED (&current->cpus_allowed)
#endif

enum tc_ns_cmd_type {
	TC_NS_CMD_TYPE_INVALID = 0,
	TC_NS_CMD_TYPE_NS_TO_SECURE,
	TC_NS_CMD_TYPE_SECURE_TO_NS,
	TC_NS_CMD_TYPE_SECURE_TO_SECURE,
	TC_NS_CMD_TYPE_SECURE_CONFIG = 0xf,
	TC_NS_CMD_TYPE_MAX
};

struct pending_entry {
	atomic_t users;
	struct task_struct *task;
#ifdef CONFIG_TA_AFFINITY
	struct cpumask ca_mask;
	struct cpumask ta_mask;
#endif
	pid_t pid;
	wait_queue_head_t wq;
	atomic_t run;
	struct list_head list;
};

#define SHADOW_NAME_LEN 16
struct shadow_work {
	struct kthread_work kthwork;
	struct work_struct work;
	uint64_t target;
	char name[SHADOW_NAME_LEN];
};

#ifdef CONFIG_BIG_SESSION
#define MAX_SMC_CMD (CONFIG_BIG_SESSION * 10)
#else
#define MAX_SMC_CMD 18
#endif

#define SIQ_DUMP_TIMEOUT 1U
#define SIQ_DUMP_SHELL   2U

typedef uint32_t smc_buf_lock_t;

struct tc_ns_smc_queue {
	/* set when CA send cmd_in, clear after cmd_out return */
	DECLARE_BITMAP(in_bitmap, MAX_SMC_CMD);
	/* set when gtask get cmd_in, clear after cmd_out return */
	DECLARE_BITMAP(doing_bitmap, MAX_SMC_CMD);
	/* set when gtask get cmd_out, clear after cmd_out return */
	DECLARE_BITMAP(out_bitmap, MAX_SMC_CMD);
	smc_buf_lock_t smc_lock;
	volatile uint32_t last_in;
	struct tc_ns_smc_cmd in[MAX_SMC_CMD];
	volatile uint32_t last_out;
	struct tc_ns_smc_cmd out[MAX_SMC_CMD];
};

#define SYM_NAME_LEN_1 7
#define SYM_NAME_LEN_2 4
#define CRASH_REG_NUM  3
#define LOW_FOUR_BITE  4

/* "+ 1" is used to end the string with '\0' */
#define SYM_NAME_LEN_MAX (SYM_NAME_LEN_1 + SYM_NAME_LEN_2 + 1)

union crash_inf {
	uint64_t crash_reg[CRASH_REG_NUM];
	struct {
		uint8_t halt_reason : LOW_FOUR_BITE;
		uint8_t app : LOW_FOUR_BITE;
		char sym_name[SYM_NAME_LEN_1];
		uint16_t off;
		uint16_t size;
		uint32_t far;
		uint32_t fault;
		union {
			char sym_name_append[SYM_NAME_LEN_2];
			uint32_t elr;
		};
	} crash_msg;
};

#define RESLEEP_TIMEOUT 3

struct tc_ns_smc_queue *get_cmd_data(void);
bool get_reserved_cmd_buffer(void);
int smc_context_init(const struct device *class_dev);
void free_smc_data(void);
int tc_ns_smc(struct tc_ns_smc_cmd *cmd);
int tc_ns_smc_with_no_nr(struct tc_ns_smc_cmd *cmd);
int teeos_log_exception_archive(unsigned int eventid, const char *exceptioninfo);
void set_cmd_send_state(void);
int init_smc_svc_thread(void);
int smc_wakeup_ca(pid_t ca);
int smc_wakeup_broadcast(void);
int smc_shadow_exit(pid_t ca);
struct task_struct *shadow_work_func(struct kthread_work *work);
struct kthread_worker *get_ipi_helper_worker(void);
void fiq_shadow_work_func(uint64_t target);
struct pending_entry *find_pending_entry(pid_t pid);
void foreach_pending_entry(void (*func)(struct pending_entry *));
void put_pending_entry(struct pending_entry *pe);
void show_cmd_bitmap(void);
void wakeup_tc_siq(uint32_t siq_mode);
void occupy_clean_cmd_buf(void);
void clr_system_crash_flag(void);
void svc_thread_release(void);
int send_smc_cmd_rebooting(uint32_t cmd_id, const struct tc_ns_smc_cmd *in_cmd);
void send_smc_cmd_buffer(bool tee_is_dead);
int parse_params_from_tee(void);
int smp_smc_send_func(struct tc_ns_smc_cmd *in, bool reuse);
struct pending_entry *init_pending_entry(void);
void release_pending_entry(struct pending_entry *pe);
bool is_in_freeze(void);
void check_freeze_flag(void);
#ifdef CONFIG_TEE_LOG_EXCEPTION
void send_ta_abnormal_event(const char *event_reason, const char *uuid, const char *ta_version);
#endif
bool check_uefi_enable(void);
#endif
