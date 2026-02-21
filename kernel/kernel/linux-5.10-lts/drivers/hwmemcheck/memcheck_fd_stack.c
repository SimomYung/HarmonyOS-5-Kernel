/*
 * memcheck_fd_stack.c
 *
 * save and read fd stack information, send signal to process
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "memcheck_fd_stack.h"
#include "memcheck_ioctl.h"
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/version.h>
#include <linux/string.h>
#include <securec.h>
#if (KERNEL_VERSION(4, 14, 0) <= LINUX_VERSION_CODE)
#include <linux/sched/signal.h>
#else
#include <linux/sched.h>
#endif

/* for fd stack information save and read */
#define FD_STACK_WAIT_TIME_SEC	5
#define FD_STACK_MAX_WAIT_NUMS 15

struct per_fd_stack_info {
	pid_t pid;
	bool is_waiting;
	void *fd_stack_buf;
	u64 fd_stack_len;
	wait_queue_head_t fd_stack_ready;
};

static u64 fd_addr_array[] = {
	/* MEMCMD_NONE */
	0,
	/* MEMCMD_ENABLE */
	ADDR_FD_ENABLE,
	/* MEMCMD_DISABLE */
	ADDR_FD_DISABLE,
	/* MEMCMD_SAVE_LOG */
	ADDR_FD_SAVE,
	/* MEMCMD_CLEAR_LOG */
	ADDR_FD_CLEAR,
};

static struct per_fd_stack_info fd_stack_info_list[FD_STACK_MAX_WAIT_NUMS];
static int fd_stack_info_list_count;
DEFINE_MUTEX(fd_stack_mutex);

static struct per_fd_stack_info* find_pid_info_struct_by_pid(pid_t pid)
{
	int idx = 0;

	for (idx = 0; idx < fd_stack_info_list_count; idx++) {
		if (fd_stack_info_list[idx].pid == pid)
			return &fd_stack_info_list[idx];
	}
	return NULL;
}

int memcheck_fd_stack_write(const void *buf, const struct fd_stack_info *info)
{
	char *tmp = NULL;
	int i = 0;
	struct per_fd_stack_info *per_pid_info = NULL;

	tmp = vzalloc(info->size + 1);
	if (!tmp)
		return -EFAULT;
	if (copy_from_user(tmp, buf + sizeof(*info), info->size)) {
		vfree(tmp);
		memcheck_err("copy_from_user failed\n");
		return -EFAULT;
	}
	tmp[info->size] = 0;

	mutex_lock(&fd_stack_mutex);
	for (i = 0; i < fd_stack_info_list_count; i++) {
		if (fd_stack_info_list[i].pid == info->id)
			per_pid_info = &fd_stack_info_list[i];
	}
	if (per_pid_info == NULL) {
		vfree(tmp);
		mutex_unlock(&fd_stack_mutex);
		return -EFAULT;
	}
	if (per_pid_info->fd_stack_buf)
		vfree(per_pid_info->fd_stack_buf);
	per_pid_info->fd_stack_buf = tmp;
	per_pid_info->fd_stack_len = info->size;
	mutex_unlock(&fd_stack_mutex);

	if (per_pid_info->is_waiting)
		wake_up_interruptible(&(per_pid_info->fd_stack_ready));

	return 0;
}

static int memcheck_wait_fd_stack_ready(struct per_fd_stack_info *per_pid_info)
{
	int ret;
	mutex_lock(&fd_stack_mutex);
	per_pid_info->is_waiting = true;

	init_waitqueue_head(&(per_pid_info->fd_stack_ready));
	mutex_unlock(&fd_stack_mutex);
	ret = wait_event_interruptible_timeout(per_pid_info->fd_stack_ready,
					per_pid_info->fd_stack_buf,
					FD_STACK_WAIT_TIME_SEC * HZ);
	mutex_lock(&fd_stack_mutex);
	if (per_pid_info->fd_stack_buf && ret > 0) {
		memcheck_info("get fd stack info successfully\n");
		ret = 0;
	} else if (!ret) {
		memcheck_err("wait for fd stack info timeout\n");
		ret = -ETIMEDOUT;
	} else if (ret < 0) {
		memcheck_err("wait for fd stack info return %d\n", ret);
		ret = -EFAULT;
	} else {
		memcheck_err("can not get fd stack info return %d\n", ret);
		ret = -EFAULT;
	}

	per_pid_info->is_waiting = false;
	mutex_unlock(&fd_stack_mutex);	
	return ret;
}

static void fd_stack_info_list_remove(struct per_fd_stack_info *per_pid_info)
{
	int i;
	int j = 0;
	int ret;

	if (per_pid_info->is_waiting) {
		memcheck_err("pid_info is waiting, donot remove\n");
		return;
	}

	vfree(per_pid_info->fd_stack_buf);
	for (i = 0; i < fd_stack_info_list_count; i++) {
		if (fd_stack_info_list[i].pid == per_pid_info->pid)
			continue;
		if (i != j)
			fd_stack_info_list[j] = fd_stack_info_list[i];
		j++;
	}
	ret = (j < fd_stack_info_list_count) ? 0 : -1;
	for (i = j; i < fd_stack_info_list_count; i++)
		(void)memset_s(&fd_stack_info_list[i], sizeof(fd_stack_info_list[i]), 0, sizeof(fd_stack_info_list[i]));
	fd_stack_info_list_count = j;
	if (ret)
		memcheck_err("get fd stackInfo finished, but remove %d from list failed, count is %d\n",
		    per_pid_info->pid, fd_stack_info_list_count);
}

