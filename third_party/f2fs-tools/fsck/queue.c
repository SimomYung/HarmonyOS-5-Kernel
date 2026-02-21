/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "fsck.h"

#ifdef POSIX_FADV_WILLNEED
struct work_reada_arg {
	struct f2fs_sb_info *sbi;
	int type;
};
static struct work_reada_arg *args[MAX_TYPE] = {NULL, NULL};

static void *work_reada_block(void *arg)
{
	struct f2fs_sb_info *sbi = ((struct work_reada_arg *)arg)->sbi;
	int type = ((struct work_reada_arg *)arg)->type;
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct ra_work *w, *t;
	struct list_head tmp_queue;

	for (; ;) {
		if ((fsck->quit_thread[type]) != 0) {
			return NULL;
		}

		pthread_mutex_lock(&fsck->mutex[type]);
		while (list_empty(&fsck->ra_queue[type]) != 0) {
			pthread_cond_wait(&fsck->cond[type], &fsck->mutex[type]);
			if (fsck->quit_thread[type] != 0) {
				pthread_mutex_unlock(&fsck->mutex[type]);
				return NULL;
			}
		}
		/* move all works to tmp_queue */
		tmp_queue = fsck->ra_queue[type];
		tmp_queue.next->prev = &tmp_queue;
		tmp_queue.prev->next = &tmp_queue;
		INIT_LIST_HEAD(&fsck->ra_queue[type]);
		pthread_mutex_unlock(&fsck->mutex[type]);

		list_for_each_entry_safe(w, t, &tmp_queue, entry) {
			list_del(&w->entry);
			if (!fsck->quit_thread[type])
				dev_reada_block(w->blkaddr);
			free(w);
		}
	}
}

void init_reada_queue(struct f2fs_sb_info *sbi)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	int i, ret;

	for (i = 0; i < MAX_TYPE; i++) {
		INIT_LIST_HEAD(&fsck->ra_queue[i]);
		fsck->quit_thread[i] = 0;
		pthread_cond_init(&fsck->cond[i], NULL);
		pthread_mutex_init(&fsck->mutex[i], NULL);

		args[i] = malloc(sizeof(struct work_reada_arg));
		if (args[i] == NULL) {
			MSG(0, "args[i] malloc failed\n");
			return;
		}
		ASSERT(args[i]);
		args[i]->sbi = sbi;
		args[i]->type = i;
		ret = pthread_create(&fsck->thread[i], NULL, work_reada_block, args[i]);
		if (ret != 0) {
            MSG(0, "pthread_create failed\n");
            return;
		}
	}

	MSG(0, "Info: readahead queue is enabled\n");
}

void exit_reada_queue(struct f2fs_sb_info *sbi)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct ra_work *w, *t;
	int i;

	/* tell thread to exit */
	for (i = 0; i < MAX_TYPE; i++) {
		fsck->quit_thread[i] = 1;
		pthread_cond_signal(&fsck->cond[i]);
	}
	for (i = 0; i < MAX_TYPE; i++) {
		pthread_join(fsck->thread[i], NULL);
		pthread_mutex_destroy(&fsck->mutex[i]);
		pthread_cond_destroy(&fsck->cond[i]);
		free(args[i]);
		args[i] = NULL;
		list_for_each_entry_safe(w, t, &fsck->ra_queue[i], entry) {
			list_del(&w->entry);
			free(w);
		}
	}
}

void queue_reada_block(struct f2fs_sb_info *sbi, block_t blkaddr, int type)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct ra_work *work;

	work = malloc(sizeof(struct ra_work));
	if (!work) {
		MSG(0, "work malloc failed\n");
		return;
	}

	INIT_LIST_HEAD(&work->entry);
	work->blkaddr = blkaddr;

	pthread_mutex_lock(&fsck->mutex[type]);
	list_add_tail(&work->entry, &fsck->ra_queue[type]);
	pthread_mutex_unlock(&fsck->mutex[type]);

	pthread_cond_signal(&fsck->cond[type]);
}

void build_sum_cache_list(struct f2fs_sb_info *sbi)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	int i, j;

	for (i = 0; i < MAX_TYPE; i++) {
		for (j = 0; j < HASHTABLE_SIZE; j++) {
			INIT_LIST_HEAD(&fsck->sum_cache_head[i][j]);
			fsck->sum_cache_cnt[i][j] = 0;
		}
	}
}

