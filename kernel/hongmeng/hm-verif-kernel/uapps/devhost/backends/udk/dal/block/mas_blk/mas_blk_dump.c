/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description:  mas block dump
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>
#include <udk/block/mas_blk/mas_blk_dump_interface.h>
#include <udk/block/mas_blk/mas_blk_latency_interface.h>
#include <udk/block/mas_blk/mas_blk_core_interface.h>

static DLIST_HEAD(dump_list);
static struct uspinlock_s dump_list_lock = USPINLOCK_INITIALIZER;

#define MAS_BLK_DUMP_BUF_LEN 512UL
static int mas_blk_io_type_op(unsigned long op, char *type_buf, int len)
{
	if (len < 1)
		return 0;

	switch (op) {
	case UDK_REQ_READ:
		type_buf[0] = 'R';
		break;
	case UDK_REQ_WRITE:
		type_buf[0] = 'W';
		break;
	case UDK_REQ_DISCARD:
		type_buf[0] = 'D';
		break;
	case UDK_REQ_FLUSH:
		type_buf[0] = 'F';
		break;
	default:
		udk_warn("%s: Unknown OP %ld!\n", __func__, op);
		return 0;
	}

	return 1;
}

static int mas_blk_io_type_flag(unsigned long flag, char *type_buf, int len)
{
	int offset = 0;

	if ((flag & UDK_REQ_FUA) && offset < len) {
		type_buf[offset] = 'A';
		offset++;
	}
	if ((flag & UDK_REQ_FLUSH) && offset < len) {
		type_buf[offset] = 'F';
		offset++;
	}

	return offset;
}

static int mas_blk_io_type_priv_flag(unsigned long mas_flag, char *type_buf, unsigned int len)
{
	int offset = 0;
	unsigned long priv_flag = mas_flag;

	if ((priv_flag & MAS_REQ_FG) && offset < (int)len) {
		type_buf[offset] = 'H';
		offset++;
	}
	if ((priv_flag & REQ_CP) && offset < (int)len) {
		type_buf[offset] = 'C';
		offset++;
	}
	if ((priv_flag & REQ_TZ) && offset < (int)len) {
		type_buf[offset] = 'Z';
		offset++;
	}
	if ((priv_flag & REQ_VIP) && offset < (int)len) {
		type_buf[offset] = 'V';
		offset++;
	}
	if ((priv_flag & REQ_TT_RB) && offset < (int)len) {
		type_buf[offset] = 'T';
		offset++;
	}
	if ((priv_flag & MAS_REQ_SYNC) && offset < (int)len) {
		type_buf[offset] = 'S';
		offset++;
	}
	return offset;
}

static char *mas_blk_io_type(unsigned long op, unsigned long flag,
	unsigned long mas_flag, char *type_buf, int len)
{
	int offset = 0;

	offset += mas_blk_io_type_op(op, type_buf, len);
	offset += mas_blk_io_type_flag(flag, type_buf + offset, len - offset);
	offset += mas_blk_io_type_priv_flag(mas_flag, type_buf + offset, len - offset);
	type_buf[offset] = '\0';

	return type_buf;
}

static char *mas_blk_io_type_rq(
	struct udk_request *rq, char *type_buf, int len, struct udk_bio *bio)
{
	if (!bio)
		return "bio null";

	return mas_blk_io_type(
		udk_bio_req_op(bio), rq->cmd_flags, rq->mas_cmd_flags, type_buf, len);
}

/*
 * This function is used to dump the request info
 */
