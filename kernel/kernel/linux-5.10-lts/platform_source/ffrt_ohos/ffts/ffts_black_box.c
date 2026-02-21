#include "ffts_black_box.h"
#include <linux/mutex.h>
#include <securec.h>
#ifdef FFTS_DUMP_BBOX
#include <linux/fs.h>
#endif
#include <liblinux/pal.h>
#include "sd_proxy.h"
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include "mntn_public_interface.h"
#include "mntn_subtype_exception.h"
#include "ffts_kernel_log.h"
#include "ffts_proc_block.h"

struct ffts_mntn_info_s {
	struct mutex rdr_mutex;
	unsigned int bbox_addr_offset;
	void *rdr_addr;
	struct rdr_register_module_result ffts_ret_info;
};

static struct rdr_exception_info_s ffts_rdr_excetption_info[] = {
	{
		.e_modid = (u32)EXC_TYPE_BLOCK_PROC_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_BLOCK_PROC_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_FFTS,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_FFTS,
		.e_exce_subtype = BLOCKPROC_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "PROCBLOCK_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_BLOCK_GPU_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_BLOCK_GPU_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_FFTS,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_FFTS,
		.e_exce_subtype = BLOCKGPU_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "GPUBLOCK_TIMEOUT",
	},
	{
		.e_modid = (u32)EXC_TYPE_BLOCK_NPU_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_BLOCK_NPU_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_FFTS,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_FFTS,
		.e_exce_subtype = BLOCKNPU_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "NPUBLOCK_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_CMO_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_CMO_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_AP,
		.e_reboot_priority  = RDR_REBOOT_NOW,
		.e_notify_core_mask = RDR_FFTS | RDR_AP,
		.e_exce_subtype = CMO_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "CMO_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_RETRY_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_RETRY_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_FFTS,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_FFTS,
		.e_exce_subtype = RETRY_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "RETRY_EXCP",
	},
	{
		.e_modid = (u32)EXC_TYPE_ABNORMAL_EXCEPTION,
		.e_modid_end = (u32)EXC_TYPE_ABNORMAL_EXCEPTION,
		.e_exce_type = FFTS_S_EXCEPTION,
		.e_reentrant = (u32)RDR_REENTRANT_DISALLOW,
		.e_from_core = RDR_FFTS,
		.e_process_priority = RDR_ERR,
		.e_reset_core_mask  = RDR_FFTS,
		.e_reboot_priority  = RDR_REBOOT_NO,
		.e_notify_core_mask = RDR_FFTS,
		.e_exce_subtype = ABNORMAL_EXCP,
		.e_upload_flag = (u32)RDR_UPLOAD_YES,
		.e_save_log_flags = (u32)(RDR_SAVE_BL31_LOG | RDR_SAVE_LOGBUF | RDR_SAVE_DMESG | RDR_SAVE_CONSOLE_MSG),
		.e_from_module = "FFTS",
		.e_desc = "ABNORMAL_EXCP",
	},
};

static struct ffts_mntn_info_s g_ffts_mntn_info = {0};

static struct ffts_rdr_exception_info g_rdr_exception_info[] = {
	/* exception code 0xc0007000-0xc00070ff */
	{EXC_TYPE_BLOCK_PROC_EXCEPTION, 1, 0},
	{EXC_TYPE_BLOCK_GPU_EXCEPTION, 1, 0},
	{EXC_TYPE_BLOCK_NPU_EXCEPTION, 1, 0},
	{EXC_TYPE_CMO_EXCEPTION, 1, 0},
	{EXC_TYPE_RETRY_EXCEPTION, 1, 0},
};

static void ffts_rdr_exception_reset(void)
{
	u32 i = 0;

	for (; i < ARRAY_SIZE(g_rdr_exception_info); i++)
		g_rdr_exception_info[i].count = 0;
}

/*
 * Description : ffts_rdr_register_exception
 * input : void
 * output : NA
 * rdr_register_exception : ret == 0 is fail. ret > 0 is success.
 */
