/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: mas block busy idle framework
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <udk/log.h>
#include <libstrict/strict.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>
#include <udk/block/mas_blk/mas_blk_busy_idle_interface.h>
#include <udk/block/mas_blk/mas_blk_busy_idle_notify.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/mm.h>
#include <udk/delay.h>
#include <devhost/file.h>

#include "../udk_block_internal.h"

#define BLK_IO_IDLE_AVOID_JITTER_TIME 5
#define BUSYIDLE_HANDLE_MAX_MS 1000U
#define BUSYIDLE_WARNING_MS 10LL

void mas_blk_busyidle_handler_latency_check_timer_expire
	(struct udk_timer *timer)
{
	struct blk_busyidle_nb *nb = (struct blk_busyidle_nb *)container_of(timer,
		struct blk_busyidle_nb, handler_dur_check_timer);

	udk_error("%s: %s process time is more than %u ms\n", __func__,
		nb->event_node.subscriber, BUSYIDLE_HANDLE_MAX_MS);
}

static enum blk_busyidle_callback_ret handle_busyidle_event(
	unsigned long event, struct blk_busyidle_nb *nb,
	const struct blk_busyidle_event_node *node, int *handler_result)
{
	enum blk_busyidle_callback_ret ret = BUSYIDLE_NO_IO;

	switch (event) {
	case BLK_IDLE_NOTIFY:
		if (likely(nb->last_state == BLK_IO_BUSY)) {
			ret = node->busyidle_callback(
				(struct blk_busyidle_event_node *)node,
				BLK_IDLE_NOTIFY);
			nb->last_state = BLK_IO_IDLE;
			if (ret == BUSYIDLE_IO_TRIGGERED)
				*handler_result = UDK_NOTIFY_OK_STOP;
		}
		break;
	case BLK_BUSY_NOTIFY:
		if (likely(nb->last_state == BLK_IO_IDLE)) {
			ret = node->busyidle_callback(
				(struct blk_busyidle_event_node *)node,
				BLK_BUSY_NOTIFY);
			nb->last_state = BLK_IO_BUSY;
		}
		break;
	default:
		mas_blk_rdr_panic("Unknown busy idle event!");
		break;
	}

	return ret;
}

int mas_blk_busyidle_notify_handler(
	const struct udk_notifier *nb, unsigned long event, const void *v)
{
	int ret;
	int timer_ret;
	int handler_result = UDK_NOTIFY_OK;
	struct timespec start_ktime = { 0 };
	struct timespec end_ktime = { 0 };
	struct blk_busyidle_nb *busyidle_nb =
		container_of(nb, struct blk_busyidle_nb, busyidle_nb);
	struct blk_busyidle_event_node *node = &(busyidle_nb->event_node);
	long long diff;
	UNUSED(v);

	if (unlikely(!node->busyidle_callback)) {
		udk_error("%s: %s NULL callback\n", __func__, node->subscriber);
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic(NO_EXTRA_MSG);
#else
		return UDK_NOTIFY_OK;
#endif
	}

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &start_ktime);
	timer_ret = udk_timer_add(&busyidle_nb->handler_dur_check_timer);

	if (handle_busyidle_event(event, busyidle_nb, node, &handler_result) ==
		BUSYIDLE_ERR) {
		udk_error("%s: %s error!\n", __func__, node->subscriber);
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_show_stack();
#endif
	}

	if (timer_ret == E_HM_OK) {
retry:
		timer_ret = udk_timer_del_sync(&busyidle_nb->handler_dur_check_timer);
		if (timer_ret != E_HM_OK) {
			udk_error("udk_timer_del_sync fail\n");
			udk_mdelay(1000);
			goto retry;
		}
	}

	if ((ret == E_HM_OK) && (hm_clock_gettime(CLOCK_MONOTONIC, &end_ktime) == E_HM_OK)) {
		diff = mas_time_sub_ns(&end_ktime, &start_ktime) / NSEC_PER_MSEC;
		if (unlikely(diff > BUSYIDLE_WARNING_MS))
			udk_error("%s: %s busy idle callback cost %lld ms\n", __func__,
				node->subscriber, diff);
	}
	return handler_result;
}