static void mas_blk_dump_request(struct udk_request *rq, enum blk_dump_scene s, struct udk_bio *bio)
{
	int i;
	char log[MAS_BLK_DUMP_BUF_LEN];
	unsigned int count = 0;
	int ret = 0;
	char io_type[IO_BUF_LEN];
	char *type = mas_blk_io_type_rq(
		rq, io_type, sizeof(io_type), bio);
	char *prefix = mas_blk_prefix_str(s);

	ret = snprintf_s(log, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"rq: 0x%llx, type:%s len:%u requeue_reason: %d tag: %d ",
			(unsigned long long)mas_get_debug_addr((void *)rq), type, rq->data_len,
			rq->mas_req.requeue_reason, rq->tag);
	if (ret > 0) {
		count += ret;
	}

	ret = snprintf_s(log + count, MAS_BLK_DUMP_BUF_LEN - count, MAS_BLK_DUMP_BUF_LEN - count - 1,
			"rq_hoq_flag: %llx rq_cp_flag: %llx ",
			req_hoq(rq), req_cp(rq));
	if (ret > 0) {
		count += ret;
	}

	ret = snprintf_s(log + count, MAS_BLK_DUMP_BUF_LEN - count, MAS_BLK_DUMP_BUF_LEN - count - 1,
			"\n");
	if (ret > 0) {
		count += ret;
	}

	for (i = 0; i < REQ_PROC_STAGE_MAX; i++) {
		if (!rq->mas_req.req_stage_ktime_flag[i])
			continue;
		if (rq->mas_req.req_stage_ktime_ret[i] == E_HM_OK)
			ret = snprintf_s(log + count,
				(MAS_BLK_DUMP_BUF_LEN - count), MAS_BLK_DUMP_BUF_LEN - count - 1,
				"%s: %ld.%ld ", req_stage_cfg[i].stage_name,
				rq->mas_req.req_stage_ktime[i].tv_sec,
				rq->mas_req.req_stage_ktime[i].tv_nsec);
		else
			ret = snprintf_s(log + count,
				(MAS_BLK_DUMP_BUF_LEN - count), MAS_BLK_DUMP_BUF_LEN - count - 1,
				"%s: null ", req_stage_cfg[i].stage_name);
		if (ret > 0) {
			count += ret;
		}
	}

	log[MAS_BLK_DUMP_BUF_LEN - 1] = 0;
	udk_error("%s: %s\n", prefix, log);
}

/*
 * This function is used to dump the bio info
 */
void mas_blk_dump_bio(const struct udk_bio *bio, enum blk_dump_scene s)
{
	int i;
	int ret = 0;
	char log[MAS_BLK_DUMP_BUF_LEN];
	int count = 0;
	struct blk_bio_cust *mas_bio = (struct blk_bio_cust *)&bio->mas_bio;
	char io_type[IO_BUF_LEN];
	char *type = mas_blk_io_type(
		udk_bio_req_op(bio), udk_bio_req_op(bio), bio->mas_bi_opf, io_type, sizeof(io_type));
	char *prefix = mas_blk_prefix_str(s);
	struct timespec current;

	ret = snprintf_s(log, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"bio: 0x%llx, type:%s len:%u ",
		(unsigned long long)mas_get_debug_addr((void *)bio),
		type, bio->bi_size);
	if (ret > 0) {
		count += ret;
	}

	for (i = 0; i < BIO_PROC_STAGE_MAX; i++) {
		if (!(mas_bio->bio_stage_ktime_flag[i]))
			continue;
		if (mas_bio->bio_stage_ktime_ret[i] == E_HM_OK)
			ret = snprintf_s(log + count, MAS_BLK_DUMP_BUF_LEN - count,
				MAS_BLK_DUMP_BUF_LEN - count - 1,
				"<%s:%ld.%ld> ", bio_stage_cfg[i].stage_name,
				mas_bio->bio_stage_ktime[i].tv_sec,
				mas_bio->bio_stage_ktime[i].tv_nsec);
		else
			ret = snprintf_s(log + count, MAS_BLK_DUMP_BUF_LEN - count,
				MAS_BLK_DUMP_BUF_LEN - count - 1,
				"<%s:null> ", bio_stage_cfg[i].stage_name);
		if (ret > 0) {
			count += ret;
		}
	}
	log[count] = '\0';
	if (hm_clock_gettime(CLOCK_MONOTONIC, &current) == E_HM_OK)
		udk_error("%s %s,current: %ld.%ld\n", prefix, log, current.tv_sec, current.tv_nsec);
	else
		udk_error("%s %s,current fail\n", prefix, log);

	if (bio->mas_bio.io_req) {
		if (s != BLK_DUMP_PANIC) {
			bio->mas_bio.io_req->mas_req.real_req = bio->mas_bio.io_req;
		}
		mas_blk_dump_request(bio->mas_bio.io_req->mas_req.real_req, s, bio);
	}
}

