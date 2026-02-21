/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: battery nvme manager
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/reboot.h>
#include <linux/workqueue.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <huawei_platform/power/batt_info_pub.h>
#include <battery_nvme_manager.h>
#include <platform_include/basicplatform/linux/power/platform/coul/battery_nvme_api.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>

#define coul_debug(fmt, args...) pr_debug("[coul_nvme]" fmt, ## args)
#define coul_info(fmt, args...) pr_info("[coul_nvme]" fmt, ## args)
#define coul_warn(fmt, args...) pr_warn("[coul_nvme]" fmt, ## args)
#define coul_err(fmt, args...) pr_err("[coul_nvme]" fmt, ## args)

#define BAT0_NV1 516
#define BAT0_NV2 517
#define BAT0_NV3 518

#define BAT1_NV1 522
#define BAT1_NV2 523
#define BAT1_NV3 524

#define BAT2_NV1 525
#define BAT2_NV2 526
#define BAT2_NV3 527

#define FID_POWER_COUL_SYNC_SEND           0xc3004A00u

struct batt_nvme_dev_info {
	int init_flag; /* 0:Fail   1:SUCC */
	struct wakeup_source *power_wake_lock;
	struct delayed_work nvme_init_delayed_work;
	struct battery_library library_nvme;
};

static struct batt_nvme_dev_info g_di[BATT_MAX];
struct mutex g_nvme_lock;

/* Print battery information */
static void print_battery_info(const struct battery_nvme_info *battery)
{
	int i;

	coul_info("sn: %s\n", battery->sn);
	coul_info("Cycle Count: %d\n", battery->cycle);
	coul_info("limit_fcc: %d\n", battery->limit_fcc);
	for (i = 0; i < MAX_TEMPS; i++)
	    coul_info("temp: %d, real_fcc %d\n", battery->temp[i], battery->real_fcc[i]);
	coul_info("qmax: %d\n", battery->qmax);
	coul_info("last_fcc_cycle: %d\n", battery->last_fcc_cycle);
	coul_info("cleared: %d\n", battery->cleared);
}

/* Print battery library information */
static void print_battery_library_info(int batt_index, const struct battery_library *library)
{
	int i;

	coul_info("Battery %d Library Information +:\n", batt_index);
	for (i = 0; i < MAX_COUNT; ++i) {
		coul_info("Battery nv index %d:\n", i);
		if (library->index == i)
			coul_info("<<<<< index %d >>>>>\n", i);
		print_battery_info(&library->batteries[i]);
	}
	coul_info("Battery Library Information -:\n");
}

static int nvme_read(int batt_index, struct batt_nvme_dev_info *di)
{
	struct opt_nve_info_user nve = {0};
	int ret;
	int size = sizeof(di->library_nvme);
	int nv_index = 0;
	int rd_size = 0;
	int nv_num_start;
	errno_t res;

	switch (batt_index) {
	case BATT_0:
		nv_num_start = BAT0_NV1;
		break;
	case BATT_1:
		nv_num_start = BAT1_NV1;
		break;
	case BATT_2:
		nv_num_start = BAT2_NV1;
		break;
	default:
		return -1;
	}

	while ((size - rd_size) > 0) {
		memset_s(&nve, sizeof(nve), 0, sizeof(nve));
		nve.nv_number = nv_num_start + nv_index;
		nve.valid_size = min(NVE_NV_DATA_SIZE, (size - rd_size));
		nve.nv_operation = NV_READ;
		ret = nve_direct_access_interface(&nve);
		if (ret) {
			coul_err("%s batt %d nve.nv_number %d error\n", __func__, batt_index, nve.nv_number);
			return -1;
		}
		res = memcpy_s((void *)&di->library_nvme + rd_size, sizeof(di->library_nvme) - rd_size,
			nve.nv_data, nve.valid_size);
		if (res) {
			coul_err("%s nve.valid_size %d error\n", __func__, nve.valid_size);
			return -1;
		}
		rd_size += nve.valid_size;
		coul_info("%s batt %d nve.valid_size %d rd_size %d\n", __func__, batt_index, nve.valid_size, rd_size);
		if (++nv_index > 3) {
			coul_err("%s, nv_index %d error\n", __func__, nv_index);
			break;
		}
	}
	if (di->library_nvme.index < 0 || di->library_nvme.index >= MAX_COUNT)
		return -1;

	return 0;
}

