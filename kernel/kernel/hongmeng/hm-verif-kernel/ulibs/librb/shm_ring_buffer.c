/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Ring buffer based on shm_node in hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 17:30:26 2020
 */

/*
 * Doc: Shared memory based ring buffer (sring)
 *
 * This is an implementation of ring buffer based on shm_node, known as sring,
 * used to share preallocated data between processes. It is a multi-producer
 * and multi-consumer design, so it is safe for more than one thread to push
 * and pop items from the sring at the same time.
 *
 * A sring basicly consists of:
 * - metadata:		a sring_info structure, stored in the beginning of the
 *			shared memory the sring resides in.
 * - item buffer:	an array of sring_item structure, containing only an
 * 			index used to locate object in obj array decribed below.
 * - obj buffer:	another shared memory array containing real objects
 *			this sring wants to represent. The index in sring_item
 *			is the element index in this array.
 *
 * sring _info has 4 basic attributes, head, tail, real_tail, all of which are
 * monotonic increasing, and item_cnt which is const.
 * - (head & (item_cnt - 1)) is the index of the first item ready to be consume
 * - (tail & (item_cnt - 1)) is the index of the next free item
 * - (real_tail & (item_cnt -1)) is the index of the item next to the last ready item
 * - item_cnt is the total count of item in the sring. Expanding or shrinking
 *   a sring dynamically is not supported.
 *
 * When a consumer pops an item from the sring, it increments head atomically.
 * When a producer starts to push an item into the sring, it increments tail
 * atomically first, which will reserve a position in the sring for this push
 * operation. And then the producer takes its time to find a usable object in
 * obj buffer, and store its index into the item. After that, it increments
 * real_tail to tell others that the push operation is finished.
 *
 * So 2 things must be realized:
 * - head, tail, real_tail are monotonic increasing infinitely. Users should
 *   modulo them by item_cnt to get the real index.
 * - tail is just a reservation of a free item for a push operation. The index
 *   has been reserved, but real data is not in the obj buffer yet. So it is not
 *   ready to be consumed yet. real_tail represents the item is already in the
 *   obj buffer. So only items in [head, real_tail) can be popped by consumers.
 *
 * As a result, we can check if the sring is full or empty as below:
 * - sring is full when	head == tail + 1
 *   When tail catches up with head, it means no free item in the sring
 *   could be used. Then the sring is full. The producer will block.
 * - sring is empty when head == real_tail
 *   When head catches up with real_tail, it means no ready item with useful
 *   object in the sring. The consumer will block.
 *
 * The main objective of real_tail is that we use it to support multi-producer
 * and multi-consumer usage. When multiple producers push items into the sring
 * at the same time, checking if the sring is full and storing object into item
 * cannot be atomic. Of course we can use a lock to do the synchronization, but
 * we choose atomic operations to gain better performance. So increment tail
 * atomically will only reserve an index for producer to push, real_tail won't
 * change until the producer has finished its job to store an object index into
 * item. When the push operation is finished, it means the item is ready to be
 * popped by consumers. Then increment real_tail atomically.
 *
 * The sring's shared memory should be mapped to process's vspace so that it
 * can be accessed. So processes from both sides of a sring maintain a local
 * sring_buf structure representing the sring. sring_buf contains fds used to
 * map item buffer and obj buffer, and the va mapped in each vspace.
 *
 * Two srings could share one obj buffer. Just assign the fd and va of the obj
 * buffer to the sring_buf in each process. In this case, make obj buffer
 * twice as large as the sring may gain better performance.
 *
 * NOTE:
 * There are APIs for users to do the operations above. Users should never
 * modify any member of sring_info and sring_buf directly.
 */

#include <librb/shm_ring_buffer.h>

#include <fcntl.h>
#include <limits.h>
#include <libhmsync/atomic.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhwsecurec/securec.h>

#define RING_BUF_CNT_MIN	2U
#define RING_BUF_CNT_MAX	256U
#define BUILD_BUG_ON(cond)	((void)sizeof(char[1 - 2*((!!(cond)) ? 1 : 0)]))