static int ffts_rdr_register_exception(void)
{
	size_t size;
	unsigned int ret;
	unsigned long index;

	size = sizeof(ffts_rdr_excetption_info) /
		sizeof(ffts_rdr_excetption_info[0]);
	for (index = 0; index < size; index++) {
		/* error return 0, ok return modid */
		ret = rdr_register_exception(&ffts_rdr_excetption_info[index]);
		if (ret == 0) {
			ffts_drv_err("register rdr exception is failed! index = %lu, ret = %u",
				index, ret);
			return -EINTR;
		}
	}
	return 0;
}

static void ffts_dump_info(u32 modid, u32 etype, u64 coreid)
{
	int ret = 0;
	struct ffts_rdr_dump_info rdr_dump_info  = { 0 };

	ffts_drv_warn("ffts_dump_info start\n");
	rdr_dump_info.modid = modid;
	rdr_dump_info.etype = etype;
	rdr_dump_info.coreid = coreid;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_DUMP_INFO), &rdr_dump_info, sizeof(struct ffts_rdr_dump_info), NULL, 0);
	if (ret != 0) {
		ffts_drv_err("ioctl ffts_dump_info failed, ret = %d\n", ret);
	}
	return;
}

/*
 * Description: ffts_mntn_copy_reg_to_bbox
 * input: const char *src_addr, unsigned int* offset, unsigned int len
 * output: NA
 * return: void
 */
int ffts_mntn_copy_reg_to_bbox(const char *src_addr,
	unsigned int len)
{
	unsigned int rdr_left_len;
	int ret;

	if (g_ffts_mntn_info.rdr_addr == NULL) {
		ffts_drv_err("rdr addr is NULL!\n");
		return -EINVAL;
	}

	if ((src_addr == NULL) || (len == 0)) {
		ffts_drv_err("input parameter is error!\n");
		return -EINVAL;
	}

	mutex_lock(&g_ffts_mntn_info.rdr_mutex);
	if (g_ffts_mntn_info.bbox_addr_offset >
		g_ffts_mntn_info.ffts_ret_info.log_len) {
		mutex_unlock(&g_ffts_mntn_info.rdr_mutex);
		ffts_drv_err("bbox_addr_offset is error!\n");
		return -EINVAL;
	}

	rdr_left_len = g_ffts_mntn_info.ffts_ret_info.log_len -
		g_ffts_mntn_info.bbox_addr_offset;

	if (len > rdr_left_len) {
		ffts_drv_warn("bbox buf is full, turn to the start to reuse, "
			"len=%u, rdr_left_len=%u\n", len, rdr_left_len);
		g_ffts_mntn_info.bbox_addr_offset = 0;
		rdr_left_len = g_ffts_mntn_info.ffts_ret_info.log_len;
	}

	if (len > rdr_left_len) {
		ffts_drv_warn("message is too long to write into bbox buffer, "
			"len=%u, rdr_left_len=%u\n", len, rdr_left_len);
		len = rdr_left_len;
	}
	ret = memcpy_s(((char *)g_ffts_mntn_info.rdr_addr +
		g_ffts_mntn_info.bbox_addr_offset), len, src_addr, len);
	if (ret != 0) {
		mutex_unlock(&g_ffts_mntn_info.rdr_mutex);
		ffts_drv_err("memcpy_s failed, ret=%d\n", ret);
		return -EINVAL;
	}

	g_ffts_mntn_info.bbox_addr_offset += len;
	mutex_unlock(&g_ffts_mntn_info.rdr_mutex);

	return 0;
}

/*
 * Description: ffts_mntn_rdr_dump
 * input: modid: module id
 *        etype:exception type
 *        coreid: core id
 *        pathname: log path
 *        pfn_cb: callback function
 * output: NA
 * return: NA
 */