static int nvme_write(int batt_index, struct battery_library *library_nvme)
{
	struct opt_nve_info_user nve = {0};
	int ret;
	int size = sizeof(*library_nvme);
	int write_size = 0;
	int nv_index = 0;
	int nv_num_start;

	switch (batt_index) {
	case BATT_0:
		nv_num_start = BAT0_NV1;
		break;
	case BATT_1:
		nv_num_start = BAT1_NV1;
		break;
	case BATT_2:
		nv_num_start = BAT2_NV1;
		break;
	default:
		return -1;
	}

	while(size > 0) {
		memset_s(&nve, sizeof(nve), 0, sizeof(nve));
		nve.nv_number = nv_num_start + nv_index;
		nve.valid_size = min(NVE_NV_DATA_SIZE, size);
		nve.nv_operation = NV_WRITE;
		memcpy_s(&nve.nv_data, NVE_NV_DATA_SIZE, (void *)library_nvme + write_size, nve.valid_size);
		ret = nve_direct_access_interface(&nve);
		if (ret) {
			coul_err("%s batt %d nv_number %d error\n", __func__, batt_index, nve.nv_number);
			break;
		}
		size -= nve.valid_size;
		write_size += nve.valid_size;
		coul_info("%s batt %d write_size %d\n", __func__, batt_index, write_size);
		if (++nv_index > MAX_COUNT) {
			coul_err("%s batt %d nv_index %d error\n", __func__, batt_index, nv_index);
			ret = -1;
			break;
		}
	}
	return ret;
}

static int tzsp_process(struct battery_library *library, const char *sn, unsigned int cmd)
{
	int ret;
	struct battery_shmem_to_tzsp tzsp_shmem;
	struct ffa_send_direct_data args = {
		.data0 = FID_POWER_COUL_SYNC_SEND,
		.data1 = (unsigned long)cmd,
		.data2 = 0,
		.data3 = 0,
		.data4 = sizeof(tzsp_shmem),
	};

	strcpy_s(tzsp_shmem.sn, MAX_BATTERY_SN_LENGTH - 1, sn);
	memcpy_s(&tzsp_shmem.library, sizeof(tzsp_shmem.library), library, sizeof(*library));

	ret = ffa_platdrv_send_msg_with_shmem(&args, (void *)&tzsp_shmem, SHMEM_INOUT);
	if (ret || args.data2 != POWER_SVC_SUCC || args.data4 != sizeof(tzsp_shmem)) {
		coul_err("%s, ret %d\n", __func__, ret);
		return ret;
	}

	memcpy_s(library, sizeof(*library), &tzsp_shmem.library, sizeof(*library));
	return ret;
}

/* 1:without SN; 0:with sn */
static int is_battery_without_sn(const char *sn)
{
	if(strlen(sn) <= 0) {
		coul_err("%s error, strlen library->sn %d \n", __func__, strlen(sn));
		return 1;
	}

	if (strcmp(sn, WITHOUT_SN) == 0)
		return 1;

	return 0;
}

void save_batt_nv_library(int batt_index, const char *batt_sn, struct battery_nvme_info *from_coul_core)
{
	int i;
	struct batt_nvme_dev_info *di = NULL;
	struct battery_nvme_info *batt_nvme = NULL;
	int ret;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return;
	}
	di = &g_di[batt_index];

	coul_info("%s batt %d + \n", __func__, batt_index);
	mutex_lock(&g_nvme_lock);
	coul_info("%s batt %d sn:%s\n", __func__, batt_index, batt_sn);

	ret = tzsp_process(&di->library_nvme, batt_sn, BATT_NVME_QUERY);
	if (ret) {
		coul_info("%s sn: %s, error %d\n", __func__, batt_sn, ret);
		mutex_unlock(&g_nvme_lock);
		return;
	}
	batt_nvme = &(di->library_nvme.batteries[di->library_nvme.index]);
	batt_nvme->cycle = from_coul_core->cycle;
	batt_nvme->limit_fcc = from_coul_core->limit_fcc;
	for (i = 0; i < MAX_TEMPS; i++) {
		batt_nvme->temp[i] = from_coul_core->temp[i];
		batt_nvme->real_fcc[i] = from_coul_core->real_fcc[i];
	}
	batt_nvme->qmax = from_coul_core->qmax;
	batt_nvme->last_fcc_cycle = from_coul_core->last_fcc_cycle;
	batt_nvme->cleared = 0;
	nvme_write(batt_index, &di->library_nvme);

	print_battery_library_info(batt_index, &di->library_nvme);
	mutex_unlock(&g_nvme_lock);
	coul_info("%s batt %d -\n", __func__, batt_index);
}

/* Only for battery without SN */
static int batt_nv_cycle_restore(int batt_index)
{
	int ret;
	struct batt_nvme_dev_info *di = NULL;
	struct battery_library *library = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return -1;
	}
	di = &g_di[batt_index];
	library = &di->library_nvme;

	if (di->init_flag == 0) {
		coul_err("%s batt %d nvme manager initialization failed.\n",  __func__, batt_index);
		return -1;
	}
	mutex_lock(&g_nvme_lock);

	ret = tzsp_process(library, WITHOUT_SN, BATT_NVME_RESTORE);
	if (ret) {
		coul_info("%s sn: %s, error %d\n", __func__, WITHOUT_SN, ret);
		mutex_unlock(&g_nvme_lock);
		return -1;
	}

	nvme_write(batt_index, library);
	print_battery_library_info(batt_index, &di->library_nvme);
	coul_debug("%s -\n", __func__);
	mutex_unlock(&g_nvme_lock);
	return ret;
}

