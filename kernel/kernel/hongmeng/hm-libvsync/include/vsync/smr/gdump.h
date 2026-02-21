/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_GDUMP_H
#define VSYNC_GDUMP_H

/*******************************************************************************
 * @file  gdump.h
 * @brief Garbage Dump is an SMR scheme, created in DRC
 *
 * @note There are different implementations of garbage dump at the moment. You
 * need to define the version passing to the compiler one of the following
 * defines: -DSMR_GDUMPV1, -DSMR_GDUMPV2, -DSMR_GDUMPV3, -DSMR_GDUMPV4
 * By default V3 is activated. We recommend the users to benchmark with
 * different versions and choose the best performing version in their
 * applications.
 *
 * ### Configurations
 * Both GDumpv3 and GDumpv2 allow cooperation between threads that call
 * gdump_deregister and those that call gdump_sync or gdump_recycle.
 *
 * Threads that deregister check if there are synchronizers that are
 * waiting on them, and if that is the case, they communicate that this
 * is no longer necessary. De-registering threads scan an array, where
 * each slot points to data related to a synchronizer thread. The data
 * caches the progress that the synchronizer had made scanning the
 * list of registered threads. The de-registering thread can know
 * via this array if that synchronizer cached data related to the de-registering
 * thread, and if that's the case, it will update the synchronizers data so that
 * they will not visit the de-registering thread's object in their next scan.
 *
 * This array has a number of slots equal to `VGDUMP_MAX_SYNCERS=5`.
 * If there are more synchronizers than slots, surplus synchronizers spin till
 * a slot becomes vacant (this spinning does not impede the progress of other
 * threads). Users that have applications where there are less or more
 * synchronizers can overwrite this number at compile time via
 * `-DVGDUMP_MAX_SYNCERS=N` where `N` is the maximum number of parallel threads
 * that can call gdump_sync/gdump_recycle.
 *
 * List of configurations that can be overwritten
 *
 * - -DVGDUMP_MAX_SYNCERS=N, maximum number of concurrent synchronizers
 * - -DV_SMR_NODE_BUFF_SIZE=N, maximum capacity of local buffer used with local
 * retirement
 *
 * ### Simple Example
 * @include eg_gdump.c
 *
 * ### Example with a data structure
 * @include eg_listset_lf.c
 *
 ******************************************************************************/
#include <vsync/smr/internal/smr_lock.h>
#include <vsync/smr/internal/smr_nodes_list.h>

typedef int (*gdump_yield_fun_t)(void);
typedef int (*gdump_delay_fun_t)(unsigned int useconds);
typedef void (*smr_node_free)(smr_node_t *, void *);
struct gdump_s;
struct gdump_thread_s;

/**
 * Initializes the given `gdump_t` object.
 *
 * @param gd address of `gdump_t` object.
 * @param lock  address of `smr_rwlock_lib_t` object containing function
 * pointers for acquiring/releasing a read-write lock.
 * @note when used in user space `pthread_rwlock` is recommended.
 */
static inline void gdump_init(struct gdump_s *gd, smr_rwlock_lib_t lock);
/**
 * Destroys all retired nodes.
 *
 * @note call it only if all threads are finished.
 *
 * @param gd address of gdump_t object.
 */
static inline void gdump_destroy(struct gdump_s *gd);
/**
 * Marks the beginning of a critical section.
 *
 * All shared resources are protected from reclamation while the thread is in a
 * critical section.
 * @post `gdump_exit`
 * @note Threads must call this function right before they call any of the
 * algo/data-structure operations, for which gd is used.
 * @param gd address of gdump_t object.
 */
static inline void gdump_enter(struct gdump_s *, struct gdump_thread_s *thrd);
/**
 * Marks the end of a critical section.
 *
 * @pre `gdump_enter`
 * @param gd address of gdump_t object.
 * @param thread address of gdump_thread_t object.
 */
static inline void gdump_exit(struct gdump_s *gd, struct gdump_thread_s *thrd);
/**
 * Registers the calling thread.
 *
 * @note must be called once per thread after the first invocation of
 * `gdump_enter`.
 * @note acquires the lock as a writer.
 * @param gd address of gdump_t object.
 * @param thread address of gdump_thread_t object.
 */
static inline void gdump_register(struct gdump_s *gd,
				  struct gdump_thread_s *thrd);
/**
 * Deregisters the calling thread.
 *
 * @note must be called once per thread after the last invocation of
 * `gdump_exit`.
 * @note acquires the lock as a writer.
 * @param gd address of gdump_t object.
 * @param thread address of gdump_thread_t object.
 */
static inline void gdump_deregister(struct gdump_s *gd,
				    struct gdump_thread_s *thrd);
/**
 * Retires the given `node` to the global retirement list.
 *
 * @note a retired node is placed on a limbo list till its safe to reclaim. In
 * order to trigger reclamation `gdump_recycle` must be periodically called.
 * @see also gdump_retire_local.
 * @param gd address of gdump_t object.
 * @param node address of smr_node_t object.
 * @param destroy_fun address of callback function, used for node destruction.
 * @param destroy_fun_arg additional argument passed to `destroy_fun`
 */
