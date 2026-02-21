/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: mas bkops core framework
 */

#define pr_fmt(fmt) "[BKOPS]" fmt

#include <udk/log.h>
#include <libstrict/strict.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/mas_blk/mas_bkops_core.h>
#include <udk/block/mas_blk/mas_blk_busy_idle_notify.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/sdproxy/api.h>
#include <udk/mm.h>

#define MAS_BKOPS_MODULE_NAME "mas_bkops"

struct mas_bkops_notify_info {
	int state;
	/* accumulated write len of the whole device */
	unsigned long write_len;
	/* accumulated discard len of the whole device */
	unsigned long discard_len;
};

static bool mas_blk_bkops_flag = false;
static bool mas_blk_get_bkops_flag(void)
{
	return mas_blk_bkops_flag;
}

static void mas_blk_set_bkops_flag(bool val)
{
	mas_blk_bkops_flag = val;
}

int mas_blk_bkops_enable(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	mas_blk_set_bkops_flag(true);
	hm_error("mas_blk_bkops_enable\n");

	return 0;
}

static enum blk_busyidle_callback_ret bkops_io_busyidle_notify_handler(
	struct blk_busyidle_event_node *event_node,
	enum blk_idle_notify_state state)
{
	enum blk_busyidle_callback_ret ret;
	struct blk_dev_lld *lld = NULL;
	struct mas_bkops_notify_info info;

	if (!mas_blk_get_bkops_flag()) {
		return BUSYIDLE_NO_IO;
	}

	if ((event_node == NULL) || (event_node->lld == NULL)) {
		return BUSYIDLE_ERR;
	}

	lld = event_node->lld;
	info.state = (int)state;
	info.write_len = lld->write_len;
	info.discard_len = lld->discard_len;

	ret = sd_proxy_bkops((void *)&info, sizeof(struct mas_bkops_notify_info));

	return ret;
}

void mas_bkops_init(struct udk_request_queue *q)
{
	int ret;
	struct blk_busyidle_event_node event_node = { 0 };
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);
	static bool init_flag = false;

	if ((init_flag == false) && (blk_lld != NULL) &&
		(blk_lld->features & BLK_LLD_BUSYIDLE_SUPPORT)) {
		ret = strncpy_s(event_node.subscriber, SUBSCRIBER_NAME_LEN,
			MAS_BKOPS_MODULE_NAME, strlen(MAS_BKOPS_MODULE_NAME) + 1);
		if (ret != 0) {
			hm_error("strcpy to subscriber failed: %d\n", ret);
			return;
		}

		event_node.subscriber[SUBSCRIBER_NAME_LEN - 1] = '\0';
		event_node.busyidle_callback = bkops_io_busyidle_notify_handler;
		ret = blk_busyidle_event_subscribe(q, &event_node);
		if (ret != E_HM_OK) {
			hm_error("bkops init fail\n");
		} else {
			init_flag = true;
			hm_error("bkops init success\n");
		}
	}
}

#ifdef CONFIG_MAS_DEBUG_FS
static size_t mas_queue_bkops_enable(char *buffer, size_t buffer_len)
{
	size_t offset = 0;
	ssize_t ret = 0;

	ret = snprintf_s(buffer + offset, (buffer_len - offset), (buffer_len - offset - 1),
		"mas_blk_bkops_enable %s\n",
		mas_blk_get_bkops_flag() ? "enable" : "disable");
	if (ret > 0) {
		offset += ret;
	}

	return (ssize_t)offset;
}

/* this is for debug purpose and page is long enough for now */
int mas_queue_bkops_enable_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize)
{
	UNUSED(ctx);
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (pos >= MAS_BLK_SYS_BUF_LEN) {
		*rsize = 0;
		return E_HM_OK;
	}

	buf = mas_malloc_sysfs_buf();
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	offset = mas_queue_bkops_enable(buf, MAS_BLK_SYS_BUF_LEN);

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
#endif
