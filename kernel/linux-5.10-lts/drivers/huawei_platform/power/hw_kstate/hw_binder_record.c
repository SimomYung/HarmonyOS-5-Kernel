/*
 * hw_binder_record.c
 *
 * maintains a binder information table and provides the add, delete, modify, and query functions.
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/rbtree.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif /* CONFIG_COMPAT */
#include <securec.h>

#include <huawei_platform/power/hw_binder_record.h>

#ifndef SYSTEM_UID
#define SYSTEM_UID ((uid_t)1000)
#endif  // SYSTEM_UID

#ifndef SYSTEM_GID
#define SYSTEM_GID ((gid_t)1000)
#endif  // SYSTEM_GID

#define BR_MIN_TIME_DIFF 10    // ms
#define BR_MAX_TIME_DIFF 1000  // ms

enum binder_record_inner_event_type {
	BR_EVT_UPDATE_TIMESTAMP,
	BR_EVT_UPDATE_ALL_FIELD,
	BR_EVT_REMOVE_RECORD,
	BR_EVT_REMOVE_ALL_RECORD,
	BR_EVT_PRINT_ALL_RECORD,
	BR_EVT_CHANGE_NODE_ATTR,
};

/* binder_record node info, associated under binder_record_base->rb_root */
struct binder_record {
	struct rb_node rb_node;                 // associated under rb_root
	struct binder_record_info binder_info;  // record binder information
};

/* binder_record base info, eg: rb_tree root node */
struct binder_record_base {
	struct mutex rb_lock;
	struct rb_root rb_root;  // rb_tree root node, to save binder_record
};

struct binder_record_last_report_info {
	struct mutex lock;
	int pid;                 // last report pid
	u64 timestamp;           // last report timestamp, ms
	uint32_t min_time_diff;  // minimum interval between two reports
};

struct binder_record_work {
	struct work_struct work_node;
	struct binder_record record;
	unsigned int event_type;
};

typedef int (*traverse_proc_fun)(struct binder_record *, void *);
typedef void (*event_proc_fun)(struct binder_record_work *);

static struct binder_record_base br_base_info;
static struct binder_record_last_report_info br_last_report_info;
static struct workqueue_struct *br_work_queue;
uint8_t binder_record_switch = 1;

static inline int binder_record_pid_cmp(const int pid1, const int pid2)
{
	if (pid1 < pid2) {
		return -1;
	} else if (pid1 > pid2) {
		return 1;
	} else {
		return 0;
	}
}

static inline void binder_record_update_data(struct binder_record *target_record,
	const struct binder_record *new_record, const unsigned int update_data_type)
{
	if (unlikely(new_record == NULL)) {
		pr_err("%s: record null\n", __func__);
		return;
	}

	switch (update_data_type) {
	case BR_EVT_UPDATE_TIMESTAMP:
		target_record->binder_info.timestamp = new_record->binder_info.timestamp;
		break;
	case BR_EVT_UPDATE_ALL_FIELD:
		(void)memcpy_s(target_record, sizeof(struct binder_record), new_record, sizeof(struct binder_record));
		break;
	default:
		pr_err("%s: unknown type(%u)\n", __func__, update_data_type);
		break;
	}
}

static void binder_record_create_record(struct rb_node *parent_node, struct rb_node **link_node,
	const struct binder_record *record)
{
	struct binder_record *new_record = NULL;
	new_record = kmalloc(sizeof(struct binder_record), GFP_KERNEL);
	if (new_record == NULL) {
		pr_err("%s: binder_record malloc fail\n", __func__);
		return;
	}

	binder_record_update_data(new_record, record, BR_EVT_UPDATE_ALL_FIELD);
	rb_link_node(&new_record->rb_node, parent_node, link_node);
	rb_insert_color(&new_record->rb_node, &br_base_info.rb_root);
	pr_debug("%s: new record pid(%d)\n", __func__, new_record->binder_info.called_pid);
}

