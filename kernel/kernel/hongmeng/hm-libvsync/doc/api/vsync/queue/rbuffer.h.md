#  [vsync](../README.md) / [queue](README.md) / rbuffer.h
_A formally verified and efficient ring buffer._ 

rbuffer provides almost non-blocking multi-producer and speculative single-consumer. The C code has been formally verified and optimized with vsyncer on weak memory models


### Example:



```c

#include <vsync/queue/rbuffer.h>
#include <vsync/common/assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE           1024U
#define BLK_NUM            8U
#define TRY_TIME           100U
#define WRITER_CNT         4U
#define N                  (1U + WRITER_CNT)
#define ENTRIES_PER_WRITER 10U
#define MAX_CONSUME        (WRITER_CNT * ENTRIES_PER_WRITER)
#define ENTRY_SIZE         sizeof(data_t)

typedef struct data_s {
    vsize_t id;
} data_t;

rbuffer_t g_rbuffer;

void
try_read(data_t *data)
{
    vsize_t sz = 0;
    do {
        sz = rb_try_read_entry(&g_rbuffer, data, ENTRY_SIZE, NULL);
    } while (sz == 0);
    ASSERT(sz == ENTRY_SIZE);
}

void
read(data_t *data)
{
    vsize_t sz = 0;
    sz         = rb_read_entry(&g_rbuffer, data, ENTRY_SIZE, NULL);
    ASSERT(sz == ENTRY_SIZE);
}

void
reader(void)
{
    data_t data = {0};
    for (vsize_t i = 0; i < MAX_CONSUME; i++) {
        // we show two possible ways of reading
        if (i % 2 == 0) {
            read(&data);
        } else {
            try_read(&data);
        }
        printf("Reader consumed entry#%zu\n", data.id);
    }
}

void
alt_write(vsize_t val)
{
    entry_t *entry = NULL;
    data_t *data   = NULL;
    do {
        entry = rb_try_alloc_entry(&g_rbuffer, ENTRY_SIZE);
    } while (entry == NULL);
    data     = entry_get_data(entry);
    data->id = val;
    rb_commit_entry(&g_rbuffer, entry);
}

void
write(vsize_t val)
{
    vsize_t sz  = 0;
    data_t data = {0};
    data.id     = val;
    do {
        sz = rb_write_entry(&g_rbuffer, &data, ENTRY_SIZE, TRY_TIME);
    } while (sz == 0);
    ASSERT(sz == ENTRY_SIZE);
}

void
writer(vsize_t tid)
{
    vsize_t val = 0;
    for (vsize_t i = 0; i < ENTRIES_PER_WRITER; i++) {
        val = (tid * i) + i;
        // we show two possible ways of writing
        if (i % 2 == 0) {
            write(val);
        } else {
            alt_write(val);
        }
        printf("writer %zu, wrote its entry#%zu\n", tid, val);
    }
}

void *
run(void *args)
{
    vsize_t tid = (vsize_t)(vuintptr_t)args;
    if (tid == 0) {
        // single-consumer
        reader();
    } else {
        // multiple producers
        writer(tid);
    }
    return NULL;
}

int
main(void)
{
    pthread_t threads[N];

    void *buffer = malloc(BUF_SIZE);
    rb_init(&g_rbuffer, buffer, BUF_SIZE, BLK_NUM, RB_RDWR);

    for (vsize_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, run, (void *)(vuintptr_t)i);
    }

    for (vsize_t i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    free(buffer);
    return 0;
}
```

 

---
# Functions 

