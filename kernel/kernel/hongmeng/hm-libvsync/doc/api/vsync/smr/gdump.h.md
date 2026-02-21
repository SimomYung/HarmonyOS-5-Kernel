#  [vsync](../README.md) / [smr](README.md) / gdump.h
_Garbage Dump is an SMR scheme, created in DRC._ 

> **Note:** There are different implementations of garbage dump at the moment. You need to define the version passing to the compiler one of the following defines: -DSMR_GDUMPV1, -DSMR_GDUMPV2, -DSMR_GDUMPV3, -DSMR_GDUMPV4 By default V3 is activated. We recommend the users to benchmark with different versions and choose the best performing version in their applications.

### Configurations

Both GDumpv3 and GDumpv2 allow cooperation between threads that call gdump_deregister and those that call gdump_sync or gdump_recycle.

Threads that deregister check if there are synchronizers that are waiting on them, and if that is the case, they communicate that this is no longer necessary. De-registering threads scan an array, where each slot points to data related to a synchronizer thread. The data caches the progress that the synchronizer had made scanning the list of registered threads. The de-registering thread can know via this array if that synchronizer cached data related to the de-registering thread, and if that's the case, it will update the synchronizers data so that they will not visit the de-registering thread's object in their next scan.

This array has a number of slots equal to `VGDUMP_MAX_SYNCERS=5`. If there are more synchronizers than slots, surplus synchronizers spin till a slot becomes vacant (this spinning does not impede the progress of other threads). Users that have applications where there are less or more synchronizers can overwrite this number at compile time via `-DVGDUMP_MAX_SYNCERS=N` where `N` is the maximum number of parallel threads that can call gdump_sync/gdump_recycle.

List of configurations that can be overwritten


- -DVGDUMP_MAX_SYNCERS=N, maximum number of concurrent synchronizers
- -DV_SMR_NODE_BUFF_SIZE=N, maximum capacity of local buffer used with local retirement


### Simple Example



```c

#include <vsync/smr/gdump.h>
#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_WRITER 3
#define N_READER 6

#define N          (N_WRITER + N_READER)
#define LEN_SHARED N

#define USE_RECYCLE

vatomicptr_t g_shared_data[LEN_SHARED]; /* this is to simulated shared data */

typedef struct data_s {
    smr_node_t smr_node; /* embed the SMR node, we put it on top in this example
                          * so that the address of the smr_node field is equal
                          * to the parent data_t address.
                          */
    vsize_t id;
} data_t;

gdump_t g_gdump;
pthread_rwlock_t g_lock;

static inline void
thread_rw_read_acq(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_rdlock(lock);
    assert(ret == 0);
}

static inline void
thread_rw_read_rel(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_unlock(lock);
    assert(ret == 0);
}
static inline void
thread_rw_write_acq(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_wrlock(lock);
    assert(ret == 0);
}
static inline void
thread_rw_write_rel(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_unlock(lock);
    assert(ret == 0);
}

smr_rwlock_lib_t g_rwlock_lib = {thread_rw_read_acq, thread_rw_read_rel,
                                 thread_rw_write_acq, thread_rw_write_rel,
                                 &g_lock};

void
free_cb(smr_node_t *snode, void *args)
{
    /* if snode was not the first field, use
     * data = V_CONTAINER_OF(snode, data_t, smr_node)
     * instead
     */
    data_t *data = (data_t *)snode;
    printf("It is safe to free: %p \n", data);
    free(data);
    (void)args;
}

void
reader(gdump_thread_t *thread)
{
    data_t *data = NULL;
    for (vsize_t i = 0; i < LEN_SHARED; i++) {
        gdump_enter(&g_gdump, thread);
        data = vatomicptr_read_rlx(&g_shared_data[i]);
        if (data) {
            printf("Read %zu\n", data->id);
        }
        gdump_exit(&g_gdump, thread);
    }
}

void
writer(gdump_thread_t *thread, vsize_t idx)
{
    data_t *data = malloc(sizeof(data_t));
    data->id     = idx;
    // publish
    vatomicptr_write_rlx(&g_shared_data[idx], data);
    usleep(1);
    // detach
    vatomicptr_write_rlx(&g_shared_data[idx], NULL);

#ifdef USE_RECYCLE
    /* use gdump_retire or gdump_retire_local depending on contention levels
     */
    // less contented retire, retired to a local list, and only when the list is
    // full it is moved to the global list
    // gdump_retire_local(&g_gdump, thread, &data->smr_node, free_cb, NULL);
    // retire to global list
    gdump_retire(&g_gdump, &data->smr_node, free_cb, NULL);
    // recycle if there is at least one node in the global list
    vsize_t count = gdump_recycle(&g_gdump, sched_yield, 1);
    printf("%zu nodes were recycled \n", count);
#else
    gdump_sync(&g_gdump, sched_yield);
    free(data);
#endif
    (void)thread;
}

void *
run(void *args)
{
    gdump_thread_t thread;
    vsize_t tid = (vsize_t)args;

    /* threads must register to gdump, before using it */
    gdump_register(&g_gdump, &thread);
    if (tid % 2 == 0) {
        writer(&thread, tid % LEN_SHARED);
    } else {
        reader(&thread);
    }
    /* threads must deregister from gdump, after they are done using it */
    gdump_deregister(&g_gdump, &thread);
    return NULL;
}

int
main(void)
{
    pthread_t threads[N];

    /* rwlock is needed if multiple threads are going to call sync/recycle. If
     * only one thread is assigned this task a normal lock can do */
    int ret = pthread_rwlock_init(&g_lock, NULL);
    assert(ret == 0);

    gdump_init(&g_gdump, g_rwlock_lib);

    for (vsize_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, run, (void *)i);
    }

    for (vsize_t i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    gdump_destroy(&g_gdump);

    ret = pthread_rwlock_destroy(&g_lock);
    assert(ret == 0);
    return 0;
}
```



