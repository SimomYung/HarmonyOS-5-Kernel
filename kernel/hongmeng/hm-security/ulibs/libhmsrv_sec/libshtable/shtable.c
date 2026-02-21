/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Shared hash table functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:50:28 2019
 */
#include "libshtable/shtable.h"

#include <string.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#include "shm.h"
#include "htable.h"

#define SHT_SHM_NAME_BUCKET     "sht_bucket"
#define SHT_SHM_NAME_NODE       "sht_node"
#define SHT_SHM_NAME_KEY        "sht_key"

static inline bool sht_cfg_is_validated(const struct shtable *sht,
					const struct sht_config *cfg)
{
	return sht != NULL && cfg != NULL &&
	       cfg->size <= SHT_MAX && cfg->size != 0U &&
	       cfg->key_desc.cmp != NULL && cfg->key_desc.clear != NULL;
}

static int sht_shm_init(struct shtable *sht, const struct sht_config *cfg)
{
	int rc;

	rc = sht_shm_open(sht, &sht->bucket_shm, SHT_SHM_NAME_BUCKET,
			  sizeof(struct sht_bucket_s));
	if (rc != E_HM_OK) {
		hm_error("failed to open bucket shm, rc=%s\n", hmstrerror(rc));
		goto exit;
	}
	rc = sht_shm_open(sht, &sht->node_shm, SHT_SHM_NAME_NODE,
			  sizeof(struct sht_node_s));
	if (rc != E_HM_OK) {
		hm_error("failed to open node shm failed, rc=%s\n", hmstrerror(rc));
		goto close_bucket_shm;
	}
	rc = sht_shm_open(sht, &sht->key_shm, SHT_SHM_NAME_KEY,
			  cfg->key_desc.size);
	if (rc != E_HM_OK) {
		hm_error("failed to open the shm of sht_key, rc=%s\n", hmstrerror(rc));
		goto close_node_shm;
	}
	goto exit;

close_node_shm:
	sht_shm_close(sht, &sht->node_shm);
close_bucket_shm:
	sht_shm_close(sht, &sht->bucket_shm);
exit:
	return rc;
}

int sht_init(struct shtable *sht, const struct sht_config *cfg, bool ctrl)
{
	int rc;

	if (!sht_cfg_is_validated(sht, cfg)) {
		hm_error("configuration is invalid, rc=%s\n", hmstrerror(E_HM_INVAL));
		rc = E_HM_INVAL;
		goto exit;
	}

	if (cfg->prefix != NULL) {
		sht->prefix = strdup(cfg->prefix);
		if (sht->prefix == NULL) {
			hm_error("failed to strdup, rc=%s\n", hmstrerror(E_HM_NOMEM));
			rc = E_HM_NOMEM;
			goto exit;
		}
	} else {
		sht->prefix = NULL;
	}

	if (ctrl) {
		sht->ctrl = malloc(sizeof(*sht->ctrl));
		if (sht->ctrl == NULL) {
			rc = E_HM_NOMEM;
			hm_error("failed to malloc, rc=%s\n", hmstrerror(rc));
			goto free_prefix;
		}
	}

	sht->size = (sht_uint_t)cfg->size;
	sht->key_desc = cfg->key_desc;
	sht->hash_desc = cfg->hash_desc;

	rc = sht_shm_init(sht, cfg);
	if (rc != E_HM_OK) {
		hm_error("failed to initialize the sht shm, rc=%s\n", hmstrerror(rc));
		goto free_ctrl;
	}

	if (ctrl) {
		sht_htable_init(sht);
	}
	goto exit;

free_ctrl:
	free((void *)sht->ctrl);
	sht->ctrl = NULL;
free_prefix:
	free((void *)sht->prefix);
	sht->prefix = NULL;
exit:
	return rc;
}

