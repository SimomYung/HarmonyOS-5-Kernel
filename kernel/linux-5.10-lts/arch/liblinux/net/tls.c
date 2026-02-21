// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <net/tls.h>
#include <securec.h>
#include <trace/hooks/liblinux.h>

#define TLS_TX_UPDATE_KEY	0x8000 /* Update transmit crypto key */
#define TLS_RX_UPDATE_KEY	0x8001 /* Update receive crypto key */

static int update_key(union tls_crypto_context *crypto_ctx,
		struct cipher_context *cctx, struct crypto_aead **aead,
		sockptr_t optval, unsigned int optlen)
{
	u16 iv_size, keysize, rec_seq_size, salt_size;
	unsigned char *iv, *rec_seq, *key, *salt;
	union tls_crypto_context tmp_crypto_ctx;
	int rc = 0;

	if (!TLS_CRYPTO_INFO_READY(&crypto_ctx->info)) {
		rc = -EINVAL;
		goto out;
	}

	rc = copy_from_sockptr(&tmp_crypto_ctx.info, optval, sizeof(struct tls_crypto_info));
	if (rc) {
		rc = -EFAULT;
		goto out;
	}

	if (tmp_crypto_ctx.info.version != crypto_ctx->info.version ||
	    tmp_crypto_ctx.info.cipher_type != crypto_ctx->info.cipher_type) {
		rc = -EINVAL;
		goto out;
	}

	switch (tmp_crypto_ctx.info.cipher_type) {
	case TLS_CIPHER_AES_GCM_128:
		rc = (optlen == sizeof(struct tls12_crypto_info_aes_gcm_128)) ? 0 : -EINVAL;
		iv = tmp_crypto_ctx.aes_gcm_128.iv;
		key = tmp_crypto_ctx.aes_gcm_128.key;
		salt = tmp_crypto_ctx.aes_gcm_128.salt;
		rec_seq = tmp_crypto_ctx.aes_gcm_128.rec_seq;
		iv_size = TLS_CIPHER_AES_GCM_128_IV_SIZE;
		keysize = TLS_CIPHER_AES_GCM_128_KEY_SIZE;
		salt_size = TLS_CIPHER_AES_GCM_128_SALT_SIZE;
		rec_seq_size = TLS_CIPHER_AES_GCM_128_REC_SEQ_SIZE;
		break;
	default:
		rc = -EINVAL;
		break;
	}

	if (rc)
		goto out;

	rc = copy_from_sockptr_offset(((void *)&tmp_crypto_ctx.info +
			sizeof(struct tls_crypto_info)), optval,
			sizeof(struct tls_crypto_info),
			optlen - sizeof(struct tls_crypto_info));
	if (rc) {
		rc = -EFAULT;
		goto out;
	}

	rc = crypto_aead_setkey(*aead, key, keysize);
	if (rc) {
		memzero_explicit(&tmp_crypto_ctx, optlen);
		goto out;
	}
	(void)memcpy_s(cctx->iv, iv_size + salt_size, salt, salt_size);
	(void)memcpy_s(cctx->iv + salt_size, iv_size, iv, iv_size);
	(void)memcpy_s(cctx->rec_seq, rec_seq_size, rec_seq, rec_seq_size);
	(void)memcpy_s(crypto_ctx, optlen, &tmp_crypto_ctx, optlen);

out:
	return rc;
}

static inline int do_tls_setsockopt_tx_update_key(struct sock *sk,
				sockptr_t optval, unsigned int optlen)
{
	struct tls_context *ctx = tls_get_ctx(sk);
	union tls_crypto_context *crypto_ctx;
	struct tls_sw_context_tx *sw_ctx_tx;
	struct cipher_context *cctx;
	struct crypto_aead **aead;

	crypto_ctx = &ctx->crypto_send;
	cctx = &ctx->tx;
	sw_ctx_tx = tls_sw_ctx_tx(ctx);
	aead = &sw_ctx_tx->aead_send;

	return update_key(crypto_ctx, cctx, aead, optval, optlen);
}

static inline int do_tls_setsockopt_rx_update_key(struct sock *sk,
				sockptr_t optval, unsigned int optlen)
{
	struct tls_context *ctx = tls_get_ctx(sk);
	union tls_crypto_context *crypto_ctx;
	struct tls_sw_context_rx *sw_ctx_rx;
	struct cipher_context *cctx;
	struct crypto_aead **aead;

	crypto_ctx = &ctx->crypto_recv;
	cctx = &ctx->rx;
	sw_ctx_rx = tls_sw_ctx_rx(ctx);
	aead = &sw_ctx_rx->aead_recv;

	return update_key(crypto_ctx, cctx, aead, optval, optlen);
}