static struct blk_busyidle_nb *busyidle_find_subscribed_node(
	const struct blk_idle_state *blk_idle,
	const struct blk_busyidle_event_node *node)
{
	struct blk_busyidle_nb *p = NULL;

	dlist_for_each_entry(p, &blk_idle->subscribed_list, struct blk_busyidle_nb, subscribed_node)
		if (!strcmp(node->subscriber, p->event_node.subscriber)
			&& node->busyidle_callback ==
			p->event_node.busyidle_callback)
			return p;

	return NULL;
}

static bool is_valid_event_node(
	const struct blk_busyidle_event_node *event_node)
{
	if (!event_node->busyidle_callback) {
		udk_error("%s: notifier_callback is NULL\n", __func__);
		return false;
	}

	if (strlen(event_node->subscriber) < 1 ||
		strlen(event_node->subscriber) >= SUBSCRIBER_NAME_LEN) {
		udk_error("%s: Invalid subscriber\n", __func__);
		return false;
	}

	return true;
}

static void setup_busy_idle_notify_nb(struct blk_busyidle_nb *nb)
{
	udk_timer_init(&nb->handler_dur_check_timer,
		__cfi_mas_blk_busyidle_handler_latency_check_timer_expire,
		(unsigned long)(uintptr_t)&nb->handler_dur_check_timer,
		BUSYIDLE_HANDLE_MAX_MS);
	nb->busyidle_nb.func = __cfi_mas_blk_busyidle_notify_handler;
	nb->busyidle_nb.priority = 0;
	nb->last_state = BLK_IO_IDLE;
}

static int mas_blk_busyidle_event_register(
	const struct blk_dev_lld *lld,
	const struct blk_busyidle_event_node *event_node)
{
	int ret;
	struct blk_busyidle_nb *nb = NULL;
	struct blk_idle_state *blk_idle =
		(struct blk_idle_state *)&lld->blk_idle;

	nb = mas_zmalloc(sizeof(struct blk_busyidle_nb));
	if (!nb)
		return E_HM_NOMEM;

	ret = memcpy_s(&nb->event_node, sizeof(struct blk_busyidle_event_node),
		event_node, sizeof(struct blk_busyidle_event_node));
	if (ret != 0) {
		udk_free(nb);
		udk_error("memcpy to event_node failed : %d\n", ret);
		return -EINVAL;
	}

	if (!is_valid_event_node(&nb->event_node)) {
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("Invalid event node!");
#else
		mas_blk_show_stack();
		ret = E_HM_INVAL;
		goto free_notify_nb;
#endif
	}

	raw_mutex_lock(&blk_idle->io_count_mutex);
	if (busyidle_find_subscribed_node(blk_idle, &nb->event_node)) {
		udk_error("%s: %s has been registered already!\n",
			__func__, nb->event_node.subscriber);
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic(NO_EXTRA_MSG);
#else
		mas_blk_show_stack();
		ret = E_HM_INVAL;
		goto unlock;
#endif
	}

	setup_busy_idle_notify_nb(nb);
	dlist_insert_tail(&blk_idle->subscribed_list, &nb->subscribed_node);
	ret = udk_blocking_notifier_list_insert(blk_idle->nh, (struct udk_notifier *)&nb->busyidle_nb);
	udk_error("%s registered to busy idle %d\n", nb->event_node.subscriber, ret);

#ifndef CONFIG_MAS_DEBUG_FS
unlock:
#endif
	raw_mutex_unlock(&blk_idle->io_count_mutex);
#ifndef CONFIG_MAS_DEBUG_FS
free_notify_nb:
	if (ret && nb)
		udk_free(nb);
	return ret;
#else
	return 0;
#endif
}