/*
* if the node exists, update it.
* if the node does not exist, create it.
*/
static void binder_record_update_record(struct binder_record_work *work)
{
	int ret;
	struct rb_node *parent_node = NULL;
	struct rb_node **link_node = NULL;
	struct binder_record *tmp_record = NULL;
	struct binder_record *record = &work->record;

	mutex_lock(&br_base_info.rb_lock);
	link_node = &br_base_info.rb_root.rb_node;
	while (*link_node) {
		parent_node = *link_node;
		tmp_record = rb_entry(parent_node, struct binder_record, rb_node);
		ret = binder_record_pid_cmp(record->binder_info.called_pid, tmp_record->binder_info.called_pid);
		if (ret < 0) {
			link_node = &parent_node->rb_left;
		} else if (ret > 0) {
			link_node = &parent_node->rb_right;
		} else {
			// node already exists, update it directly.
			binder_record_update_data(tmp_record, record, work->event_type);
			goto out;
		}
	}

	// node not exist, create one
	binder_record_create_record(parent_node, link_node, record);
out:
	mutex_unlock(&br_base_info.rb_lock);
}

/* no lock in this function. need lock when invoking the function. */
static struct binder_record *binder_record_query_record(const int pid)
{
	int ret;
	struct rb_node *tmp_node = NULL;
	struct binder_record *tmp_record = NULL;

	tmp_node = br_base_info.rb_root.rb_node;
	while (tmp_node) {
		tmp_record = rb_entry(tmp_node, struct binder_record, rb_node);
		ret = binder_record_pid_cmp(pid, tmp_record->binder_info.called_pid);
		if (ret < 0) {
			tmp_node = tmp_node->rb_left;
		} else if (ret > 0) {
			tmp_node = tmp_node->rb_right;
		} else {
			return tmp_record;
		}
	}

	pr_debug("%s: not find pid(%d)\n", __func__, pid);
	return NULL;
}

static void binder_record_remove_record(struct binder_record_work *work)
{
	struct binder_record *record = NULL;
	int pid = work->record.binder_info.called_pid;

	mutex_lock(&br_base_info.rb_lock);
	record = binder_record_query_record(pid);
	if (record == NULL) {
		mutex_unlock(&br_base_info.rb_lock);
		pr_debug("%s: cannot find pid=%d\n", __func__, pid);
		return;
	}

	rb_erase(&record->rb_node, &br_base_info.rb_root);
	mutex_unlock(&br_base_info.rb_lock);

	kfree(record);
	pr_debug("%s: pid=%d\n", __func__, pid);
}

static void binder_record_remove_all_record(struct binder_record_work *work)
{
	unsigned int cnt = 0;
	struct rb_node *node = NULL;
	struct binder_record *record = NULL;

	mutex_lock(&br_base_info.rb_lock);
	while ((node = rb_last(&br_base_info.rb_root))) {
		record = rb_entry(node, struct binder_record, rb_node);
		rb_erase(&record->rb_node, &br_base_info.rb_root);
		kfree(record);
		cnt++;
	}
	mutex_unlock(&br_base_info.rb_lock);
	pr_info("%s: total cnt=%u\n", __func__, cnt);
}

static void binder_record_traverse_record(traverse_proc_fun fun, void *param)
{
	int ret;
	struct rb_node *node = NULL;
	struct binder_record *record = NULL;

	mutex_lock(&br_base_info.rb_lock);
	for (node = rb_first(&br_base_info.rb_root); node != NULL; node = rb_next(node)) {
		record = rb_entry(node, struct binder_record, rb_node);
		ret = fun(record, param);
		if (ret < 0) {
			pr_err("%s: traverse_proc_fun fail(%d) at(%d).\n", __func__, ret, record->binder_info.called_pid);
			break;
		}
	}
	mutex_unlock(&br_base_info.rb_lock);
}

static int binder_record_print_single_record(struct binder_record *record, void *param)
{
	unsigned int *cnt = (unsigned int *)param;
	(*cnt)++;
	pr_info("%-10d %-20llu\n", record->binder_info.called_pid, record->binder_info.timestamp);
	return 0;
}

static void binder_record_print_all_record(struct binder_record_work *work)
{
	unsigned int cnt = 0;

	pr_info("binder_record_info:\n");
	pr_info("switch=%u\n", binder_record_switch);
	mutex_lock(&br_last_report_info.lock);
	pr_info("last_report pid=%d, timestamp=%llu, min_time_diff=%u\n", br_last_report_info.pid,
		br_last_report_info.timestamp, br_last_report_info.min_time_diff);
	mutex_unlock(&br_last_report_info.lock);

	pr_info("current timestamp=%llu\n", BINDER_RECORD_CUR_TIMESTAMP);
	pr_info("%-10s %-20s\n", "pid", "timestamp");
	binder_record_traverse_record(binder_record_print_single_record, &cnt);
	pr_info("binder_record_end count=%u\n", cnt);
}