static int __mas_blk_dump_printf(char *buf, int len, const char *str)
{
	int offset = 0;
	int ret = 0;

	if (buf && len > 0) {
		ret = snprintf_s(buf, len, len - 1, "%s", str);
		if (ret > 0) {
			offset += ret;
		}
	} else {
		udk_error("%s", str);
	}

	return offset;
}

static int mas_blk_q_dump_header(char *buf, unsigned long len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"Queue Status:\n");
	return __mas_blk_dump_printf(buf, len, tmp_str);
}

static int mas_blk_q_dump_opt_features(
	struct udk_request_queue *q, char *buf, unsigned long len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];
	int offset = 0;
	int ret = 0;

	if (q->mas_queue.io_latency_enable)
		ret = snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tIO Latency Warning: enable\tThreshold(ms): %u\n",
			q->mas_queue.io_lat_warning_thresh);
	else
		ret = snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tIO Latency Warning: disable\n");

	if (ret < 0) {
		udk_error("%s write buffer failed : %d\n", __func__, ret);
		return -EINVAL;
	}

	offset += __mas_blk_dump_printf(buf, len, tmp_str);

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tFlush Reduce: %s\n",
		q->mas_queue.flush_optimize ? "enable" : "disable");
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);
	return offset;
}

int mas_blk_dump_queue_status2(
	struct udk_request_queue *q, char *buf, int len)
{
	int offset = 0;

	offset += mas_blk_q_dump_header(buf, len);
	offset += mas_blk_q_dump_opt_features(q, buf + offset, len - offset);
	if (buf)
		buf[offset] = '\0';

	return offset;
}

/*
 * This function is used to dump request queue status
 */
void mas_blk_dump_queue_status(
	const struct udk_request_queue *q, enum blk_dump_scene s)
{
#ifdef CONFIG_MAS_DEBUG_FS
	udk_error("q: 0x%llx\n", (unsigned long long)(uintptr_t)q);
#endif

	if (s == BLK_DUMP_PANIC)
		mas_blk_dump_queue_status2((struct udk_request_queue *)q, NULL, 0);

	if (q->mas_queue_ops && q->mas_queue_ops->blk_status_dump_fn)
		q->mas_queue_ops->blk_status_dump_fn(
			(struct udk_request_queue *)q, s);
}

static int mas_blk_lld_dump_latency(
	struct blk_dev_lld *lld, char *buf, int len)
{
	int ret = 0;
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];

	if (lld->features & BLK_LLD_LATENCY_SUPPORT)
		ret = snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tLatency Warning:enable\tThreshold(ms):%u\n",
			lld->latency_warning_threshold_ms);
	else
		ret = snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tLatency Warning: disable\n");
	
	if (ret < 0) {
		udk_error("%s write buffer failed : %d\n", __func__, ret);
		return -EINVAL;
	}
	
	return __mas_blk_dump_printf(buf, len, tmp_str);
}

static int mas_blk_lld_dump_ioscheduler(
	struct blk_dev_lld *lld, char *buf, int len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];
	int offset = 0;

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tIO Scheduler: ");
	offset += __mas_blk_dump_printf(buf, len, tmp_str);
#ifdef CONFIG_MAS_BLK_NVME
	if (lld->features & BLK_LLD_IOSCHED_NVME_MQ)
#else
	if (lld->features & BLK_LLD_IOSCHED_UFS_MQ)
#endif
		(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"MAS MQ\n");
	else
		(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"Default Scheduler\n");
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);

	return offset;
}

static int mas_blk_lld_dump_busyidle(
	struct blk_dev_lld *lld, char *buf, int len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];
	int offset = 0;

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tBusy/Idle Notifier: %s\t",
		(lld->features & BLK_LLD_BUSYIDLE_SUPPORT) ? "enable"
							     : "disable");
	offset += __mas_blk_dump_printf(buf, len, tmp_str);

	switch (lld->blk_idle.idle_state) {
	case BLK_IO_BUSY:
		(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tState: BUSY\t");
		break;
	case BLK_IO_IDLE:
		(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
			"\tState: IDLE\t");
		break;
	default:
		return offset;
	}
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tIO Count: %u\n",
		vatomic32_read(&lld->blk_idle.io_count));
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);

	return offset;
}

