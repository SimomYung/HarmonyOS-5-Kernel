

/* ---- includes ---- */
#include <linux/rtc.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/namei.h>
#include <linux/security.h>
#include <linux/fs.h>
#include <crypto/hash.h>
#include <crypto/sha.h>
#include <linux/syscalls.h>
#include <linux/mutex.h>

#include <hwbootfail/core/boot_detector.h>
#include <hwbootfail/chipsets/common/bootfail_common.h>
#include <hwbootfail/chipsets/common/bootfail_chipsets.h>

#include <linux/blackbox.h>
#include <linux/blackbox_common.h>
#include <linux/blackbox_storage.h>
#include <securec.h>
#include <trace/hooks/liblinux.h>
#include <mntn_public_interface.h>

#ifndef BF_SIZE_2K
#define BF_SIZE_2K (BF_SIZE_1K * 2)
#endif
#define RECORDS_COUNT 40
#define SHUT_FAIL_ERRNO (-1)
#define MAX_LOG_COUNT 10

static const int recovery_success = 1;

static char error_name_bl1_core_dev_fault[] = "BL1 core device fault";
static char error_name_bl1_non_core_dev_fault[] = "BL1 non core device fault";
static char error_name_bl1_img_fault[] = "BL1 image damaged or verification failed";
static char error_name_bl1_system_crash[] = "BL1 crash";
static char error_name_bl1_system_freeze[] = "BL1 freeze";
static char error_name_bl1_bad_env[] = "BL1 bad working env";
static char error_name_bl2_core_dev_fault[] = "BL2 core device fault";
static char error_name_bl2_non_core_dev_fault[] = "BL2 non core device fault";
static char error_name_bl2_img_fault[] = "BL2 image damaged or verification failed";
static char error_name_bl2_system_crash[] = "BL2 crash";
static char error_name_bl2_system_freeze[] = "BL2 freeze";
static char error_name_bl2_bad_env[] = "BL2 bad working env";
static char error_name_kernel_core_dev_fault[] = "Kernel core device fault";
static char error_name_kernel_non_core_dev_fault[] = "Kernel non core device fault";
static char error_name_kernel_img_fault[] = "Kernel image damaged or verification failed";
static char error_name_kernel_subsys_fault[] = "Kernel susystem fault";
static char error_name_kernel_system_crash[] = "Kernel crash";
static char error_name_kernel_system_freeze[] = "Kernel freeze";
static char error_name_kernel_bad_env[] = "Kernel bad working env";
static char error_name_native_core_dev_fault[] = "Native core device fault";
static char error_name_native_non_core_dev_fault[] = "Native non core device fault";
static char error_name_native_service_crash[] = "Native critical service crash";
static char error_name_native_service_freeze[] = "Native critical service freeze";
static char error_name_native_data_part_mount_fail[] = "Native /data mount failed";
static char error_name_native_data_part_mount_ro[] = "Native /data mounted RO";
static char error_name_native_data_damaged[] = "Native critical data damaged";
static char error_name_native_data_mount_fail[] = "Native critical part mount failed";
static char error_name_native_subsys_fault[] = "Native subsystem fault";
static char error_name_native_system_crash[] = "Native crash";
static char error_name_native_system_freeze[] = "Native freeze";
static char error_name_native_bad_env[] = "Native bad working env";
static char error_name_framework_core_dev_fault[] = "Framework core device fault";
static char error_name_framework_non_core_dev_fault[] = "Framework non core device fault";
static char error_name_framework_service_crash[] = "Framework critical service crash";
static char error_name_framework_service_freeze[] = "Framework critical service freeze";
static char error_name_framework_data_damaged[] = "Framework critical data damaged";
static char error_name_framework_subsys_fault[] = "Framework subsystem fault";
static char error_name_framework_system_crash[] = "Framework crash";
static char error_name_framework_system_freeze[] = "Framework freeze";
static char error_name_framework_bad_env[] = "Framework bad working env";
static char error_name_hardware_core_dev_fault[] = "Hardware core dev fault";
static char error_name_hardware_storage_fault[] = "Hardware storage fault";
static char error_name_hardware_non_core_dev_fault[] = "Hardware non core dev fault";

static char invalid_boot_stage[] = "INVALID STAGE";
static char invalid_errno[] = "INVALID_ERRNO";

static char shut_fail[] = "SHUTDOWN FAIL";

unsigned int g_write_index = 0;
struct mutex mtx_lock;
struct bootfail_log_content {
	unsigned int bl_log_size;
	unsigned int kernel_log_size;
	unsigned int bf_info_start;
	char bf_info_content[BF_SIZE_1K];
	char bl_log_content[SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE];
	char kernel_log_content[LAST_KMSG_SIZE];
};
static boot_stage_map_st stage_map[] = {
	{BL1_STAGE, "BL1_STAGE"},
	{BL2_STAGE, "BL2_STAGE"},
	{KERNEL_STAGE, "KERNEL_STAGE"},
	{NATIVE_STAGE, "NATIVE_STAGE"},
	{FRAMEWORK_STAGE, "FRAMEWORK_STAGE"},
	{BOOT_SUCC_STAGE, "BOOT_SUCC_STAGE"},
#ifdef CONFIG_SHUT_DETECTOR
	{SHUT_STAGE_FRAMEWORK_START, "SHUT_STAGE_FRAMEWORK_START"},
	{SHUT_STAGE_FRAMEWORK_FINISH, "SHUT_STAGE_FRAMEWORK_FINISH"},
	{SHUT_STAGE_NATIVE_START, "SHUT_STAGE_NATIVE_START"},
	{SHUT_STAGE_NATIVE_FINISH, "SHUT_STAGE_NATIVE_FINISH"},
	{SHUT_STAGE_KERNEL, "SHUT_STAGE_KERNEL"},
#endif
};

