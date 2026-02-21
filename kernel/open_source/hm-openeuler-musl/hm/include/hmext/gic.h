/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 25 11:45:32 2022
 */

#ifndef HM_INCLUDE_HMEXT_GIC_H
#define HM_INCLUDE_HMEXT_GIC_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <hmsyscall.h>

/* One user-level gic controller used by products for easy maintenance.
 * The old gic controller is implemented as user-level driver. The result is
 * irq handling needs to go through vfs(fd)->devhost(irq handling thread) then
 * user-gic. To reduce the overhead, we need user-gic to communicate with
 * hongmeng kernel directly via vsyscall->ulibs->kernel */

#define HM_GIC_IOCTL_CMD_REGIST				0x0
#define HM_GIC_IOCTL_CMD_ENABLE				0x1
#define HM_GIC_IOCTL_CMD_DISABLE			0x2
#define HM_GIC_IOCTL_CMD_WAITIRQ			0x3
#define HM_GIC_IOCTL_CMD_SET_SERVER			0x4
#define HM_GIC_IOCTL_CMD_SET_TRIGGER			0x5
#define HM_GIC_IOCTL_CMD_STAT				0x6

/* encode init function as one cmd to reduce arg number in vsyscall */
#define HM_GIC_CMD_INIT					0x10
#define HM_GIC_CMD_THREAD_CREF				0x11

enum hm_gic_trigger_type {
	HM_GIC_TRIGGER_EDGE_FALL,
	HM_GIC_TRIGGER_LEVEL_LOW,
	HM_GIC_TRIGGER_EDGE_RISE,
	HM_GIC_TRIGGER_LEVEL_HIGH,
	NR_HM_GIC_TRIGGER_TYPE,
};

/*
 * @param: gic_name: name of gic, not used now
 * @param: name_len: length of name, not used now
 * return: irqctrl_id: irqctrl_id of gic
 * */
int hm_gic_init(const char *gic_name, size_t name_len);

/*
 * @param: cmd: command for ioctl
 * @param: irq: on which the irq is processed
 * @param: arg: used by concrete cmd to process irq
 * @param: arg_len: the size of arg
 * return: errcode
 * */
int hm_gic_ioctl(int cmd, uint32_t irq, void *arg, size_t arg_len);

int hm_gic_thread_self(uint64_t *self_cref);

static inline int hm_gic_open(const char *mod_name)
{
	int err = 0;
	if (mod_name == NULL) {
		err = EINVAL;
	} else {
		err = hm_gic_init(mod_name, strlen(mod_name));
	}

	return err;
}

static inline int hm_gic_close(uint64_t mod_rref)
{
	return ENOSYS;
}

#endif
