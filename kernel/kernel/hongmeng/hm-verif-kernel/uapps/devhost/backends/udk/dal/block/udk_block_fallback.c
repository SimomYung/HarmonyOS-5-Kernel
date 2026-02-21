/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Devhost operations in UDK block
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 12 15:00:00 2019
 */
#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/mm.h>
#include <udk/mempool.h>
#include <udk/mutex.h>
#include <udk/block/udk_block_fallback.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <intapi/fusion/fusion.h>
#include <intapi/mem/sysmem.h>
#include <libmem/types.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_timer.h>
#include <udk/cache.h>
#include <udk/trace/events/trace_list.h>
#include <udk/mm_fusion.h>
#include <udk/bitops.h>
#include <libsysif/devhost/bd_types.h>

struct blk_crypto_info g_blk_crypto;
static struct udk_workqueue *g_mem_free_queue = NULL;
static struct udk_work *g_mem_free_work = NULL;
static bool g_cando_work = true;
static int g_mem_free_num = 0;
static DLIST_HEAD(mem_free_head);
STATIC_DEFINE_RAW_RWLOCK(g_list_lock);

static inline void list_write_lock(void)
{
	(void)raw_rwlock_wrlock(&g_list_lock);
}

static inline void list_write_unlock(void)
{
	(void)raw_rwlock_unlock(&g_list_lock);
}

static inline int list_write_trylock(void)
{
	return raw_rwlock_trywrlock(&g_list_lock);
}

// mempool get
struct blk_crypto_info *udk_crypto_mempool_get(void)
{
	return &g_blk_crypto;
}

/* mempool insert tail algo */
static void udk_crypto_mempool_insert_tail(struct mem_pool_s *pool, struct mem_pool_range_s *range)
{
	if ((pool != NULL) && (range != NULL)) {
		dlist_insert_tail(&pool->dnode, &range->node);
	}
}

// mempool init
int udk_crypto_mempool_init(struct blk_crypto_info *info)
{
	int ret = 0;
	uint64_t paddr;
	struct udk_mempool *mempool = NULL;
	uintptr_t vaddr = INVALID_VADDR;

	// 1、create udk_mempool
	mempool = udk_mempool_create(info->one_size);
	if (mempool == NULL) {
		udk_error("crypto mempool create failed\n");
		ret = -ENOENT;
		goto out_ret;
	}
	udk_mempool_set_insert_behavior(mempool, udk_crypto_mempool_insert_tail);

	// 2、insert mem
	vaddr = (uintptr_t)sysmem_palloc(info->insert_num * info->one_size);
	if (!vaddr) {
		udk_error("crypto mempool alloc fail\n");
		ret = -ENOMEM;
		goto out_destroy;
	}
	paddr = fusion_sys_va_to_pa(vaddr);

	ret = udk_mempool_insert(mempool, vaddr, paddr, info->insert_num * info->one_size);
	if (ret < 0) {
		udk_error("crypto mempool insert failed: %s\n", strerror(-ret));
		goto out_pfree;
	}

	info->mempool = mempool;
	info->vaddr = vaddr;

	// for mem alloc
	udk_mutex_init(&info->mutex);

	return ret;

out_pfree:
	sysmem_pfree(vaddr);
out_destroy:
	udk_mempool_destroy(mempool);
out_ret:
	return ret;
}

// mempool exit
int udk_crypto_mempool_exit(struct blk_crypto_info *info)
{
	int ret;

	// 1. remove
	ret = udk_mempool_remove(info->mempool, info->vaddr, info->insert_num * info->one_size);
	if (ret < 0)
		udk_error("crypto mempool remove fail\n");

	// 2. free
	if (info->vaddr)
		sysmem_pfree(info->vaddr);

	// 3. destory
	if (info->mempool)
		udk_mempool_destroy(info->mempool);

	return ret;
}

