#  [vsync](../README.md) / [map](README.md) / hashtable_evictable.h
_This is a lock-free listset based hashtable with eviction support._ 

**Groups:** [Lock-free](../GROUP_lock_free.md), [SMR-required](../GROUP_requires_smr.md)

This hashtable uses a lock-free listset per bucket. The listset is ordered by keys, and keys are expected to be unique.

In addition to the standard hashtable operations, this implementation offers `vhashtable_evict` API. Which allows for evicting/retiring hashtable entries. The eviction policy favors longer buckets and oldest entries within those buckets. Note that the eviction FIFO is not global to the hashtable, but local to the buckets.

This feature is useful for controlling the buckets growth and can be useful in cache applications where dropping entries does not affect the soundness of applications.

The table consists of `VHASHTABLE_BUCKET_COUNT` number of buckets with a default value of `1024`. This can be overwritten by the user by compiling with `-DVHASHTABLE_BUCKET_COUNT=N`. User control the mapping between keys and buckets by passing `hash_idx` to the APIs. It is expected that users use their own hash functions that map a key to a certain bucket index. Thus the given `hash_idx` is expected to be `< VHASHTABLE_BUCKET_COUNT`.

This is a concurrent lock-free implementation and it requires users to deploy a safe memory reclamation scheme in order to free/recycle evicted/detached nodes safely.


### Example:



