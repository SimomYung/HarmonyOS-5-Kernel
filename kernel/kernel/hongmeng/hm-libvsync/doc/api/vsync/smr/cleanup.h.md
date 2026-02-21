#  [vsync](../README.md) / [smr](README.md) / cleanup.h
_Performant SMR scheme for user-space._ 

It relies on quiescent periods to determine when it is safe to reclaim nodes. Cleanup creates an additional thread that handles the reclamation of memory. This thread has a minimal interaction with running threads. Periodically, if there is something to reclaim, cleanup requests signals from the all registered threads. Once the signals are received, cleanup proceeds to recycle the retired nodes that were added before the signal request is placed.

Progress Condition: only `cleanup_register_thread` and `cleanup_deregister_thread` are blocking operations. Ideally, these functions are called only once per thread, and before accessing the data-structure. This means the data-structure will not compromise its progress-condition, by using this scheme.

Performance: The overhead of this scheme on the concurrent algorithm is negligible, but it can be affected by the threshold passed to `cleanup_init` the threshold is the number of microseconds a cleanup should wait before running the next cleanup cycle.

Memory Bound: one drawback of cleanup is that there is no bound on how large the retired nodes' list can grow. One reason for that is a thread can misbehave or delay giving the needed signal indefinitely. If your application scenario fall in that category then [gdump.h](gdump.h.md) might be a better fit.


### Example:



```c

#include <vsync/queue/unbounded_queue_lf.h>
// maximum number of participating threads must be known at compile time
#define SMR_MAX_NTHREADS 12
#include <vsync/smr/cleanup.h>
#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD_MICRO_SEC 1000
#define IT                  1000

typedef struct data_s {
    vsize_t id;
} data_t;

cleanup_t g_cleanup;
vqueue_ub_t g_queue;
pthread_mutex_t g_lock;

static inline void
lock_acq(void *arg)
{
    pthread_mutex_t *lock = (pthread_mutex_t *)arg;
    int ret               = pthread_mutex_lock(lock);
    assert(ret == 0);
}

static inline void
lock_rel(void *arg)
{
    pthread_mutex_t *lock = (pthread_mutex_t *)arg;
    int ret               = pthread_mutex_unlock(lock);
    assert(ret == 0);
}

smr_lock_lib_t g_lock_lib = {lock_acq, lock_rel, &g_lock};

void
free_cb(smr_node_t *node, void *args)
{
    vqueue_ub_node_t *qnode = V_CONTAINER_OF(node, vqueue_ub_node_t, smr_node);
    free(qnode);
    (void)args;
}

void
retire_cb(vqueue_ub_node_t *qnode, void *args)
{
    cleanup_add_to_delete(&g_cleanup, &qnode->smr_node, free_cb, NULL);
    (void)args;
}

void
destroy_cb(vqueue_ub_node_t *qnode, void *args)
{
    free(qnode);
    (void)args;
}

void *
run(void *args)
{
    data_t *data = NULL;
    vsize_t tid  = (vsize_t)args;

    cleanup_register_thread(&g_cleanup);

    for (vsize_t i = 0; i < IT; i++) {
        cleanup_set_signal(&g_cleanup);

        data = vqueue_ub_deq(&g_queue, retire_cb, NULL);
        if (data == NULL) {
            data                    = malloc(sizeof(data_t));
            data->id                = i;
            vqueue_ub_node_t *qnode = malloc(sizeof(vqueue_ub_node_t));
            printf("[T%zu] enq %zu\n", tid, data->id);
            vqueue_ub_enq(&g_queue, qnode, data);
        } else {
            printf("[T%zu] deq %zu\n", tid, data->id);
            free(data);
        }
    }

    cleanup_deregister_thread(&g_cleanup);

    return NULL;
}

int
main(void)
{
    pthread_t threads[SMR_MAX_NTHREADS];
    data_t *data = NULL;

    int ret = pthread_mutex_init(&g_lock, NULL);
    assert(ret == 0);

    /* When NULL is passed as a third parameter the cleaner thread is created
     * with pthread */
    cleanup_init(&g_cleanup, THRESHOLD_MICRO_SEC, NULL, &g_lock_lib);

    vqueue_ub_init(&g_queue);

    for (vsize_t i = 0; i < SMR_MAX_NTHREADS; i++) {
        pthread_create(&threads[i], NULL, run, (void *)i);
    }

    for (vsize_t i = 0; i < SMR_MAX_NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* dequeue all remaining nodes, to be able to destroy data */
    while (data = vqueue_ub_deq(&g_queue, destroy_cb, NULL), data) {
        free(data);
    }

    /* destroy the queue to destroy the remaining sentinel */
    vqueue_ub_destroy(&g_queue, destroy_cb, NULL);

    cleanup_destroy(&g_cleanup);

    ret = pthread_mutex_destroy(&g_lock);
    assert(ret == 0);
    return 0;
}
```

 

