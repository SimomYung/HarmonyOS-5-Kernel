/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_CLEANUP_H
#define VSYNC_CLEANUP_H

/*******************************************************************************
 * @file  cleanup.h
 * @brief Performant SMR scheme for user-space.
 *
 * It relies on quiescent periods to determine when it is safe to reclaim
 * nodes. Cleanup creates an additional thread that handles the reclamation of
 * memory. This thread has a minimal interaction with running threads.
 * Periodically, if there is something to reclaim, cleanup requests signals
 * from the all registered threads. Once the signals are received, cleanup
 * proceeds to recycle the retired nodes that were added before the signal
 * request is placed.
 *
 * Progress Condition: only `cleanup_register_thread` and
 * `cleanup_deregister_thread` are blocking operations. Ideally, these functions
 * are called only once per thread, and before accessing the data-structure.
 * This means the data-structure will not compromise its progress-condition, by
 * using this scheme.
 *
 * Performance: The overhead of this scheme on the concurrent algorithm is
 * negligible, but it can be affected by the threshold passed to `cleanup_init`
 * the threshold is the number of microseconds a cleanup should wait before
 * running the next cleanup cycle.
 *
 * Memory Bound: one drawback of cleanup is that there is no bound on how large
 * the retired nodes' list can grow. One reason for that is a thread can
 * misbehave or delay giving the needed signal indefinitely. If your application
 * scenario fall in that category then gdump.h might be a better fit.
 *
 *
 * @example
 * @include eg_cleanup.c
 ******************************************************************************/
#include <vsync/vtypes.h>
#include <vsync/common/compiler.h>
#include <vsync/common/verify.h>
#include <vsync/common/dbg.h>
#include <vsync/common/assert.h>
#include <vsync/utils/string.h>
#include <vsync/smr/internal/smr_threads.h>
#include <vsync/smr/internal/smr_nodes_list.h>
#include <vsync/smr/internal/smr_lock.h>
#include <vsync/spinlock/ttaslock.h>

#ifndef SMR_MAX_NTHREADS
#error "Missing define -DSMR_MAX_NTHREADS=N, where N is the maximum number of threads that can be registered in the SMR"
#endif

#define CLEANUP_MAX_NUM_CONTAINERS 2

/* pair of thread id and cleanup signal  */
typedef struct cleanup_signal_s {
	smr_tid_t tid;
	vatomic32_t cleanup_signal;
} cleanup_signal_t;

/* array of the tid/signal pairs along with its length */
typedef struct signals_containers_s {
	vsize_t max_len;
	vsize_t signals_len;
	cleanup_signal_t signals[SMR_MAX_NTHREADS];
} signals_containers_t;

typedef struct smr_threads_s {
	vsize_t idx;
	smr_tid_t id[SMR_MAX_NTHREADS];
	ttaslock_t default_lock;
	smr_lock_lib_t lock;
} smr_threads_t;

typedef void (*smr_node_free)(smr_node_t *, void *);
typedef void (*smr_node_set_next)(void *, void *);
typedef void *(*smr_node_get_next)(void *);

/* cleanup toolkit object */
typedef struct cleanup_s {
	smr_threads_t threads;
	smr_nodes_list_t del_lst;

	signals_containers_t containers[CLEANUP_MAX_NUM_CONTAINERS];
	vsize_t container_idx;

	vatomicptr(signals_containers_t *) signals_container;

	vuint64_t del_threshold;
	vuint64_t cleanup_count;
	vuint64_t cycle_len;

	smr_tid_t cleanup_thread;
	vatomic32_t stop_cleanup;

	smr_thread_lib_t tlib;
	smr_node_get_next fun_get_next;
} cleanup_t;

/* -- Private Functions -- */
static inline void _cleanup_free_list(cleanup_t *smr, smr_node_t *lst);
static inline void *_cleanup_cleanup(void *args);
static inline cleanup_signal_t *
_cleanup_get_signal(cleanup_t *cleanup, signals_containers_t *container,
		    smr_tid_t tid_a);
static inline void _cleanup_await_signals_on(cleanup_t *cleanup,
					     signals_containers_t *container);