static int mas_blk_busyidle_event_unregister(
	const struct blk_busyidle_event_node *event_node)
{
	int ret;
	struct blk_busyidle_nb *nb = NULL;
	struct blk_dev_lld *lld = event_node->lld;
	struct blk_idle_state *blk_idle = &lld->blk_idle;

	raw_mutex_lock(&blk_idle->io_count_mutex);
	nb = busyidle_find_subscribed_node(blk_idle, event_node);
	if (!nb) {
		udk_error("Trying to unregister an unregistered event_node!\n");
		ret = -EINVAL;
		goto out;
	}

	ret = udk_blocking_notifier_list_delete(blk_idle->nh, (struct udk_notifier *)&nb->busyidle_nb);
	udk_error("%s unregistered from busy idle module ret %d\n",
		nb->event_node.subscriber, ret);
	dlist_delete(&nb->subscribed_node);
	udk_free(nb);

out:
	raw_mutex_unlock(&blk_idle->io_count_mutex);
	return ret;
}

static void blk_busy_to_idle(struct blk_idle_state *blk_idle)
{
	blk_idle->idle_state = BLK_IO_IDLE;
#ifdef CONFIG_MAS_DEBUG_FS
	blk_idle->total_idle_count++;
	blk_idle->last_idle_ktime_ret =
		hm_clock_gettime(CLOCK_MONOTONIC, &blk_idle->last_idle_ktime);
	if ((blk_idle->last_busy_ktime_ret == E_HM_OK) &&
		(blk_idle->last_idle_ktime_ret == E_HM_OK))
		blk_idle->total_busy_ktime +=
			mas_time_sub_ns(&blk_idle->last_idle_ktime, &blk_idle->last_busy_ktime);
#endif
	udk_blocking_notifier_list_call(blk_idle->nh,
		(unsigned long)BLK_IDLE_NOTIFY, NULL);
}

#define IDLE_WAIT_TIME_US 100
#define IDLE_RETRY_CNT 5
void mas_blk_idle_notify_work(unsigned long val)
{
	int ret;
	int retry_cnt = 0;
	struct udk_timer *timer = (struct udk_timer *)val;
	struct blk_idle_state *blk_idle = (struct blk_idle_state *)container_of(
		timer, struct blk_idle_state,
		idle_notify_worker);

	if (blk_idle->idle_intr_support) {
		vatomic32_init(&blk_idle->io_count, 0);
	}

retry:
	ret = raw_mutex_trylock(&blk_idle->io_count_mutex);
	if (ret != E_HM_OK) {
		if (vatomic32_read(&blk_idle->is_del_timer)) {
			return;
		}
		if (retry_cnt >= IDLE_RETRY_CNT) {
			return;
		}
		retry_cnt++;
		hm_usleep(IDLE_WAIT_TIME_US);
		goto retry;
	}

	if (!vatomic32_read(&blk_idle->io_count) &&
		(blk_idle->idle_state != BLK_IO_IDLE))
		blk_busy_to_idle(blk_idle);
	raw_mutex_unlock(&blk_idle->io_count_mutex);
}

static void blk_lld_idle_notify(struct blk_dev_lld *lld)
{
	if (likely(lld->init_magic == BLK_LLD_INIT_MAGIC)) {
		udk_timer_mod(&lld->blk_idle.idle_notify_worker, BLK_IO_IDLE_AVOID_JITTER_TIME);
	}
}

#ifdef CONFIG_MAS_DEBUG_FS
static void blk_busyidle_update_dur(struct blk_idle_state *blk_idle)
{
	long long blk_idle_dur_ms;

	if ((blk_idle->last_busy_ktime_ret != E_HM_OK) ||
		(blk_idle->last_idle_ktime_ret != E_HM_OK))
		return;

	blk_idle_dur_ms = mas_time_sub_ns(&blk_idle->last_busy_ktime,
		&blk_idle->last_idle_ktime) / NSEC_PER_MSEC;
	if (blk_idle_dur_ms > blk_idle->max_idle_dur)
		blk_idle->max_idle_dur = blk_idle_dur_ms;

	if (blk_idle_dur_ms < BLK_IDLE_100MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_100MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_500MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_500MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_1000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_1000MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_2000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_2000MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_4000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_4000MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_6000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_6000MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_8000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_8000MS]++;
	else if (blk_idle_dur_ms < BLK_IDLE_10000MS)
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_10000MS]++;
	else
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_FOR_AGES]++;
}