/* chown system:system /dev/binder_record */
static void binder_record_change_node_attr(struct binder_record_work *work)
{
	char *path = "/dev/binder_record";
	long ret = ksys_chown((const char __user *)path, SYSTEM_UID, SYSTEM_GID);
	if (ret)
		pr_err("%s: chown fail(%ld)\n", __func__, ret);
}

static const event_proc_fun event_proc_tbl[] = {
	[BR_EVT_UPDATE_TIMESTAMP] = binder_record_update_record,
	[BR_EVT_REMOVE_RECORD] = binder_record_remove_record,
	[BR_EVT_REMOVE_ALL_RECORD] = binder_record_remove_all_record,
	[BR_EVT_PRINT_ALL_RECORD] = binder_record_print_all_record,
	[BR_EVT_CHANGE_NODE_ATTR] = binder_record_change_node_attr,
};

static void binder_record_event_handler(struct work_struct *node)
{
	struct binder_record_work *work = NULL;
	if (unlikely(!node)) {
		pr_err("%s: node null\n", __func__);
		return;
	}

	work = container_of(node, struct binder_record_work, work_node);
	pr_debug("%s: event_type(%u) pid(%d)\n", __func__, work->event_type, work->record.binder_info.called_pid);
	if (unlikely(work->event_type >= sizeof(event_proc_tbl) / sizeof(event_proc_tbl[0]))) {
		pr_err("%s: unknown event_type=%u\n", __func__, work->event_type);
		goto out;
	}

	if (likely(event_proc_tbl[work->event_type])) {
		event_proc_tbl[work->event_type](work);
	}
out:
	kfree(work);
}

static void binder_record_send_event(const unsigned int event_type, const struct binder_record *record)
{
	struct binder_record_work *work = NULL;
	if (unlikely(br_work_queue == NULL)) {
		pr_err("%s: work_queue is null\n", __func__);
		return;
	}

	work = kmalloc(sizeof(struct binder_record_work), GFP_KERNEL);
	if (unlikely(work == NULL)) {
		pr_err("%s: malloc work fail\n", __func__);
		return;
	}

	// some events do not need *record, eg: print all records.
	if (record != NULL)
		(void)memcpy_s(&work->record, sizeof(work->record), record, sizeof(*record));
	work->event_type = event_type;
	INIT_WORK(&work->work_node, binder_record_event_handler);
	queue_work(br_work_queue, &work->work_node);
}

void binder_record_remove_pid(const int pid)
{
	pr_debug("%s: pid=%d\n", __func__, pid);
	struct binder_record record = {0};

	record.binder_info.called_pid = pid;
	binder_record_send_event(BR_EVT_REMOVE_RECORD, &record);
}

/* one pid will call the function many times at the same time,
 * so add a filter here to avoid sending too many same events */
bool binder_record_need_filter_timestamp(const int pid, const u64 timestamp)
{
	if (!mutex_trylock(&br_last_report_info.lock)) {
		// if trylock fail, add record directly
		pr_debug("%s: try lock fail, pid(%d) timestamp(%llu)\n", __func__, pid, timestamp);
		return false;
	}
	if (pid != br_last_report_info.pid) {
		br_last_report_info.pid = pid;
		br_last_report_info.timestamp = timestamp;
		mutex_unlock(&br_last_report_info.lock);
		return false;
	}
	if (timestamp > br_last_report_info.timestamp &&
		timestamp - br_last_report_info.timestamp >= br_last_report_info.min_time_diff) {
		br_last_report_info.timestamp = timestamp;
		mutex_unlock(&br_last_report_info.lock);
		return false;
	}

	pr_debug("%s: ignore pid(%d) timestamp(%llu-%llu)\n", __func__, pid, timestamp, br_last_report_info.timestamp);
	mutex_unlock(&br_last_report_info.lock);
	return true;
}

void binder_record_add_timestamp(const int pid, const u64 timestamp)
{
	if (binder_record_need_filter_timestamp(pid, timestamp)) {
		return;
	}

	struct binder_record record = {0};
	record.binder_info.called_pid = pid;
	record.binder_info.timestamp = timestamp;
	binder_record_send_event(BR_EVT_UPDATE_TIMESTAMP, &record);
}

