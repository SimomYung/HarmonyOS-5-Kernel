/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Adapted interfaces for xvm module test
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 14:59:23 2024
 */
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmpsf/adapter/module_test.h>

#ifdef CONFIG_DEBUG_BUILD
int xvm_module_test_attach(unsigned int id)
{
	return actvcall_hmcall_test_hmpsf_attach(id);
}

int xvm_module_test_detach(unsigned int id, bool force_invalidate)
{
	return actvcall_hmcall_test_hmpsf_detach(id, force_invalidate);
}

int xvm_module_test_exec(struct hmpsf_test_config_s *config)
{
	return actvcall_hmcall_test_hmpsf_exec(config);
}
#else
int xvm_module_test_attach(unsigned int id)
{
	(void)id;
	return E_HM_NOSYS;
}

int xvm_module_test_detach(unsigned int id, bool force_invalidate)
{
	(void)id;
	(void)force_invalidate;
	return E_HM_NOSYS;
}

int xvm_module_test_exec(struct hmpsf_test_config_s *config)
{
	(void)config;
	return E_HM_NOSYS;
}
#endif