int mas_queue_busyidle_enable_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
{
	UNUSED(pos);
	struct udk_disk *disk = NULL;
	size_t len;
	char *write_buf = NULL;
	int ret;
	unsigned long val;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	write_buf = mas_malloc_sysfs_buf();
	if (write_buf == NULL) {
		return E_HM_NOMEM;
	}

	len = (size >= MAS_BLK_SYS_BUF_LEN) ? MAS_BLK_SYS_BUF_LEN : size;
	ret = udk_copy_from_user(write_buf, src, len);
	if (ret != E_HM_OK) {
		udk_free(write_buf);
		udk_warn("copy_from_user failed, ret=%d\n", ret);
		return ret;
	}

	val = strtoul(write_buf, NULL, MAS_DECIMAL);
	if (val) {
		udk_error("busy idle enable\n");
		blk_queue_busyidle_enable(disk->bd_queue, 1);
	} else {
		udk_error("busy idle disable\n");
		blk_queue_busyidle_enable(disk->bd_queue, 0);
	}

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}

int mas_queue_busyidle_statistic_reset_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
{
	UNUSED(pos);
	struct udk_disk *disk = NULL;
	size_t len;
	char *write_buf = NULL;
	int ret;
	unsigned long val;
	struct blk_dev_lld *lld = NULL;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	write_buf = mas_malloc_sysfs_buf();
	if (write_buf == NULL) {
		return E_HM_NOMEM;
	}

	len = (size >= MAS_BLK_SYS_BUF_LEN) ? MAS_BLK_SYS_BUF_LEN : size;
	ret = udk_copy_from_user(write_buf, src, len);
	if (ret != E_HM_OK) {
		udk_free(write_buf);
		udk_warn("copy_from_user failed, ret=%d\n", ret);
		return ret;
	}

	val = strtoul(write_buf, NULL, MAS_DECIMAL);
	lld = mas_blk_get_lld(disk->bd_queue);
	if (val && lld) {
		udk_error("io busy idle statistic result reset!\n");
		lld->blk_idle.total_busy_ktime = 0;
		lld->blk_idle.total_idle_ktime = 0;
		lld->blk_idle.total_idle_count = 0ULL;
		ret = memset_s(lld->blk_idle.blk_idle_dur, sizeof(lld->blk_idle.blk_idle_dur),
			0, sizeof(lld->blk_idle.blk_idle_dur));
		if (ret != 0) {
			udk_free(write_buf);
			return E_HM_INVAL;
		}

		lld->blk_idle.max_idle_dur = 0;
	}

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}

static size_t mas_queue_busyidle_get_statistic(struct udk_request_queue *q,
	char *buffer, size_t buffer_len)
{
	size_t offset = 0;
	int ret = 0;
	struct blk_dev_lld *lld = mas_blk_get_lld(q);
	struct blk_idle_state *blk_idle = &(lld->blk_idle);

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"Total Busy Time: %llu ms  Total Idle Time: %llu ms\r\n",
		lld->blk_idle.total_busy_ktime / NSEC_PER_MSEC,
		lld->blk_idle.total_idle_ktime / NSEC_PER_MSEC);
	if (ret > 0) {
		offset += ret;
	}
	
	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"Total Idle Count: %llu \r\n", lld->blk_idle.total_idle_count);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"block idle interval statistic:\n");
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"max_idle_duration: %lldms\n", blk_idle->max_idle_dur);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 100ms: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_100MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 500ms: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_500MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 1s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_1000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 2s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_2000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 4s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_4000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 6s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_6000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 8s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_8000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"less than 10s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_10000MS]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"more than 10s: %lld\n",
		blk_idle->blk_idle_dur[BLK_IDLE_DUR_IDX_FOR_AGES]);
	if (ret > 0) {
		offset += ret;
	}

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"idle total counts: %llu\n", blk_idle->total_idle_count);
	if (ret > 0) {
		offset += ret;
	}

	return (ssize_t)offset;
}

