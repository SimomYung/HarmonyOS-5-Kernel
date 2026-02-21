// SPDX-License-Identifier: GPL-2.0-only

#include <asm/memory.h>
#include <linux/atomic.h>
#include <linux/debugfs.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/buddy_tracker.h>
#include <linux/slab.h>
#include <linux/hash.h>
#include <linux/list.h>
#include <securec.h>
#include <uapi/linux/mman.h>

static struct buddy_tracker *tracker_instance = NULL;
static unsigned long long reserve_size;
static DEFINE_SPINLOCK(caller_hlist_lock);
static struct buddy_tracker *get_buddy_tracker_instance(void)
{
	return tracker_instance;
}

static unsigned long *get_caller_addr(struct buddy_tracker *tracker, struct page *page)
{
	return (tracker->callermap + page_to_pfn(page));
}

struct tracker_pair *is_caller_exist(struct buddy_tracker *tracker, unsigned long __caller)
{
	int hash_key = __caller % MAX_ENTRIES;
	struct tracker_pair *iterpair;
	rcu_read_lock();
	hlist_for_each_entry_rcu(iterpair, (tracker->tracker_hashmap + hash_key), hnode) {
		if (iterpair->caller == __caller) {
			rcu_read_unlock();
			return iterpair;
		}
	}
	rcu_read_unlock();
	return NULL;
}

unsigned long get_caller_from_page(struct buddy_tracker *tracker, struct page *page)
{
	unsigned long *caller_addr = get_caller_addr(tracker, page);
	unsigned long caller = *caller_addr;
	*caller_addr = 0;
	return caller;
}

static struct tracker_pair *add_caller_entry_to_map(struct buddy_tracker *tracker, unsigned long __caller)
{
	spin_lock(&caller_hlist_lock);
	struct tracker_pair *current_caller = is_caller_exist(tracker, __caller);
	if (current_caller == NULL) {
		int curr_index = atomic_read(&(tracker->entry_index));
		int hash_key = __caller % MAX_ENTRIES;
		(tracker->trackmap + curr_index)->caller = __caller;
		atomic_set(&((tracker->trackmap + curr_index)->value), 0);
		INIT_HLIST_NODE(&((tracker->trackmap + curr_index)->hnode));
		hlist_add_head_rcu(&((tracker->trackmap + curr_index)->hnode), (tracker->tracker_hashmap + hash_key));
		current_caller = tracker->trackmap + curr_index;
		atomic_add(1, &(tracker->entry_index));
	}
	spin_unlock(&caller_hlist_lock);
	return current_caller;
}

void liblinux_add_caller_use_cnt(struct page *page, unsigned int order, unsigned long caller)
{
	struct buddy_tracker *tracker = get_buddy_tracker_instance();
	if (!tracker || !tracker->initialized) {
		return;
	}
	struct tracker_pair *current_position = is_caller_exist(tracker, caller);
	if (current_position) {
		atomic_add(PAGE_ORDER_TO_KB(order), &current_position->value);
	} else {
		current_position = add_caller_entry_to_map(tracker, caller);
		if (!current_position) {
			return;
		}
		atomic_add(PAGE_ORDER_TO_KB(order), &current_position->value);
	}

	for (int i = 0; i < (1 << order); i++) {
		unsigned long *caller_addr = get_caller_addr(tracker, page + i);
		if (caller) {
			*caller_addr = caller;
		}
	}
}

void liblinux_sub_caller_use_cnt(struct page *page, unsigned int order)
{
	struct buddy_tracker *tracker = get_buddy_tracker_instance();
	unsigned long __caller;
	struct tracker_pair *current_position;
	int i;

	if (!tracker || !tracker->initialized) {
		return;
	}

	for (i = 0; i < (1 << order); i++) {
		__caller = get_caller_from_page(tracker, page + i);
		if (__caller == 0) {
			continue;
		}
		current_position = is_caller_exist(tracker, __caller);
		if (current_position) {
			atomic_sub(1 << PAGE_SHIFT_KB, &current_position->value);
		}
	}
}
void liblinux_dump_ldk_buddy_tracker_info(void)
{
	struct buddy_tracker *tracker = get_buddy_tracker_instance();
	if (!tracker || !tracker->initialized) {
		return;
	}
	int curr_index = atomic_read(&(tracker->entry_index));
	for (int i = 0; i < curr_index; i++) {
		if (atomic_read(&((tracker->trackmap + i)->value)) == 0) {
			continue;
		}
		pr_info("%pS %d KB\n", (void *)((tracker->trackmap + i)->caller),
			atomic_read(&((tracker->trackmap + i)->value)));
	}
}