static inline signals_containers_t *_cleanup_setup_signals(cleanup_t *cleanup);
static inline void _smr_threads_init(smr_threads_t *threads, smr_lock_lib_t *);
static inline vbool_t _smr_threads_get_idx(cleanup_t *cleanup, smr_tid_t id,
					   vsize_t *out_idx);
static inline void _smr_threads_add(cleanup_t *cleanup, smr_tid_t id);

/**
 * Initializes the given cleanup object.
 *
 * @note creates a cleaner thread that runs in the background.
 *
 * @param cleanup address of cleanup_t object.
 * @param threshold in microseconds, determine how long the cleanup thread
 * should sleep before triggering a new cleanup cycle.
 * @param tlib address of smr_thread_lib_t object. Contains function
 * pointers for thread creation and join. If NULL is given, pthread lib is used.
 * @param lock address of smr_lock_lib_t object.
 */
static void cleanup_init(cleanup_t *cleanup, vuint64_t threshold,
			 smr_thread_lib_t *tlib, smr_lock_lib_t *lock)
{
	vsize_t i = 0;
	ASSERT(cleanup);
	ASSERT(threshold);

	_smr_threads_init(&cleanup->threads, lock);

	smr_nodes_list_init(&cleanup->del_lst);

	/* initially cleanup is not active */
	vatomicptr_write_rlx(&cleanup->signals_container, NULL);

	cleanup->del_threshold = threshold;
	cleanup->cleanup_count = 0;
	cleanup->cycle_len = 0;

	for (i = 0; i < CLEANUP_MAX_NUM_CONTAINERS; i++) {
		cleanup->containers[i].max_len = SMR_MAX_NTHREADS;
		cleanup->containers[i].signals_len = 0;
	}
	cleanup->container_idx = 0;
	vatomic32_write_rlx(&cleanup->stop_cleanup, 0);

	if (tlib) {
		// make sure each field was set
		smr_thread_lib_not_null(tlib);
		// copy the struct
		cleanup->tlib = *tlib;
	} else {
#if defined(DEFAULT_SMR_THREAD_LIB)
		// use the default values, if defined, when tlib == NULL
		cleanup->tlib = DEFAULT_SMR_THREAD_LIB();
#else
#if !defined(VSYNC_VERIFICATION)
		// avoid passing the NULL test, because of possibly trash values
		int r = memset_s(&cleanup->tlib, sizeof(cleanup->tlib), 0,
				 sizeof(cleanup->tlib));
		BUG_ON(r == 0);
#endif
#endif
		// verify the fields were set
		smr_thread_lib_not_null(&cleanup->tlib);
	}

#if !defined(SMR_CLEANUP_DONT_SPAWN_BG_THREAD)
	cleanup->tlib.thread_create(&cleanup->cleanup_thread, _cleanup_cleanup,
				    cleanup);
#endif
}
/**
 * Destroys all associated resources with the given cleanup object.
 *
 * In addition to destroying all previously retired objects. It stops the
 * cleaning background thread from running.
 *
 * @note call it only after all threads are finished.
 *
 * @param cleanup address of cleanup_t object.
 */
static void cleanup_destroy(cleanup_t *cleanup)
{
	ASSERT(cleanup);

#if defined(SMR_CLEANUP_DONT_SPAWN_BG_THREAD)
	smr_node_t *lst_to_free =
		smr_nodes_list_get_and_empty(&cleanup->del_lst);
	if (lst_to_free)
		_cleanup_free_list(cleanup, lst_to_free);
#else
	vatomic32_write_rel(&cleanup->stop_cleanup, 1);
	cleanup->tlib.thread_join(cleanup->cleanup_thread);
#endif

	ASSERT(smr_nodes_list_is_empty(&cleanup->del_lst));
}
/**
 * Gives the signal that a new operation is about to start.
 *
 * By calling this function, threads give their signal to the cleanup thread
 * indicating that they are about to call a concurrent  operation. The signal is
 * given if and only if it has been requested.
 *
 * @note Threads must call this function right before they call any of the
 * algo/data-structure operations, for which cleanup is used.
 * Note that all objects obtained from previous operations are considered no
 * longer safe to access after the signal is given.
 *
 * @param cleanup address of cleanup_t object.
 */