error_no_map_st error_no_map[] = {
	{BL1_CORE_DEV_FAULT, error_name_bl1_core_dev_fault},
	{BL1_NON_CORE_DEV_FAULT, error_name_bl1_non_core_dev_fault},
	{BL1_IMG_FAULT, error_name_bl1_img_fault},
	{BL1_SYSTEM_CRASH, error_name_bl1_system_crash},
	{BL1_SYSTEM_FREEZE, error_name_bl1_system_freeze},
	{BL1_BAD_ENV, error_name_bl1_bad_env},
	{BL2_CORE_DEV_FAULT, error_name_bl2_core_dev_fault},
	{BL2_NON_CORE_DEV_FAULT, error_name_bl2_non_core_dev_fault},
	{BL2_IMG_FAULT, error_name_bl2_img_fault},
	{BL2_SYSTEM_CRASH, error_name_bl2_system_crash},
	{BL2_SYSTEM_FREEZE, error_name_bl2_system_freeze},
	{BL2_BAD_ENV, error_name_bl2_bad_env},
	{KERNEL_CORE_DEV_FAULT, error_name_kernel_core_dev_fault},
	{KERNEL_NON_CORE_DEV_FAULT, error_name_kernel_non_core_dev_fault},
	{KERNEL_IMG_FAULT, error_name_kernel_img_fault},
	{KERNEL_SUBSYS_FAULT, error_name_kernel_subsys_fault},
	{KERNEL_SYSTEM_CRASH, error_name_kernel_system_crash},
	{KERNEL_SYSTEM_FREEZE, error_name_kernel_system_freeze},
	{KERNEL_BAD_ENV, error_name_kernel_bad_env},
	{NATIVE_CORE_DEV_FAULT, error_name_native_core_dev_fault},
	{NATIVE_NON_CORE_DEV_FAULT, error_name_native_non_core_dev_fault},
	{NATIVE_SERVICE_CRASH, error_name_native_service_crash},
	{NATIVE_SERVICE_FREEZE, error_name_native_service_freeze},
	{NATIVE_DATA_PART_MOUNT_FAIL, error_name_native_data_part_mount_fail},
	{NATIVE_DATA_PART_MOUNT_RO, error_name_native_data_part_mount_ro},
	{NATIVE_DATA_DAMAGED, error_name_native_data_damaged},
	{NATIVE_PART_MOUNT_FAIL, error_name_native_data_mount_fail},
	{NATIVE_SUBSYS_FAULT, error_name_native_subsys_fault},
	{NATIVE_SYSTEM_CRASH, error_name_native_system_crash},
	{NATIVE_SYSTEM_FREEZE, error_name_native_system_freeze},
	{NATIVE_BAD_ENV, error_name_native_bad_env},
	{FRAMEWORK_CORE_DEV_FAULT, error_name_framework_core_dev_fault},
	{FRAMEWORK_NON_CORE_DEV_FAULT, error_name_framework_non_core_dev_fault},
	{FRAMEWORK_SERVICE_CRASH, error_name_framework_service_crash},
	{FRAMEWORK_SERVICE_FREEZE, error_name_framework_service_freeze},
	{FRAMEWORK_DATA_DAMAGED, error_name_framework_data_damaged},
	{FRAMEWORK_SUBSYS_FAULT, error_name_framework_subsys_fault},
	{FRAMEWORK_SYSTEM_CRASH, error_name_framework_system_crash},
	{FRAMEWORK_SYSTEM_FREEZE, error_name_framework_system_freeze},
	{FRAMEWORK_BAD_ENV, error_name_framework_bad_env},
	{HARDWARE_CORE_DEV_FAULT, error_name_hardware_core_dev_fault},
	{HARDWARE_STORAGE_FAULT, error_name_hardware_storage_fault},
	{HARDWARE_NON_CORE_DEV_FAULT, error_name_hardware_non_core_dev_fault},
};

static int sha256_check(char *data, unsigned int len, char hash[SHA256_DIGEST_SIZE])
{
	int ret = 0;
	struct crypto_shash *tfm;
	size_t desc_size;
	struct shash_desc *desc;

	tfm = crypto_alloc_shash("sha256", 0, 0);
	if (IS_ERR(tfm)) {
		ret = PTR_ERR(tfm);
		goto out;
	}

	desc_size = crypto_shash_descsize(tfm) + sizeof(struct shash_desc);
	desc = kzalloc(desc_size, GFP_KERNEL);
	if (!desc) {
		ret = -ENOMEM;
		goto out_free_tfm;
	}
	desc->tfm = tfm;

	ret = crypto_shash_init(desc);
	if (ret < 0)
		goto out_free_desc;

	ret = crypto_shash_update(desc, data, len);
	if (ret)
		goto out_free_desc;

	ret = crypto_shash_final(desc, hash);

out_free_desc:
	kfree(desc);
out_free_tfm:
	crypto_free_shash(tfm);
out:
	return ret;
}