/* this is for debug purpose and page is long enough for now */
int mas_queue_busyidle_statistic_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if  ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	if (pos >= MAS_BLK_SYS_BUF_LEN) {
		*rsize = 0;
		return E_HM_OK;
	}

	buf = mas_malloc_sysfs_buf();
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	offset = mas_queue_busyidle_get_statistic(disk->bd_queue, buf, MAS_BLK_SYS_BUF_LEN);

	/* copy output buf to dst addr */
	buf_size = (size <= offset) ? size : offset;
	if (pos >= buf_size) {
		*rsize = 0;
		udk_free(buf);
		return E_HM_OK;
	}

	ret = udk_copy_to_user(dst, buf + pos, buf_size - pos);
	if (ret != E_HM_OK) {
		udk_free(buf);
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}

	udk_free(buf);
	*rsize = buf_size - pos;
	return E_HM_OK;
}

static int mas_queue_get_idle_state(struct udk_request_queue *q,
	char *buffer, size_t buffer_len)
{
	struct blk_dev_lld *lld = mas_blk_get_lld(q);
	struct blk_idle_state *blk_idle = &(lld->blk_idle);
	ssize_t n = 0;
	int ret = 0;

	ret = snprintf_s(buffer, buffer_len, buffer_len -1, "idle_state: %s\n",
		(blk_idle->idle_state == BLK_IO_IDLE) ? "idle" : "busy");
	if (ret > 0) {
		n += ret;
	}

	ret = snprintf_s(buffer + n, (buffer_len - n), (buffer_len - n - 1), "io_count: %d\n",
		vatomic32_read(&blk_idle->io_count));
	if (ret > 0) {
		n += ret;
	}

	return n;
}

int mas_queue_idle_state_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if  ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	if (pos >= MAS_BLK_SYS_BUF_LEN) {
		*rsize = 0;
		return E_HM_OK;
	}

	buf = mas_malloc_sysfs_buf();
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	offset = mas_queue_get_idle_state(disk->bd_queue, buf, MAS_BLK_SYS_BUF_LEN);

	/* copy output buf to dst addr */
	buf_size = (size <= offset) ? size : offset;
	if (pos >= buf_size) {
		*rsize = 0;
		udk_free(buf);
		return E_HM_OK;
	}

	ret = udk_copy_to_user(dst, buf + pos, buf_size - pos);
	if (ret != E_HM_OK) {
		udk_free(buf);
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}

	udk_free(buf);
	*rsize = buf_size - pos;
	return E_HM_OK;
}

#endif /* CONFIG_MAS_DEBUG_FS */

static void blk_idle_count(struct blk_dev_lld *lld)
{
	struct blk_idle_state *blk_idle = &lld->blk_idle;

	if (unlikely(!(lld->features & BLK_LLD_BUSYIDLE_SUPPORT)))
		return;

	if (blk_idle->idle_intr_support)
		return;
	if (unlikely(!vatomic32_read(&lld->blk_idle.io_count))) {
		udk_error("%s: io_count has been zero\n", __func__);
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic(NO_EXTRA_MSG);
#else
		return;
#endif
	}

	if (unlikely(!vatomic32_dec_get(&lld->blk_idle.io_count)))
		blk_lld_idle_notify(lld);
}

static void blk_idle_to_busy(struct blk_idle_state *blk_idle)
{
	blk_idle->idle_state = BLK_IO_BUSY;
#ifdef CONFIG_MAS_DEBUG_FS
	blk_idle->last_busy_ktime_ret =
		hm_clock_gettime(CLOCK_MONOTONIC, &blk_idle->last_busy_ktime);
	blk_busyidle_update_dur(blk_idle);
	if ((blk_idle->last_busy_ktime_ret == E_HM_OK) &&
		(blk_idle->last_idle_ktime_ret == E_HM_OK))
		blk_idle->total_idle_ktime +=
			mas_time_sub_ns(&blk_idle->last_busy_ktime, &blk_idle->last_idle_ktime);
#endif

	udk_blocking_notifier_list_call(blk_idle->nh, BLK_BUSY_NOTIFY, NULL);
}