static void ffts_mntn_rdr_dump(u32 modid, u32 etype, u64 coreid,
	char *pathname, pfn_cb_dump_done pfn_cb)
{
	int ret = 0;
	ffts_drv_warn("modid=0x%x, etype=0x%x, coreid=0x%llx",
		modid, etype, coreid);
	if (pathname == NULL) {
		ffts_drv_err("pathname is empty!");
		return;
	}

	ffts_dump_info(modid, etype, coreid);

	if (pfn_cb != NULL)
		pfn_cb(modid, coreid);
}

/*
 * Description: ffts_mntn_rdr_reset
 * input: modid:module id
 *        etype:exception type
 *        coreid:core id
 * output: NA
 * return: NA
 */
static void ffts_mntn_rdr_reset(u32 modid, u32 etype, u64 coreid)
{
	ffts_drv_warn("modid=0x%x, etype=0x%x, coreid=0x%llx",
		modid, etype, coreid);
}

/*
 * Description : ffts_rdr_addr_map
 * input : void
 * output : NA
 * return : int:  0 is OK. other value is Error No.
 */
static int ffts_rdr_addr_map(void)
{
	int ret;
	g_ffts_mntn_info.rdr_addr =
		dfx_bbox_map((phys_addr_t)g_ffts_mntn_info.ffts_ret_info.log_addr,
		g_ffts_mntn_info.ffts_ret_info.log_len);
	if (g_ffts_mntn_info.rdr_addr == NULL) {
		ffts_drv_err("map dfx bbox is failed!");
		return -EFAULT;
	}
	ret = memset_s((char *)g_ffts_mntn_info.rdr_addr,
		g_ffts_mntn_info.ffts_ret_info.log_len, 0,
		g_ffts_mntn_info.ffts_ret_info.log_len);
	if (ret != 0) {
		ffts_drv_err("memset_s failed. ret=%d\n", ret);
		dfx_bbox_unmap(g_ffts_mntn_info.rdr_addr);
		g_ffts_mntn_info.rdr_addr = NULL;
		return -EINVAL;
	}
	g_ffts_mntn_info.bbox_addr_offset = 0;
	mutex_init(&g_ffts_mntn_info.rdr_mutex);

	return 0;
}

/*
 * Description : ffts_rdr_addr_unmap
 * input : void
 * output : NA
 * return : void
 */
static void ffts_rdr_addr_unmap(void)
{
	dfx_bbox_unmap(g_ffts_mntn_info.rdr_addr);
	g_ffts_mntn_info.rdr_addr = NULL;
}

/*
 * Description : ffts_rdr_register_core
 * input : void
 * output : NA
 * rdr_register_module_ops : ret <0 is fail. ret >= 0 is success.
 */
static int ffts_rdr_register_core(void)
{
	int ret;
	struct rdr_module_ops_pub s_soc_ops;

	s_soc_ops.ops_dump = ffts_mntn_rdr_dump;
	s_soc_ops.ops_reset = ffts_mntn_rdr_reset;
	/* register npu core dump and reset function */
	ret = rdr_register_module_ops((u64)RDR_FFTS, &s_soc_ops,
		&g_ffts_mntn_info.ffts_ret_info);
	if (ret < 0) {
		ffts_drv_err("register rdr module_ops is failed! ret = 0x%08x", ret);
		return ret;
	}

	ret = ffts_rdr_addr_map();
	if (ret < 0) {
		(void)rdr_unregister_module_ops((u64)RDR_FFTS);
		ffts_drv_err("rdr addr map failed! ret = %d", ret);
		return ret;
	}

	return ret;
}

/*
 * Description : ffts_rdr_unregister_core
 * input : void
 * output : NA
 * rdr_unregister_module_ops : ret < 0 is fail. ret >= 0 is success.
 */
static int ffts_rdr_unregister_core(void)
{
	int ret;

	ffts_rdr_addr_unmap();

	/* unregister ffts core dump and reset function */
	ret = rdr_unregister_module_ops((u64)RDR_FFTS);
	if (ret < 0)
		ffts_drv_err("unregister rdr module_ops is failed! ret = %d", ret);
	g_ffts_mntn_info.ffts_ret_info.log_addr = 0;
	g_ffts_mntn_info.ffts_ret_info.log_len = 0;

	return ret;
}

