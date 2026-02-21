

#include <linux/vmalloc.h>
#include <linux/blackbox_common.h>
#include <linux/blackbox_storage.h>

#include <linux/rtc.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/zlib.h>
#include <linux/kernel.h>
#include <crypto/hash.h>
#include <crypto/sha.h>

#include <hwbootfail/chipsets/common/bootfail_common.h>
#include <hwbootfail/chipsets/common/bootfail_timer.h>
#include <hwbootfail/chipsets/common/bootfail_chipsets.h>
#include <hwbootfail/core/boot_detector.h>
#include <linux/blackbox.h>
#include <linux/blackbox_common.h>
#include <linux/blackbox_storage.h>
#include <securec.h>

#define SECONDS_PER_MINUTE 60
#define SHA256_SIZE 32
#define LOG_MAX_COUNT 50
#define DEV_PATH_SIZE 128
#define BF_SIZE_16 16
#define BF_SIZE_1M (BF_SIZE_1K * BF_SIZE_1K)
#define KMSG_BUFF (BF_SIZE_1M * 20)
#define LOG_OFFSET 0x300
#define TIMEOUT_VALUE (1000 * 60 * 1)

struct save_logs_to_mem {
	unsigned int kmsg_idx;
	char kmsg[KMSG_BUFF];
} __packed;

struct save_logs_param {
	unsigned long long rtc_time;
	unsigned int bootup_time;
	unsigned int stage;
	struct save_logs_to_mem mem_param;
} __packed;

struct save_logs_param* g_log_param = NULL;
static DEFINE_SEMAPHORE(g_log_param_sem);

static void save_kmsg_to_mem(struct save_logs_to_mem* logs_to_mem, int count)
{
	char* pbuf = NULL;
	unsigned int buf_size = 0;
	print_info("bootfail save_kmsg_to_mem called!\n");
	if (logs_to_mem == NULL) {
		print_err("bootfail logs_to_mem == NULL!\n");
		return;
	}

	if (logs_to_mem->kmsg_idx >= sizeof(logs_to_mem->kmsg)) {
		print_err("bootfail kmsg_idx >= kmsg!\n");
		return;
	}

	if (count == 0) {
		buf_size = BF_SIZE_1M * 5; // 5M
	} else {
		buf_size = BF_SIZE_512K;
	}

	if (logs_to_mem->kmsg_idx + buf_size > sizeof(logs_to_mem->kmsg)) {
		buf_size = sizeof(logs_to_mem->kmsg) - logs_to_mem->kmsg_idx;
	}

	if (buf_size <= 1) { // 1 means end symbol
		print_err("bootfail bufsize is too small\n");
		return;
	}

	print_info("bootfail buf_size is %u!\n", buf_size);
	pbuf = (char*)vzalloc(buf_size);
	if (pbuf == NULL) {
		print_err("bootfail pbuf vzalloc failed!\n");
		return;
	}

	get_kernel_log(pbuf, buf_size);
	if (buf_size <= 1) { // 1 means end symbol
		print_err("bootfail bufsize is too small\n");
		vfree(pbuf);
		return;
	}

	pbuf[buf_size - 1] = '\n';
	int ret = memcpy_s(&logs_to_mem->kmsg[logs_to_mem->kmsg_idx],
		sizeof(logs_to_mem->kmsg) - logs_to_mem->kmsg_idx, pbuf, buf_size);
	if (ret != 0) {
		print_err("pbuf -> kmsg memcpy_s failed %d!\n", ret);
	}
	logs_to_mem->kmsg_idx += buf_size;
	vfree(pbuf);
	print_info("bootfail logs_to_mem->kmsg_idx %u!\n", logs_to_mem->kmsg_idx);
}

static int deflate_compress(const char *src, size_t slen, char **dst, unsigned int *dlen)
{
	int error;
	struct z_stream_s strm;
	void *stgbuf, *dstbuf;
	size_t stglen = deflateBound(slen);

	memset_s((void *)&strm, sizeof(strm), 0, sizeof(strm));
 
	if (stglen < slen)
		return -EFBIG;
 
	strm.workspace = vzalloc(zlib_deflate_workspacesize(MAX_WBITS,
							     MAX_MEM_LEVEL));
	if (!strm.workspace)
		return -ENOMEM;
 
	error = zlib_deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (error != Z_OK) {
		error = -ENOMEM;
		goto fail_deflate_init;
	}
 
	stgbuf = vzalloc(stglen);
	if (!stgbuf) {
		error = -ENOMEM;
		goto fail_deflate_init;
	}
 
	strm.next_in = src;
	strm.avail_in = slen;
	strm.next_out = stgbuf;
	strm.avail_out = stglen;
 
	error = zlib_deflate(&strm, Z_FINISH);
	if (error != Z_STREAM_END) {
		error = -EINVAL;
		goto fail_deflate;
	}
	error = 0;
 
	dstbuf = vzalloc(strm.total_out);
	if (dstbuf) {
		int ret = memcpy_s(dstbuf, strm.total_out, stgbuf, min(stglen, strm.total_out));
		if (ret != 0) {
			print_err("dstbuf memcpy_s failed %d!\n", ret);
		}
	}
	*dst = dstbuf;
	*dlen = strm.total_out;

	zlib_deflateEnd(&strm);
fail_deflate:
	vfree(stgbuf);
fail_deflate_init:
	vfree(strm.workspace);
	return error;
}

