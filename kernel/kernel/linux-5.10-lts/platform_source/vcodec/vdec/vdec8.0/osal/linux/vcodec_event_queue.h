/*
 * drv_venc_osal.h
 *
 * This is for venc drv.
 *
 * Copyright (c) 2009-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __DRV_VDEC_OSAL_H__
#define __DRV_VDEC_OSAL_H__

#include <linux/rtc.h>
#include <linux/kfifo.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include "dbg.h"
#include "vcodec_osal.h"


#define VCODEC_FAILURE       (-1)
typedef struct
{
	wait_queue_head_t queue_head;
	bool is_done;
} vcodec_event;

#define time_period(begin, end) (((end) >= (begin)) ? ((end) - (begin)) : (~0LL - (begin) + (end)))

#define OSAL_WAIT_EVENT_TIMEOUT(event, condtion, timeout_in_ms) \
({ \
    int _ret = timeout_in_ms; \
    uint64_t start_time, cur_time; \
    start_time = vcodec_osal_get_time_in_ms(); \
    while (!(condtion) && (_ret != 0)) { \
        _ret = wait_event_interruptible_timeout(((event)->queue_head), (condtion), \
            (long)(msecs_to_jiffies(timeout_in_ms))); \
        if (_ret < 0) { \
            cur_time = vcodec_osal_get_time_in_ms(); \
            if (time_period(start_time, cur_time) > (uint64_t)(timeout_in_ms)) { \
                dprint(PRN_ERROR, "waitevent time out, time : %llu", \
                    time_period(start_time, cur_time)); \
                _ret = 0; \
                break; \
            } \
        } \
    } \
    if (_ret == 0) \
        dprint(PRN_WARN, "waitevent timeout"); \
    if ((condtion)) { \
        _ret = 0; \
    } else { \
        _ret = VCODEC_FAILURE; \
    } \
    _ret; \
})

#define queue_is_empty(queue) kfifo_is_empty(&queue->fifo)
#define queue_is_null(queue) (queue->fifo.kfifo.data == NULL)

#define create_queue(type) \
({ \
    type *queue = NULL; \
    type *tmp = NULL; \
    tmp = (type *)kzalloc(sizeof(type), GFP_KERNEL); \
    if (!IS_ERR_OR_NULL(tmp)) { \
        if (vcodec_lock_create(&tmp->lock)) { \
            dprint(PRN_ERROR, "alloc lock failed"); \
            kfree(tmp); \
        } else { \
            vcodec_init_event(&tmp->event); \
            queue = tmp; \
        } \
    } else { \
        dprint(PRN_ERROR, "alloc queue failed"); \
    } \
    queue; \
})

#define alloc_queue(queue, size) \
({ \
    int32_t _ret = 0; \
    if (kfifo_alloc(&queue->fifo, size, GFP_KERNEL)) { \
        _ret = -ENOMEM; \
        dprint(PRN_ERROR, "alloc kfifo failed"); \
    } \
    _ret; \
})

/* queue->fifo.kfifo.data is set to NULL in kfifo_free. */
#define free_queue(queue) \
({ \
    unsigned long _flag; \
    spin_lock_irqsave(queue->lock, _flag); \
    if (!queue_is_null(queue)) \
        kfifo_free(&queue->fifo); \
    spin_unlock_irqrestore(queue->lock, _flag); \
})

#define destroy_queue(queue) \
({ \
    vcodec_lock_destroy(queue->lock); \
    kfree(queue); \
    queue = NULL; \
})

#define pop(queue, buf) \
({ \
    uint32_t _len; \
    int32_t _ret = -EFAULT; \
    unsigned long _flag; \
    spin_lock_irqsave(queue->lock, _flag); \
    do { \
        if (queue_is_null(queue)) { \
            dprint(PRN_ERROR, "pop data is failed"); \
            break; \
        } \
        _len = kfifo_out(&queue->fifo, buf, 1); \
        _ret = (_len == 1) ? 0 : -EFAULT; \
    } while (0); \
    spin_unlock_irqrestore(queue->lock, _flag); \
    _ret; \
})

#define push(queue, buf) \
({ \
    uint32_t _len; \
    int32_t _ret = -ENOSPC; \
    unsigned long _flag; \
    spin_lock_irqsave(queue->lock, _flag); \
    do { \
        if (queue_is_null(queue)) { \
            dprint(PRN_ERROR, "push data is failed"); \
            break; \
        } \
        _len = kfifo_in(&queue->fifo, buf, 1); \
        _ret = (_len == 1) ? 0 : -ENOSPC; \
    } while (0); \
    spin_unlock_irqrestore(queue->lock, _flag); \
    _ret; \
})


int32_t vcodec_lock_create(spinlock_t **phlock);
void vcodec_lock_destroy(spinlock_t *hlock);

int32_t vcodec_init_event(vcodec_event *event);
int32_t vcodec_give_event(vcodec_event *event);

#endif