/*
 * Description : ffts_rdr_unregister_exception
 * input : void
 * output : NA
 * rdr_unregister_exception : ret < 0 is fail. ret >= 0 success.
 */
static int ffts_rdr_unregister_exception(void)
{
	int ret;
	size_t size;
	unsigned long index;

	size = sizeof(ffts_rdr_excetption_info) /
		sizeof(ffts_rdr_excetption_info[0]);
	for (index = 0; index < size; index++) {
		/* ret < 0 is fail, ret >= 0 is success */
		ret = rdr_unregister_exception(
			ffts_rdr_excetption_info[index].e_modid);
		if (ret < 0) {
			ffts_drv_err("unregister rdr exception is failed! index = %lu, ret = %d",
				index, ret);
			return -EINTR;
		}
	}
	return 0;
}

/*
 * Description : ffts_rdr_exception_is_count_exceeding
 * input : u32
 * output : NA
 * return : u32: 0 means not exceeding limitation and allowed to report,
 *          otherwise not allowed.
 */
static u32 ffts_rdr_exception_is_count_exceeding(u32 error_code)
{
	u32 i;

	if (error_code == EXC_TYPE_ABNORMAL_EXCEPTION)
		return 0;

	for (i = 0; i < ARRAY_SIZE(g_rdr_exception_info); i++)
		if (g_rdr_exception_info[i].error_code == error_code) {
			if (g_rdr_exception_info[i].count >=
				g_rdr_exception_info[i].limitation) {
				ffts_drv_warn("error code %u exceeds the limitation %u",
					error_code, g_rdr_exception_info[i].limitation);
				return 1;
			}
			g_rdr_exception_info[i].count++;
			ffts_drv_debug("error code %u occurs %u time(s)",
				error_code, g_rdr_exception_info[i].count);
			return 0;
		}

	ffts_drv_warn("error code %u is undefined", error_code);

	return 1;
}