static bool check_values(struct bootfail_meta_log *bf_meta_log)
{
	if (bf_meta_log == NULL) {
		print_err("check_values: bf_meta_log is NULL!\n");
		return false;
    }

	if ((bf_meta_log->log_max_count < 0) || (bf_meta_log->log_max_count > MAX_LOG_COUNT)) {
		print_err("check_values: log_max_count is invalid!\n");
        return false;
    }

	if ((bf_meta_log->log_count < 0) || (bf_meta_log->log_count > bf_meta_log->log_max_count)) {
    	print_err("check_values: log_count is invalid!\n");
    	return false;
	}

	if (bf_meta_log->write_idx < 0 || bf_meta_log->write_idx >= bf_meta_log->log_max_count) {
		print_err("check_values: write_index is invalid!\n");
		return false;
	}
	return true;
}

static bool get_meta_log(struct bootfail_meta_log *bf_meta_log)
{
	int ret = 0;
	char hash[SHA256_DIGEST_SIZE];
	memset_s(bf_meta_log, sizeof(struct bootfail_meta_log), 0, sizeof(struct bootfail_meta_log));
	memset_s(hash, SHA256_DIGEST_SIZE, 0, SHA256_DIGEST_SIZE);

	ret = read_part(get_bfi_dev_path(), sizeof(union bfi_part_header) + sizeof(struct bootfail_proc_param),
				(char *)bf_meta_log, sizeof(struct bootfail_meta_log));
	if (ret != 0) {
		print_err("read rrecord part failed!\n");
		return false;
	}

	ret = sha256_check((char *)bf_meta_log + sizeof(bf_meta_log->sha256),
					   sizeof(struct bootfail_meta_log) - sizeof(bf_meta_log->sha256),
					   hash);
	if (ret != 0) {
		print_err("sha256_check failed!\n");
		return false;
	}

	if (memcmp(hash, bf_meta_log->sha256, SHA256_DIGEST_SIZE) != 0) {
		print_err("sha256 failed!\n");
		return false;
	}

	return check_values(bf_meta_log);
}

static int read_content(char *content, unsigned int start, unsigned int size)
{
	int ret;

	ret = read_part(get_bfi_dev_path(), start, (char *)content, size);
	if (ret != 0) {
		print_err("read log from record partition failed!");
		return -1;
	}
	return 0;
}

static bool write_content(char *content, unsigned int start, unsigned int size)
{
	int ret = write_part(get_bfi_dev_path(), start, (char *)content, size);
	if (ret != 0) {
		print_err("write log to record partition failed!");
		return false;
	}
	return true;
}

static void clear_bfi_misc(void) {
	union bfi_part_header header;
	memset_s(&header, sizeof(union bfi_part_header), 0, sizeof(union bfi_part_header));
	bool ret = write_content((char *)&header, 0, sizeof(union bfi_part_header));
	if (!ret) {
		print_err("clear bfi misc msg header failed");
		(void)write_content((char *)&header, 0, sizeof(union bfi_part_header));
	}
}

static void clear_recovery_info(struct bootfail_meta_log *bf_meta_log)
{
	int record_bak_addr = bf_meta_log->rrecord_offset + RECORDS_COUNT * sizeof(struct rrecord) +
		sizeof(struct rrecord_header);
	struct rrecord_header rheader;
	memset_s(&rheader, sizeof(struct rrecord_header), 0, sizeof(struct rrecord_header));

	struct rrecord *records = (struct rrecord *)vmalloc(sizeof(struct rrecord) * RECORDS_COUNT);
	if (records == NULL) {
		print_err("failed to alloc size for rrecord, exit\n");
		return;
	}

	memset_s(records, sizeof(struct rrecord) * RECORDS_COUNT, 0, sizeof(struct rrecord) * RECORDS_COUNT);

	bool ret = write_content((char *)(&rheader), bf_meta_log->rrecord_offset, sizeof(struct rrecord_header));
	if (!ret) {
		print_err("clear rrecord header failed");
		write_content((char *)(&rheader), bf_meta_log->rrecord_offset, sizeof(rheader.sha256));
	}

	ret = write_content((char *)records, bf_meta_log->rrecord_offset + sizeof(struct rrecord_header),
		sizeof(struct rrecord) * RECORDS_COUNT);
	if (!ret)
		print_err("clear rrecord  failed ");

	ret = write_content((char *)(&rheader), record_bak_addr, sizeof(struct rrecord_header));
	if (!ret) {
		print_err("clear rrecord bak header failed");
		write_content((char *)(&rheader), record_bak_addr, sizeof(rheader.sha256));
	}

	ret = write_content((char *)records, record_bak_addr + sizeof(struct rrecord_header),
		sizeof(struct rrecord) * RECORDS_COUNT);
	if (!ret)
		print_err("clear rrecord bak failed ");

	vfree(records);
	records = NULL;
}

