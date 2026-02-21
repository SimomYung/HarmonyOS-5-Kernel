/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: MAS MCQ header file
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __MAS_MCQ_H__
#define __MAS_MCQ_H__
#include <linux/blk_types.h>
#include <linux/blk-mq.h>

extern struct blk_tagset_ops mas_mcq_tagset_ops;
extern struct blk_queue_ops mas_ufs_blk_queue_ops;

/*
 * This struct defines all the variable for ufs mcq io-scheduler per lld.
 */
struct mas_mcq_sched_ds_lld {
	struct blk_dev_lld *lld;

	atomic_t per_cpu_tag_used[NR_CPUS + 1];
	atomic_t per_cpu_hp_req_num[NR_CPUS + 1];

	atomic_t per_cpu_sync_req_inflt[NR_CPUS + 1];
	atomic_t per_cpu_vip_req_inflt[NR_CPUS + 1];
	atomic_t per_cpu_fg_req_inflt[NR_CPUS + 1];
	atomic_t per_cpu_cp_req_inflt[NR_CPUS + 1];

	atomic_t per_cpu_req_wait[NR_CPUS + 1];
	struct list_head per_cpu_req_wait_list[NR_CPUS + 1];
	spinlock_t per_cpu_req_wait_lock[NR_CPUS + 1];

	atomic_t async_io_inflt_cnt;
	atomic_t ref_cnt;

	ktime_t last_sync_queue_tm[NR_CPUS];
	ktime_t last_sync_enter_tm[NR_CPUS];
	ktime_t last_sync_exit_tm[NR_CPUS];

	ktime_t last_async_queue_tm;
	ktime_t last_async_enter_tm;
	ktime_t last_async_exit_tm;
};

struct mas_mcq_tags {
	struct sbitmap_queue mcq_tags[NR_CPUS + 1];
	unsigned int cur_tags[NR_CPUS + 1];
	unsigned int sum_tags[NR_CPUS + 1];
};

/* mas_blk_iosched_ufs_mq.c */
struct delayed_work *dfa_mcq_get_sync_work(
	const struct request_queue *q);
struct delayed_work *dfa_mcq_get_async_work(
	const struct request_queue *q);
struct workqueue_struct *dfa_mcq_get_sync_wq(void);
struct workqueue_struct *dfa_mcq_get_async_wq(void);

/* mas_mcq_iosched.c */
int dfa_mcq_sched_ds_lld_init(struct request_queue *q);
void dfa_mcq_sched_ds_lld_exit(struct request_queue *q);
struct mas_mcq_sched_ds_lld *mas_mcq_get_lld(const struct request_queue *q);
unsigned int dfa_mcq_get_cpu(struct request * req);
unsigned int dfa_mcq_tagset_bt_get(struct blk_mq_alloc_data *data,
	struct sbitmap_queue *bt, struct blk_mq_hw_ctx *hctx, unsigned int cpu);
int dfa_mcq_fs_io_limit(struct request *rq);
void dfa_mcq_rq_inflt_update(const struct request *rq);
void dfa_mcq_insert_sync_list(struct request *rq, bool head);
void dfa_mcq_insert_async_list(struct request *rq, bool head);
void dfa_mcq_sync_dispatch(const struct request_queue *q);
int dfa_mcq_sync_queue_rq(struct blk_mq_queue_data *bd,
	struct request *rq, const struct blk_mq_alloc_data *alloc_data);
void dfa_mcq_run_sync_list(const struct request_queue *q, int specific_cpu);
void dfa_mcq_requeue_sync_list(struct request *rq, struct request_queue *q);
void dfa_mcq_run_delay_async_list(
	const struct request_queue *q, unsigned long delay_jiffies);
void dfa_mcq_run_queue(const struct request_queue *q);
void dfa_mcq_exec_queue(const struct request_queue *q);
void dfa_mcq_io_guard_queue(const struct request_queue *q);
bool dfa_mcq_async_attempt_merge(struct bio *bio,
	const struct request_queue *q, unsigned int nr_segs);
bool dfa_mcq_list_is_empty(const struct request_queue *q, unsigned int cpu);
void dfa_mcq_async_dispatch(const struct request_queue *q);
void dfa_mcq_dump_work_time(const struct request_queue *q);
void dfa_mcq_status_dump(const struct request_queue *q, enum blk_dump_scene s);
bool mas_mcq_dfa_enable(const struct request_queue *q);
void mas_mcq_dfa_set(struct blk_mq_tag_set *set, int mas_mcq_enable, int dfa_enable);

/* mas_mcq_tag.c */
int dfa_mcq_tag_wakeup_all(struct blk_mq_tags *tags);
unsigned int dfa_mcq_tag_get(const struct blk_mq_alloc_data *data);
int dfa_mcq_tag_put(const struct blk_mq_hw_ctx *hctx,
	unsigned int tag, const struct request *rq);
void blk_mcq_tag_wakeup_all(struct blk_mq_tags *tags);
void blk_mcq_wake_waiters(struct request_queue *q);

/* mas_mcq_sysfs.c */
#ifdef CONFIG_MAS_DEBUG_FS
ssize_t mas_mcq_per_cpu_tags_show(struct request_queue *q, char *page);
ssize_t mas_mcq_dfa_enable_show(struct request_queue *q, char *page);
#endif
#endif