static int mas_blk_lld_dump_opt_feature(
	struct blk_dev_lld *lld, char *buf, int len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];
	int offset = 0;

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tFlush Reduce: %s\n",
		(lld->features & BLK_LLD_FLUSH_REDUCE_SUPPORT) ? "enable" :
								"disable");
	offset += __mas_blk_dump_printf(buf, len, tmp_str);
#ifndef CONFIG_MAS_BLK_NVME
	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tUFS command priority: %s\n",
		mas_blk_get_cp_flag() ? "enable" : "disable");
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tUFS order preserving: %s\n",
		(lld->features & BLK_LLD_UFS_ORDER_EN) ? "enable" : "disable");
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);
#endif
	return offset;
}

static int mas_blk_lld_dump_header(
	struct blk_dev_lld *lld, char *buf, int len)
{
	char tmp_str[MAS_BLK_DUMP_BUF_LEN];
	int offset = 0;

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"LLD Status:\n");
	offset += __mas_blk_dump_printf(buf, len, tmp_str);

	(void)snprintf_s(tmp_str, MAS_BLK_DUMP_BUF_LEN, MAS_BLK_DUMP_BUF_LEN - 1,
		"\tPanic Dump: %s\n",
		(lld->features & BLK_LLD_DUMP_SUPPORT) ? "enable" : "disable");
	// cppcheck-suppress *
	offset += __mas_blk_dump_printf(buf + offset, len - offset, tmp_str);

	return offset;
}

/*
 * This function is used to dump low level driver object status
 */
int mas_blk_dump_lld_status(struct blk_dev_lld *lld, char *buf, int len)
{
	int offset = 0;

	offset += mas_blk_lld_dump_header(lld, buf, len);
	offset += mas_blk_lld_dump_latency(lld, buf + offset, len - offset);
	offset += mas_blk_lld_dump_busyidle(lld, buf + offset, len - offset);
	offset += mas_blk_lld_dump_ioscheduler(lld, buf + offset, len - offset);
	offset += mas_blk_lld_dump_opt_feature(lld, buf + offset, len - offset);
	if (buf)
		buf[offset] = '\0';

	return offset;
}

#define MAX_DUMP_IO_CNT 32
static struct udk_bio g_dump_bio_list[MAX_DUMP_IO_CNT];
static struct udk_request g_dump_req_list[MAX_DUMP_IO_CNT];
static static struct raw_mutex dump_io_mutex = RAW_MUTEX_INITIALIZER;

static void mas_blk_dump_counted_io(
	struct blk_dev_lld *lld, struct udk_request_queue *q,
	enum blk_dump_scene s)
{
	int io_cnt = 0;
	int loop;
	int ret = 0;

	raw_mutex_lock(&dump_io_mutex);
	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	if (!dlist_empty(&lld->blk_idle.bio_list)) {
		struct udk_bio *pos = NULL;

		dlist_for_each_entry(pos, &lld->blk_idle.bio_list, struct udk_bio, cnt_list) {
			if (pos->mas_bio.q == q) {
				ret = memcpy_s(&g_dump_bio_list[io_cnt], sizeof(struct udk_bio),
					pos, sizeof(struct udk_bio));
				if (ret != 0) {
					uspinlock_unlock(&lld->blk_idle.counted_list_lock);
					raw_mutex_unlock(&dump_io_mutex);
					return;
				}

				if (pos->mas_bio.io_req != NULL) {
					ret = memcpy_s(&g_dump_req_list[io_cnt], sizeof(struct udk_request),
						pos->mas_bio.io_req, sizeof(struct udk_request));
					if (ret != 0) {
						uspinlock_unlock(&lld->blk_idle.counted_list_lock);
						raw_mutex_unlock(&dump_io_mutex);
						return;
					}

					g_dump_bio_list[io_cnt].mas_bio.io_req = &g_dump_req_list[io_cnt];
					g_dump_req_list[io_cnt].mas_req.real_req = &g_dump_req_list[io_cnt];
				} else {
					g_dump_bio_list[io_cnt].mas_bio.io_req = NULL;
				}
				io_cnt++;
				if (io_cnt >= MAX_DUMP_IO_CNT) {
					break;
				}
			}
		}
	}
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);

	if (io_cnt > 0) {
		udk_error("bio_list:\n");
	}

	for (loop = 0; loop < io_cnt; loop++) {
		mas_blk_dump_bio(&g_dump_bio_list[loop], s);
	}
	raw_mutex_unlock(&dump_io_mutex);
}