static inline void cleanup_set_signal(cleanup_t *cleanup)
{
	cleanup_signal_t *my_signal = NULL;
	signals_containers_t *container = NULL;
	smr_tid_t tid;

	ASSERT(cleanup);

	container = (signals_containers_t *)vatomicptr_read_rlx(
		&cleanup->signals_container);

	if (unlikely(container)) {
		container = (signals_containers_t *)vatomicptr_read_acq(
			&cleanup->signals_container);
		// there is indeed a container, which means a signal is requested and we
		// need to give it
		if (container) {
			ASSERT(container);
			// fetch the associated signal location of this thread
			tid = cleanup->tlib.thread_self();
			my_signal =
				_cleanup_get_signal(cleanup, container, tid);
			if (my_signal) {
				// give the signal
				vatomic32_write_rel(&my_signal->cleanup_signal,
						    1);
			}
			/* if we do not have a signal it either means we registered after
             * the cleanup cycle began, which is fine and the cycle can complete
             * without our confirmation. Or, we are trying to access the data
             * structure without prior registration, which is a violation of the
             * operating conditions of the SMR. TODO: we need a mechanism to
             * assert that without lock acquisition! */
		}
	}
}
/**
 * Adds the calling thread to the registered threads' list.
 *
 * From this point on, cleanup will request this thread's signal
 *
 * @param cleanup address of cleanup_t object.
 */
static inline void cleanup_register_thread(cleanup_t *cleanup)
{
	ASSERT(cleanup);
	smr_tid_t thread_id = cleanup->tlib.thread_self();
	_smr_threads_add(cleanup, thread_id);
}
/**
 * Removes the calling thread from the registered threads' list.
 *
 * From this point on, cleanup will not wait for this thread's signal.
 *
 * @param cleanup address of cleanup_t object.
 */
static inline void cleanup_deregister_thread(cleanup_t *cleanup)
{
	ASSERT(cleanup);
	vsize_t idx = 0;

	smr_threads_t *threads = &cleanup->threads;
	smr_tid_t id = cleanup->tlib.thread_self();

	threads->lock.acq(threads->lock.arg);

	if (_smr_threads_get_idx(cleanup, id, &idx)) {
		// reuse the slot
		threads->id[idx] = threads->id[threads->idx - 1];
		threads->idx = threads->idx - 1;
	} else {
		ASSERT(0 &&
		       "a thread is trying to deregister when it has never been "
		       "registered");
	}

	/* in case the cleanup was waiting for the signal */
	cleanup_set_signal(cleanup);

	threads->lock.rel(threads->lock.arg);
}
/**
 * Switches the signals' container to the backup one, and activates the signals
 * place-holders for the registered threads at the time.
 *
 *  @note since it operates on the threads' list it acquires the lock of the
 * thread's list
 *
 * @param cleanup address of cleanup_t object.
 * @return signals_containers_t* address of the new published container object.
 */
static inline signals_containers_t *_cleanup_setup_signals(cleanup_t *cleanup)
{
	ASSERT(cleanup);
	vsize_t i = 0;
	smr_threads_t *threads = &cleanup->threads;
	signals_containers_t *signals_container = NULL;

	threads->lock.acq(threads->lock.arg);

	/* allocate memory */
	cleanup->container_idx =
		(cleanup->container_idx + 1) % CLEANUP_MAX_NUM_CONTAINERS;

	signals_container = &cleanup->containers[cleanup->container_idx];

	ASSERT(signals_container);

	signals_container->signals_len = threads->idx;
	ASSERT(signals_container->signals_len <= signals_container->max_len);

	/* fill in the active threads' thread-ids in the signal array */
	for (i = 0; i < threads->idx; i++) {
		signals_container->signals[i].tid = threads->id[i];
		vatomic32_write_rlx(
			&signals_container->signals[i].cleanup_signal, 0);
	}

	/* publish the new container, do it in the CS,
     * when a thread deregisters, it also gives the signal
     * we need to avoid a thread giving the signal for the old
     * container.
     */
	vatomicptr_write_rel(&cleanup->signals_container, signals_container);

	threads->lock.rel(threads->lock.arg);

	return signals_container;
}
/**
 * Waits for all registered threads to give their signals.
 *
 * A blocking-function called by the cleanup-thread
 * returns once all registered threads have given their signal.
 *
 * @param cleanup address of cleanup_t object.
 * @param container address of the container object, where signals are received.
 */