static void clean_bootfail_info(void)
{
	struct bootfail_meta_log bf_meta_log;
	memset_s(&bf_meta_log, sizeof(struct bootfail_meta_log), 0, sizeof(struct bootfail_meta_log));
	bool ret = get_meta_log(&bf_meta_log);
	if (!ret) {
		print_err("update_rrecord_part, get_meta_log faild!\n");
		return;
	}

	clear_recovery_info(&bf_meta_log);
	bf_meta_log.log_count = 0;
	int result = sha256_check((char *)(&bf_meta_log) + sizeof(bf_meta_log.sha256),
		sizeof(struct bootfail_meta_log) - sizeof(bf_meta_log.sha256),
		(char *)(&bf_meta_log));
	if (result != 0) {
		print_err("sha256_check failed!\n");
		memset_s((char *)&bf_meta_log, sizeof(struct bootfail_meta_log), 0, sizeof(struct bootfail_meta_log));
	}
	write_content((char *)(&bf_meta_log), sizeof(union bfi_part_header) +
				sizeof(struct bootfail_proc_param), sizeof(struct bootfail_meta_log));
}

static void convert_stageno_to_str(char *stage, int stage_no)
{
	unsigned int index = 0;
	for (index; index < ARRAY_SIZE(stage_map); index++) {
		if (stage_map[index].stage == stage_no) {
			errno_t ret = strncpy_s(stage, BF_SIZE_128 - 1, stage_map[index].stage_dec, strlen(stage_map[index].stage_dec));
			if (ret != EOK)
				print_err("stage_map[index].stage_dec strncpy_s failed\n");
			return;
		}
	}
}

static void convert_errno_to_str(char *errnum, int errno)
{
	int ret = 0;
	unsigned int index = 0;

	if (errno == SHUT_FAIL_ERRNO) {
		ret = strncpy_s(errnum, BF_SIZE_128 - 1, shut_fail, strlen(shut_fail));
		if (ret < 0)
			print_err("convert_errno_to_str copy shutfail failed!\n");
		return;
	}
	for (index; index < ARRAY_SIZE(error_no_map); index++) {
		if (error_no_map[index].error == errno) {
			ret = strncpy_s(errnum, BF_SIZE_128 - 1, error_no_map[index].err_dec, strlen(error_no_map[index].err_dec));
				if (ret < 0)
					print_err("convert_errno_to_str copy bootfail failed!\n");
			return;
		}
	}
}

static void convert_orig_data(char *stage, char *errnum, int ori_stage, int ori_errno)
{
	int ret = 0;
	convert_stageno_to_str(stage, ori_stage);
	if (strlen(stage) == 0) {
		print_err("convert_orig_data: convert stage failed!\n");
		ret = strncpy_s(stage, BF_SIZE_128 - 1, invalid_boot_stage, strlen(invalid_boot_stage));
		if (ret)
			print_err("convert_orig_data copy boot stage failed!\n");
	}

	convert_errno_to_str(errnum, ori_errno);
	if (strlen(errnum) == 0) {
		print_err("convert_orig_data: convert errno failed!\n");
		ret = strncpy_s(errnum, BF_SIZE_128 - 1, invalid_errno, strlen(invalid_errno));
		if (ret)
			print_err("convert_orig_data copy errno failed!\n");
	}
}

static void send_bbox_notify(struct error_info *info, char *stage, char *errnum)
{
	errno_t ret;
	ret = strncpy_s(info->event, EVENT_MAX_LEN - 1, EVENT_BOOTFAIL, strlen(EVENT_BOOTFAIL));
	if (ret != EOK)
		print_err("info->event strncpy_s failed\n");

	ret = strncpy_s(info->module, MODULE_MAX_LEN - 1, MODULE_BOOTFAIL, strlen(MODULE_BOOTFAIL));
	if (ret != EOK)
		print_err("info->module strncpy_s failed\n");

	ret = strncpy_s(info->category, CATEGORY_MAX_LEN - 1, CATEGORY_SYSTEM_BOOTFAIL, strlen(CATEGORY_SYSTEM_BOOTFAIL));
	if (ret != EOK)
		print_err("info->category strncpy_s failed\n");

	ret = strncpy_s(info->log_path, ROOT_LOG_PATH_LEN - 1, CONFIG_BLACKBOX_LOG_ROOT_PATH,
				strlen(CONFIG_BLACKBOX_LOG_ROOT_PATH));
	if (ret != EOK)
		print_err("info->log_path strncpy_s failed\n");

	ret = strncpy_s((char *)info->top_category, CATEGORY_MAX_LEN - 1, stage, strlen(stage));
	if (ret != EOK)
		print_err("info->top_category strncpy_s failed\n");

	ret = strncpy_s((char *)info->error_desc, ERROR_DESC_MAX_LEN - 1, errnum, strlen(errnum));
	if (ret != EOK)
		print_err("info->error_desc strncpy_s failed\n");

	bbox_notify_error(EVENT_BOOTFAIL, MODULE_BOOTFAIL, (char *)info, false);
}