### Example with a data structure



```c

#include <vsync/map/listset_lf.h>
#include <vsync/smr/gdump.h>
#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 11

typedef struct data_s {
    vlistset_node_t lst_node; /* embed list set node */
    vlistset_key_t key;
    smr_node_t smr_node; /* embed smr node */
} data_t;

#define MIN_KEY 0
#define MAX_KEY 10

gdump_t g_gdump;

vlistset_t g_lst;

pthread_rwlock_t g_lock;

static inline void
thread_rw_read_acq(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_rdlock(lock);
    assert(ret == 0);
}

static inline void
thread_rw_read_rel(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_unlock(lock);
    assert(ret == 0);
}
static inline void
thread_rw_write_acq(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_wrlock(lock);
    assert(ret == 0);
}
static inline void
thread_rw_write_rel(void *arg)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)arg;
    int ret                = pthread_rwlock_unlock(lock);
    assert(ret == 0);
}

smr_rwlock_lib_t g_rwlock_lib = {thread_rw_read_acq, thread_rw_read_rel,
                                 thread_rw_write_acq, thread_rw_write_rel,
                                 &g_lock};

void
free_cb(smr_node_t *snode, void *args)
{
    data_t *data = V_CONTAINER_OF(snode, data_t, smr_node);
    free(data);
    (void)args;
}

int
cmp_cb(vlistset_node_t *node, vlistset_key_t key)
{
    data_t *data = (data_t *)node;
    if (data->key == key) {
        return 0;
    } else if (data->key < key) {
        return -1;
    } else {
        return 1;
    }
}

void
retire_cb(vlistset_node_t *node, void *args)
{
    data_t *data = (data_t *)node;
    gdump_retire(&g_gdump, &data->smr_node, free_cb, NULL);
    (void)args;
}

vatomic8_t g_stop = VATOMIC_INIT(0);

void
reclaim(void)
{
    while (vatomic8_read(&g_stop) == 0) {
        vsize_t count = gdump_recycle(&g_gdump, sched_yield, 1);
        if (count > 0) {
            printf("%zu node(s) were reclaimed\n", count);
        }
    }
}

void
reader(gdump_thread_t *thread)
{
    data_t *data = NULL;
    gdump_register(&g_gdump, thread);
    for (vlistset_key_t key = MIN_KEY; key <= MAX_KEY; key++) {
        gdump_enter(&g_gdump, thread);
        data = (data_t *)vlistset_get(&g_lst, key);
        if (data) {
            assert(key == data->key);
            printf("Reader found key %lu\n", data->key);
        }
        gdump_exit(&g_gdump, thread);
    }
    gdump_deregister(&g_gdump, thread);
}

void
writer(gdump_thread_t *thread)
{
    data_t *data    = NULL;
    vbool_t success = false;

    gdump_register(&g_gdump, thread);
    for (vlistset_key_t key = MIN_KEY; key <= MAX_KEY; key++) {
        gdump_enter(&g_gdump, thread);
        success = vlistset_remove(&g_lst, key);
        gdump_exit(&g_gdump, thread);
        if (success) {
            printf("Removed node with key %lu\n", key);
        } else {
            data      = malloc(sizeof(data_t));
            data->key = key;
            gdump_enter(&g_gdump, thread);
            success = vlistset_add(&g_lst, key, &data->lst_node);
            gdump_exit(&g_gdump, thread);
            if (success) {
                printf("Added node with key %lu\n", key);
            } else {
                free(data);
            }
        }
    }
    gdump_deregister(&g_gdump, thread);

    // writer is done, it can trigger recycle if it wishes
    vsize_t count = gdump_recycle(&g_gdump, sched_yield, 1);
    printf("writer reclaimed %zu nodes\n", count);
}

void *
run(void *args)
{
    gdump_thread_t thread;
    vsize_t tid = (vsize_t)args;

    if (tid == 0) {
        reclaim();
    } else if (tid % 2 == 0) {
        writer(&thread);
    } else {
        reader(&thread);
    }

    return NULL;
}

int
main(void)
{
    pthread_t threads[N];

    int ret = pthread_rwlock_init(&g_lock, NULL);
    assert(ret == 0);

    gdump_init(&g_gdump, g_rwlock_lib);
    vlistset_init(&g_lst, retire_cb, NULL, cmp_cb);

    for (vsize_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, run, (void *)i);
    }

    for (vsize_t i = 1; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    vatomic8_write(&g_stop, 1);
    pthread_join(threads[0], NULL);

    vlistset_destroy(&g_lst);
    gdump_destroy(&g_gdump);

    ret = pthread_rwlock_destroy(&g_lock);
    assert(ret == 0);
    return 0;
}
```

 