static bool create_log_path(char log_path_tmp[BF_SIZE_128])
{
	print_info("bootfail create_log_path!\n");
	char timestamp[BF_SIZE_64] = {0};
	const char save_log_path[] = "/log/reliability/boot_fail/log";

	if (create_log_dir(save_log_path) != 0)
		return false;

	get_timestamp(timestamp, BF_SIZE_64);
	int ret = scnprintf(log_path_tmp, BF_SIZE_128 - 1,
		"%s/%s", save_log_path, timestamp);
	if (ret < 0 || ret >= BF_SIZE_128 - 1) {
		return false;
	}
	if (create_log_dir(log_path_tmp) == 0) {
		return true;
	}

	return false;
}

static int write_file(const char *log_path, char *content, unsigned int size)
{
	int ret;
	struct file *fp = NULL;
 
	fp = filp_open(log_path, O_CREAT | O_RDWR, 0664);
	if (IS_ERR_OR_NULL(fp)) {
		print_err("BootDetector bbox_dump filp_open failed!\n");
		return -1;
	}
 
	ret = vfs_write(fp, content, size, &fp->f_pos);
	if (ret < 0)
		print_err("BootDetector bbox_dump write is error res %d!\n", ret);
 
	filp_close(fp, NULL);
	return ret;
}

static bool save_kmsg_to_log(char* log_path, char *log_file)
{
	print_info("bootfail save_kmsg_to_log!\n");
	if (g_log_param == NULL) {
		print_info("bootfail g_log_param == NULL!\n");
		return false;
	}
 
	struct save_logs_to_mem* mem_param = &(g_log_param->mem_param);
	if (mem_param == NULL) {
		print_info("bootfail mem_param == NULL!\n");
		return false;
	}
 
	char* dst = NULL;
	unsigned int dlen = 0;
	int error = deflate_compress(mem_param->kmsg, mem_param->kmsg_idx, &dst, &dlen);
	if (error) {
		print_err("bootfail deflate_compress error: %d!\n", error);
		return false;
	}
	if (dst == NULL) {
		print_err("bootfail deflate_compress dst = NULL!\n");
		return false;
	}

	char timestamp[BF_SIZE_64] = {0};
	get_timestamp(timestamp, BF_SIZE_64);
	int ret = scnprintf(log_file, BF_SIZE_256 - 1,
		"%s/log_%s.gz", log_path, timestamp);
	if (ret < 0 || ret >= BF_SIZE_256 - 1) {
		return false;
	}
	write_file(log_file, dst, dlen);
	vfree(dst);
	return true;
}

static void write_kmsg_log(const char* log_temp_path)
{
	struct save_logs_to_mem* mem_param = NULL;
	down(&g_log_param_sem);
	if (g_log_param != NULL)
		mem_param = &(g_log_param->mem_param);

	while ((mem_param != NULL) && mem_param->kmsg_idx > 0) {
		print_info("save_kmsg_to_templog mem_param->kmsg_idx > 0.");
		char timestamp[BF_SIZE_64] = {0};
		char log_file[BF_SIZE_256] = {0};
		get_timestamp(timestamp, BF_SIZE_64);
		int ret = scnprintf(log_file, BF_SIZE_256 - 1,
			"%s/kmsgs_%s.log", log_temp_path, timestamp);
		if (ret < 0 || ret >= BF_SIZE_256 - 1) {
			print_err("save_kmsg_to_templog kmsgs log_file filed\n");
			break;
		}
		write_file(log_file, mem_param->kmsg, mem_param->kmsg_idx);
		mem_param->kmsg_idx = 0;
		memset_s((void *)mem_param->kmsg, sizeof(mem_param->kmsg), 0, sizeof(mem_param->kmsg));
	}
	up(&g_log_param_sem);
}

