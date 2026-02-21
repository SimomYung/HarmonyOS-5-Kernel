/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vfs audit interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 26 20:04:27 2021
 */

#include <vfs.h>

#include <hongmeng/errno.h>
#include <libhmaudit/audit.h>
#include <libhmlog/hmlog.h>

#include <fd.h>

#include "internal.h"

#ifdef CONFIG_FSLIB_AUDIT
int vfs_audit_init(void)
{
	int err;

	err = actvcap_fscall(vfs_init_audit);
	if (err != E_HM_OK) {
		hm_error("init audit for fs failed\n");
	}

	return err;
}

int vfs_add_audit_rule(const char *path, uint16_t rule_id, uint16_t rule_num, uint32_t flags)
{
	return actvxcap_fscall(vfs_add_audit_rule, true, true, path, rule_id, rule_num, flags, -1, 0, 0);
}

int vfs_del_audit_rule(const char *path, uint64_t rule_id, uint32_t flags)
{
	return actvxcap_fscall(vfs_del_audit_rule, true, true, path, rule_id, flags, -1, 0, 0);
}

int vfs_match_audit_watch_file(uint64_t rule_id, const struct audit_watch_field *field)
{
	int err;
	rref_t fsmgr_rref;

	if (strcmp(field->srv_name, FSMGR_SVRNAME) != 0) {
		hm_warn("multi-server not supported, srv_name: %s\n", field->srv_name);
		return E_HM_OPNOTSUPP;
	}

	err = hm_path_acquire(FSMGR_PATH_ACTV, &fsmgr_rref);
	if (err != E_HM_OK) {
		return WATCH_NOT_MATCH;
	}
	err = actvxactcapcall_fscall_vfs_match_audit_watch_file(false, true, fsmgr_rref,
								rule_id, field->dev_id, field->index);
	(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
	return err;
}

int vfs_match_audit_watch_dir(uint64_t rule_id, int me_idx, const struct audit_watch_field *field)
{
	int err;
	rref_t fsmgr_rref;

	if (strcmp(field->srv_name, FSMGR_SVRNAME) != 0) {
		hm_warn("multi-server not supported, srv_name: %s\n", field->srv_name);
		return E_HM_OPNOTSUPP;
	}

	err = hm_path_acquire(FSMGR_PATH_ACTV, &fsmgr_rref);
	if (err != E_HM_OK) {
		return WATCH_NOT_MATCH;
	}
	err = actvxactcapcall_fscall_vfs_match_audit_watch_dir(false, true, fsmgr_rref,
							       rule_id, me_idx);
	(void)hm_path_release(FSMGR_PATH_ACTV, fsmgr_rref);
	return err;
}

static void audit_wp_start_default(void)
{
	return;
}

static void audit_wp_end_default(bool is_exec, const char *path, const char *srv_name)
{
	UNUSED(path);
	UNUSED(is_exec);
	UNUSED(srv_name);
	return;
}

static audit_wp_start_hook_func_t wp_start = audit_wp_start_default;
static audit_wp_end_hook_func_t wp_end = audit_wp_end_default;
void vfs_set_audit_wp_hook(audit_wp_start_hook_func_t start, audit_wp_end_hook_func_t end)
{
	wp_start = start;
	wp_end = end;
}

void vfs_audit_watch_point_start(void)
{
	wp_start();
}

void vfs_audit_watch_point_end(bool is_exec, const char *path, const char *srv_name)
{
	wp_end(is_exec, path, srv_name);
}
#else
int vfs_audit_init(void)
{
	return E_HM_NOSYS;
}

int vfs_add_audit_rule(const char *path, uint16_t rule_id, uint16_t rule_num, uint32_t flags)
{
	UNUSED(path, rule_id, rule_num, flags);

	return E_HM_NOSYS;
}

int vfs_del_audit_rule(const char *path, uint64_t rule_id, uint32_t flags)
{
	UNUSED(path, rule_id, flags);

	return E_HM_NOSYS;
}

int vfs_match_audit_watch_file(uint64_t rule_id, const struct audit_watch_field *field)
{
	UNUSED(rule_id, field);

	return E_HM_NOSYS;
}

int vfs_match_audit_watch_dir(uint64_t rule_id, int me_idx, const struct audit_watch_field *field)
{
	UNUSED(rule_id, me_idx, field);

	return E_HM_NOSYS;
}

void vfs_set_audit_wp_hook(audit_wp_start_hook_func_t start, audit_wp_end_hook_func_t end)
{
	UNUSED(start, end);

	return;
}

void vfs_audit_watch_point_start(void)
{
	return;
}

void vfs_audit_watch_point_end(bool is_exec, const char *path, const char *srv_name)
{
	UNUSED(is_exec, path, srv_name);

	return;
}
#endif