---
# Functions 

| Function | Description |
|---|---|
| [gdump_init](gdump.h.md#function-gdump_init) | Initializes the given `gdump_t` object.  |
| [gdump_destroy](gdump.h.md#function-gdump_destroy) | Destroys all retired nodes.  |
| [gdump_enter](gdump.h.md#function-gdump_enter) | Marks the beginning of a critical section.  |
| [gdump_exit](gdump.h.md#function-gdump_exit) | Marks the end of a critical section.  |
| [gdump_register](gdump.h.md#function-gdump_register) | Registers the calling thread.  |
| [gdump_deregister](gdump.h.md#function-gdump_deregister) | Deregisters the calling thread.  |
| [gdump_retire](gdump.h.md#function-gdump_retire) | Retires the given `node` to the global retirement list.  |
| [gdump_retire_local](gdump.h.md#function-gdump_retire_local) | Retires the given `node` to a thread's local buffer.  |
| [gdump_sync](gdump.h.md#function-gdump_sync) | Synchronizes active threads.  |
| [gdump_recycle](gdump.h.md#function-gdump_recycle) | Recycles/frees all retired nodes.  |

##  Function `gdump_init`

```c
static void gdump_init(struct gdump_s *gd, smr_rwlock_lib_t lock)
``` 
_Initializes the given_ `gdump_t` _object._ 




**Parameters:**

- `gd`: address of `gdump_t` object. 
- `lock`: address of `smr_rwlock_lib_t` object containing function pointers for acquiring/releasing a read-write lock.


> **Note:** when used in user space `pthread_rwlock` is recommended. 


##  Function `gdump_destroy`

```c
static void gdump_destroy(struct gdump_s *gd)
``` 
_Destroys all retired nodes._ 


> **Note:** call it only if all threads are finished.



**Parameters:**

- `gd`: address of gdump_t object. 




##  Function `gdump_enter`

```c
static void gdump_enter(struct gdump_s *, struct gdump_thread_s *thrd)
``` 
_Marks the beginning of a critical section._ 


All shared resources are protected from reclamation while the thread is in a critical section. 
**Postcondition:** `gdump_exit`

> **Note:** Threads must call this function right before they call any of the algo/data-structure operations, for which gd is used. 

**Parameters:**

- `gd`: address of gdump_t object. 




##  Function `gdump_exit`

```c
static void gdump_exit(struct gdump_s *gd, struct gdump_thread_s *thrd)
``` 
_Marks the end of a critical section._ 



**Precondition:** `gdump_enter` 


**Parameters:**

- `gd`: address of gdump_t object. 
- `thread`: address of gdump_thread_t object. 




##  Function `gdump_register`

```c
static void gdump_register(struct gdump_s *gd, struct gdump_thread_s *thrd)
``` 
_Registers the calling thread._ 


> **Note:** must be called once per thread after the first invocation of `gdump_enter`.

> **Note:** acquires the lock as a writer. 

**Parameters:**

- `gd`: address of gdump_t object. 
- `thread`: address of gdump_thread_t object. 




##  Function `gdump_deregister`

```c
static void gdump_deregister(struct gdump_s *gd, struct gdump_thread_s *thrd)
``` 
_Deregisters the calling thread._ 


> **Note:** must be called once per thread after the last invocation of `gdump_exit`.

> **Note:** acquires the lock as a writer. 

**Parameters:**

- `gd`: address of gdump_t object. 
- `thread`: address of gdump_thread_t object. 




##  Function `gdump_retire`

```c
static void gdump_retire(struct gdump_s *gd, smr_node_t *node, smr_node_destroy_fun destroy_fun, void *destroy_fun_arg)
``` 
_Retires the given_ `node` _to the global retirement list._ 


> **Note:** a retired node is placed on a limbo list till its safe to reclaim. In order to trigger reclamation `gdump_recycle` must be periodically called.  (see also [gdump_retire_local](gdump.h.md#function-gdump_retire_local). )


**Parameters:**

- `gd`: address of gdump_t object. 
- `node`: address of smr_node_t object. 
- `destroy_fun`: address of callback function, used for node destruction. 
- `destroy_fun_arg`: additional argument passed to `destroy_fun` 




##  Function `gdump_retire_local`

```c
static void gdump_retire_local(struct gdump_s *gd, struct gdump_thread_s *thread, smr_node_t *node, smr_node_destroy_fun destroy_fun, void *destroy_fun_arg)
``` 
_Retires the given_ `node` _to a thread's local buffer._ 


> **Note:** This API can be used to reduce contention on global retirement list. It is useful when retirement is frequent among threads. Users may choose to only use `gdump_retire`. However, in some cases local retirement might provide performance benefits. When the buffer is full nodes are automatically moved to the global list, and the buffer is emptied. Users can overwrite the default capacity of the local buffer by compiling with `-DV_SMR_NODE_BUFF_SIZE=N`. Where N is the capacity of threads' local buffers.



**Parameters:**

- `gd`: address of gdump_t object. 
- `thread`: address of gdump_thread_t object. 
- `node`: address of smr_node_t object. 
- `destroy_fun`: address of callback function, used for node destruction. 
- `destroy_fun_arg`: additional argument passed to `destroy_fun`


> **Note:** This feature is only available when `VGDUMP_ENABLE_BUFF` is defined. 


##  Function `gdump_sync`

```c
static void gdump_sync(struct gdump_s *gd, gdump_yield_fun_t yield)
``` 
_Synchronizes active threads._ 


This function ensures that all active threads, i.e. executing inside a critical section marked with gdump_enter/gdump_exit, have exited their witnessed critical section by the time this function returns.

> **Note:** this function can be used for reclamation in synchronous way.


### Example:



```
detach(n);
gdump_sync(gd, sched_yield);
free(n);
```



> **Note:** it is recommended to use background recycling and not call `gdump_sync` directly.  (see [`gdump_recycle`](gdump.h.md#function-gdump_recycle))

> **Note:** This function acquires the lock as a reader and is blocking.

> **Note:** DO NOT call this function inside a critical section. 

**Parameters:**

- `gd`: address of gdump_t object. 
- `yield`: a function pointer to a function like `sched_yield` to use during spinning. 




##  Function `gdump_recycle`

```c
static vsize_t gdump_recycle(struct gdump_s *gd, gdump_yield_fun_t yield, vuint64_t min_count)
``` 
_Recycles/frees all retired nodes._ 


The recycling happens if the count of the retired nodes is `>= min_count`.

> **Note:** don't call while in in a critical section.

> **Note:** It is recommended to dedicate a specific thread for periodic recycling.

> **Note:**


### Example:



```
// continuously recycle if there is at least 10 retired nodes
while(true) {
 gdump_recycle(gd, sched_yield, 10);
 sleep(1);
}
```

 

**Parameters:**

- `gd`: address of gdump_t object. 
- `yield`: a function pointer to a function like `sched_yield` to use while spinning. 
- `min_count`: the minimum count to trigger recycling. 


**Returns:** number of recycled nodes. 




---