#define RING_BUF_SHM_FLAG		(SHM_O_CREAT | SHM_O_RDWR)
#define RING_BUF_SHM_REGRANT_FLAG	(SHM_O_CREAT | SHM_O_GRANT_TRANS | SHM_O_RDWR)

/*
 * sring_item:	element of a shared memory ring buffer
 *
 * There should be no pointer member in ring_item because it is used in both
 * sides of the shared memory. It must be an offset of another shared memory
 * buffer which is used to store objects in this ring buffer.
 */
struct sring_item {
	uint64_t idx;		/* Object offset in obj shared memory */
};

/*
 * sring_info:	shared memory ring buffer's meta data
 *
 * There should be no pointer member in sring_info because it is put in
 * the same shared memory with the ring buffer itself, in the very beginning
 * of the shared memory.
 */
struct sring_info {
	raw_atomic_int_t head;		/* First ready item to be consumed */
	raw_atomic_int_t tail;		/* Next free item */
	raw_atomic_int_t real_tail;		/* Item next to then last ready one */

	size_t item_cnt;	/* Max number of available items in the sring */
	size_t size;		/* Total size of shared memory,
				 * sizeof(sring_info) + item_cnt * sizeof(sring_item) */

	uint64_t data_fd;	/* Object array shm_node fd */
	size_t obj_size;	/* Object size the item pointing to */
	size_t obj_cnt;		/* Max number of objects in the object array */
	raw_atomic_int_t state;	/* If the consumer is accessing the sring */
};

static inline struct sring_info *sring_buf_info_of(struct sring_buf *srbuf)
{
	return (struct sring_info *)(srbuf->ring_vaddr);
}

void sring_buf_set_info_data_fd(struct sring_buf *srbuf, uint64_t data_fd)
{
	struct sring_info *srinfo = NULL;

	if (srbuf != NULL) {
		srinfo = sring_buf_info_of(srbuf);
		srinfo->data_fd = data_fd;
	}
}

static bool sring_buf_check_size(size_t item_cnt)
{
	bool ret = true;

	if (item_cnt < RING_BUF_CNT_MIN || item_cnt > RING_BUF_CNT_MAX) {
		hm_warn("invalid ring buffer item count %"PRIu64", out of range [%u, %u)\n",
			(uint64_t)item_cnt, RING_BUF_CNT_MIN, RING_BUF_CNT_MAX);
		ret = false;
	}

	return ret;
}

// make our mod logic could run happily in all conditions
static bool sring_buf_check_overflow(size_t item_cnt)
{
	/*
	 * Check item_cnt is power of two
	 * when item_cnt is power of two, we got two features:
	 * 1.
	 * [foo % (item_cnt)] equals to [foo & (item_cnt -1)]
	 * 2.
	 * foo % (item_cnt) is always continuous when foo increases, even
	 * when foo overflows, e.g, -1 to 0, INT_MAX to INT_MIN
	 */
	return (item_cnt > 0U) && ((item_cnt & (item_cnt - 1U)) == 0U);
}

static struct sring_buf *__sring_buf_create(size_t item_cnt, uint64_t shm_flag)
{
	int ring_fd;
	size_t ring_size;
	struct sring_buf *srbuf = NULL;

	/*
	 * futex_wait in sring_buf_push/sring_buf_pop depends on type
	 * conversion between int and raw_atomic_int_t
	 */
	BUILD_BUG_ON(sizeof(int) != sizeof(raw_atomic_int_t));

	if (!sring_buf_check_size(item_cnt)) {
		hm_warn("item_cnt=%zu is invalid\n", item_cnt);
	} else if (!sring_buf_check_overflow(item_cnt)) {
		hm_warn("item_cnt=%zu is not power of two\n", item_cnt);
	} else {
		ring_size = sizeof(struct sring_info) +
			    item_cnt * sizeof(struct sring_item);
		ring_fd = hm_ashm_open_auth(0, 0U, ring_size, shm_flag, NULL);
		if (ring_fd <= 0) {
			hm_warn("failed to open shm\n");
		} else {
			srbuf = calloc(1, sizeof(struct sring_buf));
			if (srbuf == NULL) {
				hm_warn("failed to alloc sring_buf\n");
				(void)hm_ashm_close(ring_fd);
				(void)hm_ashm_unlink(ring_fd);
			} else {
				srbuf->ring_fd = ring_fd;
				srbuf->ring_size = ring_size;
				raw_rwlock_init(&srbuf->rwlock, NULL);
			}
		}
	}

