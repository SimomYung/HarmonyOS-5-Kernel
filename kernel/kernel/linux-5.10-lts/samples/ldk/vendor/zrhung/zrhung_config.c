// SPDX-License-Identifier: GPL-2.0
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "sysbunch.h"

#define MAGIC_NUM 0x9517
#define MAX_NEED_LOG_SIZE (4 * 1024) // 4KB

struct cstack_header {
	uint32_t magic;
	pid_t tid;
};

struct cstack_val {
	struct cstack_header header;
	char hstack_log_buff[MAX_NEED_LOG_SIZE];
};

int __override xcollie_get_stack(struct file *file, void __user *arg)
{
	struct cstack_header header = {0};
	char *stack_buf = NULL;
	struct cstack_val __user *input_cstack = (struct cstack_val __user *)arg;
	int ret = -EFAULT;

	if (!input_cstack || !file) {
		return -EINVAL;
	}

	if (copy_from_user(&header, input_cstack, sizeof(header))) {
		pr_err("copy tid from user failed\n");
		goto err_val;
	}

	if ((header.magic != MAGIC_NUM) || (header.tid <= 0)) {
		pr_err("tid error\n");
		goto err_val;
	}

	stack_buf = kzalloc(MAX_NEED_LOG_SIZE, GFP_KERNEL);
	if (!stack_buf) {
		return -ENOMEM;
	}

	ret = save_cur_stack_trace_tsk(header.tid, stack_buf, MAX_NEED_LOG_SIZE);
	if (ret != 0) {
		pr_err("save stack failed, ret=%d", ret);
		goto err_val;
	}

	if (copy_to_user(input_cstack->hstack_log_buff, stack_buf, MAX_NEED_LOG_SIZE)) {
		ret = -EFAULT;
		pr_err("Failed to copy kernel stack to user\n");
	}

err_val:
	kfree(stack_buf);
	return ret;
}
