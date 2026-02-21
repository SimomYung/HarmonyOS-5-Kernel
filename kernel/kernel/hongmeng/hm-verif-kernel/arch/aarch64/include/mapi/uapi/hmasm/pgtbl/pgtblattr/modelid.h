/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: modelid related apis
 * Author: Huawei OS Kernel Lab
 * Created: Fri Jun 24 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_MODELID_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_MODELID_H

#include <hmasm/pgtbl/pgtblattr/model.h>
#include <hmkernel/mm/pgtable.h>
#include <hmkernel/compiler.h>
#include <hmasm/mm/pgtable.h>

static inline __always_inline enum __pgtblattr_modelid_e __pgtblattr_hint_to_modelid_stage2(__u64 hint)
{
	enum __pgtblattr_modelid_e modelid;
	if ((hint & PGTABLE_ATTR_DEVICE) != 0UL) {
		if ((hint & PGTABLE_ATTR_UWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(DEV_S2);
		} else {
			modelid = __PGTBLATTR_MODELID(RODEV_S2);
		}
	} else if ((hint & PGTABLE_ATTR_KEXEC) != 0UL || (hint & PGTABLE_ATTR_UEXEC) != 0UL) {
		if ((hint & PGTABLE_ATTR_UWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(KUWCODE_S2);
		} else if ((hint & PGTABLE_ATTR_KEXEC) == 0UL) {
			modelid = __PGTBLATTR_MODELID(UCODE_S2);
		} else if ((hint & PGTABLE_ATTR_UEXEC) == 0UL) {
			modelid = __PGTBLATTR_MODELID(KCODE_S2);
		} else {
			modelid = __PGTBLATTR_MODELID(KUCODE_S2);
		}
	} else {
		if ((hint & PGTABLE_ATTR_UWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(DATA_S2);
		} else {
			modelid = __PGTBLATTR_MODELID(RODATA_S2);
		}
	}
	return modelid;
}

static inline __always_inline enum __pgtblattr_modelid_e __pgtblattr_hint_to_modelid_privilege(__u64 hint)
{
	enum __pgtblattr_modelid_e modelid;
	if ((hint & PGTABLE_ATTR_DEVICE) != 0UL) {
		modelid = __PGTBLATTR_MODELID(SDEV);
	} else if ((hint & PGTABLE_ATTR_KEXEC) != 0UL) {
		if ((hint & PGTABLE_ATTR_KWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(SWCODE);
		} else {
			modelid = __PGTBLATTR_MODELID(SCODE);
		}
	} else {
		if ((hint & PGTABLE_ATTR_NOCACHE) != 0UL) {
			modelid = (hint & PGTABLE_ATTR_KWRITE) != 0U ?
				   __PGTBLATTR_MODELID(SNCDATA) :
				   __PGTBLATTR_MODELID(SNCRODATA);
		} else if ((hint & PGTABLE_ATTR_KWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(SDATA);
		} else {
			modelid = __PGTBLATTR_MODELID(SRODATA);
		}
	}
	return modelid;
}

static inline __always_inline enum __pgtblattr_modelid_e __pgtblattr_hint_to_modelid_user(__u64 hint)
{
	enum __pgtblattr_modelid_e modelid;
	__u32 sh;
	if ((hint & PGTABLE_ATTR_DEVICE) != 0UL &&
	    (hint & PGTABLE_ATTR_UVDEV) != 0UL) {
		modelid = __PGTBLATTR_MODELID(UVDEV);
	} else if ((hint & PGTABLE_ATTR_DEVICE) != 0UL &&
		   (hint & PGTABLE_ATTR_UWRITE) != 0UL) {
		modelid = __PGTBLATTR_MODELID(UDEV);
	} else if ((hint & PGTABLE_ATTR_UEXEC) != 0UL &&
		   (hint & PGTABLE_ATTR_DEVICE) != 0UL) {
		modelid = __PGTBLATTR_MODELID(DAXCODE);
	} else if ((hint & PGTABLE_ATTR_DEVICE) != 0UL) {
		modelid = __PGTBLATTR_MODELID(URODEV);
	} else if ((hint & PGTABLE_ATTR_UEXEC) != 0UL) {
		if ((hint & PGTABLE_ATTR_UWRITE) != 0UL) {
			modelid = __PGTBLATTR_MODELID(UWCODE);
		} else if ((hint & PGTABLE_ATTR_UREAD) != 0UL) {
			modelid = ((hint & PGTABLE_ATTR_BTI) != 0UL) ?
						__PGTBLATTR_MODELID(UPCODE) :
						__PGTBLATTR_MODELID(UCODE);
		} else {
			modelid = __PGTBLATTR_MODELID(XOM);
		}
	} else {
		sh = (hint >> PGTABLE_ATTR_A64_SH_SHIFT) & 0x3UL;
		if ((hint & PGTABLE_ATTR_NOCACHE) != 0UL) {
			modelid = (hint & PGTABLE_ATTR_UWRITE) != 0UL ?
				   __PGTBLATTR_MODELID(UNCDATA) :
				   __PGTBLATTR_MODELID(UNCRODATA);
		} else if ((hint & PGTABLE_ATTR_UWRITE) != 0UL) {
			if (sh == __PGTBLATTR_A64LOWERBLKATTR_SH_OUTER_SHAREABLE)
				modelid = __PGTBLATTR_MODELID(UDATA_OSH);
			else
				modelid = __PGTBLATTR_MODELID(UDATA);
		} else if ((hint & PGTABLE_ATTR_UREAD) != 0U) {
			modelid = __PGTBLATTR_MODELID(URODATA);
		} else {
			modelid = __PGTBLATTR_MODELID(NONEMAP);
		}
	}
	return modelid;
}

/*
 * This function is introduced for temporary use.
 * It should be removed when hint are eliminated totally.
 */
static inline __always_inline enum __pgtblattr_modelid_e __pgtblattr_hint_to_modelid(__u64 hint)
{
	enum __pgtblattr_modelid_e modelid;

	if ((hint & PGTABLE_ATTR_A64_STAGE2) != 0UL) {
		modelid = __pgtblattr_hint_to_modelid_stage2(hint);
	} else if ((hint & PGTABLE_ATTR_PSMAP) != 0UL) {
		modelid = __pgtblattr_hint_to_modelid_privilege(hint);
	} else {
		modelid = __pgtblattr_hint_to_modelid_user(hint);
	}
	return modelid;
}

#endif