static void append_log_path(struct bootfail_log_header *bf_bootfail_info, char *logs, int space)
{
	int ret = 0;
	char *buf = logs;
	size_t i = 0;

	for (i; i < (sizeof(bf_bootfail_info->log_path) / sizeof(bf_bootfail_info->log_path[0])); i++) {
		if (strlen(bf_bootfail_info->log_path[i]) <= 0)
			continue;

		ret = snprintf_s(buf, space, strlen(bf_bootfail_info->log_path[i]), "%s\n", bf_bootfail_info->log_path[i]);
		if (ret > 0) {
			space -= ret;
			buf += ret;
		} else {
			print_err("%d append string failed", __LINE__);
		}
	}
}

static void save_boofail_info(char *bf_content, struct bootfail_log_header *bf_bootfail_info, struct error_info *info,
							char *errno, char *stage)
{
	struct rtc_time real_time;
	char *logs = NULL;

	int logPaths = sizeof(bf_bootfail_info->log_path) / sizeof(bf_bootfail_info->log_path[0]);
	int space = sizeof(bf_bootfail_info->log_path) + logPaths;

	logs = vmalloc(space);
	if (logs != NULL) {
		int ret = memset_s(logs, space, 0, space);
		if (ret == 0) {
			append_log_path(bf_bootfail_info, logs, space);
		} else {
			print_err("memset log path buffer failed!\n");
			vfree(logs);
			logs = NULL;
		}
	}

	rtc_time64_to_tm(bf_bootfail_info->binfo.rtc_time, &real_time);
	convert_orig_data(stage, errno, bf_bootfail_info->binfo.stage, bf_bootfail_info->binfo.bootfail_errno);

	int len = snprintf_s((char *)bf_content, BF_SIZE_1K, BF_SIZE_1K - 1,
				"                   Generated by HiviewDFX@HarmonyOS               \n"
				"==========================Parsed data=============================\n"
				"Time of Error Occoured: %d/%02d/%02d-%02d:%02d:%02d\nBoot Stage when Error Occoured: %s\n"
				"Fault Reason: %s\nBoot Up Time: %d seconds\nDMD Error No: %d\nIs Root: %s\n"
				"Space Left On Device: %uMB\nInodes Left On Device: %u\nError Description: %s\n"
				"\n=======================Original data=============================\n"
				"Error No: 0x%x\nBoot Stage: 0x%x\nException Type: %d\nSub Exception Type: %d\n"
				"Suggest Recovery Method: %d\nException Stage: %d\nVersion: %s\nLogPath:%s\n",
				real_time.tm_year + 1900, real_time.tm_mon + 1, real_time.tm_mday, real_time.tm_hour,
				real_time.tm_min, real_time.tm_sec, stage, errno, bf_bootfail_info->binfo.bootup_time,
				bf_bootfail_info->binfo.dmd_errno, bf_bootfail_info->binfo.is_rooted == 0 ? "False" : "True",
				bf_bootfail_info->space_left, bf_bootfail_info->inodes_left, bf_bootfail_info->detail_info,
				bf_bootfail_info->binfo.bootfail_errno, bf_bootfail_info->binfo.stage,
				bf_bootfail_info->binfo.exception_type, bf_bootfail_info->binfo.sub_exception_type,
				bf_bootfail_info->binfo.suggest_recovery_method, bf_bootfail_info->binfo.exception_stage,
				bf_bootfail_info->version, logs == NULL ? "No additional log detected" : logs);
	if (len <= 0) {
		print_err("%d append bootfail_info failed", __LINE__);
		goto end;
	}

	len = snprintf_s(info->error_time, TIMESTAMP_MAX_LEN, TIMESTAMP_MAX_LEN - 1,
			"%04d%02d%02d%02d%02d%02d-%08d", real_time.tm_year + 1900, real_time.tm_mon + 1, real_time.tm_mday,
			real_time.tm_hour, real_time.tm_min, real_time.tm_sec, bf_bootfail_info->binfo.bootup_time);
	if (len <= 0)
		print_err("%d append error_time failed", __LINE__);

end:
	if (logs != NULL) {
		vfree(logs);
		logs = NULL;
	}
}

static int write_file(const char *log_path, char *content, unsigned int size)
{
	int ret;
	struct file *fp = NULL;
	mm_segment_t fs;

	fp = filp_open(log_path, O_CREAT | O_RDWR, 0664);
	if (IS_ERR_OR_NULL(fp)) {
		print_err("BootDetector bbox_dump filp_open failed!\n");
		return -1;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);

	ret = vfs_write(fp, content, size, &fp->f_pos);
	if (ret < 0)
		print_err("BootDetector bbox_dump write is error res %d!\n", ret);

	set_fs(fs);
	filp_close(fp, NULL);

	print_info("write file logpath is %s, ret is  %d \n", log_path, ret);
	return ret;
}

static int save_log(const char *path, char *content, unsigned int start, unsigned int size)
{
	int ret = 0;
	ret = read_content(content, start, size);
	if (ret != 0) {
		print_err("read content failed !\n");
		return ret;
	}
	ret = write_file(path, content, size);
	if (ret < 0) {
		print_err("save_log: write file failed!\n");
		return ret;
	}
	return ret;
}