static inline void _cleanup_await_signals_on(cleanup_t *cleanup,
					     signals_containers_t *container)
{
	vsize_t i = 0;
	ASSERT(container);

	for (i = 0; i < container->signals_len; i++) {
		while (vatomic32_read(&container->signals[i].cleanup_signal) !=
		       1U) {
			verification_ignore();
			cleanup->tlib.thread_yield();
		}
	}
}

/**
 * Returns the address of the signal's slot associated with thread `tid_a`.
 *
 * @param container address of the container object, where signals are received.
 * @param cleanup address of cleanup_t object.
 * @param tid_a thread id.
 * @return address of cleanup_signal_t object associated with `tid_a`.
 * @return NULL if no slot was associated with `tid_a`.
 */
static inline cleanup_signal_t *
_cleanup_get_signal(cleanup_t *cleanup, signals_containers_t *container,
		    smr_tid_t tid_a)
{
	vsize_t i = 0;
	vsize_t signals_len = 0;
	cleanup_signal_t *signals = NULL;
	smr_tid_t tid_b;

	ASSERT(container);
	signals = container->signals;
	signals_len = container->signals_len;

	for (i = 0; i < signals_len; i++) {
		tid_b = signals[i].tid;
		if (cleanup->tlib.thread_eq(tid_a, tid_b)) {
			return &signals[i];
		}
	}
	return NULL;
}
/**
 * Appends the given `node` to the retired nodes' list.
 *
 * `node` becomes subject for memory reclamation afterwards.
 *
 * @param cleanup address of cleanup_t object.
 * @param node address of smr_node_t object.
 */
static inline void cleanup_add_to_delete(cleanup_t *cleanup, smr_node_t *node,
					 smr_node_destroy_fun destroy,
					 void *destroy_args)
{
	ASSERT(cleanup);
	ASSERT(node);
	smr_nodes_list_add(&cleanup->del_lst, node, destroy, destroy_args);
}
/**
 * Destroys the given list of nodes starting from `head`.
 *
 * @param cleanup address of cleanup_t object.
 * @param head address of smr_node_t object. The head of the retired nodes' list
 * to destroy.
 */
static inline void _cleanup_free_list(cleanup_t *cleanup, smr_node_t *head)
{
	cleanup->cycle_len += smr_nodes_list_destroy(head);
	cleanup->cleanup_count++;
}
/**
 * Runs a cleanup cycle.
 *
 * @note should be called only from a single thread.
 *
 * @param cleanup address of cleanup_t object.
 *
 * @return true some retired nodes were freed.
 * @return false the retired list was empty. Nothing was freed.
 */
static inline vbool_t cleanup_recycle(cleanup_t *cleanup)
{
	signals_containers_t *new_container = NULL;
	void *lst_to_free = NULL;

	lst_to_free = smr_nodes_list_get_and_empty(&cleanup->del_lst);

	verification_assume(lst_to_free != NULL);

	if (!lst_to_free) {
		return false;
	}

	/* creates a hash-set to communicate with the threads */
	new_container = _cleanup_setup_signals(cleanup);

	/* waits for all signals to be on */
	_cleanup_await_signals_on(cleanup, new_container);
	/*
     * we already got the signals we need to recycle/free the current list.
     * we stop requesting signals, by setting the container to NULL
     */
	vatomicptr_write_rlx(&cleanup->signals_container, NULL);

	/* we clean up the list now */
	_cleanup_free_list(cleanup, lst_to_free);

	return true;
}
/**
 * Runs cleanup cycles as long as required.
 *
 * Periodically checks if retired nodes' list is not empty, if so then
 *  - pops all the retired nodes
 *  - it places a signals' request and waits till all signals are received
 *  - it triggers recycling/freeing of the popped nodes
 *
 * @note Cleanup thread function.
 *
 * @param args unused.
 * @return void* NULL.
 */