static void udk_crypto_mempool_mem_alloc_insert(struct blk_crypto_info *info, unsigned int size, unsigned int extend_num)
{
	int ret, num = 0;
	uint64_t paddr;
	void *vaddr = NULL;

	paddr = udk_alloc_pages(IOFAST_NO_SLOW, udk_size_to_page_order(size * extend_num));
	if (paddr != INVALID_PADDR) {
		vaddr = fusion_pa_to_sys_va(paddr);
		ret = udk_mempool_insert(info->mempool, vaddr, paddr, size * extend_num);
		if (ret < 0) {
			udk_error("nvme crypto mempool insert full failed: %s\n", strerror(-ret));
			udk_free_pages(paddr);
		} else {
			num = extend_num;
			goto out;
		}
	}

	while (info->insert_num < info->max_num) {
		paddr = udk_alloc_pages(IOFAST_NO_SLOW, udk_size_to_page_order(size));
		if (paddr == INVALID_PADDR) {
			udk_error("nvme crypto mempool alloc fail\n");
			break;
		}
		vaddr = fusion_pa_to_sys_va(paddr);
		ret = udk_mempool_insert(info->mempool, vaddr, paddr, size);
		if (ret < 0) {
			udk_error("nvme crypto mempool insert failed: %s\n", strerror(-ret));
			udk_free_pages(paddr);
			break;
		}
		num++;
		if (extend_num == num)
			break;
	}

out:
	udk_mutex_lock(&info->mutex);
	info->insert_num += num;
	if (info->insert_num > info->max_num)
		udk_info("nvme crypto mempool insert num: %d\n", info->insert_num);
	udk_mutex_unlock(&info->mutex);
}

void *udk_crypto_mempool_mem_alloc(struct blk_crypto_info *info, unsigned int size, unsigned int extend_num, bool add_work)
{
	int ret;
	void *vaddr = NULL;

	while (true) {
		vaddr = udk_mempool_mem_alloc(info->mempool, size);
		if (vaddr != NULL)
			return vaddr;

		if (info->insert_num < info->max_num) {
			udk_crypto_mempool_mem_alloc_insert(info, size, extend_num);
			continue;
		}

		if (add_work) {
			list_write_lock();
			if (g_cando_work) {
				ret = udk_workqueue_add_work(g_mem_free_queue, g_mem_free_work);
				if (ret < 0) {
					udk_info("nvme crypto add work error ret: %s\n", strerror(-ret));
				} else {
					g_cando_work = false;
				}
			}
			list_write_unlock();
		}
	}
}

static void udk_blk_crypto_dun_to_iv(unsigned long index, union udk_blk_crypto_iv *iv)
{
	int i;
	uint64_t curr_dun[UDK_BLK_CRYPTO_DUN_ARRAY_SIZE];

	curr_dun[0] = index;
	curr_dun[1] = 0; // padding the upper 8 bytes with 0

	for (i = 0; i < UDK_BLK_CRYPTO_DUN_ARRAY_SIZE; i++)
		iv->dun[i] = htole64(curr_dun[i]);
}

static inline void udk_request_set_crypt(struct blk_crypto_request *req, struct udk_scatterlist *src_sgl,
	struct udk_scatterlist *dst_sgl, unsigned int sg_size, void *iv)
{
	req->src.sgl = src_sgl;
	req->dst.sgl = dst_sgl;
	req->src.nents = sg_size;
	req->dst.nents = sg_size;
	req->iv = iv;
}

static int udk_blk_crypto_endecrypt(struct blk_crypto_request *req, bool encrypt_flag)
{
	struct blk_crypto_alg *alg;

	alg = udk_blk_crypto_get("nvme-crypto-aes-xts");
	if (!alg) {
		udk_error("nvme crypto encrypt alg get failed!\n");
		return -EIO;
	}

	if (encrypt_flag)
		return alg->encrypt(req);
	else
		return alg->decrypt(req);
}

