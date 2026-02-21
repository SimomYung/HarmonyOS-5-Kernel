/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Crypto plugin core
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 04 10:17:10 2021
 */
#include <libsysif/devhost/server.h>

#include <devhost_api.h>
#include <devhost/plugin.h>
#include <hongmeng/errno.h>
#include <libhmsrv_crypt/crypt.h>
#include <libhmsrv_crypt/crypt_server.h>

#include "crypto.h"
#include "devhost.h"
#include <devhost/log.h>

static rref_t g_crypto_rref;

PUBLIC_SYMBOL
void get_random_bytes(void *buf, int nbytes)
{
	int ret = E_HM_OK;

	ret = hmsrv_crypt_urandom_bytes(g_crypto_rref, CRYPT_RNG_SYSTEM, (unsigned char *)buf, (unsigned int)nbytes);
	if (ret != E_HM_OK) {
		dh_error("failed to get random bytes, err=%s\n", hmstrerror(ret));
	}

	return;
}

PUBLIC_SYMBOL
int devhost_get_random_u64(uint64_t *value)
{
	int ret;
	ret = hmsrv_crypt_urandom_u64(g_crypto_rref, CRYPT_RNG_SYSTEM, value);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_core_entropy_read, dh_entropy_read_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, num)
{
	return hdr_devhost_core_entropy_read(sender, credential, num);
}

static int dh_crypto_barrier(void *arg)
{
	int err;
	UNUSED(arg);

	/* wait until crypto server is ready */
	do {
		(void)hm_thread_yield();
		g_crypto_rref = crypt_get_crypto_rref();
		err = REF_TO_ERR(g_crypto_rref);
	} while (err == E_HM_POSIX_NOENT);
	if (err != E_HM_OK) {
		dh_error("setup crypto service failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

static int dh_crypto_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret = E_HM_OK;

	UNUSED(plugin);

	ret = devhost_register_barrier(dh_crypto_barrier, NULL, DEVHOST_BARRIER_LEVEL_CORE);
	if (ret < 0) {
		dh_error("register crypto barrier failed, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

/*
 * crypto module, support for getting random numbers
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_core_entropy_read`
 *
 * export symbol:
 *  - func: `devhost_add_randomness`
 *  - func: `get_random_bytes`
 *  - func: `devhost_get_random_u64`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(crypto, dh_crypto_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_core_entropy_read, dh_entropy_read_ext)
);