```c
#include <vsync/smr/gdump.h>
#include <vsync/map/hashtable_evictable.h>
#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N         12
#define ITER      10
#define THRESHOLD 2

typedef struct user_key_s {
    vuint32_t x;
    vuint32_t y;
    vuint16_t z;
} user_key_t;

typedef struct entry_s {
    vhashtable_entry_t ht_entry;
    user_key_t key;
    smr_node_t smr_node;
} entry_t;

#define PRINT printf

pthread_rwlock_t g_lock;

vhashtable_t g_hash_tbl;

gdump_t g_gdump;

vatomic8_t g_stop = VATOMIC_INIT(0);

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
free_callback(smr_node_t *node, void *arg)
{
    entry_t *entry = V_CONTAINER_OF(node, entry_t, smr_node);
    free(entry);
    V_UNUSED(arg);
}

void
retire_callback(vhashtable_entry_t *node, void *arg)
{
    entry_t *entry = V_CONTAINER_OF(node, entry_t, ht_entry);
    gdump_retire(&g_gdump, &entry->smr_node, free_callback, NULL);
    V_UNUSED(arg);
}

int
cmp_callback(vhashtable_entry_t *node, vhashtable_key_t k)
{
    user_key_t given_key = (*(user_key_t *)k);
    entry_t *entry       = V_CONTAINER_OF(node, entry_t, ht_entry);
    user_key_t entry_key = entry->key;

    if (entry_key.x != given_key.x) {
        if (entry_key.x < given_key.x)
            return -1;
        return 1;
    } else if (entry_key.y != given_key.y) {
        if (entry_key.y < given_key.y)
            return -1;
        return 1;
    } else if (entry_key.z != given_key.z) {
        if (entry_key.z < given_key.z)
            return -1;
        return 1;
    } else {
        ASSERT(entry_key.z == given_key.z);
        ASSERT(entry_key.x == given_key.x);
        ASSERT(entry_key.y == given_key.y);
        return 0;
    }
}

vsize_t
hashit(user_key_t key)
{
    return (key.x ^ (key.y << 2) ^ (key.z << 4)) &
           (VHASHTABLE_BUCKET_COUNT - 1);
}

void
producer(vsize_t tid)
{
    gdump_thread_t thread;
    entry_t *entry  = NULL;
    vbool_t success = false;

    gdump_register(&g_gdump, &thread);

    for (vuint16_t i = 0; i < ITER; i++) {
        user_key_t user_key  = {.x = i, .z = i, .y = i};
        vhashtable_key_t key = (vhashtable_key_t)&user_key;
        vsize_t idx          = hashit(user_key);

        gdump_enter(&g_gdump, &thread);

        vhashtable_entry_t *node = vhashtable_get(&g_hash_tbl, key, idx);

        if (node == NULL) {
            entry      = malloc(sizeof(entry_t));
            entry->key = user_key;
            success =
                vhashtable_insert(&g_hash_tbl, key, &entry->ht_entry, idx);

            if (success) {
                printf("T%zu added entry with key {%u, %u, %u} \n", tid,
                       user_key.x, user_key.z, user_key.y);
            } else {
                printf("T%zu key {%u, %u, %u} already exist \n", tid,
                       user_key.x, user_key.z, user_key.y);
                free(entry);
            }
        }

        gdump_exit(&g_gdump, &thread);

        vuint64_t count = vhashtable_get_entries_count(&g_hash_tbl);
        if (count > THRESHOLD) {
            gdump_enter(&g_gdump, &thread);
            vuint64_t evicted =
                vhashtable_evict(&g_hash_tbl, count - THRESHOLD);
            printf("T%zu Evicted %lu\n", tid, evicted);
            gdump_exit(&g_gdump, &thread);
        }
    }

    gdump_deregister(&g_gdump, &thread);
}

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

void *
run(void *args)
{
    vsize_t tid = (vsize_t)args;
    if (tid == 0) {
        reclaim();
    } else {
        producer(tid);
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
    vhashtable_init(&g_hash_tbl, retire_callback, NULL, cmp_callback);

    for (vsize_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, run, (void *)i);
    }

    for (vsize_t i = 1; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    vatomic8_write(&g_stop, 1);
    pthread_join(threads[0], NULL);

    vhashtable_destroy(&g_hash_tbl);
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
| [vhashtable_init](hashtable_evictable.h.md#function-vhashtable_init) | Initializes the hashtable.  |
| [vhashtable_destroy](hashtable_evictable.h.md#function-vhashtable_destroy) | Evicts all entries in the hashtable.  |
| [vhashtable_insert](hashtable_evictable.h.md#function-vhashtable_insert) | Inserts the given `entry` into the hashtable.  |
| [vhashtable_remove](hashtable_evictable.h.md#function-vhashtable_remove) | Removes the entry associated with the given key from the the hashtable.  |
| [vhashtable_get](hashtable_evictable.h.md#function-vhashtable_get) | Looks for the entry associated with the given `key`.  |
| [vhashtable_evict](hashtable_evictable.h.md#function-vhashtable_evict) | Tries to evict given `count` number of entries.  |
| [vhashtable_get_entries_count](hashtable_evictable.h.md#function-vhashtable_get_entries_count) | Returns the count of entries currently available in the hashtable.  |

##  Function `vhashtable_init`

```c
static void vhashtable_init(vhashtable_t *table, vhashtable_entry_retire_t evict_cb, void *evict_cb_arg, vhashtable_cmp_key_t cmp_cb)
``` 
_Initializes the hashtable._ 




**Parameters:**

- `table`: address of vhashtable_t object. 
- `evict_cb`: address of a callback function of type `vhashtable_entry_retire_t`. The function is called whenever an entry is evicted from the hashtable. 
- `evict_cb_arg`: address of an extra argument of `evict_cb`. 
- `cmp_cb`: address of callback function of type `vhashtable_cmp_key_t`. Used for comparing an entry key to a given key.


> **Note:** must be called before threads start accessing the hashtable. 


##  Function `vhashtable_destroy`

```c
static void vhashtable_destroy(vhashtable_t *table)
``` 
_Evicts all entries in the hashtable._ 




**Parameters:**

- `table`: address of vhashtable_t object.


> **Note:** this function is not thread-safe, can be called iff all threads are done accessing the hashtable. 


##  Function `vhashtable_insert`

```c
static vbool_t vhashtable_insert(vhashtable_t *table, vhashtable_key_t key, vhashtable_entry_t *entry, vsize_t hash_idx)
``` 
_Inserts the given_ `entry` _into the hashtable._ 




**Parameters:**

- `table`: address of vhashtable_t object. 
- `key`: value of the key object. 
- `entry`: address of vhashtable_entry_t object. 
- `hash_idx`: a hash value of the key used as a bucket index. must be `< VHASHTABLE_BUCKET_COUNT` 


**Returns:** true successful insertion. 

**Returns:** false failed insertion, an entry associated with the given `key` already exists.

> **Note:** this function must be called inside an SMR critical section. 


##  Function `vhashtable_remove`

```c
static vbool_t vhashtable_remove(vhashtable_t *table, vhashtable_key_t key, vsize_t hash_idx)
``` 
_Removes the entry associated with the given key from the the hashtable._ 




**Parameters:**

- `table`: address of vhashtable_t object. 
- `key`: value of the key object. 
- `hash_idx`: a hash value of the key used as a bucket index. must be `< VHASHTABLE_BUCKET_COUNT`. 


**Returns:** true successful remove, the entry associated with the given key was removed. 

**Returns:** false failed remove, no entry associated with the given key exists. 



##  Function `vhashtable_get`

```c
static vhashtable_entry_t* vhashtable_get(vhashtable_t *table, vhashtable_key_t key, vsize_t hash_idx)
``` 
_Looks for the entry associated with the given_ `key`_._ 




**Parameters:**

- `table`: address of vhashtable_t object. 
- `key`: value of the key object. 
- `hash_idx`: a hash value of the key used as a bucket index. must be `< VHASHTABLE_BUCKET_COUNT` 


**Returns:** vhashtable_entry_t* address of the `vhashtable_entry_t` object associated with the given key, if found. 

**Returns:** `NULL` if key is not found.

> **Note:** this function must be called inside an SMR critical section. 


##  Function `vhashtable_evict`

```c
static vuint64_t vhashtable_evict(vhashtable_t *table, vuint64_t count)
``` 
_Tries to evict given_ `count` _number of entries._ 


Eviction from a bucket will favor FIFO, i.e. oldest entries in a bucket will be evicted first. However, the policy is local to the bucket. Buckets with length greater than average will be choosen to evict first. After that there is no guarantee which bucket shall evict first. 

**Parameters:**

- `table`: address of vhashtable_t object. 
- `count`: number of entries to evict. 


**Returns:** vuint64_t number of entries actually evicted by the calling thread.

> **Note:** the return value can be `<= count` this can happen if the hashtable does not have that number of entries. Or a parallel insert/evict operation is running and helped with this eviction. In that case only the number of entries evicted by the calling thread are counted.

> **Note:** Evicted entries are handed back to the user through the callback registered via `vhashtable_init`. And they are not safe to recycle/free immediately. It is expected that the user deploys an SMR scheme and retires the nodes to the SMR in the said callback.

> **Note:** this function must be called inside an SMR critical section. 


##  Function `vhashtable_get_entries_count`

```c
static vuint64_t vhashtable_get_entries_count(vhashtable_t *table)
``` 
_Returns the count of entries currently available in the hashtable._ 




**Parameters:**

- `table`: address of vhashtable_t object. 


**Returns:** vuint64_t an approximate count of the entries available in the hashtable. 




---