static void mas_blk_dump_queue(
	struct udk_request_queue *q, enum blk_dump_scene s)
{
	struct blk_dev_lld *lld = mas_blk_get_lld(q);

	if (lld->dump_fn) {
		mas_blk_dump_lld_status(lld, NULL, 0);
		udk_error("bio_count = %lu req_count = %lu\n",
			lld->blk_idle.bio_count, lld->blk_idle.req_count);
		lld->dump_fn(q, BLK_DUMP_PANIC);
		lld->dump_fn = NULL;
	}

	mas_blk_dump_queue_status(q, BLK_DUMP_PANIC);

	mas_blk_dump_counted_io(lld, q, s);
}

/*
 * ecall mas_blk_dump() to test blk panic dump functionality
 * don't change this function name or you'll need to correct
 * the ST case too.
 */
static void mas_blk_panic_dump(void)
{
	struct udk_request_queue *q = NULL;
	struct blk_queue_cust *mas_queue = NULL;

	dlist_for_each_entry(mas_queue, &dump_list, struct blk_queue_cust, dump_list) {
		q = (struct udk_request_queue *)container_of(
			mas_queue, struct udk_request_queue, mas_queue);
		if (q->queuedata)
			mas_blk_dump_queue(q, BLK_DUMP_PANIC);
	}
}

int mas_blk_panic_dump_notify(void *args)
{
	UNUSED(args);
	mas_blk_panic_dump();

	return 0;
}

int mas_blk_panic_notify(unsigned int cmd, unsigned long arg)
{
	struct timespec time_spec;
	UNUSED(arg);
	UNUSED(cmd);

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &time_spec);
	udk_error("%s: ++ current = %ld %ld\n", __func__, time_spec.tv_sec, time_spec.tv_nsec);
	mas_blk_panic_dump();
	udk_error("%s: --\n", __func__);
	return 0;
}

/*
 * This function is used to add the request queue into the dump list
 */
void mas_blk_dump_register_queue(struct udk_request_queue *q)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	if (blk_lld->features & BLK_LLD_DUMP_SUPPORT) {
		if (!dlist_empty(&q->mas_queue.dump_list))
			return;

		uspinlock_lock(&dump_list_lock);
		dlist_insert_tail(&dump_list, &q->mas_queue.dump_list);
		uspinlock_unlock(&dump_list_lock);
	}
}

/*
 * This function is used to remove the request queue from the dump list
 */
void mas_blk_dump_unregister_queue(struct udk_request_queue *q)
{
	struct udk_request_queue *q_node = NULL;
	struct blk_queue_cust *mas_queue = NULL;
	struct blk_queue_cust *mas_queue_backup = NULL;

	uspinlock_lock(&dump_list_lock);
	dlist_for_each_entry_safe(
		mas_queue, mas_queue_backup, &dump_list, struct blk_queue_cust, dump_list) {
		q_node = (struct udk_request_queue *)container_of(
			mas_queue, struct udk_request_queue, mas_queue);
		if (q_node == q) {
			dlist_delete(&q_node->mas_queue.dump_list);
			break;
		}
	}
	uspinlock_unlock(&dump_list_lock);
}

/*
 * This function is used to enable the dump function on MQ tagset
 */
void blk_mq_tagset_dump_register(
	struct udk_blk_mq_tag_set *tag_set, lld_dump_status_fn func)
{
	struct blk_dev_lld *blk_lld = &tag_set->lld_func;

	blk_lld->dump_fn = func;
	blk_lld->features |= BLK_LLD_DUMP_SUPPORT;
}

/*
 * This function is used to enable the dump function on request queue
 */
void blk_queue_dump_register(struct udk_request_queue *q, lld_dump_status_fn func)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	blk_lld->dump_fn = func;
	blk_lld->features |= BLK_LLD_DUMP_SUPPORT;
	mas_blk_dump_register_queue(q);
}
