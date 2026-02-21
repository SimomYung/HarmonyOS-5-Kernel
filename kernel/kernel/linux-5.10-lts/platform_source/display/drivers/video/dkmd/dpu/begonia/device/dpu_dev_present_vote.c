/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "dpu_dev_present_vote.h"
#include "dpu_gfx.h"
#include "dkmd_log.h"

#define VOTE_NUM_WAITE_TIME (300)

static struct present_vote g_present_vote;
 
void init_present_vote(void)
{
    dpu_pr_info("init_present_vote enter");
    g_present_vote.vote_num = 0;
    sema_init(&g_present_vote.vote_sem, 1);
    init_waitqueue_head(&g_present_vote.vote_num_wait);
    return;
}
 
int32_t increase_present_vote(struct dpu_composer *dpu_comp)
{
    if (unlikely(!dpu_comp))
        return -1;
    down(&g_present_vote.vote_sem);
    if (!composer_check_power_status(dpu_comp)) {
        dpu_pr_err("comp is power down");
        up(&g_present_vote.vote_sem);
        return -1;
    }
    g_present_vote.vote_num++;
    up(&g_present_vote.vote_sem);
    return 0;
}
 
int32_t decrease_present_vote(void)
{
    down(&g_present_vote.vote_sem);
    if (g_present_vote.vote_num > 0) {
        g_present_vote.vote_num--;
        if (g_present_vote.vote_num == 0) {
            wake_up_interruptible_all(&g_present_vote.vote_num_wait);
        }
    } else {
        dpu_pr_err("vote_num is less than 0");
        up(&g_present_vote.vote_sem);
        return -1;
    }
    up(&g_present_vote.vote_sem);
    return 0;
}
 
void present_vote_sema_down(void)
{
    down(&g_present_vote.vote_sem);
}

void present_vote_sema_up(void)
{
    up(&g_present_vote.vote_sem);
}

void wait_for_zero_votes_and_sema_down(void)
{
    int ret;
    down(&g_present_vote.vote_sem);
    while (g_present_vote.vote_num != 0) {
        up(&g_present_vote.vote_sem);
        ret = wait_event_interruptible_timeout(
            g_present_vote.vote_num_wait, g_present_vote.vote_num == 0, (long)(msecs_to_jiffies(VOTE_NUM_WAITE_TIME)));
        if (ret <= 0) {
            dpu_pr_err("Timeout waiting vote_num == 0, ret = %d", ret);
            return;
        }
        down(&g_present_vote.vote_sem); /* Ensure that registers cannot be set after power-off */
    }
}