---
# Functions 

| Function | Description |
|---|---|
| [cleanup_init](cleanup.h.md#function-cleanup_init) | Initializes the given cleanup object.  |
| [cleanup_destroy](cleanup.h.md#function-cleanup_destroy) | Destroys all associated resources with the given cleanup object.  |
| [cleanup_set_signal](cleanup.h.md#function-cleanup_set_signal) | Gives the signal that a new operation is about to start.  |
| [cleanup_register_thread](cleanup.h.md#function-cleanup_register_thread) | Adds the calling thread to the registered threads' list.  |
| [cleanup_deregister_thread](cleanup.h.md#function-cleanup_deregister_thread) | Removes the calling thread from the registered threads' list.  |
| [cleanup_add_to_delete](cleanup.h.md#function-cleanup_add_to_delete) | Appends the given `node` to the retired nodes' list.  |
| [cleanup_recycle](cleanup.h.md#function-cleanup_recycle) | Runs a cleanup cycle.  |

##  Function `cleanup_init`

```c
static void cleanup_init(cleanup_t *cleanup, vuint64_t threshold, smr_thread_lib_t *tlib, smr_lock_lib_t *lock)
``` 
_Initializes the given cleanup object._ 


> **Note:** creates a cleaner thread that runs in the background.



**Parameters:**

- `cleanup`: address of cleanup_t object. 
- `threshold`: in microseconds, determine how long the cleanup thread should sleep before triggering a new cleanup cycle. 
- `tlib`: address of smr_thread_lib_t object. Contains function pointers for thread creation and join. If NULL is given, pthread lib is used. 
- `lock`: address of smr_lock_lib_t object. 




##  Function `cleanup_destroy`

```c
static void cleanup_destroy(cleanup_t *cleanup)
``` 
_Destroys all associated resources with the given cleanup object._ 


In addition to destroying all previously retired objects. It stops the cleaning background thread from running.

> **Note:** call it only after all threads are finished.



**Parameters:**

- `cleanup`: address of cleanup_t object. 




##  Function `cleanup_set_signal`

```c
static void cleanup_set_signal(cleanup_t *cleanup)
``` 
_Gives the signal that a new operation is about to start._ 


By calling this function, threads give their signal to the cleanup thread indicating that they are about to call a concurrent operation. The signal is given if and only if it has been requested.

> **Note:** Threads must call this function right before they call any of the algo/data-structure operations, for which cleanup is used. Note that all objects obtained from previous operations are considered no longer safe to access after the signal is given.



**Parameters:**

- `cleanup`: address of cleanup_t object. 




##  Function `cleanup_register_thread`

```c
static void cleanup_register_thread(cleanup_t *cleanup)
``` 
_Adds the calling thread to the registered threads' list._ 


From this point on, cleanup will request this thread's signal



**Parameters:**

- `cleanup`: address of cleanup_t object. 




##  Function `cleanup_deregister_thread`

```c
static void cleanup_deregister_thread(cleanup_t *cleanup)
``` 
_Removes the calling thread from the registered threads' list._ 


From this point on, cleanup will not wait for this thread's signal.



**Parameters:**

- `cleanup`: address of cleanup_t object. 




##  Function `cleanup_add_to_delete`

```c
static void cleanup_add_to_delete(cleanup_t *cleanup, smr_node_t *node, smr_node_destroy_fun destroy, void *destroy_args)
``` 
_Appends the given_ `node` _to the retired nodes' list._ 


`node` becomes subject for memory reclamation afterwards.



**Parameters:**

- `cleanup`: address of cleanup_t object. 
- `node`: address of smr_node_t object. 




##  Function `cleanup_recycle`

```c
static vbool_t cleanup_recycle(cleanup_t *cleanup)
``` 
_Runs a cleanup cycle._ 


> **Note:** should be called only from a single thread.



**Parameters:**

- `cleanup`: address of cleanup_t object.


**Returns:** true some retired nodes were freed. 

**Returns:** false the retired list was empty. Nothing was freed. 




---
