/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Wrappers for methods of secmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 17:01:53 2019
 */

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_security.h>
#include <libsysif/sysmgr/api.h>

static uint32_t policy_mode = HM_SEC_POLICY_MODE_NORMAL;
static bool kasan_enabled = false;
static uintptr_t shadow_pgref_va = 0;

int hm_sec_cred_cfg(struct sec_cred_cfg *cfg)
{
	int rc;
	struct __actvret_hmcall_sec_cred_cfg ret;

	mem_zero_s(ret);
	rc = actvcall_hmcall_sec_cred_cfg(&ret);
	if (rc < 0) {
		hm_warn("Failed to get credential configuration: %s\n",
			 hmstrerror(rc));
		return rc;
	}

	if (cfg != NULL) {
		cfg->proc_num = (size_t)(ssize_t)ret.proc_num;
		cfg->cfg_mask = (uint8_t)ret.mask;
	}

	return E_HM_OK;
}

int hm_sec_setexeccon(const char *context)
{
	int err;
	struct secmgr_sec_context secctx;

	err = strcpy_s(secctx.context, sizeof(secctx.context), context);
	if (err != 0) {
		return E_HM_INVAL;
	}

	err = actvcall_hmcall_sec_setexeccon(secctx);
	if (err < 0) {
		hm_debug("Failed to set execution context for %s: %s\n",
			 secctx.context, hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int hm_xpm_mode_read(char *buf, size_t len)
{
	int ret;
	ret = actvxactcall_hmcall_sec_xpm_mode(true, true, HM_XPM_MODE_ACTION_READ, buf, len);
	return ret;
}

int hm_xpm_mode_write(char *buf, size_t len)
{
	int ret;
	ret = actvxactcall_hmcall_sec_xpm_mode(true, true, HM_XPM_MODE_ACTION_WRITE, buf, len);
	return ret;
}

int hm_sec_set_debug(const char *context, unsigned cnode_idx,
		     unsigned *log_enable)
{
	int err;
	struct secmgr_sec_context secctx = {0};
	struct __actvret_hmcall_sec_debug ret;
	mem_zero_s(ret);

	if (context == NULL || log_enable == NULL) {
		return E_HM_INVAL;
	}
	err = strcpy_s(secctx.context, sizeof(secctx.context), context);
	if (err != 0) {
		return E_HM_INVAL;
	}

	err = actvcall_hmcall_sec_debug(secctx, cnode_idx, &ret);
	if (err != E_HM_OK) {
		hm_debug("Failed to set sec debug cmd for %s: %s\n",
			 secctx.context, hmstrerror(err));
		return err;
	} else {
		*log_enable = ret.log_enable;
	}

	return E_HM_OK;
}

int hm_sec_umask(int mask)
{
	return lsyscall_sec_umask(mask);
}

uintptr_t _hm_sec_shconf_base __ro_after_init;
uintptr_t _hm_sec_shconf_ro_base __ro_after_init;

int hm_sec_shconf_init(void)
{
	int rc = E_HM_OK;
	size_t len = 0;
	void *addr = NULL;
	int fd;
	if (_hm_sec_shconf_base == 0) {
		fd = hm_mem_shm_open(HM_SEC_SHCONF_SHM_NAME, 0, O_RDONLY, &len);
		if (fd >= 0) {
			addr = hm_mem_mmap(0, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
			if (addr == MAP_FAILED) {
				rc = E_HM_NOMEM;
			} else {
				_hm_sec_shconf_base = (uintptr_t)addr;
			}
			(void)hm_mem_shm_close(fd);
		} else {
			rc = E_HM_POSIX_NOENT;
		}
	}

	if (rc == E_HM_OK && _hm_sec_shconf_ro_base == 0) {
		fd = hm_mem_shm_open(HM_SEC_SHCONF_SHM_RO_NAME, 0, O_RDONLY, &len);
		if (fd >= 0) {
			addr = hm_mem_mmap(0, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
			if (addr == MAP_FAILED) {
				rc = E_HM_NOMEM;
			} else {
				_hm_sec_shconf_ro_base = (uintptr_t)addr;
			}
			(void)hm_mem_shm_close(fd);
		} else {
			rc = E_HM_POSIX_NOENT;
		}
	}
	return rc;
}

int hm_sec_shconf_set_field(unsigned int field, unsigned int value)
{
	return actvcall_hmcall_sec_shconf_set_field(field, value);
}

void hm_sec_policy_set_mode(uint32_t mode)
{
	policy_mode = mode;
}

uint32_t hm_sec_policy_get_mode(void)
{
	return policy_mode;
}

int hm_sec_register_service(const char *name, cref_t actv_cref,
			    unsigned int method_id, uref_t *actv_uref)
{
	struct __actvpool_grant grant;
	struct secmgr_sec_reg_service_name name_arg = {.name = {0}};
	uref_t uref;
	int ret;

	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;

	ret = strcpy_s(name_arg.name, sizeof(name_arg.name), name);
	if (ret != EOK) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		uref = hm_ucap_export(0, hmsrv_sysmgr_cnode_idx(),
				      CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX,
				      actv_cref, CRIGHT_ACTVPOOL_CALL,
				      ptr_to_ulong(&grant));
		ret = REF_TO_ERR(uref);
	}
	if (ret == E_HM_OK) {
		ret = actvcall_hmcall_sec_register_service(uref, method_id, name_arg);
		if (ret != E_HM_OK) {
			(void)hm_ucap_revoke(0, uref);
		}
	}
	if (ret == E_HM_OK && actv_uref != NULL) {
		*actv_uref = uref;
	}
	return ret;
}

int hm_sel_load_policy(const void *policy, size_t size)
{
	struct hm_actv_buf buf;
	hm_actv_buf_init(&buf, policy, size);
	return actvcall_hmcall_sec_sel_load_policy(buf);
}

int hm_sel_load_policy_fwd(const void *policy, size_t size)
{
	struct hm_actv_buf buf;
	hm_actv_buf_init(&buf, policy, size);
	return actvxactcall_hmcall_sec_sel_load_policy(true, true, buf);
}

void hm_sec_set_kasan_state(bool val)
{
	kasan_enabled = val;
}

bool hm_sec_is_kasan_enabled(void)
{
	return kasan_enabled;
}

void hm_sec_set_shadow_pgref_va(uintptr_t addr)
{
	shadow_pgref_va = addr;
}

uintptr_t hm_sec_get_shadow_pgref_va(void)
{
	return shadow_pgref_va;
}