static bool udk_blk_crypto_fallback_encrypt_bio(struct udk_bio *bio)
{
	unsigned int i;
	union udk_blk_crypto_iv iv;
	struct blk_crypto_request req;
	bool ret = false;
	unsigned int sg_size = 0;
	unsigned int index = 0;
	struct hisi_inline_crypto_key *crypto_cfg = (struct hisi_inline_crypto_key *)bio->crypto_key;

	trace_encrypt_bio_entry(bio, bio->bi_part_dev->devt);
	req.key_slot_id = crypto_cfg->ci_key_index;
	req.metadata = crypto_cfg->ci_metadata;

	// 1. alloc scatterlist
	for (i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		for (int j = 0; j < enc_bvec->len; j += UDK_BLK_CRYPTO_DATA_UNIT_SIZE)
			sg_size++;
	}

	req.src.sgl = udk_malloc(sg_size * sizeof(struct udk_scatterlist));
	if (req.src.sgl == NULL) {
		bio->bi_error = -ENOMEM;
		goto out_ret;
	}

	req.dst.sgl = udk_malloc(sg_size * sizeof(struct udk_scatterlist));
	if (req.dst.sgl == NULL) {
		bio->bi_error = -ENOMEM;
		goto out_free_src_sg;
	}

	req.src.nents = sg_size;
	req.dst.nents = sg_size;
	req.iv = iv.bytes;
	req.endecrypt_for_fs = false; // default false for udk

	udk_blk_crypto_dun_to_iv(crypto_cfg->index, &iv);

	// 2. fill scatterlist
	for (i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		unsigned long long plaintext_pfn = enc_bvec->pfn;

		void *ciphertext_vir = udk_crypto_mempool_mem_alloc(udk_crypto_mempool_get(), UDK_BLK_CRYPTO_CIPHERTEXT_SIZE, UDK_CRYPTO_MEMPOOL_FREE_SIZE , true);
		unsigned long long ciphertext_pfn = fusion_sys_va_to_pa((uintptr_t)ciphertext_vir) >> UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT;

		enc_bvec->pfn = ciphertext_pfn;

		if (!ciphertext_vir) {
			bio->bi_error = -ENOMEM;
			udk_error("crypto ciphertext buffer alloc failed!\n");
			goto out_free_bounce_pages;
		}

		for (int j = 0; j < enc_bvec->len; j += UDK_BLK_CRYPTO_DATA_UNIT_SIZE) {
			struct udk_scatterlist *src_tmp = &req.src.sgl[index];
			src_tmp->dma_address = (plaintext_pfn << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			src_tmp->dma_length = UDK_BLK_CRYPTO_DATA_UNIT_SIZE;

			struct udk_scatterlist *dst_tmp = &req.dst.sgl[index];
			dst_tmp->dma_address = (ciphertext_pfn << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			dst_tmp->dma_length = UDK_BLK_CRYPTO_DATA_UNIT_SIZE;

			index++;
		}
	}

	// 3. encrypt
	if (udk_blk_crypto_endecrypt(&req, true)) {
		bio->bi_error = -EIO;
		udk_error("crypto encrypt failed!\n");
		goto out_free_bounce_pages;
	}

	ret = true;
	goto out_free_dst_sg;

out_free_bounce_pages:
	while (i > 0)
		udk_mempool_mem_free(udk_crypto_mempool_get()->mempool, fusion_pa_to_sys_va((bio->bio_io_pvec[--i].pfn) << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT),
			UDK_BLK_CRYPTO_CIPHERTEXT_SIZE);
out_free_dst_sg:
	udk_free(req.dst.sgl);
out_free_src_sg:
	udk_free(req.src.sgl);
out_ret:
	trace_encrypt_bio_exit(bio, bio->bi_part_dev->devt, ret);
	return ret;
}

static int blk_crypto_decrypt(struct blk_crypto_request *req, struct udk_bio_pvec *bio_io_pvec, unsigned int bi_vec_num,
								unsigned long crypto_index, bool endecrypt_for_fs)
{
	union udk_blk_crypto_iv iv;
	struct udk_scatterlist *sg;
	unsigned int sg_size = 0;
	unsigned int index = 0;
	int ret = E_HM_OK;

	// 1. alloc scatterlist
	for (int i = 0; i < bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio_io_pvec[i];
		for (int j = 0; j < enc_bvec->len; j += UDK_BLK_CRYPTO_DATA_UNIT_SIZE)
			sg_size++;
	}

	sg = udk_malloc(sg_size * sizeof(struct udk_scatterlist));
	if (!sg) {
		ret = -ENOMEM;
		goto out_ret;
	}

	udk_request_set_crypt(req, sg, sg, sg_size, iv.bytes);
	udk_blk_crypto_dun_to_iv(crypto_index, &iv);

	// 2. fill scatterlist
	for (int i = 0; i < bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio_io_pvec[i];
		unsigned long long pfn = enc_bvec->pfn;

		for (int j = 0; j < enc_bvec->len; j += UDK_BLK_CRYPTO_DATA_UNIT_SIZE) {
			struct udk_scatterlist *sg_tmp = &sg[index];
			sg_tmp->dma_address = (pfn << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			sg_tmp->dma_length = UDK_BLK_CRYPTO_DATA_UNIT_SIZE;

			index++;
		}
	}

	// 3. decrypt
	if (udk_blk_crypto_endecrypt(req, false)) {
		ret = -EIO;
		udk_error("crypto decrypt failed!\n");
	} else {
		if (req->endecrypt_for_fs) {
			for (int i = 0; i < req->dst.nents; i++) {
				struct udk_scatterlist *sg_tmp = &req->dst.sgl[i];

				// inval
				udk_dma_sync_dev_to_cpu_by_pa(sg_tmp->dma_address, sg_tmp->dma_address + sg_tmp->dma_length);
			}
		}
	}
	udk_free(sg);

out_ret:
	return ret;
}

bool udk_blk_crypto_fallback_decrypt_endio(struct udk_bio *bio)
{
	struct blk_crypto_request req;
	bool ret = false;
	struct hisi_inline_crypto_key *crypto_cfg = (struct hisi_inline_crypto_key *)bio->crypto_key;

	trace_decrypt_endio_entry(bio, bio->bi_part_dev->devt);
	req.key_slot_id = crypto_cfg->ci_key_index;
	req.metadata = crypto_cfg->ci_metadata;
	req.endecrypt_for_fs = false; // default false for udk

	int ret_code = blk_crypto_decrypt(&req, bio->bio_io_pvec, bio->bi_vec_num, crypto_cfg->index, false);
	if (ret_code != E_HM_OK) {
		bio->bi_error = ret_code;
	} else {
		ret = true;
	}

	trace_decrypt_endio_exit(bio, bio->bi_part_dev->devt, ret);
	return ret;
}

bool udk_blk_crypto_fallback_bio_prep(struct udk_bio *bio)
{
	if (udk_bio_data_dir(bio) == UDK_REQ_WRITE)
		return udk_blk_crypto_fallback_encrypt_bio(bio);
	return true;
}

int crypto_aes_endecrypt_for_fs(void *src_vec, void *dst_vec, unsigned long vec_cnt, void* crypt_ctx, bool enc,
								bool endecrypt_for_fs)
{
	int ret;
	struct blk_crypto_request req;

	struct udk_bio_pvec *src_pvec = (struct udk_bio_pvec *)src_vec;
	struct udk_bio_pvec *dst_pvec = (struct udk_bio_pvec *)dst_vec;
	struct bd_io_fs_hisi_crypt_ctx *ctx = (struct bd_io_fs_hisi_crypt_ctx *)crypt_ctx;

	if (src_pvec == NULL) {
		udk_error("nvme crypto src_pvec is NULL!\n");
		return -EINVAL;
	}

	if (ctx == NULL) {
		udk_error("nvme crypto ctx is NULL!\n");
		return -EINVAL;
	}
	if (ctx->ci_key == NULL) {
		udk_error("nvme crypto ctx->ci_key is NULL!\n");
		return -EINVAL;
	}

	req.key_slot_id = ctx->ci_key_index;
	req.metadata = ctx->ci_metadata;
	req.endecrypt_for_fs = endecrypt_for_fs;

	ret = blk_crypto_decrypt(&req, src_pvec, vec_cnt, ctx->index, endecrypt_for_fs);
	return ret;
}

static int bio_end_mem_free_handler(struct udk_work *work)
{
	struct udk_crypto_io_info *info = NULL;
	struct udk_mempool *pool = udk_crypto_mempool_get()->mempool;
	DLIST_HEAD(rm_head);

	list_write_lock();
	while (!dlist_empty(&mem_free_head)) {
		info = dlist_first_entry(&mem_free_head, struct udk_crypto_io_info, queuelist);
		dlist_delete(&info->queuelist);
		dlist_insert(&rm_head, &info->queuelist);
		if (g_mem_free_num - info->bi_vec_num >= 0)
			g_mem_free_num -= info->bi_vec_num;
		else
			udk_error("%s mem free num fail.\n", __func__);
	}
	list_write_unlock();

	while (!dlist_empty(&rm_head)) {
		info = dlist_first_entry(&rm_head, struct udk_crypto_io_info, queuelist);
		for (int i = 0; i < info->bi_vec_num; i++)
			udk_mempool_mem_free(pool, fusion_pa_to_sys_va(info->pfn_arr[i] << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT),
				UDK_BLK_CRYPTO_CIPHERTEXT_SIZE);

		dlist_delete(&info->queuelist);
		udk_free(info);
	}
	udk_work_destroy(work);
	g_cando_work = true;
	return 0;
}

int udk_crypto_workqueue_init(void)
{
	int ret;
	struct udk_workqueue_attr attr = {0};

	attr.nr_workers = 1;
	attr.nr_queues = 1;
	ret = snprintf_s(attr.name, UDK_THREADPOOL_NAME_LEN, UDK_THREADPOOL_NAME_LEN - 1, "%s", "nvme-pool-free");
	if (ret < 0) {
		udk_error("%s snprintf work queue %s fail.\n", __func__, "nvme-pool-free");
		return -ENOMEM;
	}

	g_mem_free_queue = udk_workqueue_alloc(&attr);
	if (g_mem_free_queue == NULL)
		return -ENOMEM;

	g_mem_free_work = udk_malloc(sizeof(struct udk_work));
	if (g_mem_free_work == NULL) {
		ret = -ENOMEM;
		goto destroy_free_queue;
	}

	ret = udk_work_init(g_mem_free_work, bio_end_mem_free_handler, 0);
	if (ret < 0)
		goto out_free_work;

	goto out_ret;

out_free_work:
	udk_free(g_mem_free_work);
destroy_free_queue:
	udk_workqueue_destroy(g_mem_free_queue);
out_ret:
	return ret;
}

static int udk_blk_crypto_fallback_encrypt_free_io_async(struct udk_bio *bio)
{
	struct udk_crypto_io_info *info = NULL;
	uint32_t lenth;
	int ret;

	if (!bio) {
		ret = -EINVAL;
		goto out;
	}

	lenth = sizeof(struct udk_crypto_io_info) + sizeof(unsigned long long) * bio->bi_vec_num;
	info = udk_malloc(lenth);
	if (info == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	mem_zero_b(info, lenth);
	dlist_init(&info->queuelist);
	info->pfn_arr = (unsigned long long *)(info + 1);
	info->bi_vec_num = bio->bi_vec_num;
	for (int i = 0; i < bio->bi_vec_num; i++) {
		info->pfn_arr[i] = bio->bio_io_pvec[i].pfn;
	}

	list_write_lock();
	dlist_insert_tail(&mem_free_head, &info->queuelist);
	g_mem_free_num += bio->bi_vec_num;
	if (g_cando_work && (g_mem_free_num >= UDK_CRYPTO_MEMPOOL_FREE_SIZE)) {
		ret = udk_workqueue_add_work(g_mem_free_queue, g_mem_free_work);
		if (ret < 0) {
			g_mem_free_num -= bio->bi_vec_num;
			dlist_delete(&info->queuelist);
			list_write_unlock();
			udk_info("udk blk add work error ret: %s\n", strerror(-ret));
			goto free;
		}
		g_cando_work = false;
	}
	list_write_unlock();
	return 0;

free:
	udk_free(info);
out:
	return ret;
}

static void udk_blk_crypto_fallback_encrypt_free_io(struct udk_bio *bio)
{
	struct udk_mempool *mempool = udk_crypto_mempool_get()->mempool;

	for (int i = 0; i < bio->bi_vec_num; i++)
		udk_mempool_mem_free(mempool, fusion_pa_to_sys_va((bio->bio_io_pvec[i].pfn) << UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT),
			UDK_BLK_CRYPTO_CIPHERTEXT_SIZE);
}

void udk_blk_crypto_fallback_encrypt_endio(struct udk_bio *bio)
{
	if (udk_blk_crypto_fallback_encrypt_free_io_async(bio))
		udk_blk_crypto_fallback_encrypt_free_io(bio);
}