static void blk_busy_count(struct blk_dev_lld *lld)
{
	struct blk_idle_state *blk_idle = &lld->blk_idle;

	if (unlikely(!(lld->features & BLK_LLD_BUSYIDLE_SUPPORT)))
		return;

	if (likely(vatomic32_read(&blk_idle->io_count)))
		goto inc_iocount;

	raw_mutex_lock(&blk_idle->io_count_mutex);
	vatomic32_init(&blk_idle->is_del_timer, 1);
	udk_timer_del_sync(&lld->blk_idle.idle_notify_worker);
	vatomic32_init(&blk_idle->is_del_timer, 0);

	/*
	 * Avoid twice busy event
	 * (The idle work may be canceled)
	 */
	if (blk_idle->idle_state == BLK_IO_IDLE)
		blk_idle_to_busy(blk_idle);

	raw_mutex_unlock(&blk_idle->io_count_mutex);

inc_iocount:
	vatomic32_inc(&blk_idle->io_count);
}

static void mas_blk_add_bio_to_counted_list(
	struct blk_dev_lld *lld, struct udk_bio *bio)
{
	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	lld->blk_idle.bio_count++;
	dlist_insert_tail(&lld->blk_idle.bio_list, &bio->cnt_list);
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);
}

static void mas_blk_remove_bio_from_counted_list(
	struct blk_dev_lld *lld, struct udk_bio *bio)
{
	if (unlikely(!lld->blk_idle.bio_count))
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("bio_count is zero!");
#else
		return;
#endif

	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	lld->blk_idle.bio_count--;
	dlist_delete(&bio->cnt_list);
	dlist_init(&bio->cnt_list);
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);
}

void mas_blk_add_rq_to_counted_list(
	struct blk_dev_lld *lld, struct udk_request *rq)
{
	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	lld->blk_idle.req_count++;
	dlist_insert_tail(&lld->blk_idle.req_list, &rq->cnt_list);
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);
}

static void mas_blk_remove_rq_from_counted_list(
	struct blk_dev_lld *lld, struct udk_request *rq)
{
	if (unlikely(!lld->blk_idle.req_count))
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("req_count is zero!");
#else
		return;
#endif

	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	lld->blk_idle.req_count--;
	dlist_delete(&rq->cnt_list);
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);
}

/*
 * This function is used to get the io count when a bio is submitted to block
 * layer
 */
void mas_blk_busyidle_check_bio(const struct udk_request_queue *q, struct udk_bio *bio)
{
	struct blk_dev_lld *lld = mas_blk_get_lld((struct udk_request_queue *)q);

	bio->mas_bio.q = (struct udk_request_queue *)q;

	if (unlikely(bio->mas_bio.io_in_count & MAS_IO_IN_COUNT_SET))
		return;

	bio->mas_bio.io_in_count |= MAS_IO_IN_COUNT_SET;
	mas_blk_add_bio_to_counted_list(lld, bio);
	blk_busy_count(lld);
}

/*
 * This function is used to get the io count when a request is inserted directly
 */
bool mas_blk_busyidle_check_request_bio(
	const struct udk_request_queue *q, const struct udk_request *rq)
{
	struct udk_bio *bio = rq->bio;

	if (bio) {
		do {
			mas_blk_busyidle_check_bio(q, bio);
			bio = bio->next;
		} while (bio);
		return true;
	}
	return false;
}

void mas_blk_busyidle_end_rq(const struct udk_request *rq, int error)
{
	struct blk_dev_lld *lld = mas_blk_get_lld(rq->queue);
	UNUSED(error);

	mas_blk_remove_rq_from_counted_list(lld, (struct udk_request *)rq);
	blk_idle_count(lld);
}

/*
 * This function is used to relase the io count when non-fs request end
 */
void mas_blk_busyidle_check_bio_endio(struct udk_bio *bio)
{
	struct blk_dev_lld *lld = NULL;
	struct udk_disk *disk = udkdev_to_disk(bio->bi_dev);
	struct udk_request_queue *q = NULL;

	if (disk != NULL) {
		q = disk->bd_queue;
	} else {
		q = bio->mas_bio.q;
	}
	if (!q)
		return;

	lld = mas_blk_get_lld(q);
	if (unlikely(!(bio->mas_bio.io_in_count & MAS_IO_IN_COUNT_SET)))
		return;

	mas_blk_remove_bio_from_counted_list(lld, bio);

	bio->mas_bio.io_in_count = 0;

	blk_idle_count(lld);
}