	/*
	 * At this time, sring memory is not allocated yet,
	 * which means srinfo is not allocated yet
	 */
	return srbuf;
}

/*
 * sring_buf_create: create a sring_buf in a process
 * item_cnt:	item count
 * path:	shm_node path
 *
 * This function is used to create a new sring. It creates a shm_node,
 * allocates memory for sring_buf. No real memory is allocated for the
 * sring yet, neither for the item buffer, nor for the obj buffer.
 *
 * Return: pointer to sring_buf on success, NULL otherwise.
 */
struct sring_buf *sring_buf_create(size_t item_cnt)
{
	return __sring_buf_create(item_cnt, RING_BUF_SHM_FLAG);
}

/*
 * sring_buf_regrant_create: create a sring_buf in a process with regrant shm flag
 * item_cnt:	item count
 * path:	shm_node path
 *
 * This function is used to create a new sring. Unlike sring_buf_create, item
 * buffer created in this function can be regranted, which means, process A
 * creates a sring buf, grants to process B, process B can also grant to process
 * C and so on. Regrant mechanism has safety hazards such as recycling problems,
 * so invoke this function with caution.
 *
 * Return: pointer to sring_buf on success, NULL otherwise.
 */
struct sring_buf *sring_buf_regrant_create(size_t item_cnt)
{
	return __sring_buf_create(item_cnt, RING_BUF_SHM_REGRANT_FLAG);
}

static int shm_open_and_check_anon(uint64_t key, uint32_t cnode_idx, int *shm_id)
{
	int ret = E_HM_OK;
	int fd = -1;

	/*
	 * O_RDWR is used because both producer and customer will
	 * read/update srinfo->head and sring->tail when push/pop
	 * from the ring buffer.
	 */
	fd = hm_ashm_open_auth(key, cnode_idx, 0, O_RDWR, NULL);
	if (fd <= 0) {
		hm_warn("failed to open anon shm mem: %"PRIu64"\n", key);
		ret = E_HM_INVAL;
	} else {
		*shm_id = fd;
	}

	return ret;
}

/*
 * sring_buf_create_from_fd: create a sring_buf from a fd
 * ring_fd:	the fd of the shm_node the sring is based on
 * include_data:also open shm_node data_fd.
 * cnode_idx:	owner of the shm_mem
 *
 * This function is used to create a sring_buf for the process on the other side
 * of an existing sring. e.g. a producer creates a sring, and a consumer wants
 * to share it, then the consumer will call this function. The producer should
 * provide the fd of the shm_node to the consumer, usually through IPC.
 * When this function is called, the creator of the sring must have allocated
 * memory for the sring because the caller of this function will need sring_info
 * to access metadata of the sring.
 *
 * Return: pointer to sring_buf on success, NULL otherwise.
 */