int memcheck_fd_stack_read(void *buf, struct fd_stack_info *info)
{
	int ret = -EFAULT;
	size_t len;
	struct per_fd_stack_info *per_pid_info;

	mutex_lock(&fd_stack_mutex);
	per_pid_info = find_pid_info_struct_by_pid(info->id);
	if (per_pid_info == NULL) {
		memcheck_err("get fd stack info failed\n");
		goto unlock;
	}
	len = min(per_pid_info->fd_stack_len, info->size - sizeof(*info));
	if (len && copy_to_user(buf + sizeof(*info), per_pid_info->fd_stack_buf, len)) {
		memcheck_err("header copy_to_user failed\n");
		goto removal;
	}

	if (len != info->size) {
		info->size = len;
		if (copy_to_user(buf, info, sizeof(*info))) {
			memcheck_err("message copy_to_user failed\n");
			goto removal;
		}
	}
	memcheck_info("read fd stack success, len=%lu\n", len);
	ret = 0;

removal:
	fd_stack_info_list_remove(per_pid_info);
unlock:
	mutex_unlock(&fd_stack_mutex);

	return ret;
}

static bool process_disappear(u64 t, const struct track_cmd *cmd)
{
	if (cmd->cmd == MEMCMD_ENABLE)
		return false;
	if (cmd->timestamp != nsec_to_clock_t(t))
		return true;

	return false;
}

#ifdef CONFIG_DFX_LIBLINUX
int memcheck_fd_do_command(const struct track_cmd *cmd)
{
	int ret = 0;
	struct per_fd_stack_info *per_pid_info = NULL;
	u64 addr = 0;
	struct kernel_siginfo info;

	clear_siginfo(&info);

	info.si_signo = SIGNO_FDTRACK;
	info.si_code = SI_TKILL;

	mutex_lock(&fd_stack_mutex);
	if (fd_stack_info_list_count >=  FD_STACK_MAX_WAIT_NUMS) {
		memcheck_err("list is full\n");
		mutex_unlock(&fd_stack_mutex);
		return -EFAULT;
	}
	per_pid_info = find_pid_info_struct_by_pid(cmd->id);
	if (per_pid_info == NULL) {
		per_pid_info =  &fd_stack_info_list[fd_stack_info_list_count];
		per_pid_info->pid = cmd->id;
		per_pid_info->is_waiting = false;
		fd_stack_info_list_count++;
	}
	mutex_unlock(&fd_stack_mutex);

	addr |= fd_addr_array[cmd->cmd];
	info.si_addr = (void *)addr;
	ret = send_memcheck_signal(cmd->id, SIGNO_FDTRACK, &info);
	memcheck_info("send native SIGNO_FDTRACK, addr is: %d, ret is: %d\n", addr, ret);

	if ((!ret) && (cmd->cmd == MEMCMD_SAVE_LOG)) {
		memcheck_wait_fd_stack_ready(per_pid_info);
	} else if ((!ret) && (cmd->cmd == MEMCMD_CLEAR_LOG)) {
		mutex_lock(&fd_stack_mutex);
		fd_stack_info_list_remove(per_pid_info);
		mutex_unlock(&fd_stack_mutex);
	}

	return ret;
}
#else
int memcheck_fd_do_command(const struct track_cmd *cmd)
{
	int ret = 0;
	struct task_struct *p = NULL;
	struct per_fd_stack_info *per_pid_info = NULL;
	u64 addr = 0;

#if (KERNEL_VERSION(5, 4, 0) <= LINUX_VERSION_CODE)
	kernel_siginfo_t info;

	clear_siginfo(&info);
#else
	struct siginfo info;

	(void)memset_s(&info, sizeof(info), 0, sizeof(info));
#endif

	mutex_lock(&fd_stack_mutex);
	if (fd_stack_info_list_count >=  FD_STACK_MAX_WAIT_NUMS) {
		memcheck_err("list is full\n");
		mutex_unlock(&fd_stack_mutex);
		return -EFAULT;
	}
	per_pid_info = find_pid_info_struct_by_pid(cmd->id);
	if (per_pid_info == NULL) {
		per_pid_info =  &fd_stack_info_list[fd_stack_info_list_count];
		per_pid_info->pid = cmd->id;
		per_pid_info->is_waiting = false;
		fd_stack_info_list_count++;
	}
	mutex_unlock(&fd_stack_mutex);

	info.si_signo = SIGNO_FDTRACK;
	info.si_errno = 0;
	info.si_code = SI_TKILL;
	info.si_pid = task_tgid_vnr(current);
	info.si_uid = from_kuid_munged(current_user_ns(), current_uid());

	rcu_read_lock();
	p = find_task_by_vpid(cmd->id);
	if (p)
		get_task_struct(p);
	rcu_read_unlock();

	if (p && (task_tgid_vnr(p) == cmd->id)) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		if (process_disappear(p->real_start_time, cmd)) {
#else
		if (process_disappear(p->start_boottime, cmd)) {
#endif
			memcheck_err("pid %d disappear\n", cmd->id);
			ret = MEMCHECK_PID_INVALID;
			goto err_pid_disappear;
		}

		addr |= fd_addr_array[cmd->cmd];
		info.si_addr = (void *)addr;
		ret = do_send_sig_info(SIGNO_FDTRACK, &info, p, false);
		memcheck_info("send native SIGNO_FDTRACK, addr is: %d, ret is: %d\n", addr, ret);
	}

err_pid_disappear:
	if (p)
		put_task_struct(p);
	if ((!ret) && (cmd->cmd == MEMCMD_SAVE_LOG)) {
		memcheck_wait_fd_stack_ready(per_pid_info);
	} else if ((!ret) && (cmd->cmd == MEMCMD_CLEAR_LOG)) {
		mutex_lock(&fd_stack_mutex);
		fd_stack_info_list_remove(per_pid_info);
		mutex_unlock(&fd_stack_mutex);
	}

	return ret;
}
#endif
