/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _MMC_CORE_BLOCK_H
#define _MMC_CORE_BLOCK_H

#ifdef CONFIG_ZODIAC_SHARE_MMC
enum mmc_part_switch {
	NO_NEED_PART_SWITCH,
	NEED_PART_SWITCH,
};
#endif

struct mmc_queue;
struct request;

void mmc_blk_cqe_recovery(struct mmc_queue *mq);

enum mmc_issued;

enum mmc_issued mmc_blk_mq_issue_rq(struct mmc_queue *mq, struct request *req);
void mmc_blk_mq_complete(struct request *req);
void mmc_blk_mq_recovery(struct mmc_queue *mq);

struct work_struct;

void mmc_blk_mq_complete_work(struct work_struct *work);

#endif