struct sring_buf *sring_buf_create_from_fd(uint64_t ring_fd, bool include_data,
					   uint32_t cnode_idx)
{
	struct sring_info *srinfo = NULL;
	struct sring_buf *srbuf = NULL;
	size_t ring_size, data_size;
	uint64_t data_fd;
	int shm_id;
	int ret;

	ret = shm_open_and_check_anon(ring_fd, cnode_idx, &shm_id);
	if (ret == E_HM_OK) {
		srinfo = hm_mem_mmap(NULL, sizeof(struct sring_info),
				     (uint32_t)PROT_READ | (uint32_t)PROT_WRITE,
				     (uint32_t)MAP_SHARED | (uint32_t)MAP_ANONYMOUS, shm_id, 0);
		if (srinfo == MAP_FAILED) {
			hm_warn("failed to map shm_id %d\n", shm_id);
			(void)hm_ashm_close(shm_id);
			srbuf = NULL;
		} else {
			ring_size = srinfo->size;
			data_size = srinfo->obj_size * srinfo->obj_cnt;
			data_fd = srinfo->data_fd;
			(void)hm_mem_munmap(srinfo, sizeof(struct sring_info));

			srbuf = calloc(1, sizeof(struct sring_buf));
			if (srbuf == NULL) {
				(void)hm_ashm_close(shm_id);
				hm_warn("failed to alloc sring_buf\n");
			}
		}
	}

	if (srbuf != NULL) {
		srbuf->ring_fd = shm_id;
		srbuf->ring_size = ring_size;
		srbuf->data_size = data_size;
		raw_rwlock_init(&srbuf->rwlock, NULL);

		if (include_data == true) {
			ret = shm_open_and_check_anon(data_fd, cnode_idx, &(srbuf->data_fd));
			if (ret != E_HM_OK) {
				(void)hm_ashm_close(shm_id);
				free(srbuf);
				hm_warn("failed to open anon shm mem ring_fd %"PRIu64"\n", ring_fd);
				srbuf = NULL;
			}
		}
	}

	return srbuf;
}

/*
 * sring_buf_destroy: destroy a sring_buf
 * srbuf:	pointer to the sring_buf to be destroyed
 * include_data:	whether need to destroy data_fd
 * unlink:	whether need to unlink the ring
 *
 * This function unmap the memory and close the shm_fd according to
 * parameters.
 */
void sring_buf_destroy(const struct sring_buf *srbuf, bool include_data,
		       bool unlink)
{
	if (srbuf == NULL) {
		hm_warn("sring_buf is NULL\n");
	} else {
		(void)hm_ashm_close(srbuf->ring_fd);
		if (unlink) {
			(void)hm_ashm_unlink(srbuf->ring_fd);
		}

		if (include_data) {
			(void)hm_ashm_close(srbuf->data_fd);
			if (unlink) {
				(void)hm_ashm_unlink(srbuf->data_fd);
			}
		}
	}

	return;
}

static int __sring_buf_install_data(struct sring_buf *srbuf, size_t obj_cnt,
				    size_t obj_size, uint64_t shm_flag)
{
	int data_fd;
	size_t data_size;
	int ret;

	if (srbuf == NULL) {
		hm_warn("srbuf is null\n");
		ret = E_HM_INVAL;
	} else if (obj_cnt == 0U || obj_size == 0U) {
		hm_warn("invalid obj_cnt=%zu, obj_size=%zu\n", obj_cnt, obj_size);
		ret = E_HM_INVAL;
	} else {
		data_size = obj_size * obj_cnt;
		data_fd = hm_ashm_open_auth(0, 0U, data_size, shm_flag, NULL);
		if (data_fd < 0) {
			hm_warn("failed to open shm\n");
			ret = E_HM_INVAL;
		} else {
			srbuf->data_fd = data_fd;
			srbuf->data_size = data_size;
			ret = E_HM_OK;
		}
	}

	return ret;
}

/*
 * sring_buf_install_data: create the shm_node for obj buffer of an existing sring
 * srbuf:	sring_buf of the caller process
 * obj_cnt:	number of objects
 * obj_size:	object size in bytes
 *
 * This function is used to create an obj buffer for an existing sring, called
 * by the creator of the sring. It just opens a shm_node, no real memory is
 * allocated. And store the fd in sring_buf.
 *
 * Return: E_HM_OK on success, E_HM_INVAL if failed to open shm_node.
 */
int sring_buf_install_data(struct sring_buf *srbuf, size_t obj_cnt,
			   size_t obj_size)
{
	return __sring_buf_install_data(srbuf, obj_cnt, obj_size,
					RING_BUF_SHM_FLAG);
}

/*
 * sring_buf_regrant_install_data:
 *     create the shm_node for obj buffer with regrant shm flag of an existing sring
 * srbuf:	sring_buf of the caller process
 * obj_cnt:	number of objects
 * obj_size:	object size in bytes
 *
 * This function is used to create an obj buffer for an existing sring, called
 * by the creator of the sring. Similar to sring_buf_regrant_create, data buffer
 * created in this function can be regranted, and has the similar safety hazards.
 * In general, it is used in conjunction with sring_buf_regrant_create, and
 * again, invoke with caution.
 *
 * Return: E_HM_OK on success, E_HM_INVAL if failed to open shm_node.
 */