static inline void gdump_retire(struct gdump_s *gd, smr_node_t *node,
				smr_node_destroy_fun destroy_fun,
				void *destroy_fun_arg);

#if defined(VGDUMP_ENABLE_BUFF) || defined(DOC)
/**
 * Retires the given `node` to a thread's local buffer.
 *
 * @note This API can be used to reduce contention on global retirement list. It
 * is useful when retirement is frequent among threads. Users may choose to only
 * use `gdump_retire`. However, in some cases local retirement might provide
 * performance benefits. When the buffer is full nodes are automatically moved
 * to the global list, and the buffer is emptied. Users can overwrite the
 * default capacity of the local buffer by compiling with
 * `-DV_SMR_NODE_BUFF_SIZE=N`. Where N is the capacity of threads' local
 * buffers.
 *
 * @param gd address of gdump_t object.
 * @param thread address of gdump_thread_t object.
 * @param node address of smr_node_t object.
 * @param destroy_fun address of callback function, used for node destruction.
 * @param destroy_fun_arg additional argument passed to `destroy_fun`
 *
 * @note This feature is only available when `VGDUMP_ENABLE_BUFF` is defined.
 */
static inline void gdump_retire_local(struct gdump_s *gd,
				      struct gdump_thread_s *thread,
				      smr_node_t *node,
				      smr_node_destroy_fun destroy_fun,
				      void *destroy_fun_arg);
#endif
/**
 * Synchronizes active threads.
 *
 * This function ensures that all active threads, i.e. executing inside a
 * critical section marked with gdump_enter/gdump_exit, have exited their
 * witnessed critical section by the time this function returns.
 *
 * @note this function can be used for reclamation in synchronous way.
 * @example
 * ``` C
 * detach(n);
 * gdump_sync(gd, sched_yield);
 * free(n);
 * ```
 * @note it is recommended to use background recycling  and not call
 * `gdump_sync` directly.
 * @see `gdump_recycle`
 * @note This function acquires the lock as a reader and is blocking.
 * @note DO NOT call this function inside a critical section.
 * @param gd address of gdump_t object.
 * @param yield a function pointer to a function like `sched_yield` to use
 * during spinning.
 */
static inline void gdump_sync(struct gdump_s *gd, gdump_yield_fun_t yield,
			      gdump_delay_fun_t delay_time);
/**
 * Recycles/frees all retired nodes.
 *
 * The recycling happens if the count of the retired nodes is `>= min_count`.
 *
 * @note don't call while in in a critical section.
 * @note It is recommended to dedicate a specific thread for periodic recycling.
 * @note
 *
 * @example
 * ```C
 * // continuously recycle if there is at least 10 retired nodes
 * while(true) {
 *  gdump_recycle(gd, sched_yield, 10);
 *  sleep(1);
 * }
 * ```
 * @param gd address of gdump_t object.
 * @param yield a function pointer to a function like `sched_yield` to use while
 * spinning.
 * @param min_count the minimum count to trigger recycling.
 * @return number of recycled nodes.
 */
static inline vsize_t gdump_recycle(struct gdump_s *gd, gdump_yield_fun_t yield,
				    gdump_delay_fun_t delay_time,
				    vuint64_t min_count);

#if defined(SMR_GDUMPV1)
#define GDUMP_VERSION "gdumpv1"
#include <vsync/smr/internal/gdump_v1.h>
#elif defined(SMR_GDUMPV2)
#define GDUMP_VERSION "gdumpv2"
#include <vsync/smr/internal/gdump_v2.h>
#elif defined(SMR_GDUMPV3)
#define GDUMP_VERSION "gdumpv3"
#include <vsync/smr/internal/gdump_v3.h>
#elif defined(SMR_GDUMPV4)
#define GDUMP_VERSION "gdumpv4"
#include <vsync/smr/internal/gdump_v4.h>
#else
// #warning "No SMR scheme is selecting using SMR_GDUMPV3 as default"
#define SMR_GDUMPV3
#include <vsync/smr/internal/gdump_v3.h>
#endif

#if defined(SMR_GDUMPV1) || defined(SMR_GDUMPV2)
/**
     * @def GDUMP_INIT
     *
     * can be used for static initialization.
     *
     * @param _l_lib_ object of type smr_rwlock_lib_t
     *
     * @note we recommend using `gdump_init` instead. `gdump_init` verifies that
     * given parameters are initialized properly.
     *
     */
#define GDUMP_INIT(_l_lib_)                                          \
	{                                                            \
		.threads = GDUMP_LIST_INIT(),                        \
		.del_lst = SMR_NODES_LIST_INIT(), .lock = (_l_lib_), \
		.glb_era = VATOMIC_INIT(1)                           \
	}

#elif defined(SMR_GDUMPV3) || defined(SMR_GDUMPV4)

#define GDUMP_INIT(_l_lib_)                                         \
	{                                                           \
		.threads = GDUMP_LIST_INIT(),                       \
		.sync_cache = { VATOMIC_INIT(NULL) },               \
		.del_lst = SMR_NODES_LIST_INIT(), .lock = (_l_lib_) \
	}

#endif

#endif