static ssize_t read_buddy_tracker(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	struct buddy_tracker *tracker = get_buddy_tracker_instance();
	char temp_buf[100];
	size_t bytes_read = 0;
	char __user *current_buf = buf;
	int offset = *ppos;

	if (!tracker || !tracker->initialized) {
		return -EFAULT;
	}
	int curr_index = atomic_read(&(tracker->entry_index));
	for (int i = 0; i < curr_index; i++) {
		if (offset-- > 0) {
			continue;
		}
		if (atomic_read(&((tracker->trackmap + i)->value)) == 0) {
			continue;
		}
		snprintf_s(temp_buf, sizeof(temp_buf), sizeof(temp_buf) - 1, "%pS %d KB\n",
			(void *)((tracker->trackmap + i)->caller), atomic_read(&((tracker->trackmap + i)->value)));
		size_t entry_length = strlen(temp_buf);
		if (copy_to_user(current_buf, temp_buf, entry_length)) {
			return bytes_read ? bytes_read : -EFAULT;
		}
		current_buf += entry_length;
		bytes_read += entry_length;
		if (bytes_read >= count) {
			break;
		}
	}

	*ppos += bytes_read;
	return bytes_read;
}
static void liblinux_buddy_tracker_disable(void)
{
	struct buddy_tracker *tracker = get_buddy_tracker_instance();
	if (tracker==NULL) {
		return;
	}
	tracker->initialized = false;
}

static ssize_t write_buddy_tracker(struct file *file, const char __user *buf, size_t size, loff_t *ppos)
{
	char strbuf[10];

	if (size == 0 || size >= 10)
		return -EINVAL;
	if (copy_from_user(strbuf, buf, size))
		return -EFAULT;

	strbuf[size] = '\0';
	unsigned long input_flag = simple_strtoul(strbuf, NULL, 16);

	if (input_flag == 1) {
		liblinux_buddy_tracker_init(reserve_size);
	} else if (input_flag == 0) {
		liblinux_buddy_tracker_disable();
	}
	return size;
}

static const struct file_operations buddy_tracker_operations = {
	.read = read_buddy_tracker,
	.write = write_buddy_tracker
};

static int __init liblinux_pageowner_debug_init(void)
{
	struct dentry *dentry = debugfs_create_file("ldk_buddy_tracker", 0600, NULL, NULL, &buddy_tracker_operations);
	if (IS_ERR_OR_NULL(dentry)) {
		pr_info("create ldk_buddy_tracker failed, ret=%ld\n", PTR_ERR(dentry));
	} else {
		pr_info("create ldk_buddy_tracker success");
	}
	return 0;
}
late_initcall(liblinux_pageowner_debug_init)

void __init liblinux_buddy_tracker_init(unsigned long long vmemmap_size)
{
	if (tracker_instance!=NULL) {
		tracker_instance->initialized = true;
	}
	static struct buddy_tracker tracker = {0};
	
	unsigned long hashmap_size = PAGE_ALIGN(MAX_ENTRIES * sizeof(struct hlist_head));
	tracker.tracker_hashmap = (struct hlist_head *)liblinux_pal_reserve_size_name(
		hashmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, "hashmap");

	for (int i = 0; i < MAX_ENTRIES; i++) {
		INIT_HLIST_HEAD(tracker.tracker_hashmap + i);
	}

	unsigned long trackmap_size = MAX_ENTRIES * sizeof(struct tracker_pair);
	trackmap_size = PAGE_ALIGN(trackmap_size);
	unsigned long callermap_size = PAGE_ALIGN(vmemmap_size);
	tracker.trackmap = (struct tracker_pair *)liblinux_pal_reserve_size_name(
		trackmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, "trackmap");
	tracker.callermap = (unsigned long *)liblinux_pal_reserve_size_name(
		callermap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, "callermap");

	if (!tracker.trackmap || !tracker.callermap) {
		pr_info("Failed to allocate memory for ldk_buddy_tracker\n");
		return;
	}
	liblinux_pal_populate_size_per_page(tracker.callermap);
	tracker.initialized = true;
	atomic_set(&(tracker.entry_index), 0);
	tracker_instance = &tracker;
}

void liblinux_buddy_tracker_set_vmemmap_size(unsigned long long vmemmap_size)
{
	reserve_size = vmemmap_size;
}