int sring_buf_regrant_install_data(struct sring_buf *srbuf, size_t obj_cnt,
				   size_t obj_size)
{
	return __sring_buf_install_data(srbuf, obj_cnt, obj_size,
					RING_BUF_SHM_REGRANT_FLAG);
}

/*
 * sring_buf_share_data: one sring_buf shares obj buffer with another sring_buf
 * srbuf_dst:	the sring_buf who wants to share other's obj buffer
 * srbuf_src:	the sring_buf who shares its obj buffer with others
 *
 * Multiple srings could share one obj buffer, this function is used in one
 * process to copy the obj buffer metadata from another sring.
 */
void sring_buf_share_data(struct sring_buf *srbuf_dst,
			  struct sring_buf *srbuf_src)
{
	if (srbuf_dst == NULL) {
		hm_warn("destination srbuf is null\n");
	} else if (srbuf_src == NULL) {
		hm_warn("source srbuf is null\n");
	} else {
		srbuf_dst->data_fd = srbuf_src->data_fd;
		srbuf_dst->data_size = srbuf_src->data_size;
		srbuf_dst->data_vaddr = srbuf_src->data_vaddr;
	}
}

void sring_buf_init_info(struct sring_buf *srbuf, size_t item_cnt,
			 size_t obj_cnt, size_t obj_size)
{
	struct sring_info *srinfo = NULL;

	if (srbuf == NULL) {
		hm_warn("sring_buf is NULL\n");
	} else if (!sring_buf_check_size(item_cnt) ||
	    !sring_buf_check_overflow(item_cnt)) {
		hm_warn("item_cnt=%zu is invalid\n", item_cnt);
	} else if (obj_cnt == 0U || obj_size == 0U) {
		hm_warn("invalid obj_cnt=%zu, obj_size=%zu\n", obj_cnt, obj_size);
	} else {
		srinfo = sring_buf_info_of(srbuf);
		/* srinfo->data_fd will be generated in devhost_grant_bio */
		srinfo->item_cnt = item_cnt;
		srinfo->obj_size = obj_size;
		srinfo->obj_cnt = obj_cnt;
		srinfo->size = srbuf->ring_size;

		(void)raw_atomic_int_set(&srinfo->state, RING_IDLE);
		(void)raw_atomic_int_set(&srinfo->head, 0);
		(void)raw_atomic_int_set(&srinfo->tail, 0);
		(void)raw_atomic_int_set(&srinfo->real_tail, 0);
	}
}

/*
 * sring_buf_connect: map item buffer and obj buffer to the process of a sring_buf
 * srbuf:	the sring_buf item buffer will be mapped to
 * map_data:	true if also want to map obj buffer
 *
 * This function will map item buffer to srbuf using srbuf->ring_fd.
 *
 * Return: E_HM_OK on success. E_HM_INVAL if either item buffer or obj buffer
 *         has been mapped. E_HM_RESFAULT if mapping failed.
 */
