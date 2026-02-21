/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Devhost operations in UDK block
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 12 15:00:00 2019
 */
#include <udk/log.h>
#include <udk/cache.h>
#include <udk/block/udk_block_fallback.h>
#include <udk/nvme_crypto/nvme_crypto.h>
#include <hongmeng/errno.h>
#include <intapi/fusion/fusion.h>
#include <udk/iolib.h>
#include <udk/mm.h>
#include <udk/block/udk_block.h>
#include <libhwsecurec/securec.h>
#include <udk/mempool.h>

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

	req.key_slot_id = crypto_cfg->ci_key_index;
	req.metadata = crypto_cfg->ci_metadata;

	// 1. alloc scatterlist
	for (i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		for (int j = 0; j < enc_bvec->len; j += NVME_CRYPTO_DATA_UNIT_SIZE)
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

	udk_blk_crypto_dun_to_iv(crypto_cfg->index, &iv);

	// 2. fill scatterlist
	for (i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		unsigned long long plaintext_pfn = enc_bvec->pfn;

		void *ciphertext_vir = udk_nvme_crypto_mempool_mem_alloc(udk_nvme_crypto_mempool_get(), NVME_CRYPTO_CIPHERTEXT_SIZE);
		unsigned long long ciphertext_pfn = fusion_sys_va_to_pa((uintptr_t)ciphertext_vir) >> NVME_CRYPTO_PAGE_SIZE_SHIFT;

		enc_bvec->pfn = ciphertext_pfn;

		if (!ciphertext_vir) {
			bio->bi_error = -ENOMEM;
			udk_error("nvme crypto ciphertext buffer alloc failed!\n");
			goto out_free_bounce_pages;
		}

		for (int j = 0; j < enc_bvec->len; j += NVME_CRYPTO_DATA_UNIT_SIZE) {
			struct udk_scatterlist *src_tmp = &req.src.sgl[index];
			src_tmp->dma_address = (plaintext_pfn << NVME_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			src_tmp->dma_length = NVME_CRYPTO_DATA_UNIT_SIZE;
			// flush
			udk_dma_sync_cpu_to_dev_by_pa(src_tmp->dma_address, src_tmp->dma_address + src_tmp->dma_length);

			struct udk_scatterlist *dst_tmp = &req.dst.sgl[index];
			dst_tmp->dma_address = (ciphertext_pfn << NVME_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			dst_tmp->dma_length = NVME_CRYPTO_DATA_UNIT_SIZE;
			// inval
			udk_dma_sync_dev_to_cpu_by_pa(dst_tmp->dma_address, dst_tmp->dma_address + dst_tmp->dma_length);

			index++;
		}
	}

	// 3. encrypt
	if (udk_blk_crypto_endecrypt(&req, true)) {
		bio->bi_error = -EIO;
		udk_error("nvme crypto encrypt failed!\n");
		goto out_free_bounce_pages;
	}

	ret = true;
	goto out_free_dst_sg;

out_free_bounce_pages:
	while (i > 0)
		udk_mempool_mem_free(udk_nvme_crypto_mempool_get(), fusion_pa_to_sys_va((bio->bio_io_pvec[--i].pfn) << NVME_CRYPTO_PAGE_SIZE_SHIFT),
			NVME_CRYPTO_CIPHERTEXT_SIZE);
out_free_dst_sg:
	udk_free(req.dst.sgl);
out_free_src_sg:
	udk_free(req.src.sgl);
out_ret:
	return ret;
}

int udk_blk_crypto_fallback_decrypt_endio(struct udk_bio *bio)
{
	union udk_blk_crypto_iv iv;
	struct blk_crypto_request req;
	struct udk_scatterlist *sg;
	bool ret = false;
	unsigned int sg_size = 0;
	unsigned int index = 0;
	struct hisi_inline_crypto_key *crypto_cfg = (struct hisi_inline_crypto_key *)bio->crypto_key;

	req.key_slot_id = crypto_cfg->ci_key_index;
	req.metadata = crypto_cfg->ci_metadata;

	// 1. alloc scatterlist
	for (int i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		for (int j = 0; j < enc_bvec->len; j += NVME_CRYPTO_DATA_UNIT_SIZE)
			sg_size++;
	}

	sg = udk_malloc(sg_size * sizeof(struct udk_scatterlist));
	if (!sg) {
		bio->bi_error = -ENOMEM;
		goto out_ret;
	}

	udk_request_set_crypt(&req, sg, sg, sg_size, iv.bytes);
	udk_blk_crypto_dun_to_iv(crypto_cfg->index, &iv);

	// 2. fill scatterlist
	for (int i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		unsigned long long pfn = enc_bvec->pfn;

		for (int j = 0; j < enc_bvec->len; j += NVME_CRYPTO_DATA_UNIT_SIZE) {
			struct udk_scatterlist *sg_tmp = &sg[index];
			sg_tmp->dma_address = (pfn << NVME_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			sg_tmp->dma_length = NVME_CRYPTO_DATA_UNIT_SIZE;

			// flush
			udk_dma_sync_cpu_to_dev_by_pa(sg_tmp->dma_address, sg_tmp->dma_address + sg_tmp->dma_length);

			index++;
		}
	}

	// 3. decrypt
	if (udk_blk_crypto_endecrypt(&req, false)) {
		bio->bi_error = -EIO;
		udk_error("nvme crypto decrypt failed!\n");
		goto out_free_sg;
	}

	for (int i = 0; i < bio->bi_vec_num; i++) {
		struct udk_bio_pvec *enc_bvec = &bio->bio_io_pvec[i];
		unsigned long long pfn = enc_bvec->pfn;

		for (int j = 0; j < enc_bvec->len; j += NVME_CRYPTO_DATA_UNIT_SIZE) {
			unsigned long dma_address = (pfn << NVME_CRYPTO_PAGE_SIZE_SHIFT) + enc_bvec->offset + j;
			unsigned int dma_length = NVME_CRYPTO_DATA_UNIT_SIZE;

			// inval
			udk_dma_sync_dev_to_cpu_by_pa(dma_address, dma_address + dma_length);
		}
	}

	ret = true;

out_free_sg:
	udk_free(sg);
out_ret:
	return ret;
}

bool udk_blk_crypto_fallback_bio_prep(struct udk_bio *bio)
{
	if (udk_bio_data_dir(bio) == UDK_REQ_WRITE)
		return udk_blk_crypto_fallback_encrypt_bio(bio);

	return true;
}

void udk_blk_crypto_mempool_mem_free(struct udk_bio *bio)
{
	struct udk_mempool *mempool = udk_nvme_crypto_mempool_get();
	for (int i = 0; i < bio->bi_vec_num; i++)
		udk_mempool_mem_free(mempool, fusion_pa_to_sys_va((bio->bio_io_pvec[i].pfn) << NVME_CRYPTO_PAGE_SIZE_SHIFT),
			NVME_CRYPTO_CIPHERTEXT_SIZE);
}