void sht_fini(struct shtable *sht)
{
	if (sht != NULL) {
		if (sht->ctrl != NULL) {
			sht_htable_fini(sht);
			free(sht->ctrl);
			sht->ctrl = NULL;
		}

		sht_shm_close(sht, &sht->bucket_shm);
		sht_shm_close(sht, &sht->node_shm);
		sht_shm_close(sht, &sht->key_shm);

		free((void *)sht->prefix);
		sht->prefix = NULL;
	}
}

int sht_mod_init(const struct shtable *sht, struct sht_mod *mod,
		 const char *name, size_t element_size)
{
	int rc = E_HM_OK;

	if (sht == NULL || mod == NULL) {
		hm_error("parameters are invalid\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = sht_shm_open(sht, &mod->shm, name, element_size);
		if (rc != E_HM_OK) {
			hm_error("failed to open the sht shm, rc=%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		mod->element_size = element_size;
		mod->sht = sht;
	}

	return rc;
}

void sht_mod_fini(struct sht_mod *mod)
{
	if (mod != NULL) {
		sht_shm_close(mod->sht, &mod->shm);
	}
}

int sht_write_value_cb(sht_idx_t node, void *arg)
{
	int rc = E_HM_OK;

	if (arg == NULL || ((struct sht_mod_cb_arg *)arg)->mod == NULL) {
		hm_error("parameters are invalid\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		struct sht_mod_cb_arg *mv = (struct sht_mod_cb_arg *)arg;
		const struct sht_mod *mod = mv->mod;

		rc = memcpy_s(sht_mod_ref(mod, node), mod->element_size, mv->value,
			      mv->size);
		if (rc != EOK) {
			hm_error("failed to malloc\n");
			rc = E_HM_RANGE;
		}
	}

	return rc;
}

int sht_read_value_cb(sht_idx_t node, void *arg)
{
	int rc = E_HM_OK;

	if (arg == NULL || ((struct sht_mod_cb_arg *)arg)->mod == NULL) {
		hm_error("parameters are invalid\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		struct sht_mod_cb_arg *mv = (struct sht_mod_cb_arg *)arg;
		const struct sht_mod *mod = mv->mod;

		rc = memcpy_s(mv->value, mv->size, sht_mod_ref(mod, node),
			      mod->element_size);
		if (rc != EOK) {
			hm_error("failed to copy memory\n");
			rc = E_HM_RANGE;
		}
	}

	return rc;
}

int sht_mod_query(const struct sht_mod *mod, const void *key, void *value)
{
	sht_idx_t node;
	seq_t seq;
	int rc = E_HM_OK;

	if (mod == NULL || mod->sht == NULL) {
		hm_error("param is invalid\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when sht_retry return E_HM_OK, Since mod keep changing
		 * in loop body, it has a bound.
		 * Loop condition is constant and reasonable:
		 * mod is limited, so the loop condition is reasonable.
		 */
		do {
			rc = sht_search(mod->sht, key, &node, &seq);
			if (rc == E_HM_OK) {
				rc = sht_mod_read(mod, node, value);
			}
			/* We checked mod->sht, so sht_retry only return OK or AGAIN */
			if (rc == E_HM_OK) {
				rc = sht_retry(mod->sht, node, seq);
			}
		} while (rc == E_HM_AGAIN);
	}

	return rc;
}

int sht_simple_init(struct shtable_simple *sht, const struct sht_config *cfg,
		    size_t value_size, bool ctrl)
{
	int rc = E_HM_OK;

	if (sht == NULL) {
		hm_error("shtable is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = sht_init(&sht->htable, cfg, ctrl);
		if (rc != E_HM_OK) {
			hm_error("failed to initialize shared hash table, err=%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		rc = sht_mod_init(&sht->htable, &sht->value, "value", value_size);
		if (rc != E_HM_OK) {
			hm_error("failed to initialize shared hash table module, err=%s\n", hmstrerror(rc));
			sht_fini(&sht->htable);
		}
	}

	return rc;
}

void sht_simple_fini(struct shtable_simple *sht)
{
	if (sht != NULL) {
		sht_mod_fini(&sht->value);
		sht_fini(&sht->htable);
	}
}