int sring_buf_connect(struct sring_buf *srbuf, bool map_data)
{
	void *ring_vaddr = NULL;
	void *data_vaddr = NULL;
	int ret;

	if (srbuf == NULL) {
		hm_warn("sring_buf is null\n");
		ret = E_HM_INVAL;
	} else if (srbuf->ring_vaddr != NULL || srbuf->data_vaddr != NULL) {
		hm_warn("sring_buf already mapped\n");
		ret = E_HM_INVAL;
	} else {
		ring_vaddr = hm_mem_mmap(NULL, srbuf->ring_size,
					 (uint32_t)PROT_READ | (uint32_t)PROT_WRITE,
					 (uint32_t)MAP_SHARED | (uint32_t)MAP_ANONYMOUS,
					 srbuf->ring_fd, 0);
		if (ring_vaddr == MAP_FAILED) {
			hm_warn("failed to map fd %d\n", srbuf->ring_fd);
			ret = E_HM_RESFAULT;
		} else {
			srbuf->ring_vaddr = ring_vaddr;
			ret = E_HM_OK;
		}
	}

	if (ret == E_HM_OK && map_data) {
		data_vaddr = hm_mem_mmap(NULL, srbuf->data_size,
					 (uint32_t)PROT_READ | (uint32_t)PROT_WRITE,
					 (uint32_t)MAP_SHARED | (uint32_t)MAP_ANONYMOUS,
					 srbuf->data_fd, 0);
		if (data_vaddr == MAP_FAILED) {
			hm_warn("failed to map fd %d\n", srbuf->data_fd);
			(void)hm_mem_munmap(ring_vaddr, srbuf->ring_size);
			ret = E_HM_RESFAULT;
		} else {
			srbuf->data_vaddr = data_vaddr;
			ret = E_HM_OK;
		}
	}

	return ret;
}

/*
 * sring_buf_disconnect: unmap item buffer and obj buffer from a sring_buf
 * srbuf:	the sring_buf to be unmapped
 *
 * Only unmap item buffer and obj buffer from srbuf. If they have already been
 * unmapped, do nothing.
 */
void sring_buf_disconnect(struct sring_buf *srbuf, bool include_data)
{
	if (srbuf != NULL) {
		if (srbuf->ring_vaddr != NULL) {
			(void)hm_mem_munmap(srbuf->ring_vaddr, srbuf->ring_size);
		}

		if (include_data && srbuf->data_vaddr != NULL) {
			(void)hm_mem_munmap(srbuf->data_vaddr, srbuf->data_size);
		}
	}
}

/*
 * sring_is_onwork: check whether the ring is being processed
 * srbuf:	the sring_buf to be check
 */
bool sring_is_onwork(struct sring_buf *srbuf)
{
	bool ret = false;
	struct sring_info *srinfo = NULL;

	if (srbuf == NULL) {
		hm_warn("srbuf is null\n");
		ret = false;
	} else {
		srinfo = sring_buf_info_of(srbuf);
		if (raw_atomic_int_read(&srinfo->state) == RING_ONWORKING) {
			ret = true;
		}
	}

	return ret;
}

/*
 * sring_set_state: set whether the ring is being processed.
 * srbuf:	the sring_buf to set
 * state:	new state of the ring
 *
 * Set whether the ring is being processed by the customer.
 * Currently we only support one customer for the ring state.
 * For multi-customers, we may need a refcount rather than
 * state.
 */
void sring_set_state(struct sring_buf *srbuf, int state)
{
	struct sring_info *srinfo = NULL;

	if (srbuf == NULL) {
		hm_warn("srbuf is null\n");
	} else if (state != RING_ONWORKING && state != RING_IDLE) {
		hm_warn("invalid ringbuf state\n");
	} else {
		srinfo = sring_buf_info_of(srbuf);
		(void)raw_atomic_int_set(&srinfo->state, state);
		barrier();
	}
}

/*
 * sring_buf_push: push an object into a sring
 * srbuf:	the sring_buf to push
 * obj:		pointer to the object to be pushed
 *
 * Push an object into a sring. It trys to reserve a position in item buffer
 * first by increment tail with CAS atomic operation. If failed, it will yield
 * CPU and try again later. Once succeeds, the index of object in obj buffer
 * is assigned to item->idx, and increments real_tail atomically to tell sring
 * the obj is ready to be consumed.
 *
 * Return: E_HM_OK on success.
 */
