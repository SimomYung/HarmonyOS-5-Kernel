#ifdef CONFIG_PLATDRV_SUPPORT_FFA
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#endif
#include "vdec_tzsp_client.h"
#include "dbg.h"

int32_t vdec_tzsp_client_send_msg(uint32_t add_flag)
{
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	struct ffa_send_direct_data vdec_args = {
		.data0 = add_flag ? VDEC_FID_ADD_SD : VDEC_FID_CLEAR_SD,
		.data1 = 0,
		.data2 = 0,
		.data3 = 0,
		.data4 = 0,
	};

	if (ffa_platdrv_send_msg(&vdec_args)) {
		dprint(PRN_ERROR, "send tzsp client msg failed");
		return -1;
	}

	if (vdec_args.data1) {
		dprint(PRN_ERROR, "execute tzsp cmd failed");
		return -1;
	}
#endif

	return 0;
}