| Function | Description |
|---|---|
| [rb_init](rbuffer.h.md#function-rb_init) | Inits ring buffer.  |
| [rb_try_alloc_entry](rbuffer.h.md#function-rb_try_alloc_entry) | Tries to alloc an entry from ring buffer.  |
| [rb_commit_entry](rbuffer.h.md#function-rb_commit_entry) | Commits an entry.  |
| [rb_write_entry](rbuffer.h.md#function-rb_write_entry) | Writes an entry to ring buffer.  |
| [rb_try_read_entry](rbuffer.h.md#function-rb_try_read_entry) | Tries to read an entry from ring buffer.  |
| [rb_read_entry](rbuffer.h.md#function-rb_read_entry) | Reads an entry from ring buffer.  |
| [rb_rewind_reader](rbuffer.h.md#function-rb_rewind_reader) | Moves reader pointer to the oldest block.  |
| [rb_get_write_offset](rbuffer.h.md#function-rb_get_write_offset) | Gets write offset from entry.  |

##  Function `rb_init`

```c
static int rb_init(struct rbuffer_s *rb, void *buf, vsize_t size, vuint16_t blk_num, vuint32_t mode)
``` 
_Inits ring buffer._ 




**Parameters:**

- `rb`: pointer to struct [rbuffer_s](structrbuffer__s) object. 
- `buf`: address of memory that used to store data. it must be align with 8. 
- `size`: size of buf. 
- `blk_num`: block number of ring buffer. blk_num should be power of 2. If not, rb_init will align down with power of 2, for example, if blk_num is 9, the actual blk_num is 8. User should be carefully with the blk_num to make block size greater than the maximum entry size. 
- `mode`: mode of ring buffer. Only RB_RDONLY and RB_RDWR are supported. Each read thread should create a ringbuffer with RB_RDONLY mode if want to multiple readers. 


**Returns:** int 0 on success, -1 on failure. 



##  Function `rb_try_alloc_entry`

```c
static struct entry_s * rb_try_alloc_entry(struct rbuffer_s *rb, vsize_t size)
``` 
_Tries to alloc an entry from ring buffer._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `size`: the desired size of the entry. 


**Returns:** struct entry_s* pointer of the entry on success. NULL, If there is no enough space in the block, it will try to move the wblk to next block and return NULL. And when other thread is trying to move wblk to next block and hold the ready bit, it also return NULL. Also return NULL when trying to move to next block and next block is not ready(there is still some thread writing on the block). Once return NULL, the caller should try again. 



##  Function `rb_commit_entry`

```c
static void rb_commit_entry(struct rbuffer_s *rb, struct entry_s *entry)
``` 
_Commits an entry._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `entry`: the entry to be commited.


> **Note:** This API may not be used by user directly, so has unused attribute. 


##  Function `rb_write_entry`

```c
static vsize_t rb_write_entry(struct rbuffer_s *rb, void *buf, vsize_t size, vuint32_t try_time)
``` 
_Writes an entry to ring buffer._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `buf`: the data buf. 
- `size`: the size of data. 
- `try_time`: try times on trying allocate entry. 


**Returns:** vsize_t entry size on success, 0 on fail.

> **Note:** This API may not be used by user directly, so has unused attribute. 


##  Function `rb_try_read_entry`

```c
static vsize_t rb_try_read_entry(struct rbuffer_s *rb, void *user_buf, vsize_t buff_size, struct read_info_s *info)
``` 
_Tries to read an entry from ring buffer._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `user_buf`: buf to store entry data, provided by user. 
- `buff_size`: the size of user_buff, the buf should be big enough to store. 
- `info`: address to [read_info_s](structread__info__s) object. The reader info contains the total entries of current block, and the index of current entry. 


**Returns:** vsize_t size of the entry on success, 0 when fails to read a valid entry, and the content in buf is invalid. 



##  Function `rb_read_entry`

```c
static vsize_t rb_read_entry(struct rbuffer_s *rb, void *buf, vsize_t buff_size, struct read_info_s *info)
``` 
_Reads an entry from ring buffer._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `buf`: buf to store entry data, provided by user. 
- `buff_size`: the size of user_buff, the buf should be big enough to store. 
- `info`: address to [read_info_s](structread__info__s) object. The reader info contains the total entries of current block, and the index of current entry. 


**Returns:** vsize_t size of the entry on success. 0 input parameter is invalid.

> **Note:** This API will keep trying to read a valid entry. It returns only when read out a valid entry from ring buffer. Be careful when using this API, make sure there is some writer writing the ring buffer. 


##  Function `rb_rewind_reader`

```c
static void rb_rewind_reader(struct rbuffer_s *rb)
``` 
_Moves reader pointer to the oldest block._ 




**Parameters:**

- `rb`: pointer to ring buffer object.


> **Note:** This API may not be used, so have a unused attribute. 


##  Function `rb_get_write_offset`

```c
static int rb_get_write_offset(struct rbuffer_s *rb, struct entry_s *entry, vsize_t entry_size)
``` 
_Gets write offset from entry._ 




**Parameters:**

- `rb`: pointer to ring buffer object. 
- `entry`: pointer to entry. 
- `entry_size`: entry size. 


**Returns:** int the write offset of current entry on success, -1 on failure. 




---
