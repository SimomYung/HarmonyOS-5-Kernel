/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh prototypes for devhost core support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 17:11:57 2020
 */
#ifndef ULIBS_LIBDEVHOST_DEVHOST_H
#define ULIBS_LIBDEVHOST_DEVHOST_H

#include <stdlib.h>
#include <hmasm/page.h>

#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmactv/actv.h>
#include <libhmsrv_io/type.h>

/* libdh actvpool default config */
#define LIBDH_ACTV_STACK_ORDER		1UL /* 8K */
#define LIBDH_ACTV_STACK_SIZE		(1UL << (LIBDH_ACTV_STACK_ORDER + (unsigned long)PAGE_SHIFT))
#define LIBDH_ACTV_NUM_INIT		8
#define LIBDH_ACTV_NUM_MAX		2048
#define LIBDH_ACTV_BUF_SIZE		2048UL
#define LIBDH_ACTV_TSD_SIZE		128U

#define LIBDH_FWTYPE_LEGACY		(0x1U)
#define LIBDH_FWTYPE_FDT		(0x2U)
#define LIBDH_FWTYPE_ALL		(0xFFU)

/*
 * Register to devmgr as a libdevhost with exist actvpool.
 *
 * Parameters:
 *  - name		name of this devhost
 *  - ap_cref		actvpool cref with required sysif handler
 *  - flags		devhost flags:
 *  			  - DH_FLAGS_STANDALONE standalone devhost
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_init_ap(const char *name, cref_t ap_cref, unsigned int flags);

/*
 * Set default actvpool configs
 *
 * Parameters:
 *  - configs			actvpool configs
 *  - ap_name			actvpool name
 */
void libdh_apconfig_set_default(struct actvpool_configs *configs, const char *ap_name);

/*
 * Create actvpool for libdh
 *
 * Parameters:
 *  - configs			actvpool configs
 *  - ap_cref_out	[OUT]	created actvpool cref
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_ap_create(struct actvpool_configs *configs, cref_t *ap_cref_out);

/*
 * Grant libdh actvpool to target
 *
 * Parameters:
 *  - tgt_rref		target rref
 *
 * Return granted uref for success, hmerrno for errors.
 */
uref_t libdh_ap_grant(rref_t tgt_rref);

/*
 * Export libdh actvpool for target
 *
 * Parameters:
 *  - tgt_cidx		target cnode-index
 *
 * Return exported uref for success, hmerrno for errors.
 */
uref_t libdh_ap_export(unsigned int tgt_cidx);

/*
 * Register to devmgr as a libdevhost with new actvpool.
 *
 * Parameters:
 *  - name		name of this devhost
 *  - flags		devhost flags, check libdh_init_ap() for details
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
static inline
int libdh_init(const char *name, struct actvpool_configs *configs, unsigned int flags)
{
	cref_t ap_cref;
	int ret;

	ret = libdh_ap_create(configs, &ap_cref);
	if (ret == E_HM_OK) {
		ret = libdh_init_ap(name, ap_cref, flags);
	}
	return ret;
}

rref_t libdh_get_devmgr_ap(void);
unsigned int libdh_get_devmgr_cnode_idx(void);

cref_t libdh_get_dh_ap(void);

/*
 * Listen for actvpool elastic event.
 *
 * Parameters:
 *  - configs		actvpool configs
 *
 * Return E_HM_OK for success, hmerrno for errors.
*/
int libdh_ap_elastic(const struct actvpool_configs *configs);

/*
 * devt region helpers
 */

/*
 * Register and reserve chrdev region to devmgr.
 * All request to registered chrdev regions will be redirected to
 * this devhost.
 *
 * Parameters:
 *  - major		major to register, 0 for auto selected
 *  - baseminor		first minor to reserved
 *  - minorct		minor count to reserved
 *  - name		identifier for this region
 *  - major_out	[OUT]	major number reserved, only valid when return success
 *  			should equal to major if major is not `0`
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_register_chrdev_region(unsigned int major, unsigned int baseminor, int minorct,
				 const char *name, unsigned int *major_out /* out */);

/*
 * Release registered chrdev region.
 * Requests will no longer redirected to this devhost after unregister.
 *
 * Parameters:
 *  - major		major to register, must equal to major returned when register
 *  - baseminor		first minor to release, must equal to baseminor when register
 *  - minorct		minor count to release, must equal to minorct when register
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
void libdh_unregister_chrdev_region(unsigned int major, unsigned int baseminor, int minorct);

/*
 * Register and reserve blkdev region to devmgr.
 * All request to registered blkdev regions will be redirected to
 * this devhost.
 *
 * Parameters:
 *  - major		major to register, 0 for auto selected
 *  - name		identifier for this region
 *  - major_out	[OUT]	major number reserved, only valid when return success.
 *  			should equal to major if major is not `0`
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_register_blkdev_region(unsigned int major, const char *name, unsigned int *major_out);

/*
 * Release registered blkdev region.
 * Requests will no longer redirected to this devhost after unregister.
 *
 * Parameters:
 *  - major		major to register, must equal to major returned when register
 *  - baseminor		first minor to release, must equal to baseminor when register
 *  - minorct		minor count to release, must equal to minorct when register
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
void libdh_unregister_blkdev_region(unsigned int major, const char *name);


/*
 * device create helpers
 */

/*
 * Create device to devmgr, the request devt must be registered before.
 * uid/gid is set to zero and mode is set to 0600
 *
 * Parameters:
 *  - name		device name
 *  - devt		device number, a.k.a. <major, minor>
 *  - devid_out	[OUT]	device identifier used by fops
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_chrdev_create(const char *name, unsigned int devt,
			int *devid_out /* out */);

/*
 * Simple wrapper to libdh_chrdev_add, register chr region and create device.
 * uid/gid is set to zero and mode is set to 0600
 *
 * Parameters:
 *  - name		device name
 *  - major		device major, 0 for auto selected
 *  - devid_out	[OUT]	device identifier used by fops
 *  - devt_out	[OUT]	selected devt
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_chrdev_create_simple(const char *name, unsigned int major,
			       int *devid_out /* out */,
			       unsigned int *devt_out /* out */);

/*
 * Simple wrapper to libdh_chrdev_add, register chr region and create device.
 *
 * Parameters:
 *  - name		device name
 *  - major		device major, 0 for auto selected
 *  - umode		device mode
 *  - devid_out	[OUT]	device identifier used by fops
 *  - devt_out	[OUT]	selected devt
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_chrdev_create_simple_ex(const char *name, unsigned int major, unsigned int umode,
				  int *devid_out /* out */,
				  unsigned int *devt_out /* out */);

/*
 * To acquire the buffer of devmgr granted shm node
 *
 * Parameters:
 *  - shm_key		shm node id
 *  - buf	[OUT]	shm buffer pointer
 *  - size	[OUT]	shm buffer size
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_firmware_data(uint64_t shm_key, void **buf, unsigned long *size);

/*
 * To acquire the bootfdt buffer after devhost is registered
 *
 * Parameters:
 *  - buf		[OUT]	bootfdt buffer pointer
 *  - size		[OUT]	bootfdt size
 *  - fwtype_mask	[IN]	bootfdt firmware type
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int libdh_acquire_bootfdt(void **buf, unsigned long *size, unsigned int fwtype_mask);

#endif	/* ULIBS_LIBDEVHOST_DEVHOST_H */