int sring_buf_push(struct sring_buf *srbuf, const void *obj)
{
	struct sring_item *sritems = NULL;
	struct sring_info *srinfo = NULL;
	uint64_t obj_idx;
	int head, tail, old_tail;
	size_t item_cnt = 0;
	int ret = E_HM_OK;

	if (srbuf == NULL) {
		hm_warn("srbuf is null\n");
		ret = E_HM_INVAL;
	} else if (obj == NULL) {
		hm_warn("obj is null\n");
		ret = E_HM_INVAL;
	} else {
		srinfo = sring_buf_info_of(srbuf);
		item_cnt = srinfo->item_cnt;
		if (!sring_buf_check_size(item_cnt)) {
			hm_warn("Invalid item_cnt: %zu\n", item_cnt);
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK && srinfo != NULL) {
		do {
			/* try again with the latest head and tail */
			head = raw_atomic_int_read(&srinfo->head);
			tail = raw_atomic_int_read(&srinfo->tail);
			/* check ring is full, still works when tail overflow */
			if (tail - head >= (int)item_cnt) {
				(void)hm_futex_wait((int *)(&srinfo->head), head, NULL, FUTEX_TYPE_SHARED);
				old_tail = tail - 1;
				continue;
			}
			old_tail = raw_atomic_int_cas(&srinfo->tail,
						      tail, tail + 1);
		} while (old_tail != tail);

		/* Now tail is reserved */
		sritems = (struct sring_item *)((uintptr_t)srbuf->ring_vaddr +
						sizeof(struct sring_info));
		obj_idx = ptr_diff(obj, srbuf->data_vaddr) / srinfo->obj_size;
		/*
		 * we don't use array_index_fixup here because we have modulo
		 * item_cnt here, so we never access sritems out of bounds
		 */
		sritems[(size_t)(long)tail & (item_cnt - 1U)].idx = obj_idx;

		old_tail = raw_atomic_int_cas(&srinfo->real_tail, tail, tail + 1);
		while (old_tail != tail) {
			(void)hm_futex_wait((int *)(&srinfo->real_tail), old_tail, NULL, FUTEX_TYPE_SHARED);
			old_tail = raw_atomic_int_cas(&srinfo->real_tail,
						      tail, tail + 1);
		}
		(void)hm_futex_wake((int *)(&srinfo->real_tail), UINT_MAX, FUTEX_TYPE_SHARED);
	}

	return ret;
}

/*
 * sring_buf_pop: pop an object from a sring
 * srbuf:	the sring_buf to pop object from
 *
 * First check if the sring is empty. If it is, return NULL. Then increment head
 * with CAS atomic operation. If succeeds, we can pop object in head position.
 * Note that only objects in range [head, real_tail) could be popped. Objects
 * in range [real_tail, tail) are being modified by pushing processes, they are
 * not ready yet.
 *
 * Return:	pointer to the popped object on success, NULL on failure.
 */
void *sring_buf_pop(struct sring_buf *srbuf)
{
	struct sring_item *sritems = NULL;
	struct sring_info *srinfo = NULL;
	int head, old_head, real_tail;
	uint64_t idx = 0;
	uint64_t offset;
	size_t item_cnt = 0;
	int err = E_HM_OK;
	void *ret = NULL;

	if (srbuf == NULL) {
		hm_warn("srbuf is null\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		srinfo = sring_buf_info_of(srbuf);
		item_cnt = srinfo->item_cnt;
		if (!sring_buf_check_size(item_cnt)) {
			hm_warn("Invalid item_cnt: %zu\n", item_cnt);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK && srinfo != NULL) {
		sritems = (struct sring_item *)((uintptr_t)srbuf->ring_vaddr +
						sizeof(struct sring_info));
		do {
			/* try again with the latest head and real_tail */
			head = raw_atomic_int_read(&srinfo->head);
			real_tail = raw_atomic_int_read(&srinfo->real_tail);
			/* ring is empty */
			if (head == real_tail) {
				err = E_HM_INVAL;
				break;
			}

			/*
			 * we don't use array_index_fixup here because we have modulo
			 * item_cnt here, so we never access sritems out of bounds
			 */
			idx = sritems[(size_t)(long)head & (item_cnt - 1U)].idx;
			old_head = raw_atomic_int_cas(&srinfo->head,
						      head, head + 1);
		} while (old_head != head);

		if (err == E_HM_OK) {
			(void)hm_futex_wake((int *)(&srinfo->head), UINT_MAX,
					    FUTEX_TYPE_SHARED);

			/* Now head is ready */
			offset = srinfo->obj_size * idx;
			ret = ptr_add(srbuf->data_vaddr, offset);
		}
	}

	return ret;
}