void destroy_sum_cache_list(struct f2fs_sb_info *sbi)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct sum_cache *sum, *tmp;
	struct list_head *head;
	int i, j;

	for (i = 0; i < MAX_TYPE; i++) {
		for (j = 0; j < HASHTABLE_SIZE; j++) {
			head = &fsck->sum_cache_head[i][j];
			list_for_each_entry_safe(sum, tmp, head, list) {
				list_del(&sum->list);
				free(sum->sum_blk);
				free(sum);
			}
			fsck->sum_cache_cnt[i][j] = 0;
		}
	}
}

struct f2fs_summary_block *get_sum_node_block_from_cache(struct f2fs_sb_info *sbi,
				unsigned int segno, int *type)
{
	struct f2fs_checkpoint *cp = F2FS_CKPT(sbi);
	struct curseg_info *curseg;
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct sum_cache *sum, *tmp;
	struct list_head *head;
	int tb_idx, i;

	for (i = 0; i < NR_CURSEG_NODE_TYPE; i++) {
		if (segno == get_cp(cur_node_segno[i])) {
			curseg = CURSEG_I(sbi, CURSEG_HOT_NODE + i);
			if (!IS_SUM_NODE_SEG(curseg->sum_blk->footer)) {
				ASSERT_MSG("segno [0x%x] indicates a data "
						"segment, but should be node",
								segno);
				*type = -SEG_TYPE_CUR_NODE;
			} else {
				*type = SEG_TYPE_CUR_NODE;
			}
			return curseg->sum_blk;
		}
	}

	tb_idx = segno % HASHTABLE_SIZE;
	head = &fsck->sum_cache_head[SUM_TYPE_NODE][tb_idx];
	list_for_each_entry_safe(sum, tmp, head, list) {
		if (sum->segno != segno)
			continue;
		list_move_tail(&sum->list, head);
		*type = SEG_TYPE_NODE;
		return sum->sum_blk;
	}

	return NULL;
}

struct f2fs_summary_block *get_sum_data_block_from_cache(struct f2fs_sb_info *sbi,
				unsigned int segno, int *type)
{
	struct f2fs_checkpoint *cp = F2FS_CKPT(sbi);
	struct curseg_info *curseg;
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct sum_cache *sum, *tmp;
	struct list_head *head;
	int tb_idx, i;

	for (i = 0; i < NR_CURSEG_DATA_TYPE; i++) {
		if (segno == get_cp(cur_data_segno[i])) {
			curseg = CURSEG_I(sbi, i);
			if (IS_SUM_NODE_SEG(curseg->sum_blk->footer)) {
				ASSERT_MSG("segno [0x%x] indicates a node "
						"segment, but should be data",
								segno);
				*type = -SEG_TYPE_CUR_DATA;
			} else {
				*type = SEG_TYPE_CUR_DATA;
			}
			return curseg->sum_blk;
		}
	}

	tb_idx = segno % HASHTABLE_SIZE;
	head = &fsck->sum_cache_head[SUM_TYPE_DATA][tb_idx];
	list_for_each_entry_safe(sum, tmp, head, list) {
		if (sum->segno != segno)
			continue;
		/* sum cache hit, move it to the end of the list */
		list_move_tail(&sum->list, head);
		*type = SEG_TYPE_DATA;
		return sum->sum_blk;
	}

	return NULL;
}

void add_sum_block_to_cache(struct f2fs_sb_info *sbi, unsigned int segno,
				int type, struct f2fs_summary_block *blk)
{
	struct f2fs_fsck *fsck = F2FS_FSCK(sbi);
	struct sum_cache *sum;
	int sum_type = type == SEG_TYPE_NODE ? SUM_TYPE_NODE : SUM_TYPE_DATA;
	int tb_idx = segno % HASHTABLE_SIZE;
	int cnt = fsck->sum_cache_cnt[sum_type][tb_idx];
	struct list_head *head = &fsck->sum_cache_head[sum_type][tb_idx];

	if (cnt < MAX_SUM_CACHE_CNT) {
		/* add to list */
		sum = malloc(sizeof(struct sum_cache));
		if (!sum) {
			MSG(0, "Memory allocation failed\n");
			return;
		}
		sum->segno = segno;
		sum->sum_blk = blk;
		INIT_LIST_HEAD(&sum->list);
		list_add_tail(&sum->list, head);
		fsck->sum_cache_cnt[sum_type][tb_idx]++;
	} else {
		/* cache list full, replace the oldest one */
		sum = list_first_entry(head, struct sum_cache, list);
		free(sum->sum_blk);
		sum->sum_blk = NULL;
		sum->segno = segno;
		sum->sum_blk = blk;
		list_move_tail(&sum->list, head);
	}
}

#endif // POSIX_FADV_WILLNEED