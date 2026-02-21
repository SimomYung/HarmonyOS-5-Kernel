
#include "ffts_interface.h"
#include "ffts_black_box.h"
#include <liblinux/pal.h>
#include "sd_proxy.h"
#include "ffts_kernel_log.h"
#include <linux/module.h>

static int sd_proxy_ffts_cb(unsigned int cmd, void *rd, size_t rd_size,
			   void *wr, size_t wr_size)
{
	int ret = 0;
	unsigned int major_cmd;
	unsigned int minor_cmd;
	major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);
	if (major_cmd != FFTS_MAJOR_CMD) {
		ffts_drv_err("ffts major_cmd match fail, %u\n", major_cmd);
		return ret;
	}

	minor_cmd = PAL_PROXY_MINOR_CMD_OF(cmd);
	switch (minor_cmd) {
		case LDK_MINOR_CMD_FFTS_SAVE_BBOX:
			ret = ffts_mntn_copy_udk_info_to_bbox(rd, rd_size);
			break;
		case LDK_MINOR_CMD_FFTS_RDR_REPORT:
			ret = ffts_udk_rdr_exception_report(rd, rd_size);
			break;
		case LDK_MINOR_CMD_FFTS_INVOKE_HW_RELEASE:
			ret = ffts_udk_invoke_hw_release(rd, rd_size);
			break;
		default:
			ret = -1;
			ffts_drv_err("ffts major_cmd is invalid, %u\n", minor_cmd);
			break;
	}
	return ret;
}

static int __init ffts_ldk_dev_init(void)
{
	int ret = 0;

    ffts_drv_info("ffts_ldk_dev start init\n");
	ret = ffts_black_box_init();
	cond_return_error(ret != 0, -1, "black box init error.\n");

    ret = liblinux_pal_proxy_register(PAL_PROXY_MAJOR(FFTS_MAJOR_CMD), sd_proxy_ffts_cb);
	cond_return_error(ret != 0, -1, "pal proxy register error.\n");
    return 0;
}

module_init(ffts_ldk_dev_init);

static void __exit ffts_ldk_dev_exit(void)
{
    ffts_black_box_exit();
}

module_exit(ffts_ldk_dev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FFTS LDK driver");
MODULE_VERSION("V1.0");