static bool get_file_dir(struct error_info *info, char *bf_info_path, char *bl_log_path, char *kernel_log_path)
{
	const char *bf_info = "bootfail_info";
	const char *last_kmsg = "last_kmsg";
	const char *bl_log = "bl_log";
	char log_dir[BF_SIZE_128];

	memset_s(log_dir, BF_SIZE_128, 0, BF_SIZE_128);
	memset_s(bf_info_path, BF_SIZE_128, 0, BF_SIZE_128);
	memset_s(bl_log_path, BF_SIZE_128, 0, BF_SIZE_128);
	memset_s(kernel_log_path, BF_SIZE_128, 0, BF_SIZE_128);

	int len = snprintf_s(log_dir, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s", CONFIG_BLACKBOX_LOG_ROOT_PATH, info->error_time);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return false;
	}

	create_log_dir(log_dir);
	mutex_lock(&mtx_lock);
	len = snprintf_s(bf_info_path, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s_%u", log_dir, bf_info, g_write_index);
	if (len <= 0) {
		print_err("%d append bf_info_path failed", __LINE__);
		mutex_unlock(&mtx_lock);
		return false;
	}

	len = snprintf_s(bl_log_path, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s_%u", log_dir, bl_log, g_write_index);
	if (len <= 0) {
		print_err("%d append bl_log_path failed", __LINE__);
		mutex_unlock(&mtx_lock);
		return false;
	}

	len = snprintf_s(kernel_log_path, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s_%u", log_dir, last_kmsg, g_write_index);
	if (len <= 0) {
		print_err("%d append kernel_log_path failed", __LINE__);
		mutex_unlock(&mtx_lock);
		return false;
	}

	g_write_index++;
	mutex_unlock(&mtx_lock);

	return true;
}

static const char* find_file_name(const char *log_path)
{
	int i = strlen(log_path) - 1;
	for (i; i >= 0; --i) {
		if (log_path[i] == '/') {
			if (i == (strlen(log_path) - 1)) {
				return NULL;
			} else {
				return log_path + i + 1; // add 1 to ignore '/'
			}
		}
	}
	return log_path;
}
 
static void save_prefabricated_files(struct error_info *info, struct bootfail_log_header *pparam)
{
	char log_dir[BF_SIZE_128] = {0};
	int i = 0;
	int len = snprintf_s(log_dir, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s", CONFIG_BLACKBOX_LOG_ROOT_PATH, info->error_time);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}

	print_info("save_prefabricated_files: log_dir! %s\n", log_dir);
	for (i; i < sizeof(pparam->log_path) / sizeof(pparam->log_path[0]); i++) {
		if (strlen(pparam->log_path[i]) <= 0)
			break;
 
		const char* name = find_file_name(pparam->log_path[i]);
		if (name == NULL) {
			continue;
		}

		char log_file[BF_SIZE_256] = {0};
		len = snprintf_s(log_file, BF_SIZE_256, BF_SIZE_256 - 1, "%s/%s", log_dir, name);
		if (len <= 0) {
			print_err("%d append string failed", __LINE__);
			return;
		}
		print_info("save_prefabricated_files: %s to %s \n", pparam->log_path[i], log_file);
#ifdef CONFIG_LIBLINUX
		long error = ksys_symlink(pparam->log_path[i], log_file);
		if (error != 0)
			print_err("log_symlink error:%d\n", error);
#endif
	}
}

static void save_log_to_file(struct error_info *info, struct bootfail_meta_log *bf_meta_log, unsigned int index,
							char *errno, char *stage)
{
	struct bootfail_log_header bf_log_header;

	char bf_info_path[BF_SIZE_128] = {0};
	char bl_log_path[BF_SIZE_128] = {0};
	char kernel_log_path[BF_SIZE_128] = {0};

	struct bootfail_log_content *log_content = (struct bootfail_log_content *)vmalloc(sizeof(struct bootfail_log_content));
	if (log_content == NULL) {
		print_err("bbox_dump: vmalloc memory for bootfail logContent failed!\n");
		return;
	}

	memset_s(&bf_log_header, sizeof(bf_log_header), 0, sizeof(bf_log_header));
	memset_s(log_content, sizeof(struct bootfail_log_content), 0, sizeof(struct bootfail_log_content));

	log_content->bl_log_size = bf_meta_log->logs_info[index].bl_log.size > SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE ?
			SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE : bf_meta_log->logs_info[index].bl_log.size;
	log_content->kernel_log_size = bf_meta_log->logs_info[index].kernel_log.size > LAST_KMSG_SIZE ?
			LAST_KMSG_SIZE : bf_meta_log->logs_info[index].kernel_log.size;
	log_content->bf_info_start = bf_meta_log->logs_info[index].bl_log.start - BF_SIZE_1K;

	if (log_content->bf_info_start < (sizeof(struct bfi_misc_msg) +
		sizeof(struct bootfail_meta_log) + sizeof(struct bootfail_proc_param))) {
		print_err("bootfail info start is invalid!\n");
        goto end;
	}

	int ret = read_content((char *)&bf_log_header, log_content->bf_info_start, BF_SIZE_1K);
	if (ret != 0) {
		print_err("save_to_log_file read_content bootfail_info failed !\n");
		goto end;
	}

	save_boofail_info(log_content->bf_info_content, &bf_log_header, info, errno, stage);

	bf_log_header.binfo.recovery_result = recovery_success;
	write_content((char *)&bf_log_header, log_content->bf_info_start, BF_SIZE_1K);

	if (!(get_file_dir(info, bf_info_path, bl_log_path, kernel_log_path)))
        goto end;

	ret = write_file(bf_info_path, log_content->bf_info_content, BF_SIZE_1K);
	if (ret < 0)
		print_err("save_to_log_file: write bootfail info file failed!\n");

	ret = save_log(bl_log_path, log_content->bl_log_content, bf_meta_log->logs_info[index].bl_log.start,
				log_content->bl_log_size);
	if (ret < 0)
		print_err("save_to_log_file: bl_log failed ! ret == %d\n", ret);

	ret = save_log(kernel_log_path, log_content->kernel_log_content, bf_meta_log->logs_info[index].kernel_log.start,
				log_content->kernel_log_size);
	if (ret < 0) 
		print_err("save_to_log_file: kernel_log failed ! ret == %d\n", ret);

	save_prefabricated_files(info, &bf_log_header);
end:
	vfree(log_content);
	log_content = NULL;
}

int parse_bootfail(void)
{
	struct bootfail_meta_log bf_meta_log;
	unsigned int i = 0;
	memset_s(&bf_meta_log, sizeof(struct bootfail_meta_log), 0, sizeof(struct bootfail_meta_log));
	bool ret = get_meta_log(&bf_meta_log);
	if (!ret) {
		print_err("parse_bootfail, get_meta_log faild!\n");
		return -1;
	}
	unsigned int log_count = bf_meta_log.log_count;
	if (log_count == 0) {
		print_err("parse_bootfail, not find bootfail!\n");
		return -1;
	}
	print_err("parse_bootfail!\n");

	struct error_info info;
	char stage[BF_SIZE_128] = {0};
	char errnum[BF_SIZE_128] = {0};
	
	for (i; i < log_count; ++i) {
		memset_s(&info, sizeof(struct error_info), 0, sizeof(struct error_info));
		memset_s(stage, BF_SIZE_128, 0, BF_SIZE_128);
		memset_s(errnum, BF_SIZE_128, 0, BF_SIZE_128);
		unsigned int index = bf_meta_log.log_max_count ? (bf_meta_log.log_max_count -
							(bf_meta_log.log_count - bf_meta_log.write_idx) + i) % bf_meta_log.log_max_count : 0;
		save_log_to_file(&info, &bf_meta_log, index, errnum, stage);
		msleep(100);
		send_bbox_notify((struct error_info *)&info, stage, errnum);
	}
	return 0;
}

#ifdef CONFIG_SHUT_DETECTOR
static void rtc_time_transfer(char *time, unsigned long long timestamp, struct error_info *info)
{
	struct rtc_time real_time;
	const int format_number = 1;

	rtc_time64_to_tm(timestamp, &real_time);
	int len = snprintf_s(time, TIMESTAMP_MAX_LEN, TIMESTAMP_MAX_LEN - 1, "%04d%02d%02d%02d%02d%02d-%08d",
		real_time.tm_year + 1900, real_time.tm_mon + 1, real_time.tm_mday,
		real_time.tm_hour, real_time.tm_min, real_time.tm_sec, format_number);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}

	len = snprintf_s(info->error_time, TIMESTAMP_MAX_LEN, TIMESTAMP_MAX_LEN - 1, "%s", time);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}
}

static void get_shutfail_log_path(char *bf_info_path, char *klog_path, char *time)
{
	char log_dir[BF_SIZE_128] = {0};
	const char *bf_info = "bootfail_info";
	const char *last_kmsg = "last_kmsg";
	static const unsigned int format_number = 0;

	int len = snprintf_s(log_dir, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s", CONFIG_BLACKBOX_LOG_ROOT_PATH, time);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}

	create_log_dir(log_dir);
	len = snprintf_s(bf_info_path, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s_%u", log_dir, bf_info, format_number);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}

	len = snprintf_s(klog_path, BF_SIZE_128, BF_SIZE_128 - 1, "%s/%s_%u", log_dir, last_kmsg, format_number);
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return;
	}
}

static void clear_shutfail_info()
{
	struct shutfail_log_header sf_log_header;
	memset_s(&sf_log_header, sizeof(struct shutfail_log_header), 0, sizeof(struct shutfail_log_header));
	int ret = write_part(get_sfi_dev_path(), get_sfi_dev_offset(), (const char *)&sf_log_header,
						sizeof(struct shutfail_log_header));
	if (ret != 0)
		print_err("clear_shutfail_info, clear shutfail log header failed!\n");
}

static int get_shut_header(struct shutfail_log_header *sf_log_header)
{
	memset_s((char *)sf_log_header, sizeof(struct shutfail_log_header), 0, sizeof(struct shutfail_log_header));

	int ret = read_part(get_sfi_dev_path(), get_sfi_dev_offset(),
		(char *)sf_log_header, sizeof(struct shutfail_log_header));
	if (ret != 0) {
		print_err("read log from bootfail_info partition failed!\n");
		return -1;
	}

	if (sf_log_header->kernel_log_size <= 0) {
		print_err("invalid shutfail kernel log size!\n");
		return -1;
	}
	return 0;
}

static int save_shutfail_info(struct shutfail_log_header *sf_log_header, struct shutfail_info *sinfo,
			char *bf_info_content, char *bf_info_path)
{
	int len = snprintf_s(bf_info_content, BF_SIZE_256, BF_SIZE_256 - 1, "error desc:%s\nSHUT_FAIL_TIME: %s\n"
			"SHUT_FAIL_STAGENO: %d\nSHUT_FAIL_STAGE:%s\n", (char *)(sinfo->errnum), (char *)(sinfo->time),
			sf_log_header->sinfo.stage, (char *)(sinfo->stage));
	if (len <= 0) {
		print_err("%d append string failed", __LINE__);
		return -1;
	}

	int ret = write_file(bf_info_path, bf_info_content, len);
	if (ret < 0) {
		print_err("parse_shutfail: write bootfail info file failed!\n");
        return -1;
    }

    return 0;
}

static int save_shutfail_kmsg(struct shutfail_log_header *sf_log_header, char *klog_path)
{
    int kmsg_size = min(BF_SIZE_512K, sf_log_header->kernel_log_size);

    char *klog = vmalloc(kmsg_size);
	if (klog == NULL) {
		print_err("malloc shutfail klog buffer failed!\n");
		return -1;
	}

	int ret = read_part(get_sfi_dev_path(), get_sfi_dev_offset() + sizeof(struct shutfail_log_header),
					klog, kmsg_size);
	if (ret != 0) {
		print_err("read part to get shutfail log failed!\n");
		vfree(klog);
		return -1;
	}

    ret = write_file(klog_path, klog, kmsg_size);
	if (ret < 0) {
		print_err("parse_shutfail: write klog file failed!\n");
		vfree(klog);
        return -1;
    }

    vfree(klog);
    return 0;
}


static int save_logs(struct shutfail_log_header *sf_log_header, struct shutfail_info *sinfo)
{
	char bf_info_content[BF_SIZE_256] = {0};
    char bf_info_path[BF_SIZE_128] = {0};
	char klog_path[BF_SIZE_128] = {0};

    get_shutfail_log_path(bf_info_path, klog_path, (char *)sinfo->time);

    int ret = save_shutfail_info(sf_log_header, sinfo, bf_info_content, bf_info_path);
    if (ret != 0) {
        print_err("shutfail save shutfail info failed!\n");
        return -1;
    }

    ret = save_shutfail_kmsg(sf_log_header, klog_path);
    if (ret != 0) {
        print_err("shutfail save shutfail kmsg failed!\n");
        return -1;
    }

    return 0;
}

int parse_shutfail()
{
	struct shutfail_log_header *sf_log_header = (struct shutfail_log_header *)vmalloc(sizeof(struct shutfail_log_header));
	if (sf_log_header == NULL) {
		print_err("vmalloc sf_log_header failed!\n");
		return -1;
	}

	memset_s((char *)sf_log_header, sizeof(struct shutfail_log_header), 0, sizeof(struct shutfail_log_header));

	int ret = get_shut_header(sf_log_header);
	if (ret != 0) {
		print_err("parse_shutfail get_shut_header failed");
		vfree(sf_log_header);
		return -1;
	}

	struct error_info *info = (struct error_info *)vmalloc(sizeof(struct error_info));
	if (info == NULL) {
		print_err("vmalloc info failed!\n");
        vfree(sf_log_header);
		return -1;
	}
 
	struct shutfail_info *sinfo = (struct shutfail_info *)vmalloc(sizeof(struct shutfail_info));
	if (sinfo == NULL) {
		print_err("vmalloc sinfo failed!\n");
        vfree(sf_log_header);
		vfree(info);
		return -1;
	}

    memset_s((char *)info, sizeof(struct error_info), 0, sizeof(struct error_info));
    memset_s((char *)sinfo, sizeof(struct shutfail_info), 0, sizeof(struct shutfail_info));

	rtc_time_transfer((char *)sinfo->time, sf_log_header->sinfo.rtc_time, info);
    convert_orig_data((char *)(sinfo->stage), (char *)(sinfo->errnum), sf_log_header->sinfo.stage, SHUT_FAIL_ERRNO);

    ret = save_logs(sf_log_header, sinfo);
    if (ret != 0) {
        print_err("save log failed!\n");
    }

    send_bbox_notify((struct error_info *)&info, (char *)(sinfo->stage), (char *)(sinfo->errnum));

	vfree(sinfo);
	vfree(info);
	vfree(sf_log_header);
    return 0;
}
#endif

void clear_info(void)
{
	clear_bfi_misc();
	clean_bootfail_info();
#ifdef CONFIG_SHUT_DETECTOR
	clear_shutfail_info();
#endif
}

int boot_completed()
{
	int ret = 0;
#ifdef CONFIG_SHUT_DETECTOR
	enum shut_stage last_shut_stage = SHUT_STAGE_INVALID;

	(void)get_shut_stage(&last_shut_stage);
	if ((last_shut_stage != SHUT_STAGE_INVALID) && (last_shut_stage != SHUT_STAGE_SUCC)) {
		print_info("start to parse shutfail!\n");
		ret = parse_shutfail();
		if (ret < 0)
			print_err("parse shutfail failed!\n");
	}
#endif
	ret = parse_bootfail();
	if (ret < 0)
		print_err("parse bootfail failed!\n");

	return 0;
}