/* Only for battery without SN */
static int batt_nv_cycle_clear(int batt_index)
{
	int ret;
	struct batt_nvme_dev_info *di = NULL;
	struct battery_library *library = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return -1;
	}
	di = &g_di[batt_index];
	library = &di->library_nvme;

	if (di->init_flag == 0) {
		coul_err("%s batt %d nvme manager initialization failed.\n", __func__, batt_index);
		return -1;
	}
	mutex_lock(&g_nvme_lock);

	if (is_battery_without_sn(library->batteries[library->index].sn) == 0) {
		mutex_unlock(&g_nvme_lock);
		coul_err("%s library[%d].sn %s\n", __func__, library->index, library->batteries[library->index].sn);
		return -1;
	}

	ret =  tzsp_process(library, WITHOUT_SN, BATT_NVME_CLEAR);
	if (ret) {
		coul_info("%s sn: %s, error %d\n", __func__, WITHOUT_SN, ret);
		mutex_unlock(&g_nvme_lock);
		return -1;
	}

	nvme_write(batt_index, library);
	print_battery_library_info(batt_index, &di->library_nvme);
	coul_debug("%s -\n", __func__);

	mutex_unlock(&g_nvme_lock);
	return ret;
}

void batt_nv_cycle_restore_all(void)
{
	(void)batt_nv_cycle_restore(BATT_0);
	(void)batt_nv_cycle_restore(BATT_1);
	(void)batt_nv_cycle_restore(BATT_2);
}

void batt_nv_cycle_clear_all(void)
{
	(void)batt_nv_cycle_clear(BATT_0);
	(void)batt_nv_cycle_clear(BATT_1);
	(void)batt_nv_cycle_clear(BATT_2);
}

int quary_batt_nv_library(int batt_index, const char *batt_sn, struct battery_nvme_info *to_coul_core)
{
	struct batt_nvme_dev_info *di = NULL;
	struct battery_nvme_info *batt_nvme = NULL;
	int ret;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return -1;
	}
	di = &g_di[batt_index];
	if (di == NULL) {
		coul_err("%s, di is null\n", __func__);
		return -1;
	}

	if (strlen(batt_sn) > MAX_BATTERY_SN_LENGTH) {
		coul_err("%s, batt %d batt sn[%s] is error\n", __func__, batt_index, batt_sn);
		return -1;
	}

	coul_info("%s, batt %d, batt_sn %s +\n", __func__, batt_index, batt_sn);
	mutex_lock(&g_nvme_lock);

	ret = nvme_read(batt_index, di);
	if (ret) {
		coul_err("%s, batt %d nvme read fail\n", __func__, batt_index);
		di->init_flag = 0;
		mutex_unlock(&g_nvme_lock);
		return ret;
	}
	print_battery_library_info(batt_index, &di->library_nvme);

	di->init_flag = 1;

	ret = tzsp_process(&di->library_nvme, batt_sn, BATT_NVME_QUERY);
	if (ret) {
		coul_err("%s sn: %s, error %d\n", __func__, batt_sn, ret);
		mutex_unlock(&g_nvme_lock);
		return -1;
	}

	nvme_write(batt_index, &di->library_nvme);
	batt_nvme = &(di->library_nvme.batteries[di->library_nvme.index]);
	memcpy_s(to_coul_core, sizeof(*to_coul_core), batt_nvme, sizeof(*batt_nvme));
	print_battery_library_info(batt_index, &di->library_nvme);

	mutex_unlock(&g_nvme_lock);
	coul_info("%s batt %d + \n", __func__, batt_index);
	return 0;
}

int __init battery_nvme_init(void)
{
	mutex_init(&g_nvme_lock);
	return 0;
}

subsys_initcall_sync(battery_nvme_init);

#ifdef CONFIG_DFX_DEBUG_FS
void batt_nvme_test_cycle(int batt_index, int index, int cycle)
{
	struct batt_nvme_dev_info *di = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return;
	}

	di = &g_di[batt_index];

	di->library_nvme.batteries[index].cycle = cycle;
	nvme_write(batt_index, &di->library_nvme);
}

void batt_nvme_test_reset(int batt_index)
{
	struct batt_nvme_dev_info *di = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_err("%s invalid batt index %d\n", __func__, batt_index);
		return;
	}
	di = &g_di[batt_index];

	memset_s(&di->library_nvme, sizeof(struct battery_library), 0, sizeof(struct battery_library));
	nvme_write(batt_index, &di->library_nvme);
}

#endif
