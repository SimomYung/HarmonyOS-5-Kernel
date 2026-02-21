/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#ifndef DPU_DEV_PRESENT_VOTE_H
#define DPU_DEV_PRESENT_VOTE_H
#include <linux/semaphore.h>
#include <linux/wait.h>
#include "dpu_comp_mgr.h"

struct present_vote {
    int32_t vote_num;
    struct semaphore vote_sem;
    wait_queue_head_t vote_num_wait;
};
void init_present_vote(void);
int32_t increase_present_vote(struct dpu_composer *dpu_comp);
int32_t decrease_present_vote(void);
void wait_for_zero_votes_and_sema_down(void);
void present_vote_sema_down(void);
void present_vote_sema_up(void);
#endif