/*
 * This function is to subscriber busy idle event from the block device
 */
int blk_busyidle_event_subscribe(
	const struct udk_request_queue *q,
	const struct blk_busyidle_event_node *event_node)
{
	if (!q || !event_node)
		return -EINVAL;

	return blk_queue_busyidle_event_subscribe(q, event_node);
}

typedef int (*busyidle_fn)(void *, int);

struct hmfs_busyidle_data {
	busyidle_fn func;
	void *sbi;
};
/*
 * This function is to subscriber busy idle event for fs
 */
static enum blk_busyidle_callback_ret hmfs_blk_busyidle_callback(struct blk_busyidle_event_node *event_node,
		enum blk_idle_notify_state state)
{
	struct hmfs_busyidle_data *ptr = (struct hmfs_busyidle_data*)event_node->param_data;
	enum blk_busyidle_callback_ret ret;

    if (ptr == NULL || ptr->func == NULL) {
        return BUSYIDLE_ERR;
    }

	ret = ptr->func(ptr->sbi, (int)state);

	return ret;
}

int hmfs_blk_busyidle_event_subscribe(struct hmsrv_io_ctx *ctx, char *name,
		int namelen, void *priv, void **res)
{
	struct blk_busyidle_event_node *event_node;
	struct filp_node *node;
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_disk *disk = NULL;

	if (ctx == NULL) {
		udk_error("ctx is NULL\n");
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx != hm_ucap_self_cnode_idx()) {
		udk_error("udk is not used\n");
		return E_HM_INVAL;
	}

	node = u64_to_ptr(ctx->filp, struct filp_node);
	if (node == NULL) {
		udk_error("ctx->node is NULL\n");
		return E_HM_INVAL;
	}

	blkdev_ctx = (struct blkdev_context *)(node->filp);
	if (blkdev_ctx == NULL) {
		udk_error("blkdev_ctx is NULL\n");
		return E_HM_INVAL;
	}

	disk = udkdev_to_disk(blkdev_ctx->udkdev);
	if (disk == NULL) {
		udk_error("get block udk disk fail\n");
		return E_HM_INVAL;
	}

	event_node = mas_zmalloc(sizeof(struct blk_busyidle_event_node));
	if (event_node == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(strncpy_s(event_node->subscriber, SUBSCRIBER_NAME_LEN, name,
		namelen));
	event_node->busyidle_callback = hmfs_blk_busyidle_callback;
	event_node->param_data = priv;
	*res = event_node;

	udk_info("start register blk busyidle for hmfs\n");
	return blk_busyidle_event_subscribe(disk->bd_queue, event_node);
}

/*
 * This function is to subscriber busy idle event from the request queue
 */
int blk_queue_busyidle_event_subscribe(
	const struct udk_request_queue *q,
	const struct blk_busyidle_event_node *event_node)
{
	if (!q || !event_node)
		return -EINVAL;

	return blk_lld_busyidle_event_subscribe(
		mas_blk_get_lld((struct udk_request_queue *)q),
		(struct blk_busyidle_event_node *)event_node);
}

/*
 * This function is to subscriber busy idle event from the lld object
 */
int blk_lld_busyidle_event_subscribe(
	const struct blk_dev_lld *lld,
	struct blk_busyidle_event_node *event_node)
{
	if (!lld || !event_node)
		return -EINVAL;

	event_node->lld = (struct blk_dev_lld *)lld;
	return mas_blk_busyidle_event_register(lld, event_node);
}

/*
 * This function is to unsubscriber busy idle event from the block device
 */
int blk_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node)
{
	if (!event_node)
		return -EINVAL;

	return mas_blk_busyidle_event_unregister(event_node);
}

int hmfs_blk_busyidle_event_unsubscribe(void *pri)
{
	struct blk_busyidle_event_node *event_node = (struct blk_busyidle_event_node*)pri;
	int ret;

	if (event_node == NULL) {
		return E_HM_INVAL;
	}

	ret = blk_busyidle_event_unsubscribe(event_node);
	udk_free(event_node);

	return ret;
}

/*
 * This function is to unsubscriber busy idle event from the request queue
 */