static void hook_do_tls_setsockopt(void *args, struct sock *sk,
		int optname, sockptr_t optval, unsigned int optlen, int *ret)
{
	if (optname != TLS_TX_UPDATE_KEY && optname != TLS_RX_UPDATE_KEY)
		return;

	if (sockptr_is_null(optval) || (optlen < sizeof(struct tls_crypto_info))) {
		*ret = -EINVAL;
		return;
	}

	lock_sock(sk);
	if (optname == TLS_TX_UPDATE_KEY)
		*ret = do_tls_setsockopt_tx_update_key(sk, optval, optlen);
	else
		*ret = do_tls_setsockopt_rx_update_key(sk, optval, optlen);
	release_sock(sk);
}

INIT_VENDOR_HOOK(ldk_rvh_do_tls_setsockopt, hook_do_tls_setsockopt);

#define MAX_SKB_LEN (64 * 1024)
static DEFINE_MUTEX(tls_sw_cache_mutex);
static bool tls_sw_cache_inuse = false;	/* Set when tls_sw_cache is used */
static struct sk_buff *tls_sw_cache = NULL;
static void skb_reinit(struct sk_buff *skb)
{
	unsigned char *head = skb->head;
	struct skb_shared_info *shinfo;

	/* Only clear those fields we need to clear, not those that we will
	 * actually initialise below. Hence, don't put any more fields after
	 * the tail pointer in struct sk_buff!
	 */
	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->data = head;
	skb_reset_tail_pointer(skb);
	skb->mac_header = (typeof(skb->mac_header))~0U;
	skb->transport_header = (typeof(skb->transport_header))~0U;

	/* make sure we initialize shinfo sequentially */
	shinfo = skb_shinfo(skb);

	memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));
}

static void skb_copy_full(struct sk_buff *nskb, struct sk_buff *skb)
{
	int headerlen = skb_headroom(skb);

	skb_reinit(nskb);

	/* Set the data pointer */
	skb_reserve(nskb, headerlen);
	/* Set the tail pointer and length */
	skb_put(nskb, skb->len);

	if (skb_copy_bits(skb, -headerlen, nskb->head, headerlen + skb->len))
		BUG();

	skb_copy_header(nskb, skb);

	if (skb->sk)
		skb_set_owner_w(nskb, skb->sk);
}

static void hook_tls_sw_cache_init(void *args, int unuse)
{
	 /* Pre-alloc a global skb to be used when memory is low. */
	if (unlikely(!tls_sw_cache)) {
		mutex_lock(&tls_sw_cache_mutex);
		if (!tls_sw_cache) {
			tls_sw_cache = __alloc_skb(MAX_SKB_LEN, GFP_KERNEL, 0,
						   NUMA_NO_NODE);
		}
		mutex_unlock(&tls_sw_cache_mutex);
	}
}

INIT_VENDOR_HOOK(ldk_rvh_tls_sw_cache_init, hook_tls_sw_cache_init);

static void hook_tls_skb_alloc(void *args, struct tls_sw_context_rx *ctx,
			    struct sk_buff **skb, struct strp_msg **rxm, int *n_sgin)
{
	struct sk_buff *old_skb = *skb;

	/* Use tls global skb when no memory is available. */
	mutex_lock(&tls_sw_cache_mutex);
	if (!tls_sw_cache || tls_sw_cache_inuse ||
		skb_end_offset(old_skb) + old_skb->data_len > tls_sw_cache->truesize) {
		mutex_unlock(&tls_sw_cache_mutex);
		return;
	}

	skb_copy_full(tls_sw_cache, old_skb);
	ctx->recv_pkt = tls_sw_cache;
	kfree_skb(old_skb);
	tls_sw_cache_inuse = true;
	*skb = tls_sw_cache;
	*rxm = strp_msg(*skb);
	*n_sgin = 1;

	mutex_unlock(&tls_sw_cache_mutex);
}

INIT_VENDOR_HOOK(ldk_rvh_tls_skb_alloc, hook_tls_skb_alloc);

static void hook_tls_consume_skb(void *args, struct tls_sw_context_rx *ctx,
			    struct sk_buff *skb, bool drop)
{
	if (skb == tls_sw_cache) {
		/* tls global skb */
		mutex_lock(&tls_sw_cache_mutex);
		tls_sw_cache_inuse = false;
		mutex_unlock(&tls_sw_cache_mutex);

		return;
	}

	if (!drop)
		consume_skb(skb);
	else
		kfree_skb(skb);
}

INIT_VENDOR_HOOK(ldk_rvh_tls_consume_skb, hook_tls_consume_skb);

static void hook_tls_ctx_update(void *args, struct tls_sw_context_rx *ctx,
			    struct sk_buff **skb, struct strp_msg **rxm, struct tls_msg **tlm)
{
	if (*skb != ctx->recv_pkt) {
		/*
		* Here just update the pointer.
		* Their content has been copied yet.
		*/
		*skb = ctx->recv_pkt;
		if (rxm)
			*rxm = strp_msg(*skb);
		if (tlm)
			*tlm = tls_msg(*skb);
	}
}

INIT_VENDOR_HOOK(ldk_rvh_tls_ctx_update, hook_tls_ctx_update);
