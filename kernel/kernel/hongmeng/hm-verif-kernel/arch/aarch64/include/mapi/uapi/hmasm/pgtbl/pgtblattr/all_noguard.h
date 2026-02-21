/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: All page table attributes with no guard
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 21 2022
 */

#include <hmasm/pgtbl/pgtblattr/kwcode.h>
#include <hmasm/pgtbl/pgtblattr/kcode.h>
#include <hmasm/pgtbl/pgtblattr/krodata.h>
#include <hmasm/pgtbl/pgtblattr/kncrodata.h>
#include <hmasm/pgtbl/pgtblattr/kdata.h>
#include <hmasm/pgtbl/pgtblattr/kncdata.h>
#include <hmasm/pgtbl/pgtblattr/kdev.h>
#include <hmasm/pgtbl/pgtblattr/hwcode.h>

#include <hmasm/pgtbl/pgtblattr/urodata.h>
#include <hmasm/pgtbl/pgtblattr/uncrodata.h>
#include <hmasm/pgtbl/pgtblattr/udata.h>
#include <hmasm/pgtbl/pgtblattr/udata_osh.h>
#include <hmasm/pgtbl/pgtblattr/uncdata.h>
#include <hmasm/pgtbl/pgtblattr/ucode.h>
#include <hmasm/pgtbl/pgtblattr/upcode.h>
#include <hmasm/pgtbl/pgtblattr/xom.h>
#include <hmasm/pgtbl/pgtblattr/uwcode.h>
#include <hmasm/pgtbl/pgtblattr/urodev.h>
#include <hmasm/pgtbl/pgtblattr/udev.h>
#include <hmasm/pgtbl/pgtblattr/uvdev.h>
#include <hmasm/pgtbl/pgtblattr/daxcode.h>
#include <hmasm/pgtbl/pgtblattr/nonemap.h>

#include <hmasm/pgtbl/pgtblattr/srodata.h>
#include <hmasm/pgtbl/pgtblattr/sncrodata.h>
#include <hmasm/pgtbl/pgtblattr/scode.h>
#include <hmasm/pgtbl/pgtblattr/swcode.h>
#include <hmasm/pgtbl/pgtblattr/sdata.h>
#include <hmasm/pgtbl/pgtblattr/sncdata.h>
#include <hmasm/pgtbl/pgtblattr/sdev.h>

#include <hmasm/pgtbl/pgtblattr/kucode_s2.h>
#include <hmasm/pgtbl/pgtblattr/ucode_s2.h>
#include <hmasm/pgtbl/pgtblattr/kcode_s2.h>
#include <hmasm/pgtbl/pgtblattr/kuwcode_s2.h>
#include <hmasm/pgtbl/pgtblattr/data_s2.h>
#include <hmasm/pgtbl/pgtblattr/rodata_s2.h>
#include <hmasm/pgtbl/pgtblattr/dev_s2.h>
#include <hmasm/pgtbl/pgtblattr/rodev_s2.h>