void save_kmsg_to_templog(const char* log_temp_path, int count)
{
	unsigned int buf_size = 0;
	const unsigned int max_save_size = 6;

	write_kmsg_log(log_temp_path);

	if (count == 0) {
		buf_size = BF_SIZE_1M * 4 + BF_SIZE_512K; // 4.5M
	} else if (count > max_save_size) {
		print_info("kmsgs are not saved for more than 6 minutes.");
		return;
	} else {
		buf_size = BF_SIZE_512K;
	}

	char* pbuf = NULL;
	print_info("bootfail buf_size is %u!\n", buf_size);
	pbuf = (char*)vzalloc(buf_size);
	if (pbuf == NULL) {
		print_err("bootfail pbuf vzalloc failed!\n");
		return;
	}

	get_kernel_log(pbuf, buf_size);
	if (buf_size <= 1) { // 1 means end symbol
		print_err("bootfail bufsize is too small\n");
		vfree(pbuf);
		return;
	}

	pbuf[buf_size - 1] = '\n';

	char timestamp[BF_SIZE_64] = {0};
	char log_file[BF_SIZE_256] = {0};
	get_timestamp(timestamp, BF_SIZE_64);
	int ret = scnprintf(log_file, BF_SIZE_256 - 1,
		"%s/kmsg_%s.log", log_temp_path, timestamp);
	if (ret < 0 || ret >= BF_SIZE_256 - 1) {
		print_err("save_kmsg_to_templog log_file filed\n");
		vfree(pbuf);
		return;
	}
	write_file(log_file, pbuf, buf_size);
	vfree(pbuf);
}

static int log_init(void)
{
	enum boot_stage stage = INVALID_STAGE;
	print_info("bootfail log_init!\n");
	down(&g_log_param_sem);
	if (g_log_param != NULL) {
		up(&g_log_param_sem);
		return 0;
	}
	g_log_param = vzalloc(sizeof(struct save_logs_param));
	if (g_log_param == NULL) {
		print_err("bootfail g_log_param vzalloc failed!\n");
		up(&g_log_param_sem);
		return -ENOMEM;
	}

	g_log_param->rtc_time = get_sys_rtc_time();
	g_log_param->bootup_time = get_bootup_time();
	int error_code = get_boot_stage(&stage);
	if (error_code != BF_OK) {
		print_err("bootfail get_boot_stage ret: %d!\n", error_code);
		stage = INVALID_STAGE;
	}
	g_log_param->stage = stage;
	print_info("bootfail log_init: rtc_time:%llu, bootup_time:%u, stage:%d!\n",
		g_log_param->rtc_time, g_log_param->bootup_time, g_log_param->stage);
	up(&g_log_param_sem);
	return 0;
}

void bootfail_callback(unsigned int state, char* ret_path, int len)
{
	static int count = 0;
	int ret = 0;
	print_info("bootfail callback!\n");

	char* log_temp_path = boot_detector_wake_up(state, count, TIMEOUT_VALUE);
	print_info("bootfail log_temp_path %s!\n", log_temp_path);
	if (strlen(log_temp_path) <= 0 && log_init() != 0) {
		return;
	}

	if (state == TIMING_SAVE_LOG) {
	    count++;
		print_info("bootfail state == TIMING_SAVE_LOG!\n");
		if (strlen(log_temp_path) <= 0) {
			down(&g_log_param_sem);
			if (g_log_param != NULL) {
				save_kmsg_to_mem(&g_log_param->mem_param, count);
			}
			up(&g_log_param_sem);
		}
	} else if (state == FAULT_SAVE_LOG) {
		char log_path[BF_SIZE_128] = {0};
		char log_file[BF_SIZE_256] = {0};
		print_info("bootfail state == FAULT_SAVE_LOG!\n");
		down(&g_log_param_sem);
		if ((g_log_param != NULL) && (g_log_param->mem_param.kmsg_idx > 0) && create_log_path(log_path)) {
			save_kmsg_to_log(log_path, log_file);
		}
		up(&g_log_param_sem);
		char* log_navite_path = boot_detector_wait_completed(6000); // 6s
		print_info("kmsg_log file is :%s!\n", log_file);
		print_info("navite_log path:%s!\n", log_navite_path);
		if (strlen(log_navite_path) > 0) {
			ret = scnprintf(ret_path, len - 1, "%s", log_navite_path);
		} else if (strlen(log_file) > 0) {
			ret = scnprintf(ret_path, len - 1, "%s", log_file);
		}

		if (ret >= len) {
			print_err("invalid path len, over buffer length, exit!\n");
			memset_s((void *)ret_path, len, 0, len);
			return;
		}
	}
}

void bootfail_logs_destructor(void)
{
	print_info("bootfail_logs_destructor!\n");
	down(&g_log_param_sem);
	if (g_log_param != NULL) {
		vfree(g_log_param);
		g_log_param = NULL;
	}
	up(&g_log_param_sem);
}