static void ffts_dump_to_file(char* file_name, void *addr, size_t size)
{
	struct file *file;
	mm_segment_t old_fs;
	loff_t pos = 0;
 
	if((file_name == NULL ) || (addr == NULL)){
		ffts_drv_err("invaild file name or addr\n");
		return;
	}
	ffts_drv_warn("start dump %s \n", file_name);

	file = filp_open(file_name, O_CREAT | O_RDWR, 660);
	if (IS_ERR(file)) {
		ffts_drv_err("Failed to open file %s,error %d\n", file_name, PTR_ERR(file));
		return;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	vfs_write(file, addr, size, &pos);
	set_fs(old_fs);

	filp_close(file, NULL);
	ffts_drv_info("dump %s success\n", file_name);
	return;
}

void ffts_rdr_exception_report(uint32_t error_code)
{
	if (ffts_rdr_exception_is_count_exceeding(error_code) == 0) {
#ifdef FFTS_DUMP_BBOX
		ffts_drv_err("call ffts dump, error_code: 0x%x\n", error_code);
		u32 etype = 0;
		u64 coreid = 0;
		for (int i = 0; i < ARRAY_SIZE(ffts_rdr_excetption_info); i++){
			if (ffts_rdr_excetption_info[i].e_modid == error_code) {
				etype = ffts_rdr_excetption_info[i].e_exce_type;
				coreid = ffts_rdr_excetption_info[i].e_from_core;
				break;
			}
		}

		ffts_dump_info(error_code, etype, coreid);
		// output to file
		ffts_dump_to_file("/data/ffts_bbox.bin", g_ffts_mntn_info.rdr_addr, g_ffts_mntn_info.bbox_addr_offset, 0);
#else 
		ffts_drv_err("call rdr_system_error, error_code: 0x%x\n", error_code);
		rdr_system_error(error_code, 0, 0);
#endif
	}
}

int ffts_mntn_copy_udk_info_to_bbox(void *rd, size_t rd_size)
{
	int ret = 0;
	unsigned long bytes_remain = 0;
	char log_buf[FFTS_BUF_LEN_MAX + 1] = {0};
	ffts_drv_debug("Start\n");
	if (rd == NULL || rd_size > FFTS_BUF_LEN_MAX) {
		ffts_drv_err("ffts_mntn_copy_udk_info_to_bbox arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}
	bytes_remain = copy_from_user((void*)log_buf, (void __user *)(uintptr_t)rd, rd_size);
	cond_return_error(bytes_remain != 0, -EFAULT, "fail to copy log_buf, bytes_remain = %lu\n", bytes_remain);
	ret = ffts_mntn_copy_reg_to_bbox(log_buf, (unsigned int)strlen(log_buf));
	return ret;
}

int ffts_udk_rdr_exception_report(void *rd, size_t rd_size)
{
	unsigned long bytes_remain = 0;
	uint32_t error_code = 0;
	ffts_drv_debug("Start\n");
	if (rd == NULL || rd_size != sizeof(uint32_t)) {
		ffts_drv_err("ffts_udk_rdr_exception_report arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}
    bytes_remain = copy_from_user(&error_code, (void __user *)(uintptr_t)rd,
		sizeof(uint32_t));
	cond_return_error(bytes_remain != 0, -EFAULT, "fail to copy ffts_rdr_exception_report, bytes_remain = %lu\n",
		bytes_remain);
	ffts_rdr_exception_report(error_code);
	return 0;
}

int ffts_udk_invoke_hw_release(void *rd, size_t rd_size)
{
	unsigned long bytes_remain = 0;
	struct ffts_proc_rel_regis_info proc_rel_regis_info = { 0 };
	ffts_drv_debug("Start\n");
	if (rd == NULL || rd_size != sizeof(proc_rel_regis_info)) {
		ffts_drv_err("ffts_udk_invoke_hw_release arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}
    bytes_remain = copy_from_user(&proc_rel_regis_info, (void __user *)(uintptr_t)rd,
		sizeof(struct ffts_proc_rel_regis_info));
	cond_return_error(bytes_remain != 0, -EFAULT, "fail to copy ffts_udk_invoke_hw_release, bytes_remain = %lu\n",
		bytes_remain);
	ffts_drv_debug("invoke hw release Start hw_type = %d tgid = %d\n", proc_rel_regis_info.hw_type, proc_rel_regis_info.tgid);

	if (proc_rel_regis_info.callback == NULL) {
		ffts_drv_err("callback is NULL\n");
		return -1;
	}

	proc_rel_regis_info.callback(proc_rel_regis_info.data, proc_rel_regis_info.tgid);
	return 0;
}

/*
 * Description : ffts_black_box_init
 * input : void
 * output : NA
 * return : int:  0 is OK. other value is Error No.
 */
int ffts_black_box_init(void)
{
	int ret;

	ffts_drv_info("Enter\n");

	/* register FFTS exception */
	ret = ffts_rdr_register_exception();
	cond_return_error(ret < 0, ret,
		"register rdr exception is faild! ret = %d", ret);

	ret = ffts_rdr_register_core();
	if (ret < 0) {
		ffts_drv_err("register rdr core is failed! ret = %d", ret);
		ffts_rdr_unregister_exception();
		return ret;
	}

	/* reset exception info */
	ffts_rdr_exception_reset();

	return 0;
}

/*
 * Description : ffts_black_box_exit
 * input : void
 * output : NA
 * return : int:  0 is OK. other value is Error No.
 */
int ffts_black_box_exit(void)
{
	int ret;

	ret = ffts_rdr_unregister_core();
	if (ret < 0)
		ffts_drv_err("unregister rdr core is faild! ret = %d", ret);

	ret = ffts_rdr_unregister_exception();
	if (ret < 0)
		ffts_drv_err("unregister rdr exception is faild! ret = %d", ret);

	return 0;
}