static void binder_record_query_timestamp(struct binder_timestamp *timestamp_info)
{
	struct binder_record *record = NULL;
	timestamp_info->cur_timestamp = BINDER_RECORD_CUR_TIMESTAMP;
	timestamp_info->last_timestamp = 0;

	mutex_lock(&br_base_info.rb_lock);
	record = binder_record_query_record(timestamp_info->pid);
	if (record != NULL)
		timestamp_info->last_timestamp = record->binder_info.timestamp;
	mutex_unlock(&br_base_info.rb_lock);
	pr_debug("%s: pid=%d, timestamp=%llu-%llu\n", __func__, timestamp_info->pid, timestamp_info->cur_timestamp,
		timestamp_info->last_timestamp);
}

static long binder_record_ioctl_get_timestamp(unsigned int cmd, unsigned long arg)
{
	int ret;
	void __user *buff = (void __user *)arg;
	struct binder_timestamp timestamp_info = {0};
	if (unlikely(_IOC_SIZE(cmd) != sizeof(timestamp_info))) {
		pr_err("%s: invalid size=%lu\n", __func__, _IOC_SIZE(cmd));
		return -EINVAL;
	}
	ret = copy_from_user(&timestamp_info, buff, sizeof(timestamp_info));
	if (ret != 0) {
		pr_err("%s: copy_from_user fail=%d\n", __func__, ret);
		return ret;
	}

	binder_record_query_timestamp(&timestamp_info);
	ret = copy_to_user(buff, &timestamp_info, sizeof(timestamp_info));
	if (ret != 0) {
		pr_err("%s: copy_to_user fail=%d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static int binder_record_get_single_record(struct binder_record *record, void *param)
{
	struct binder_record_list *list = (struct binder_record_list *)param;
	if (list->num >= BINDER_RECORD_MAX_NUM) {
		pr_err("%s: num over\n", __func__);
		return -1;
	}

	list->node[list->num++] = record->binder_info;
	return 0;
}

static long binder_record_ioctl_get_all(unsigned int cmd, unsigned long arg)
{
	struct binder_record_list __user *p = (struct binder_record_list __user *)arg;
	struct binder_record_list list;
	list.num = 0;
	binder_record_traverse_record(binder_record_get_single_record, &list);

	if (unlikely(_IOC_SIZE(cmd) < sizeof(list))) {
		pr_err("%s: size %lu < %lu\n", __func__, _IOC_SIZE(cmd), sizeof(list));
		return -EINVAL;
	}
	if (put_user(BINDER_RECORD_CUR_TIMESTAMP, &p->cur_timestamp)) {
		pr_err("%s: set timestamp fail\n", __func__);
		return -EFAULT;
	}
	if (put_user(list.num, &p->num)) {
		pr_err("%s: set num fail\n", __func__);
		return -EFAULT;
	}
	if (copy_to_user(p->node, list.node, sizeof(list.node[0]) * list.num)) {
		pr_err("%s: copy node fail\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static long binder_record_ioctl_remove_pid(unsigned int cmd, unsigned long arg)
{
	int32_t pid = 0;
	if (unlikely(_IOC_SIZE(cmd) != sizeof(int32_t))) {
		pr_err("%s: invalid size=%lu\n", __func__, _IOC_SIZE(cmd));
		return -EINVAL;
	}
	if (unlikely(get_user(pid, (int32_t __user *)arg))) {
		pr_err("%s: get pid fail\n", __func__);
		return -EFAULT;
	}

	binder_record_remove_pid(pid);
	return 0;
}

static long binder_record_ioctl_set_min_time_diff(unsigned int cmd, unsigned long arg)
{
	uint32_t time_diff = 0;
	if (unlikely(_IOC_SIZE(cmd) != sizeof(uint32_t))) {
		pr_err("%s: invalid size=%lu\n", __func__, _IOC_SIZE(cmd));
		return -EINVAL;
	}
	if (unlikely(get_user(time_diff, (uint32_t __user *)arg))) {
		pr_err("%s: get time_diff fail\n", __func__);
		return -EFAULT;
	}

	if (time_diff < BR_MAX_TIME_DIFF) {
		mutex_lock(&br_last_report_info.lock);
		br_last_report_info.min_time_diff = time_diff;
		mutex_unlock(&br_last_report_info.lock);
	}
	pr_info("%s: time_diff=%u\n", __func__, time_diff);
	return 0;
}

/* since this command is rarely executed, concurrency conflicts are not a concern. */
static long binder_record_ioctl_set_switch(unsigned int cmd, unsigned long arg)
{
	uint8_t old_switch = binder_record_switch;
	uint8_t new_switch = 0;
	if (unlikely(_IOC_SIZE(cmd) != sizeof(uint8_t))) {
		pr_err("%s: invalid size=%lu\n", __func__, _IOC_SIZE(cmd));
		return -EINVAL;
	}
	if (unlikely(get_user(new_switch, (uint8_t __user *)arg))) {
		pr_err("%s: get arg fail\n", __func__);
		return -EFAULT;
	}
	new_switch = (new_switch > 0) ? 1 : 0;
	pr_info("%s: switch %u->%u\n", __func__, old_switch, new_switch);
	if (new_switch == old_switch) {
		return 0;
	}

	binder_record_switch = new_switch;
	if (old_switch == 1 && new_switch == 0) {
		binder_record_send_event(BR_EVT_REMOVE_ALL_RECORD, NULL);
	}
	return 0;
}

/* provides interfaces for the user layer */
static long binder_record_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	switch (cmd) {
	case BINDER_RECORD_GET_TIMESTAMP:
		ret = binder_record_ioctl_get_timestamp(cmd, arg);
		break;
	case BINDER_RECORD_GET_ALL:
		ret = binder_record_ioctl_get_all(cmd, arg);
		break;
	case BINDER_RECORD_REMOVE_PID:
		ret = binder_record_ioctl_remove_pid(cmd, arg);
		break;
	case BINDER_RECORD_REMOVE_ALL:
		binder_record_send_event(BR_EVT_REMOVE_ALL_RECORD, NULL);
		break;
	case BINDER_RECORD_PRINT_ALL:
		binder_record_send_event(BR_EVT_PRINT_ALL_RECORD, NULL);
		break;
	case BINDER_RECORD_SET_MIN_TIME_DIFF:
		ret = binder_record_ioctl_set_min_time_diff(cmd, arg);
		break;
	case BINDER_RECORD_SET_SWITCH:
		ret = binder_record_ioctl_set_switch(cmd, arg);
		break;
	case BINDER_RECORD_CHANGE_NODE_ATTR:
		binder_record_send_event(BR_EVT_CHANGE_NODE_ATTR, NULL);
		break;
	default:
		ret = -EINVAL;
		pr_err("%s: unknown cmd=%u\n", __func__, cmd);
		break;
	};

	pr_debug("%s: cmd=%u, ret=%ld arg=%llu\n", __func__, cmd, ret, arg);
	return ret;
}

#ifdef CONFIG_COMPAT
static long binder_record_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	// normally, not go here
	pr_info("%s: cmd=%u\n", __func__, cmd);
	return binder_record_ioctl(filp, cmd, (unsigned long)compat_ptr(arg));
}
#endif

static int binder_record_open(struct inode *inode, struct file *filp)
{
	pr_info("%s: enter\n", __func__);
	return 0;
}
static int binder_record_release(struct inode *inode, struct file *filp)
{
	pr_info("%s: enter\n", __func__);
	return 0;
}

static const struct file_operations binder_record_device_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = binder_record_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = binder_record_compat_ioctl,
#endif
	.open = binder_record_open,
	.release = binder_record_release,
};

static struct miscdevice binder_record_device = {
	.name = "binder_record",
	.fops = &binder_record_device_fops,
	.minor = MISC_DYNAMIC_MINOR,
};

static int __init binder_record_init(void)
{
	if (misc_register(&binder_record_device)) {
		pr_err("%s: misc_register fail\n", __func__);
		return -EFAULT;
	}

	mutex_init(&br_base_info.rb_lock);
	br_base_info.rb_root = RB_ROOT;
	mutex_init(&br_last_report_info.lock);
	br_last_report_info.pid = 0;
	br_last_report_info.timestamp = 0;
	br_last_report_info.min_time_diff = BR_MIN_TIME_DIFF;

	br_work_queue = alloc_workqueue("%s", WQ_UNBOUND | WQ_HIGHPRI, 1, "binder_record_work_queue");
	if (!br_work_queue) {
		misc_deregister(&binder_record_device);
		pr_err("%s: alloc_workqueue failed\n", __func__);
		return -ENOMEM;
	}

	pr_info("%s end.\n", __func__);
	return 0;
}

static void __exit binder_record_exit(void)
{
	misc_deregister(&binder_record_device);

	if (br_work_queue) {
		flush_workqueue(br_work_queue);
		destroy_workqueue(br_work_queue);
		br_work_queue = NULL;
	}

	binder_record_remove_all_record(NULL);
	pr_info("%s end.\n", __func__);
}

module_init(binder_record_init);
module_exit(binder_record_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Huawei Binder Record Driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");