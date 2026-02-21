/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Entry point of liblinux backend
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#include <lnxbase/lnxbase.h>

#include <elf.h>
#include <errno.h>
#include <string.h>
#include <hmdsosym.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libsysif/devmgr/types.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>

#ifdef CONFIG_LNXBASE_VERIFY_SIG
#include <libhmsrv_sys/hm_ashm.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/devhost.h>
#endif
#include <devhost/log.h>

#include "internal.h"

#define KO_SUFFIX ".ko"
#define KO_SUFFIX_LEN     (sizeof(KO_SUFFIX) - 1)
#define LDK_SUFFIX ".ldk"
#define LDK_SUFFIX_LEN    (sizeof(LDK_SUFFIX) - 1)

static int
__lnxbase_mod_is_compatible(struct devhost_loader *loader,
			    const char *libname, const void *data, size_t size)
{
	struct lnxbase_ctx *ctx = container_of(loader, struct lnxbase_ctx, mod_loader);
	const char *p_end = libname + strlen(libname);

	BUG_ON(loader == NULL);

	if (ctx->mod_ops->is_compatible != NULL) {
		return ctx->mod_ops->is_compatible(libname, data, size);
	}

	/* no ops, juse check libname */
	return (((strlen(libname) > KO_SUFFIX_LEN) &&
		(strcmp(p_end - KO_SUFFIX_LEN, KO_SUFFIX) == 0)) ||
		((strlen(libname) > LDK_SUFFIX_LEN) &&
		(strcmp(p_end - LDK_SUFFIX_LEN, LDK_SUFFIX) == 0)));
}

