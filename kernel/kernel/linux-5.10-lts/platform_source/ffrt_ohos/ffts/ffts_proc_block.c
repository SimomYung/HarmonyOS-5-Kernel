#include "ffts_proc_block.h"
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <liblinux/pal.h>
#include "sd_proxy.h"
#include "ffts_kernel_log.h"
#include "ffts_black_box.h"

void ffts_proc_block_enable(int uid)
{
    int ret;
	ffts_drv_debug("ffrt_ohos ldk call proc block enable register.\n");
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_PROC_BLOCK_ENABLE), &uid, sizeof(int), NULL, 0);
	if (ret != 0) {
		ffts_drv_err("call proc block enable failed, ret = %d\n", ret);
	}
}
EXPORT_SYMBOL(ffts_proc_block_enable);

int ffts_proc_release_register(ffts_hw_t hw_type, void (*callback)(void *data, int32_t tgid), void *data, int32_t tgid)
{
    int ret;
	ffts_drv_debug("ffrt_ohos ldk call proc release register.\n");
	struct ffts_proc_rel_regis_info regis_info = {0};
	regis_info.hw_type = hw_type;
	regis_info.callback = callback;
	regis_info.data = data;
	regis_info.tgid = tgid;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_PROC_REL_HW_REGISTER), &regis_info, sizeof(struct ffts_proc_rel_regis_info), NULL, 0);
	if (ret != 0) {
		ffts_drv_err("call proc release register failed, ret = %d\n", ret);
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(ffts_proc_release_register);