int blk_queue_busyidle_event_unsubscribe(
	const struct blk_busyidle_event_node *event_node)
{
	if (!event_node)
		return -EINVAL;

	return mas_blk_busyidle_event_unregister(event_node);
}

/*
 * This function is to enable the busy idle on the request queue
 */
void blk_queue_busyidle_enable(const struct udk_request_queue *q, int enable)
{
	struct blk_dev_lld *blk_lld = NULL;

	if (!q)
		return;

	blk_lld = mas_blk_get_lld((struct udk_request_queue *)q);

	if (enable)
		blk_lld->features |= BLK_LLD_BUSYIDLE_SUPPORT;
	else
		blk_lld->features &= ~BLK_LLD_BUSYIDLE_SUPPORT;
}

/*
 * This function is to enable the busy idle on the MQ tagset
 */
void blk_mq_tagset_busyidle_enable(struct udk_blk_mq_tag_set *tag_set, int enable)
{
	struct blk_dev_lld *blk_lld = NULL;

	if (!tag_set)
		return;

	blk_lld = &tag_set->lld_func;

	if (enable)
		blk_lld->features |= BLK_LLD_BUSYIDLE_SUPPORT;
	else
		blk_lld->features &= ~BLK_LLD_BUSYIDLE_SUPPORT;
}

/* define 1 worker and 1 queue to process block works */
#define BUSYIDLE_WORK_NUM_DEFAULT 1
#define BUSYIDLE_WORK_PRIO_NUM 1
/* define workqueue priority 80 */
#define BUSYIDLE_QUEUE_PRIORITY 80

int mas_blk_busyidle_state_init(struct blk_idle_state *blk_idle)
{
	int ret = 0;
#ifdef CONFIG_MAS_DEBUG_FS
	blk_idle->total_busy_ktime = 0;
	blk_idle->total_idle_ktime = 0;
	blk_idle->last_idle_ktime_ret = E_HM_INVAL;
	blk_idle->last_busy_ktime_ret = E_HM_INVAL;
	ret = memset_s(&blk_idle->last_busy_ktime, sizeof(struct timespec),
		0, sizeof(struct timespec));
	if (ret != 0) {
		udk_error("memset to last_busy_ktime faied : %d\n", ret);
		return -EINVAL;
	}

	ret = memset_s(&blk_idle->last_idle_ktime, sizeof(struct timespec),
			0, sizeof(struct timespec));
	if (ret != 0) {
		udk_error("memset to last_idle_ktime faied : %d\n", ret);
		return -EINVAL;
	}

	ret = memset_s(blk_idle->blk_idle_dur, sizeof(blk_idle->blk_idle_dur),
		0, sizeof(blk_idle->blk_idle_dur));
	if (ret != 0) {
		udk_error("memset to blk_idle_dur faied : %d\n", ret);
		return -EINVAL;
	}

	blk_idle->max_idle_dur = 0;
	blk_idle->total_idle_count = 0ULL;
#endif

	blk_idle->idle_notify_delay_ms = BLK_IO_IDLE_AVOID_JITTER_TIME;

	udk_timer_init(&blk_idle->idle_notify_worker,
			__cfi_mas_blk_idle_notify_work,
			(unsigned long)(uintptr_t)&blk_idle->idle_notify_worker,
			BLK_IO_IDLE_AVOID_JITTER_TIME);

	vatomic32_init(&blk_idle->io_count, 0);
	vatomic32_init(&blk_idle->is_del_timer, 0);
	raw_mutex_init(&blk_idle->io_count_mutex);
	dlist_init(&blk_idle->subscribed_list);
	blk_idle->nh = udk_blocking_notifier_list_alloc();
	if (!blk_idle->nh) {
		udk_error("udk_blocking_notifier_list_alloc fail\n");
		return E_HM_NOMEM;
	}
	blk_idle->idle_state = BLK_IO_IDLE;

	blk_idle->bio_count = 0;
	blk_idle->req_count = 0;
	dlist_init(&blk_idle->bio_list);
	dlist_init(&blk_idle->req_list);
	uspinlock_init(&blk_idle->counted_list_lock);

	return 0;
}