static int
__lnxbase_mod_load_module(struct devhost_loader *loader,
			  const char *libname, const void *data, size_t size, char *args)
{
	struct lnxbase_ctx *ctx = container_of(loader, struct lnxbase_ctx, mod_loader);
	int ret;

	BUG_ON(loader == NULL);

	if (ctx->mod_ops->load_module == NULL) {
		return E_HM_NOSYS;
	}

	ret = ctx->mod_ops->load_module(libname, data, size, args);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

static inline size_t __suffix_len(const char *libname)
{
	const char *p_end = libname + strlen(libname);

	if ((strlen(libname) > LDK_SUFFIX_LEN) &&
	    (strcmp(p_end - LDK_SUFFIX_LEN, LDK_SUFFIX) == 0)) {
		return LDK_SUFFIX_LEN;
	}

	return KO_SUFFIX_LEN;
}

/*
 * Refer to `filename2modname` function in
 * busybox/modutils/modprobe-small.c
 */
static inline
int __libname_to_modname(const char *libname, char *modname, size_t namelen)
{
	size_t size, i = 0;

	size = strlen(libname) - __suffix_len(libname);
	if (size >= namelen) {
		return E_HM_INVAL;
	}

	for (;i < size; i++) {
		modname[i] = libname[i] == '-' ? '_' : libname[i];
	}
	modname[i] = '\0';

	return 0;
}

static int
__lnxbase_mod_unload_module(struct devhost_loader *loader, const char *libname)
{
	struct lnxbase_ctx *ctx = NULL;
	char modname[DRIVER_NAME_LEN] = { 0 };
	int ret;

	BUG_ON(loader == NULL);

	/*
	 * Some module files are named `xx-xx.ko`,
	 * but their module names are `xx_xx`.
	 */
	ret = __libname_to_modname(libname, modname, DRIVER_NAME_LEN);
	if (ret < 0) {
		return ret;
	}

	ctx = container_of(loader, struct lnxbase_ctx, mod_loader);
	if (ctx->mod_ops->unload_module == NULL) {
		return E_HM_NOSYS;
	}

	ret = ctx->mod_ops->unload_module((const char *)modname);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

int lnxbase_enable_module(struct lnxbase_ctx *ctx, struct lnxbase_module_ops *ops)
{
	struct devhost_loader *loader = NULL;
	int ret;

	if ((ctx == NULL) || (ops == NULL)) {
		return -EINVAL;
	}
	if (ctx->mod_ops != NULL) {
		return -EBUSY;
	}

	ctx->mod_ops = ops;

	loader = &ctx->mod_loader;
	loader->is_compatible = __lnxbase_mod_is_compatible;
	loader->load_module = __lnxbase_mod_load_module;
	loader->unload_module = __lnxbase_mod_unload_module;

	ret = devhost_register_loader(loader);
	if (ret < 0) {
		ctx->mod_ops = NULL;
		return ret;
	}

	ctx->features |= LNXBASE_FEATURE_MODULE;

	return 0;
}

int lnxbase_load_module_chk(const struct lnxbase_ctx *ctx, const char *modname,
			    const char *chksym)
{
	/* load module only-if checked symbol not exists */
	if ((chksym == NULL) ||
	    (ctx->mod_ops->symbol_get(chksym) == NULL)) {
		return liblinux_pal_request_module(modname);
	}
	return 0;
}

int lnxbase_load_symbols(const struct lnxbase_ctx *ctx, const char *modname,
			 const struct lnxbase_syminfo *si)
{
	if (ctx == NULL) {
		return -EINVAL;
	}
	if ((ctx->mod_ops == NULL) ||
	    (ctx->mod_ops->load_symbol_module == NULL)) {
		return -ENOSYS;
	}
	return ctx->mod_ops->load_symbol_module(modname, si);
}

int lnxbase_fill_mod_symbol(const char *name, const struct liblinux_mod_syminfo *si)
{
	struct dso_sym_entry entry;
	const char *mod_anon_name = NULL;
	char anon_name_buf[VR_ANON_NAME_MAX];
	int ret;

	if ((name == NULL) || (si->symtab == NULL) || (si->strtab == NULL) ||
	    (si->module_core == NULL)) {
		return -EINVAL;
	}

	entry.elf_type = ET_REL;
	entry.fname = (unsigned long)(uintptr_t)name;
	entry.fname_len = (unsigned int)(strlen(name) + 1);
	entry.symtab_addr = (unsigned long)(uintptr_t)(si->symtab);
	entry.symtab_len  = si->symtab_len;
	entry.strtab_addr = (unsigned long)(uintptr_t)(si->strtab);
	entry.strtab_len = si->strtab_len;
	entry.base = (unsigned long)(uintptr_t)(si->module_core);
	entry.base_len = si->core_size;
	ret = fill_dso_syminfo(&entry);
	if (ret < 0) {
		return ret;
	}

	/* build and set anon name for module */
	ret = snprintf_s(anon_name_buf, VR_ANON_NAME_MAX, VR_ANON_NAME_MAX - 1,
			 MODULE_ANON_NAME_FMT, name);
	if (likely(ret > 0)) {
		mod_anon_name = anon_name_buf;
	} else {
		mod_anon_name = MODULE_NONAME;
	}
	(void)hm_mem_set_anon_name((void *)(si->module_core),
				   (size_t)(si->core_size), mod_anon_name);

	return 0;
}

void lnxbase_drop_mod_symbol(const void *module_core)
{
	if (module_core != NULL) {
		drop_dso_syminfo(module_core);
	}
}

#ifdef CONFIG_LNXBASE_VERIFY_SIG
int lnxbase_verify_cms_data(const void *data, unsigned long data_len,
			    const void *sig, unsigned long sig_len)
{
	int ret;
	int shm_id;
	size_t shm_len;
	void *shm_addr = NULL;
	uint64_t shm_key;
	xref_t devmgr_tgt = { .rref = libdh_get_devmgr_ap() };

	if (data == NULL || sig == NULL || data_len + sig_len < data_len) {
		return -EINVAL;
	}

	shm_id = hm_ashm_open_auth(0, 0U, data_len + sig_len,
				   SHM_O_CREAT | SHM_O_EXCL | SHM_O_RDWR,
				   &shm_len);
	if (shm_id < 0) {
		return -hmerrno2posix(shm_id);
	}
	shm_addr =  hm_mem_mmap_posix(NULL, shm_len, MPROT_READ | MPROT_WRITE,
				      MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE,
				      shm_id, 0, &ret);
	if (shm_addr == MAP_FAILED) {
		(void)hm_ashm_close(shm_id);
		(void)hm_ashm_unlink(shm_id);
		return -hmerrno2posix(ret);
	}
	NOFAIL(memcpy_s(shm_addr, shm_len, data, data_len));
	NOFAIL(memcpy_s(ptr_add(shm_addr, data_len), shm_len - data_len, sig, sig_len));
	(void)hm_mem_munmap(shm_addr, shm_len);

	ret = hm_ashm_grant(shm_id, SHM_GRANT_RDONLY, devmgr_tgt, &shm_key);
	if (ret == E_HM_OK) {
		ret = actvcapcall_drvcall_devmgr_fw_cms_verify(libdh_get_devmgr_ap(),
							       shm_key, data_len, sig_len);
	}
	(void)hm_ashm_close(shm_id);
	(void)hm_ashm_unlink(shm_id);
	return -hmerrno2posix(ret);
}
#endif
