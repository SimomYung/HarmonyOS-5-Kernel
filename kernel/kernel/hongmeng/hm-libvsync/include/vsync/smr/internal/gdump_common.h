/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_GARBAGE_DUMP_COMMON_H
#define VSYNC_GARBAGE_DUMP_COMMON_H

#if defined(VGDUMP_ENABLE_BUFF)
static inline void _gdump_merge_local_list(gdump_t *gd, gdump_thread_t *thread);
#endif

static inline void gdump_init(gdump_t *gd, smr_rwlock_lib_t lock)
{
	ASSERT(gd);

	gdump_list_init(&gd->threads);

	smr_nodes_list_init(&gd->del_lst);

	_gdump_init(gd);

	ASSERT(smr_rwlock_lib_is_set(&lock));

	smr_rwlock_lib_copy(&gd->lock, &lock);
}

static inline void gdump_destroy(gdump_t *gd)
{
	ASSERT(gd);
	smr_node_t *head = NULL;
#if defined(VGDUMP_ENABLE_BUFF)
	gdump_list_iter_t iter;
	gdump_list_node_t *curr = NULL;
	gdump_list_iter_init(&iter, &gd->threads);

	while (curr = gdump_list_iter_next(&iter), curr) {
		gdump_thread_t *thread =
			V_CONTAINER_OF(curr, gdump_thread_t, lst_node);
		// merge the given thread's retire local list into the global list.
		_gdump_merge_local_list(gd, thread);
	}
#endif
	head = smr_nodes_list_get_and_empty(&gd->del_lst);
	smr_nodes_list_destroy(head);
}

static inline void _gdump_exit(gdump_t *gd, gdump_thread_t *thread)
{
	ASSERT(gd);
	ASSERT(thread);
	ASSERT(thread->enter_count);
	thread->enter_count--;
	V_UNUSED(gd);
}

#if defined(VGDUMP_ENABLE_BUFF)
static inline void _gdump_merge_local_list(gdump_t *gd, gdump_thread_t *thread)
{
	smr_node_t *head = NULL;
	smr_node_t *tail = NULL;
	vsize_t len = smr_node_buff_popall(&thread->buff, &head, &tail);
	if (len > 0) {
		smr_nodes_list_merge(&gd->del_lst, head, tail, (vuint64_t)len);
	}
}
static inline void gdump_retire_local(gdump_t *gd, gdump_thread_t *thread,
				      smr_node_t *node,
				      smr_node_destroy_fun destroy_fun,
				      void *destroy_fun_arg)
{
	ASSERT(gd);
	ASSERT(thread);
	ASSERT(node);
	ASSERT(destroy_fun);
	vbool_t inserted = smr_node_buff_insert(&thread->buff, node,
						destroy_fun, destroy_fun_arg);
	if (!inserted) {
		_gdump_merge_local_list(gd, thread);
		inserted = smr_node_buff_insert(&thread->buff, node,
						destroy_fun, destroy_fun_arg);
		ASSERT(inserted);
	}
}
#endif
static inline void gdump_retire(gdump_t *gd, smr_node_t *node,
				smr_node_destroy_fun destroy_fun,
				void *destroy_fun_arg)
{
	ASSERT(gd);
	ASSERT(node);
	smr_nodes_list_add(&gd->del_lst, node, destroy_fun, destroy_fun_arg);
}
static inline void gdump_register(gdump_t *gd, gdump_thread_t *thread)
{
	ASSERT(gd);
	ASSERT(thread);
	thread->enter_count = 0;
	_gdump_init_thread(gd, thread);
#if defined(VGDUMP_ENABLE_BUFF)
	smr_node_buff_init(&thread->buff);
#endif
	gdump_list_add(&gd->threads, &thread->lst_node);
#if defined(SMR_GDUMPV4)
	vatomic_fence();
#endif
}

static inline void gdump_deregister(gdump_t *gd, gdump_thread_t *thread)
{
	ASSERT(gd);
	ASSERT(thread);

	/* tolerate deregister on active CS, call exit if needed */
	for (vsize_t i = 0; i < thread->enter_count; i++) {
		gdump_exit(gd, thread);
	}
#if defined(VGDUMP_ENABLE_BUFF)
	// move anything in the local list in the global list
	_gdump_merge_local_list(gd, thread);
#endif
	gd->lock.write_acq(gd->lock.arg);
	gdump_list_rem(&gd->threads, &thread->lst_node);
#if defined(SMR_GDUMPV4) || defined(SMR_GDUMPV3)
	// the caches must be updated inside the lock's critical section
	_gdump_remove_from_caches(gd, thread);
#endif
	gd->lock.write_rel(gd->lock.arg);

#ifdef COPY_STAT
	// gdump v2 uses this
	COPY_STAT();
#endif
}

static inline vsize_t gdump_recycle(gdump_t *gd, gdump_yield_fun_t yield,
				    gdump_delay_fun_t delay_time,
				    vuint64_t min_count)
{
	vuint64_t count = 0;
	smr_node_t *nodes = NULL;
	vsize_t freed_nodes_count = 0;

	count = smr_nodes_list_get_length(&gd->del_lst);
	/* check if the count of the retired nodes meets the lower limit for
     * recycling */
	if (count >= min_count) {
		nodes = smr_nodes_list_get_and_empty(&gd->del_lst);
		if (nodes) {
			/* make sure all active threads exited their CS */
			gdump_sync(gd, yield, delay_time);
			/* free popped nodes */
			freed_nodes_count = smr_nodes_list_destroy(nodes);
		}
	}
	return freed_nodes_count;
}
#endif