static inline void *_cleanup_cleanup(void *args)
{
	ASSERT(args);
	cleanup_t *cleanup = (cleanup_t *)args;
	void *lst_to_free = NULL;

	while (!vatomic32_read_acq(&cleanup->stop_cleanup)) {
		cleanup->tlib.thread_usleep(cleanup->del_threshold);
		cleanup_recycle(cleanup);
	} /* signal was given */

	/* the system is shut down, now we have to delete whatever is in there */
	lst_to_free = smr_nodes_list_get_and_empty(&cleanup->del_lst);
	if (lst_to_free) {
		_cleanup_free_list(cleanup, lst_to_free);
	}
	return NULL;
}
/**
 * Acquires the lock.
 *
 * @param l address of lock object.
 */
static inline void _default_acq(void *l)
{
	ttaslock_t *lock = l;
	ttaslock_acquire(lock);
}
/**
 * Releases the lock.
 *
 * @param l address of lock object.
 */
static inline void _default_rel(void *l)
{
	ttaslock_t *lock = l;
	ttaslock_release(lock);
}
/**
 * Initialises the given object `threads`.
 *
 * @param threads address of smr_threads_t object.
 * @param lock address of smr_lock_lib_t object.
 */
static inline void _smr_threads_init(smr_threads_t *threads,
				     smr_lock_lib_t *lock)
{
	ASSERT(threads);
	threads->idx = 0;
	if (lock) {
		ASSERT(smr_lock_lib_is_set(lock));
#if defined(VSYNC_VERIFICATION)
		threads->lock.arg = &lock->arg;
		threads->lock.acq = lock->acq;
		threads->lock.rel = lock->rel;
#else
		threads->lock = *lock;
#endif
	} else {
		ttaslock_init(&threads->default_lock);
		threads->lock.arg = &threads->default_lock;
		threads->lock.acq = _default_acq;
		threads->lock.rel = _default_rel;
	}
}
/**
 * Looks for the index associated with the given thread `id`.
 *
 * @note acquire the lock before calling this function.
 *
 * @param id thread id.
 * @param out_idx the index associated with `id` if found.
 * @param cleanup address of cleanup_t object.
 * @return true if thread has been already registered and index is found.
 * @return false thread has never been registered, no associated index is found.
 */
static inline vbool_t _smr_threads_get_idx(cleanup_t *cleanup, smr_tid_t id,
					   vsize_t *out_idx)
{
	vsize_t i = 0;
	smr_threads_t *threads = NULL;
	ASSERT(cleanup);
	threads = &cleanup->threads;
	for (i = 0; i < threads->idx; i++) {
		if (cleanup->tlib.thread_eq(id, threads->id[i])) {
			*out_idx = i;
			return true;
		}
	}
	return false;
}
/**
 * Reservers a slot for the thread associated with the given `id`.
 *
 * @param cleanup address of cleanup_t object.
 * @param id thread id.
 */
static inline void _smr_threads_add(cleanup_t *cleanup, smr_tid_t id)
{
	vsize_t idx = 0;
	smr_threads_t *threads = &cleanup->threads;
	ASSERT(threads);

	threads->lock.acq(threads->lock.arg);

	ASSERT(threads->idx < SMR_MAX_NTHREADS &&
	       "the system is wrongly configured more threads are using the system "
	       "than anticipated");

	/* already exists */
	if (_smr_threads_get_idx(cleanup, id, &idx)) {
		/* this thread has never deregistered */
		ASSERT(0 && "this thread has been registered before");
	} else {
		/* does not exist, we append */
		idx = threads->idx;
		threads->idx = threads->idx + 1;
	}

	threads->id[idx] = id;
	threads->lock.rel(threads->lock.arg);
}
#undef CLEANUP_MAX_NUM_CONTAINERS
#endif
