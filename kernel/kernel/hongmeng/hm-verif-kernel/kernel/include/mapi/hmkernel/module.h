/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Module
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 01:29:43 2018
 */
#ifndef MAPI_HMKERNEL_MODULE_H
#define MAPI_HMKERNEL_MODULE_H

#include <lib/dlist.h>
#include <hmkernel/compiler.h>

struct CNode_s;

extern void earlydev_init(void);
extern void coredev_init(void);
#ifdef PAC_FWCFI_KERNEL
extern void pacfix_init(void);
#endif
extern void modules_init(void);
extern void percpu_init(unsigned int cpu);
extern void earlypercpu_init(unsigned int cpu);
extern void rootcnode_init(struct CNode_s *cnode);

typedef int (*earlydev_init_fn_t)(void);
typedef int (*coredev_init_fn_t)(void);
typedef void (*pacfix_init_fn_t)(int, char **, char **);
#ifndef __ELFLOADER_NG__
typedef int (*module_init_fn_t)(void);
#else
typedef int (*module_init_fn_t)(void *);
#endif

typedef int (*percpu_init_fn_t)(unsigned int cpu);
typedef int (*earlypercpu_init_fn_t)(unsigned int cpu);
typedef int (*rootcnode_init_fn_t)(struct CNode_s *root_cnode);

#define __earlydev_init(fn, lv) static earlydev_init_fn_t __earlydev_init_##fn##lv __used \
	__attribute__((section(".earlydevinit" #lv ".init"))) = fn;

#define __coredev_init(fn, lv)	static module_init_fn_t __module_init_##fn##lv __used \
	__attribute__((section(".coredevinit" #lv ".init"))) = fn;

#define __module_init(fn, lv)	static module_init_fn_t __module_init_##fn##lv __used \
	__attribute__((section(".moduleinit" #lv ".init"))) = fn;

#define __earlypercpu_init(fn, lv)	static earlypercpu_init_fn_t __earlypercpu_init_##fn##lv __used \
	__attribute__((section(".earlypercpuinit" #lv ".init"))) = fn;

#define __percpu_init(fn, lv)	static percpu_init_fn_t __percpu_init_##fn##lv __used \
	__attribute__((section(".percpuinit" #lv ".init"))) = fn;

#define __rootcnode_init(fn, lv) static rootcnode_init_fn_t __rootcnode_init_##fn##lv __used \
	__attribute__((section(".rootcnodeinit" #lv ".init"))) = fn;

/* early console device */
#define kernel_earlydev_init(fn)	__earlydev_init(fn, 1)

#define serial_device_init(fn)		__coredev_init(fn, 1)
#define power_device_init(fn)		__coredev_init(fn, 2)
#define mem_device_init(fn)		__coredev_init(fn, 3)
#define gic_device_init(fn)		__coredev_init(fn, 4)
#define timer_device_init(fn)		__coredev_init(fn, 5)
#define ipi_device_init(fn)		__coredev_init(fn, 6)

/* console device */
#define early_device_init(fn)		__module_init(fn, 1)
#define key_device_init(fn)		__module_init(fn, 2)
#define device_init(fn)			__module_init(fn, 3)
#define late_device_init(fn)		__module_init(fn, 4)

#define early_device_percpu_pre_init(fn)	__earlypercpu_init(fn, 1)
#define key_device_percpu_pre_init(fn)		__earlypercpu_init(fn, 2)
#define device_percpu_pre_init(fn)		__earlypercpu_init(fn, 3)
#define late_device_percpu_pre_init(fn)		__earlypercpu_init(fn, 4)

#define device_percpu_init(fn)		__percpu_init(fn, 1)
#define module_percpu_init(fn)		__percpu_init(fn, 2)

#define early_device_rootcnode_init(fn)	__rootcnode_init(fn, 1)
#define kernel_rootcnode_init(fn)	__rootcnode_init(fn, 2)
#define key_device_rootcnode_init(fn)	__rootcnode_init(fn, 3)
#define device_rootcnode_init(fn)	__rootcnode_init(fn, 4)

#define extension_init(fn)		__module_init(fn, 1)
#ifdef __HOST_LLT__
void llt_setup_cpu_rq_mapping(void);
#endif
#endif
