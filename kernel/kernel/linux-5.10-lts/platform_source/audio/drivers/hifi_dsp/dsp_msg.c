/*
 * dsp msg.c
 *
 * dsp msg driver.
 *
 * Copyright (c) 2015-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "dsp_msg.h"
#include "dsp_mmap.h"

#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>


#include "audio_common_msg.h"
#include "audio_hifi.h"
#include "drv_mailbox_msg.h"
#include "bsp_drv_ipc.h"
#include "dsp_om.h"
#include "om_debug.h"
#ifdef CONFIG_HUAWEI_AUDIO
#include "om_bigdata.h"
#endif
#ifdef CONFIG_USB_AUDIO_DSP
#include "usbaudio_ioctl.h"
#endif
#ifdef CONFIG_SOUND_TRIGGER
#include "soundtrigger_socdsp_mailbox.h"
#endif

#include <linux/mm_lb/mm_lb.h>
#include "dsp_lpp.h"
#include "securec.h"

#define FPGA_TIMEOUT_LEN_MS 10000
#define ASIC_TIMEOUT_LEN_MS 2000
#define SIZE_LIMIT_PARAM 256 /* IOCTL: io size */

#define SEND_MSG_TO_DSP mailbox_send_msg


LIST_HEAD(recv_sync_work_queue_head);

struct misc_msg_info {
	unsigned short msg_id;
	const char * const info;
};

struct dsp_msg_priv {
	spinlock_t recv_sync_lock;
	struct completion completion;
	unsigned int sn;
	unsigned char *base_virt;
	unsigned char *base_phy;
};

struct dsp_msg_priv g_msg_priv;

static DEFINE_SEMAPHORE(s_dsp_sync_cmd_sem);

static const struct misc_msg_info g_msg_info[] = {
	{ ID_AP_AUDIO_SET_DTS_ENABLE_CMD, "ID_AP_AUDIO_SET_DTS_ENABLE_CMD" },
	{ ID_AP_AUDIO_SET_DTS_DEV_CMD, "ID_AP_AUDIO_SET_DTS_DEV_CMD" },
	{ ID_AP_AUDIO_SET_DTS_GEQ_CMD, "ID_AP_AUDIO_SET_DTS_GEQ_CMD" },
	{ ID_AP_AUDIO_SET_DTS_GEQ_ENABLE_CMD, "ID_AP_AUDIO_SET_DTS_GEQ_ENABLE_CMD" },
	{ ID_AP_DSP_VOICE_RECORD_START_CMD, "ID_AP_DSP_VOICE_RECORD_START_CMD" },
	{ ID_AP_DSP_VOICE_RECORD_STOP_CMD, "ID_AP_DSP_VOICE_RECORD_STOP_CMD" },
	{ ID_AP_VOICEPP_START_REQ, "ID_AP_VOICEPP_START_REQ" },
	{ ID_VOICEPP_AP_START_CNF, "ID_VOICEPP_AP_START_CNF" },
	{ ID_AP_VOICEPP_STOP_REQ, "ID_AP_VOICEPP_STOP_REQ" },
	{ ID_VOICEPP_AP_STOP_CNF, "ID_VOICEPP_AP_STOP_CNF" },
	{ ID_AP_AUDIO_PLAY_START_REQ, "ID_AP_AUDIO_PLAY_START_REQ" },
	{ ID_AUDIO_AP_PLAY_START_CNF, "ID_AUDIO_AP_PLAY_START_CNF" },
	{ ID_AP_AUDIO_PLAY_PAUSE_REQ, "ID_AP_AUDIO_PLAY_PAUSE_REQ" },
	{ ID_AUDIO_AP_PLAY_PAUSE_CNF, " ID_AUDIO_AP_PLAY_PAUSE_CNF" },
	{ ID_AUDIO_AP_PLAY_DONE_IND, "ID_AUDIO_AP_PLAY_DONE_IND" },
	{ ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD, "ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD" },
	{ ID_AP_AUDIO_PLAY_QUERY_TIME_REQ, "ID_AP_AUDIO_PLAY_QUERY_TIME_REQ" },
	{ ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ, "ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ" },
	{ ID_AUDIO_AP_PLAY_QUERY_TIME_CNF, "ID_AUDIO_AP_PLAY_QUERY_TIME_CNF" },
	{ ID_AP_AUDIO_PLAY_SET_BUF_WATERLINE_REQ, "ID_AP_AUDIO_PLAY_SET_BUF_WATERLINE_REQ" },
	{ ID_AUDIO_AP_PLAY_SET_BUF_WATERLINE_CNF, "ID_AUDIO_AP_PLAY_SET_BUF_WATERLINE_CNF" },
	{ ID_AP_AUDIO_PLAY_SET_VOL_CMD, "ID_AP_AUDIO_PLAY_SET_VOL_CMD" },
	{ ID_AP_AUDIO_RECORD_PCM_HOOK_CMD, "ID_AP_AUDIO_RECORD_PCM_HOOK_CMD" },
	{ ID_AP_DSP_ENHANCE_START_REQ, "ID_AP_DSP_ENHANCE_START_REQ" },
	{ ID_DSP_AP_ENHANCE_START_CNF, "ID_DSP_AP_ENHANCE_START_CNF" },
	{ ID_AP_DSP_ENHANCE_STOP_REQ, "ID_AP_DSP_ENHANCE_STOP_REQ" },
	{ ID_DSP_AP_ENHANCE_STOP_CNF, "ID_DSP_AP_ENHANCE_STOP_CNF" },
	{ ID_AP_DSP_ENHANCE_SET_DEVICE_REQ, "ID_AP_DSP_ENHANCE_SET_DEVICE_REQ" },
	{ ID_DSP_AP_ENHANCE_SET_DEVICE_CNF, "ID_DSP_AP_ENHANCE_SET_DEVICE_CNF" },
	{ ID_AP_AUDIO_ENHANCE_SET_DEVICE_IND, "ID_AP_AUDIO_ENHANCE_SET_DEVICE_IND" },
	{ ID_AP_AUDIO_CMD_SET_SOURCE_CMD, "ID_AP_AUDIO_CMD_SET_SOURCE_CMD" },
	{ ID_AP_AUDIO_CMD_SET_DEVICE_CMD, "ID_AP_AUDIO_CMD_SET_DEVICE_CMD" },
	{ ID_AP_DSP_SET_IO_SAMPLE_RATE_CMD, "ID_AP_DSP_SET_IO_SAMPLE_RATE_CMD" },
	{ ID_AP_AUDIO_CMD_SET_MODE_CMD, "ID_AP_AUDIO_CMD_SET_MODE_CMD" },
	{ ID_AP_AUDIO_CMD_SET_ANGLE_CMD, "ID_AP_AUDIO_CMD_SET_ANGLE_CMD" },
	{ ID_AP_AUDIO_CMD_SET_LATENCY_MEASURE_CMD, "ID_AP_AUDIO_CMD_SET_LATENCY_MEASURE_CMD" },
	{ ID_AP_AUDIO_AVLPP_START_READY_CMD, "ID_AP_AUDIO_AVLPP_START_READY_CMD" },
	{ ID_AP_AUDIO_ROUTING_COMPLETE_REQ, "ID_AP_AUDIO_ROUTING_COMPLETE_REQ" },
	{ ID_AUDIO_AP_OM_DUMP_CMD, "ID_AUDIO_AP_OM_DUMP_CMD" },
	{ ID_AUDIO_AP_FADE_OUT_REQ, "ID_AUDIO_AP_FADE_OUT_REQ" },
	{ ID_AP_ENABLE_MODEM_LOOP_REQ, "ID_AP_ENABLE_MODEM_LOOP_REQ" },
	{ ID_AP_ENABLE_AT_DSP_LOOP_REQ, "ID_AP_ENABLE_AT_DSP_LOOP_REQ" },
	{ ID_AP_AUDIO_DYN_EFFECT_GET_PARAM, "ID_AP_AUDIO_DYN_EFFECT_GET_PARAM" },
	{ ID_AP_AUDIO_DYN_EFFECT_GET_PARAM_CNF, "ID_AP_AUDIO_DYN_EFFECT_GET_PARAM_CNF" },
	{ ID_AP_AUDIO_DYN_EFFECT_TRIGGER, "ID_AP_AUDIO_DYN_EFFECT_TRIGGER" },
	{ ID_AP_HIFI_REQUEST_SET_PARA_CMD, "ID_AP_HIFI_REQUEST_SET_PARA_CMD" },
	{ ID_AP_HIFI_REQUEST_GET_PARA_CMD, "ID_AP_HIFI_REQUEST_GET_PARA_CMD" },
	{ ID_AP_HIFI_REQUEST_GET_PARA_CNF, "ID_AP_HIFI_REQUEST_GET_PARA_CNF" },
	{ ID_AP_DSP_I2S_TEST_POWER_REQ, "ID_AP_DSP_I2S_TEST_POWER_REQ" },
	{ ID_DSP_AP_I2S_TEST_POWER_CNF, "ID_DSP_AP_I2S_TEST_POWER_CNF" },
	{ ID_AP_HIFI_SET_FM_CMD, "ID_AP_HIFI_SET_FM_CMD" },
	{ ID_AP_HIFI_LP_MADTEST_START_CMD, "ID_AP_HIFI_LP_MADTEST_START_CMD" },
	{ ID_AP_HIFI_LP_MADTEST_STOP_CMD, "ID_AP_HIFI_LP_MADTEST_STOP_CMD" },
	{ ID_AP_DSP_HEARING_PROTECTION_SWITCH_CMD, "ID_AP_DSP_HEARING_PROTECTION_SWITCH_CMD" },
	{ ID_AP_DSP_HEARING_PROTECTION_NOTIFY_READ_PARAM_CMD, "ID_AP_DSP_HEARING_PROTECTION_NOTIFY_READ_PARAM_CMD" },
	{ ID_AP_DSP_HEARING_PROTECTION_GET_VOLUME_CMD, "ID_AP_DSP_HEARING_PROTECTION_GET_VOLUME_CMD" },
	{ ID_AP_DSP_HEARING_PROTECTION_GET_VOLUME_CNF, "ID_AP_DSP_HEARING_PROTECTION_GET_VOLUME_CNF" },
#ifdef CONFIG_HUAWEI_AUDIO
	{ ID_AP_HIFI_SET_SOUND_ENHANCE_SWITCH_CMD, "ID_AP_HIFI_SET_SOUND_ENHANCE_SWITCH_CMD" },
	{ ID_AP_HIFI_SET_SOUND_ENHANCE_SCENE_CMD, "ID_AP_HIFI_SET_SOUND_ENHANCE_SCENE_CMD" },
	{ ID_AP_HIFI_SET_SOUND_ENHANCE_PARA_CMD, "ID_AP_HIFI_SET_SOUND_ENHANCE_PARA_CMD" },
#endif
	{ ID_AP_DSP_GET_REC_START_DMA_STAMP_REQ, "ID_AP_DSP_GET_REC_START_DMA_STAMP_REQ" },
	{ ID_DSP_AP_GET_REC_START_DMA_STAMP_CNF, "ID_DSP_AP_GET_REC_START_DMA_STAMP_CNF" },
	{ ID_AP_AUDIO_CMD_SET_CODEC_PACKET_INFO, "ID_AP_AUDIO_CMD_SET_CODEC_PACKET_INFO" },
	{ ID_AP_DSP_SPATIAL_AUDIO_INIT_REQ, "ID_AP_DSP_SPATIAL_AUDIO_INIT_REQ" },
	{ ID_DSP_AP_SPATIAL_AUDIO_INIT_CNF, "ID_DSP_AP_SPATIAL_AUDIO_INIT_CNF" },
	{ ID_AP_DSP_SPATIAL_AUDIO_DEINIT_REQ, "ID_AP_DSP_SPATIAL_AUDIO_DEINIT_REQ" },
	{ ID_DSP_AP_SPATIAL_AUDIO_DEINIT_CNF, "ID_DSP_AP_SPATIAL_AUDIO_DEINIT_CNF" },
	{ ID_AP_DSP_AUDIO_NN_INIT_REQ, "ID_AP_DSP_AUDIO_NN_INIT_REQ" },
	{ ID_DSP_AP_AUDIO_NN_INIT_CNF, "ID_DSP_AP_AUDIO_NN_INIT_CNF" },
	{ ID_AP_DSP_AUDIO_NN_DEINIT_REQ, "ID_AP_DSP_AUDIO_NN_DEINIT_REQ" },
	{ ID_DSP_AP_AUDIO_NN_DEINIT_CNF, "ID_DSP_AP_AUDIO_NN_DEINIT_CNF" },
	{ ID_AP_DSP_AUDIO_NN_SET_PARAMS_REQ, "ID_AP_DSP_AUDIO_NN_SET_PARAMS_REQ" },
	{ ID_DSP_AP_AUDIO_NN_SET_PARAMS_CNF, "ID_DSP_AP_AUDIO_NN_SET_PARAMS_CNF" },
	{ ID_AP_DSP_BT_COMBINED_DENOISING_CNF, "ID_AP_DSP_BT_COMBINED_DENOISING_CNF" },
	{ ID_AP_AUDIO_CMD_SET_VOLUME, "ID_AP_AUDIO_CMD_SET_VOLUME"},
};

static unsigned short sync_msg_list[] = {
	ID_AP_AUDIO_PLAY_START_REQ,
	ID_AP_AUDIO_PLAY_PAUSE_REQ,
	ID_AP_AUDIO_PLAY_SET_BUF_WATERLINE_REQ,
	ID_AP_AUDIO_PLAY_QUERY_TIME_REQ,
	ID_AP_AUDIO_CMD_SET_AUDIO_EFFECT_PARAM_CMD,
	ID_AP_HIFI_REQUEST_VOICE_PARA_REQ,
    ID_AP_HIFI_REQUEST_COMBINE_NOISE_REQ,
	ID_AP_HIFI_REQUEST_GET_PARA_CMD,
	ID_AP_AUDIO_DYN_EFFECT_GET_PARAM,
	ID_AP_DSP_HEARING_PROTECTION_GET_VOLUME_CMD,
	ID_AP_DSP_GET_REC_START_DMA_STAMP_REQ,
	ID_AP_HIFI_SOUNDTRIGGER_MIC_STATE_GET_REQ,
	ID_AP_DSP_SPATIAL_AUDIO_INIT_REQ,
	ID_AP_DSP_SPATIAL_AUDIO_DEINIT_REQ,
	ID_AP_AUDIO_MMAP_GET_POSITION,
	ID_AP_AUDIO_GET_DSP_INFO,
	ID_AP_DSP_AUDIO_NN_INIT_REQ,
	ID_AP_DSP_AUDIO_NN_DEINIT_REQ,
	ID_AP_DSP_AUDIO_NN_SET_PARAMS_REQ,
	/* uncertain msg id */
	ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ,
	ID_AP_AUDIO_LOOP_BEGIN_REQ,
	ID_AP_AUDIO_LOOP_END_REQ,
	ID_AP_HIFI_REQUEST_SMARTPA_PARA_REQ,
	ID_AP_DSP_I2S_TEST_POWER_REQ,
	ID_AP_DSP_BT_COMBINED_DENOISING_CMD,
	/* uncertain msg id */
};

typedef void (*sync_buff_proc)(unsigned char *buff, unsigned int len); 
struct sync_msg_proc_info{
    unsigned short msg_id;
    sync_buff_proc buff_proc;
};


static const struct sync_msg_proc_info g_sync_msg_proc[] = {
};

static unsigned short async_msg_list[] = {
	ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD,
	ID_AP_AUDIO_REAL_TIME_ACCESS_REQ,
	ID_AP_AUDIO_PLAY_SET_VOL_CMD,
	ID_AP_AUDIO_PCM_OPEN_REQ,
	ID_AP_AUDIO_PCM_CLOSE_REQ,
	ID_AP_AUDIO_PCM_TRIGGER_REQ,
	ID_AP_NOTIFY_HIFI_MULTI_CHANNEL_STREAM_TYPE,
	ID_AP_AUDIO_PCM_SET_VOIP_EFFECT_STATUS_REQ,
	ID_AP_HIFI_SET_FM_CMD,
	ID_AP_AUDIO_SET_DTS_ENABLE_CMD,
	ID_AP_AUDIO_CMD_SET_ANGLE_CMD,
	ID_AP_AUDIO_SET_EXCODEC_BYPASS_CMD,
	ID_AP_AUDIO_SET_ENABLE_AEC_CMD,
	ID_AP_AUDIO_CMD_SET_SOURCE_CMD,
	ID_AP_AUDIO_CMD_SET_DEVICE_CMD,
    ID_AP_DSP_SET_IO_SAMPLE_RATE_CMD,
	ID_AP_AUDIO_CMD_SET_LATENCY_MEASURE_CMD,
	ID_AP_AUDIO_AVLPP_START_READY_CMD,
	ID_AP_AUDIO_CMD_SET_TYPEC_DEVICE_MASK_CMD,
	ID_AP_AUDIO_CMD_SET_MODE_CMD,
	ID_AP_ENABLE_MODEM_LOOP_REQ,
	ID_AP_ENABLE_AT_DSP_LOOP_REQ,
	ID_AP_AUDIO_CMD_MLIB_SET_PARA_CMD,
	ID_AP_AUDIO_CMD_SET_MMI_MODE_CMD,
	ID_AP_AUDIO_ROUTING_COMPLETE_REQ,
	ID_AP_AUDIO_BSD_CONTROL_REQ,
	ID_AP_HIFI_REQUEST_SET_PARA_CMD,
	ID_AP_HIFI_REQUEST_GET_PARA_CMD,
	ID_AP_AUDIO_CMD_SET_MONO_CHANNEL_MODE_CMD,
	ID_AP_AUDIO_CMD_SET_COMBINE_RECORD_FUNC_CMD,
	ID_AP_HIFI_SET_STREAM_MUTE_CMD,
	ID_AP_HIFI_SET_OUTPUT_MUTE_CMD,
	ID_AP_HIFI_SET_INPUT_MUTE_CMD,
	ID_AP_HIFI_SET_AUDIO2VOICETX_CMD,
	ID_AP_HIFI_SET_AUDIOMIX2VOICETX_CMD,
	ID_AP_HIFI_SET_MOS_TEST_CMD,
	ID_AP_DSP_SET_FORCE_MUTE_MIC_CMD,
	ID_AP_AUDIO_CMD_SET_WAKEUP_SCENE_CMD,
	ID_AP_DSP_SET_BBIC_CMD,
	ID_AP_HIFI_VIBRATOR_STATUS_CHANGE_CMD,
	ID_AP_AUDIO_DYN_EFFECT_TRIGGER,
	ID_AP_HIFI_LP_MADTEST_START_CMD,
	ID_AP_HIFI_LP_MADTEST_STOP_CMD,
	ID_AP_DSP_SET_SCENE_DENOISE_CMD,
	ID_AP_DSP_HEARING_PROTECTION_SWITCH_CMD,
	ID_AP_DSP_HEARING_PROTECTION_NOTIFY_READ_PARAM_CMD,
	ID_AP_HIFI_SET_SOUND_ENHANCE_SWITCH_CMD,
	ID_AP_HIFI_SET_SOUND_ENHANCE_SCENE_CMD,
	ID_AP_HIFI_SET_SOUND_ENHANCE_PARA_CMD,
	ID_AP_HIFI_SET_DPRX_CMD,
	ID_AP_AUDIO_CMD_SET_CODEC_PACKET_INFO,
	ID_AP_AUDIO_CMD_SET_SCO_SCENE,
	ID_AP_AUDIO_SET_EC_CHANNEL_CMD,
	/* uncertain msg id */
	ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ,
	ID_AP_AUDIO_LOOP_BEGIN_REQ,
	ID_AP_AUDIO_LOOP_END_REQ,
	ID_AP_HIFI_REQUEST_SMARTPA_PARA_REQ,
	ID_AP_DSP_I2S_TEST_POWER_REQ,
	ID_AP_DSP_BT_COMBINED_DENOISING_CMD,
	/* uncertain msg id */
	ID_AP_AUDIO_CMD_SET_VOLUME,
	ID_AP_DSP_LPC_BLOW_COGNITION_UPDATE_CMD,
	/* audio haptic sync */
	ID_AP_DSP_AUDIO_HAPTIC_SYNC_SET_PARA_CMD,
};

extern int dpu_dptx_set_aparam(unsigned int channel_num,
	unsigned int data_width, unsigned int sample_rate, int port_id);
extern int dpu_dptx_get_spec(void *data, unsigned int size, unsigned int *ext_acount, int port_id);

int rcv_dsp_sync_msg(struct list_head *node, unsigned int sn)
{
	if (sn == g_msg_priv.sn) {
		spin_lock_bh(&g_msg_priv.recv_sync_lock);
		list_add_tail(node, &recv_sync_work_queue_head);
		spin_unlock_bh(&g_msg_priv.recv_sync_lock);

		complete(&g_msg_priv.completion);
		return 0;
	}

	if (sn != ACPU_TO_HIFI_ASYNC_CMD)
		loge("msg priv sn: %u, cmd para sn: %u\n", g_msg_priv.sn, sn);

	return -EINVAL;
}

static void dsp_msg_info(const unsigned short msg_id)
{
	unsigned int i;
	size_t size;

	if ((msg_id == ID_AP_AUDIO_PLAY_QUERY_TIME_REQ) ||
			(msg_id == ID_AUDIO_AP_PLAY_QUERY_TIME_CNF))
		return;

	size = ARRAY_SIZE(g_msg_info);
	for (i = 0; i < size; i++) {
		if (g_msg_info[i].msg_id == msg_id) {
			logi("msg: %s\n", g_msg_info[i].info);
			break;
		}
	}

	if (i == size)
		logw("not defined msg id: 0x%x\n", msg_id);
}

static int get_dsp_time(uintptr_t uaddr)
{
	char data[MAX_DSP_TIME_INFO_STR_LEN] = {'\0'};
	struct dsp_time_info param;
	int ret;
 
	(void)memset_s(&param, sizeof(param), 0, sizeof(param));

	if (try_copy_from_user(&param, (void __user *)uaddr, sizeof(param))) {
		loge("copy from user failed\n");
		return -EFAULT;
	}
 
	if (!param.pdata) {
		loge("user buffer is null\n");
		return -EINVAL;
	}
 
	if (param.data_len < MAX_DSP_TIME_INFO_STR_LEN) {
		loge("userspace len: %u is less than data len: %u\n",
			param.data_len, MAX_DSP_TIME_INFO_STR_LEN);
		return -EINVAL;
	}
 
	ret = get_dsp_time_info_str((char *)data, MAX_DSP_TIME_INFO_STR_LEN);
	if (ret != 0) {
		loge("get dsp time info error, ret is %d\n", ret);
		return ret;
	}
 
	if (try_copy_to_user((void __user *)param.pdata, data, MAX_DSP_TIME_INFO_STR_LEN)) {
		loge("copy to user failed\n");
		return -EFAULT;
	}
 
	return 0;
}

static bool is_valid_msg(unsigned short msg_id, unsigned short *msg_list, unsigned int msg_cnt)
{
	unsigned int i;

	for (i = 0; i < msg_cnt; i++) {
		if (msg_id == msg_list[i])
			return true;
	}

	return false;
}

static int async_write(const unsigned char *arg, unsigned int len)
{
	int ret;
	unsigned short msg_id;

	if (!arg) {
		loge("input arg is null\n");
		return ERROR;
	}

	msg_id = *(unsigned short *)arg;
	if (!is_valid_msg(msg_id, async_msg_list, ARRAY_SIZE(async_msg_list))) {
		loge("async msg 0x%x is invalid\n", msg_id);
		return ERROR;
	}

	ret = SEND_MSG_TO_DSP(MAILBOX_MAILCODE_ACPU_TO_HIFI_MISC, arg, len);
	if (ret != OK) {
		loge("msg send to dsp fail, ret is %d\n", ret);
		return ERROR;
	}

	return OK;
}

static int sync_write(unsigned char *buff, unsigned int len)
{
	int ret;
    int i;
	unsigned long wait_result;
	static bool is_write_success;
	unsigned short msg_id;

	if (!buff) {
		loge("input arg is null\n");
		return ERROR;
	}

	msg_id = *(unsigned short *)buff;
	if (!is_valid_msg(msg_id, sync_msg_list, ARRAY_SIZE(sync_msg_list))) {
		loge("sync msg 0x%x is invalid\n", msg_id);
		return ERROR;
	}

	for (i = 0; i < ARRAY_SIZE(g_sync_msg_proc); i++) {
        if (g_sync_msg_proc[i].msg_id == msg_id) {
            g_sync_msg_proc[i].buff_proc(buff, len);
			break;
        }
    }

	reinit_completion(&g_msg_priv.completion);
	ret = SEND_MSG_TO_DSP(MAILBOX_MAILCODE_ACPU_TO_HIFI_MISC, buff, len);
	if (ret != OK) {
		loge("msg send to dsp fail, ret: %d\n", ret);
		return ERROR;
	}

	if (dsp_misc_get_platform_type() == DSP_PLATFORM_FPGA)
		wait_result = wait_for_completion_timeout(&g_msg_priv.completion,
			msecs_to_jiffies(FPGA_TIMEOUT_LEN_MS));
	else
		wait_result = wait_for_completion_timeout(&g_msg_priv.completion,
			msecs_to_jiffies(ASIC_TIMEOUT_LEN_MS));

	g_msg_priv.sn++;
	if (unlikely(g_msg_priv.sn & 0x10000000))
		g_msg_priv.sn = 0;

	if (wait_result == 0) {
		if (is_write_success) {
			loge("wait completion timeout\n");
			socdsp_dump_panic_log();
		}
		ret = ERROR;
	} else {
		is_write_success = true;
		ret = OK;
	}

	return ret;
}

static int handle_input_param(const void *para_in, unsigned int para_size_in)
{
	unsigned short cmd = *(const unsigned short *)para_in;

	logd("handle input param id %u\n", para_size_in);

	if (cmd == ID_AP_DSP_SPATIAL_AUDIO_INIT_REQ)
		return dsp_misc_spatial_audio_proc_sync(para_in, para_size_in);

	return 0;
}

static int get_input_param(unsigned int usr_para_size, const void *usr_para_addr,
	unsigned int *kernel_para_size, void **kernel_para_addr)
{
	void *para_in = NULL;
	unsigned int para_size_in;

	if ((usr_para_size == 0) ||
		(usr_para_size > SIZE_LIMIT_PARAM - SIZE_CMD_ID)) {
		loge("usr para size: %u exceed limit: 0/%u\n",
			usr_para_size, SIZE_LIMIT_PARAM - SIZE_CMD_ID);
		return ERROR;
	}

	para_size_in = usr_para_size + SIZE_CMD_ID;
	para_in = kzalloc(para_size_in, GFP_KERNEL);
	if (!para_in) {
		loge("kzalloc fail\n");
		return ERROR;
	}

	if (!usr_para_addr) {
		loge("usr para addr is null no user data\n");
		kfree(para_in);
		return ERROR;
	}

	if (try_copy_from_user(para_in, usr_para_addr, usr_para_size)) {
		loge("copy from user fail\n");
		kfree(para_in);
		return ERROR;
	}

	*kernel_para_size = para_size_in;
	*kernel_para_addr = para_in;

	dsp_msg_info(*(const unsigned short *)para_in);

	return OK;
}

static void input_param_free(void **kernel_para_addr)
{
	if (*kernel_para_addr) {
		kfree(*kernel_para_addr);
		*kernel_para_addr = NULL;
	} else {
		loge("kernel para addr to free is null\n");
	}
}

static void get_ap_timestamp(const void *kernel_para_addr, unsigned int kernel_para_size)
{
	struct record_start_dma_stamp_get_cnf *cnf_msg = (struct record_start_dma_stamp_get_cnf *)kernel_para_addr;
	long long time_s;
	long time_us;

	spinlock_t read_time_lock;
	unsigned long flags;

	if (sizeof(*cnf_msg) > kernel_para_size)
		return;

	if ((cnf_msg->msg_id != ID_DSP_AP_GET_REC_START_DMA_STAMP_CNF) &&
		(cnf_msg->msg_id != ID_AUDIO_AP_MMAP_POSITION_ACK))
		return;

	spin_lock_init(&read_time_lock);
	spin_lock_irqsave(&read_time_lock, flags);

	if (cnf_msg->msg_id == ID_DSP_AP_GET_REC_START_DMA_STAMP_CNF)
		get_ap_cur_time(&time_s, &time_us, false, true);
	else
		get_ap_cur_time(&time_s, &time_us, false, false);

	cnf_msg->kernel_stamp = DSP_STAMP;
	cnf_msg->kernel_time_s = time_s;
	cnf_msg->kernel_time_us = time_us;
	spin_unlock_irqrestore(&read_time_lock, flags);
}

static int get_output_param(unsigned int kernel_para_size, const void *kernel_para_addr,
	unsigned int *usr_para_size, void __user *usr_para_addr)
{
	int ret = OK;
	void __user *para_to = NULL;
	unsigned int para_n;

	if (!kernel_para_addr) {
		loge("kernel para addr is null\n");
		return -EINVAL;
	}

	if (!usr_para_addr || !usr_para_size) {
		loge("usr size p: 0x%pK, usr addr: 0x%pK\n",
			usr_para_size, usr_para_addr);
		ret = -EINVAL;
		goto end;
	}

	para_to = usr_para_addr;
	para_n = kernel_para_size;
	if ((para_n == 0) || (para_n > SIZE_LIMIT_PARAM)) {
		loge("para n exceed limit: %u, %d\n", para_n, SIZE_LIMIT_PARAM);
		ret = -EINVAL;
		goto end;
	}

	if (para_n > *usr_para_size) {
		loge("para n exceed usr size: %u, %u\n", para_n, *usr_para_size);
		ret = -EINVAL;
		goto end;
	}

	get_ap_timestamp(kernel_para_addr, kernel_para_size);

	/* Copy data from kernel space to user space: to from n */
	if (try_copy_to_user(para_to, kernel_para_addr, para_n)) {
		loge("copy to user fail\n");
		ret = COPYFAIL;
		goto end;
	}

	*usr_para_size = para_n;

end:
	dsp_msg_info(*(const unsigned short *)kernel_para_addr);
	return ret;
}

static int async_cmd(uintptr_t arg)
{
	int ret = OK;
	struct misc_io_async_param param;
	void *para_kernel_in = NULL;
	unsigned int para_kernel_size_in = 0;
	struct dsp_chn_cmd *cmd_para = NULL;
	void *para_addr_in = NULL;

	if (try_copy_from_user(&param, (void *)arg,
		sizeof(struct misc_io_async_param))) {
		loge("copy from user fail\n");
		ret = ERROR;
		goto end;
	}

	para_addr_in = INT_TO_ADDR(param.para_in_l, param.para_in_h);
	ret = get_input_param(param.para_size_in, para_addr_in,
		&para_kernel_size_in, &para_kernel_in);
	if (ret != OK) {
		loge("get ret: %d\n", ret);
		goto end;
	}
	/* add cmd id and sn  */
	cmd_para = (struct dsp_chn_cmd *)(para_kernel_in + para_kernel_size_in - SIZE_CMD_ID);
	cmd_para->cmd_type = HIFI_CHN_SYNC_CMD;
	cmd_para->sn = ACPU_TO_HIFI_ASYNC_CMD;

	ret = async_write(para_kernel_in, para_kernel_size_in);
	if (ret != OK) {
		loge("async write ret: %d\n", ret);
		goto end;
	}

	if (*(unsigned short *)para_kernel_in == ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD)
		release_update_buff_wakelock();

end:
	input_param_free(&para_kernel_in);
	return ret;
}

static int rev_msg_get(unsigned char *mail_buf, unsigned int *mail_len)
{
	int ret = OK;
	struct recv_request *recv = NULL;
	unsigned int len;

	spin_lock_bh(&g_msg_priv.recv_sync_lock);

	if (list_empty(&recv_sync_work_queue_head) == 0) {
		recv = list_entry(recv_sync_work_queue_head.next,
			struct recv_request, recv_node);

		len = recv->rev_msg.mail_buff_len - SIZE_CMD_ID;
		if (len <= *mail_len) {
			memcpy(mail_buf, recv->rev_msg.mail_buff, len);
		} else {
			loge("mail buff error buf length: %u\n", len);
			ret = -EINVAL;
		}

		*mail_len = len;
		list_del(&recv->recv_node);

		kfree(recv->rev_msg.mail_buff);
		kfree(recv);
	}

	spin_unlock_bh(&g_msg_priv.recv_sync_lock);

	return ret;
}

static int cmd_para_init(struct misc_io_sync_param param,
	unsigned int *para_kernel_size_in, void **para_kernel_in)
{
	void __user *para_addr_in = NULL;
	struct dsp_chn_cmd *cmd_para = NULL;
	int ret;

	logd("para size in: %u\n", param.para_size_in);

	para_addr_in = INT_TO_ADDR(param.para_in_l, param.para_in_h);
	ret = get_input_param(param.para_size_in, para_addr_in,
		para_kernel_size_in, para_kernel_in);
	if (ret != OK) {
		loge("get input param fail: ret: %d\n", ret);
		return ret;
	}

	ret = handle_input_param((const void *)(*para_kernel_in), *para_kernel_size_in);
	if (ret != 0) {
		input_param_free(para_kernel_in);
		loge("handle input param fail: ret: %d\n", ret);
		return ret;
	}
	/* add cmd id and sn  */
	cmd_para = (struct dsp_chn_cmd *)(*para_kernel_in + *para_kernel_size_in - SIZE_CMD_ID);
	cmd_para->cmd_type = HIFI_CHN_SYNC_CMD;
	cmd_para->sn = g_msg_priv.sn;

	return OK;
}

static int sync_cmd(uintptr_t arg)
{
	int ret;
	struct misc_io_sync_param param;
	void *para_kernel_in = NULL;
	unsigned int para_kernel_size_in = 0;
	void __user *para_addr_out = NULL;
	unsigned char *mail_buf = NULL;
	unsigned int mail_len = SIZE_LIMIT_PARAM;

	if (try_copy_from_user(&param, (void *)arg, sizeof(param))) {
		loge("copy from user fail\n");
		ret = ERROR;
		goto end;
	}

	para_addr_out = INT_TO_ADDR(param.para_out_l, param.para_out_h);

	ret = cmd_para_init(param, &para_kernel_size_in, &para_kernel_in);
	if (ret != OK)
		goto end;

	ret = sync_write(para_kernel_in, para_kernel_size_in);
	if (ret != OK) {
		loge("sync write ret: %d\n", ret);
		goto end;
	}

	mail_buf = kzalloc(SIZE_LIMIT_PARAM, GFP_KERNEL);
	if (!mail_buf) {
		loge("alloc mail buffer failed\n");
		ret = -ENOMEM;
		goto end;
	}

	ret = rev_msg_get(mail_buf, &mail_len);
	if (ret != OK)
		goto end;

	ret = get_output_param(mail_len, mail_buf, &param.para_size_out, para_addr_out);
	if (ret != OK) {
		loge("get output param fail, ret: %d\n", ret);
		goto end;
	}

	if (try_copy_to_user((void *)arg, &param, sizeof(param))) {
		loge("copy to_user fail\n");
		ret = COPYFAIL;
		goto end;
	}

end:
	if (mail_buf)
		kfree(mail_buf);

	input_param_free(&para_kernel_in);

	return ret;
}

static int sync_cmd_with_sem(uintptr_t arg)
{
	int ret;

	ret = down_interruptible(&s_dsp_sync_cmd_sem);
	if (ret != 0) {
		loge("user syncmsg wake up by other irq err: %d\n", ret);
		return ret;
	}

	ret = sync_cmd(arg);
	up(&s_dsp_sync_cmd_sem);

	return ret;
}

int sync_cmd_from_kernel(void *para_kernel_in, unsigned int para_kernel_size_in,
	void *para_kernel_out, unsigned int para_kernel_size_out)
{
	int ret = 0;
	unsigned int mail_len = para_kernel_size_out;
	void *para_kernel_in_with_cmd = NULL;
	unsigned int kernel_in_with_cmd_size = 0;
	struct dsp_chn_cmd *cmd_para = NULL;

	if (para_kernel_out == NULL || para_kernel_size_out == 0 ||
		para_kernel_size_in > (SIZE_LIMIT_PARAM - SIZE_CMD_ID) ||
		para_kernel_size_out > (SIZE_LIMIT_PARAM - SIZE_CMD_ID)) {
		loge("sync write param err: in size %u out size %u \n",
			para_kernel_size_in, para_kernel_size_out);
		goto end;
	}

	kernel_in_with_cmd_size = para_kernel_size_in + SIZE_CMD_ID;
	para_kernel_in_with_cmd = kzalloc(kernel_in_with_cmd_size, GFP_KERNEL);
	if (para_kernel_in_with_cmd == NULL) {
		loge("para_kernel_para_kernel_in_with_cmd malloc err\n");
		goto end;
	}

	memcpy(para_kernel_in_with_cmd, para_kernel_in, para_kernel_size_in);

	ret = down_interruptible(&s_dsp_sync_cmd_sem);
	if (ret != 0) {
		loge("kernel syncmsg wake up by other irq err: %d\n", ret);
		goto end;
	}

	/* add cmd id and sn  */
	cmd_para = (struct dsp_chn_cmd *)(para_kernel_in_with_cmd + para_kernel_size_in);
	cmd_para->cmd_type = HIFI_CHN_SYNC_CMD;
	cmd_para->sn = g_msg_priv.sn;

	ret = sync_write(para_kernel_in_with_cmd, kernel_in_with_cmd_size);
	if (ret != OK) {
		loge("sync write ret: %d\n", ret);
		up(&s_dsp_sync_cmd_sem);
		goto end;
	}

	up(&s_dsp_sync_cmd_sem);

	ret = rev_msg_get(para_kernel_out, &mail_len);
	if (ret != OK)
		goto end;

	if (para_kernel_size_out != mail_len) {
		loge("para_kernel_size_out err %u mail len %u\n", para_kernel_size_out, mail_len);
		goto end;
	}

end:
	if (para_kernel_in_with_cmd != NULL)
		kfree(para_kernel_in_with_cmd);

	return ret;
}

#ifdef CONFIG_HIFI_EXTEND_MISC
static bool is_valid_mmap_param(unsigned int flag)
{
	return (flag == AUDIO_PCM_DEVICE_MMAP ||
		(flag >= AUDIO_PCM_DEVICE_MMAP_EXTEND && flag <= AUDIO_PCM_DEVICE_MMAP_MAX + 1));
}

static unsigned int get_mmap_type(unsigned int flag)
{
    if (flag == AUDIO_PCM_DEVICE_MMAP)
		return DSP_MMAP_TYPE_MMAP;
	else
        return DSP_MMAP_TYPE_MMAP + (flag - AUDIO_PCM_DEVICE_MMAP_EXTEND + 1);
}
#endif

static int get_phys_cmd(uintptr_t arg)
{
	int ret = OK;
	struct misc_io_get_phys_param param;
	unsigned long para_addr_in;

	if (try_copy_from_user(&param, (void *)arg, sizeof(param))) {
		loge("copy from user fail\n");
		return ERROR;
	}

	if (param.flag == 0) {
		para_addr_in = dsp_misc_get_phys();
		param.phys_addr_l = GET_LOW32(para_addr_in);
		param.phys_addr_h = GET_HIG32(para_addr_in);
		logd("para addr in: 0x%pK\n", (void *)(uintptr_t)para_addr_in);
#ifdef CONFIG_HIFI_EXTEND_MISC
	} else if (is_valid_mmap_param(param.flag)) {
		unsigned int mmap_type = get_mmap_type(param.flag);
		if (mmap_type > DSP_MMAP_TYPE_MMAP_MAX) {
			loge("invalid mmap_type: %u\n", mmap_type);
			return ERROR;
		}

		para_addr_in = dsp_misc_get_mmap_phys(mmap_type);
		param.phys_addr_l = GET_LOW32(para_addr_in);
		param.phys_addr_h = GET_HIG32(para_addr_in);
		logd("para addr in: 0x%pK\n", (void *)(uintptr_t)para_addr_in);
#endif
	} else {
		ret = ERROR;
		loge("invalid flag: %u\n", param.flag);
	}

	if (try_copy_to_user((void *)arg, &param,
			sizeof(struct misc_io_get_phys_param))) {
		loge("copy to user fail\n");
		ret = ERROR;
	}

	return ret;
}

static int write_param(uintptr_t arg)
{
	int ret = OK;
	void *vir_addr = NULL;
	void *para_addr_in = NULL;
	void *para_addr_out = NULL;
	struct misc_io_sync_param para;

	if (try_copy_from_user(&para, (void *)arg, sizeof(para))) {
		loge("copy from user fail\n");
		return ERROR;
	}

	para_addr_in  = INT_TO_ADDR(para.para_in_l, para.para_in_h);
	para_addr_out = INT_TO_ADDR(para.para_out_l, para.para_out_h);

	vir_addr = (unsigned char *)(g_msg_priv.base_virt +
		(DSP_AP_NV_DATA_ADDR - DSP_UNSEC_BASE_ADDR));

	logd("vir addr: 0x%pK vir data: 0x%x\n", vir_addr, (*(int *)vir_addr));
	logd("user addr: 0x%pK, size: %u\n", para_addr_in, para.para_size_in);

	if ((para.para_size_in != NVPARAM_TOTAL_SIZE) && (para.para_size_in != NVPARAM_TOTAL_SIZE_EXP)) {
		loge("the para size in: %u is not equal to: %u or exp: %u\n",
			para.para_size_in, (unsigned int)(NVPARAM_TOTAL_SIZE), (unsigned int)(NVPARAM_TOTAL_SIZE_EXP));
		return ERROR;
	}

	if (try_copy_from_user(vir_addr, (void __user *)para_addr_in,
			para.para_size_in)) {
		loge("copy data to dsp error\n");
		ret = ERROR;
	}

	if (para.para_size_out != sizeof(ret)) {
		loge("the para size out: %u is not equal to sizeof ret: %zu\n",
			para.para_size_out, sizeof(ret));
		return ERROR;
	}

	if (try_copy_to_user((void __user *)para_addr_out, &ret, sizeof(ret))) {
		loge("copy data to user fail\n");
		ret = ERROR;
	}

	return ret;
}

static int write_audio_param(unsigned int addr, unsigned int len, uintptr_t arg)
{
	int ret = OK;
	unsigned long copy_ret = 0;
	void *vir_addr = NULL;
	void *para_addr_in = NULL;
	void *para_addr_out = NULL;
	struct misc_io_sync_param para;

	logi("addr:0x%pK, len:%u\n", (void *)(uintptr_t)addr, len);
	if (try_copy_from_user(&para, (void *)arg, sizeof(para))) {
		loge("copy from user fail\n");
		return ERROR;
	}

	if (para.para_size_in > len) {
		loge("the para size in: %u is greater than: %u\n", para.para_size_in, len);
		return ERROR;
	}

	if (para.para_size_out != sizeof(ret)) {
		loge("the para size out: %u is not equal to sizeof ret: %zu\n",
			para.para_size_out, sizeof(ret));
		return ERROR;
	}

	para_addr_in  = INT_TO_ADDR(para.para_in_l, para.para_in_h);
	para_addr_out = INT_TO_ADDR(para.para_out_l, para.para_out_h);
	vir_addr = (unsigned char *)(g_msg_priv.base_virt +
		(addr - DSP_UNSEC_BASE_ADDR));
	logd("vir addr: 0x%pK vir data: 0x%x\n", vir_addr, (*(int *)vir_addr));
	logd("user addr: 0x%pK, size: %u\n", para_addr_in, para.para_size_in);

	copy_ret = try_copy_from_user(vir_addr,
		(void __user *)para_addr_in, para.para_size_in);
	if (copy_ret != 0) {
		loge("copy data to dsp error ret: %lu\n", copy_ret);
		ret = ERROR;
	}

	copy_ret = try_copy_to_user((void __user *)para_addr_out, &ret, sizeof(ret));
	if (copy_ret != 0) {
		loge("copy data to user fail ret: %lu\n", copy_ret);
		ret = ERROR;
	}

	return ret;
}

static int write_audio_effect_param(uintptr_t arg)
{
	return write_audio_param(DSP_AUDIO_EFFECT_PARAM_ADDR,
		DSP_AUDIO_EFFECT_PARAM_BUFF_SIZE, arg);
}

static int write_audio_effect_histen_param(uintptr_t arg)
{
	return write_audio_param(DSP_AUDIO_EFFECT_HISTEN_PARAM_ADDR,
		DSP_AUDIO_EFFECT_HISTEN_PARAM_BUFF_SIZE, arg);
}

static int write_smartpa_param(uintptr_t arg)
{
	return write_audio_param(AP_AUDIO_PA_ADDR, AP_AUDIO_PA_BUFF_SIZE, arg);
}

static int write_psz_scene_param(uintptr_t arg)
{
	return write_audio_param(DSP_AUDIO_PSZ_SCENE_ADDR, DSP_AUDIO_PSZ_SCENE_SIZE, arg);
}

static int write_psz_calib_param(uintptr_t arg)
{
	return write_audio_param(DSP_AUDIO_PSZ_CALIB_ADDR, DSP_AUDIO_PSZ_CALIB_SIZE, arg);
}

#ifdef CONFIG_HUAWEI_AUDIO
static int write_sound_enhance_param(uintptr_t arg)
{
	return write_audio_param(HIFI_SOUND_ENHANCE_PARA_ADDR,
		DSP_SOUND_ENHANCE_BUFF_SIZE, arg);
}
#endif

#ifdef CONFIG_USB_AUDIO_DSP
static int usbaudio_cmd(uintptr_t arg)
{
	void *para_addr_in = NULL;
	void *para_addr_out = NULL;
	struct usbaudio_ioctl_input input;
	struct usbaudio_info output;
	struct misc_io_sync_param para;

	memset(&input, 0, sizeof(input));
	memset(&output, 0, sizeof(output));
	memset(&para, 0, sizeof(para));

	if (try_copy_from_user(&para, (void *)arg, sizeof(para))) {
		loge("copy from user fail\n");
		return ERROR;
	}

	if (para.para_size_out != sizeof(output)) {
		loge("wrong ioctl output size: %u should be %zu\n",
			para.para_size_out, sizeof(output));
		return ERROR;
	}

	para_addr_in  = INT_TO_ADDR(para.para_in_l, para.para_in_h);
	para_addr_out = INT_TO_ADDR(para.para_out_l, para.para_out_h);

	if (para.para_size_in != sizeof(input)) {
		loge("wrong ioctl input size.size: %u should be %zu\n",
			para.para_size_in, sizeof(input));
		return ERROR;
	}

	if (try_copy_from_user(&input, (void __user *)para_addr_in,
			para.para_size_in)) {
		loge("copy ioctl input data to kernel error\n");
		return ERROR;
	}

	if (input.msg_type == (unsigned int)USBAUDIO_QUERY_INFO) {
		/* query controller state */
		usbaudio_ctrl_query_info(input.input1, &output);
	} else if (input.msg_type == (unsigned int)USBAUDIO_USB_POWER_RESUME) {
		output.sr_status = usbaudio_ctrl_usb_resume();
	} else if (input.msg_type == (unsigned int)USBAUDIO_NV_ISREADY) {
		usbaudio_set_nv_ready();
	} else {
		/* do nothing */
		loge("message type error %x\n", input.msg_type);
	}

	if (try_copy_to_user((void __user *)para_addr_out, &output, sizeof(output))) {
		loge("copy data to user fail\n");
		return ERROR;
	}

	return 0;
}
#endif

#ifdef CONFIG_SOUND_TRIGGER
static int cmd_switch(const struct soundtrigger_sync_msg *input, uint32_t input_para_size, int *output)
{
	switch (input->msg_type) {
	case WAKEUP_CHN_MSG_START:
		*output = start_recognition_msg(input->msg_type, input->module_id, false);
		break;
	case LP_WAKEUP_CHN_MSG_START:
		*output = start_recognition_msg(input->msg_type, input->module_id, true);
		break;
	case WAKEUP_CHN_MSG_PARAMETER_SET:
	case LP_WAKEUP_CHN_MSG_PARAMETER_SET:
		*output = parameter_set_msg(input->msg_type,
			input->module_id, &input->set_param, input_para_size);
		break;
	case WAKEUP_CHN_MSG_PARAMETER_GET:
	case LP_WAKEUP_CHN_MSG_PARAMETER_GET:
	case LP_WAKEUP_CHN_MSG_STATUS_GET:
		get_handle_msg(input->msg_type, output);
		break;
	case WAKEUP_CHN_MSG_STOP:
		*output = stop_recognition_msg(input->msg_type, input->module_id, false);
		break;
	case LP_WAKEUP_CHN_MSG_STOP:
		*output = stop_recognition_msg(input->msg_type, input->module_id, true);
		break;
	default:
		/* do nothing */
		loge("message type error%x\n", input->msg_type);
		return ERROR;
	}

	return OK;
}

static int soundtrigger_cmd(uintptr_t arg)
{
	int ret;
	void *para_addr_in = NULL;
	void *para_addr_out = NULL;
	struct soundtrigger_sync_msg *input = NULL;
	int output = 0;
	struct misc_io_sync_param para;

	memset(&para, 0, sizeof(para));

	if (try_copy_from_user(&para, (void *)arg, sizeof(para))) {
		loge("copy from user fail\n");
		return ERROR;
	}

	if (para.para_size_out != sizeof(output)) {
		loge("wrong ioctl output size.size: %u should be: %zu\n",
			para.para_size_out, sizeof(output));
		return ERROR;
	}

	para_addr_in  = INT_TO_ADDR(para.para_in_l, para.para_in_h);
	para_addr_out = INT_TO_ADDR(para.para_out_l, para.para_out_h);

	if (para.para_size_in < sizeof(*input) ||
		para.para_size_in > (sizeof(*input) + AP_AUDIO_WAKEUP_MODEL_SIZE)) {
		loge("wrong ioctl input size.size: %u\n", para.para_size_in);
		return ERROR;
	}

	input = vmalloc(para.para_size_in);
	if (!input) {
		loge("vmalloc fail\n");
		return ERROR;
	}

	if (try_copy_from_user((void *)input,
		(void __user *)para_addr_in, para.para_size_in)) {
		loge("copy ioctl input data to kernel error\n");
		vfree(input);
		return ERROR;
	}

	ret = cmd_switch(input, (para.para_size_in - sizeof(*input)), &output);

	if (try_copy_to_user((void __user *)para_addr_out, &output, sizeof(output))) {
		loge("copy data to user fail\n");
		ret = ERROR;
	}

	vfree(input);
	return ret;
}
#endif



#define msg_func(id, func) { id, func, #id }

static const struct dsp_ioctl_cmd cmd_func[] = {
	msg_func(HIFI_MISC_IOCTL_ASYNCMSG, async_cmd),
	msg_func(HIFI_MISC_IOCTL_GET_PHYS, get_phys_cmd),
	msg_func(HIFI_MISC_IOCTL_WRITE_PARAMS, write_param),
	msg_func(HIFI_MISC_IOCTL_AUDIO_EFFECT_PARAMS, write_audio_effect_param),
#ifdef CONFIG_HUAWEI_AUDIO
	msg_func(HIFI_MISC_IOCTL_SOUND_ENHANCE_PARAMS, write_sound_enhance_param),
#endif
	msg_func(HIFI_MISC_IOCTL_DUMP_HIFI, socdsp_dump),
#ifdef CONFIG_HUAWEI_AUDIO
	msg_func(HIFI_MISC_IOCTL_GET_VOICE_BSD_PARAM, socdsp_om_get_voice_bsd_param),
#endif
	msg_func(HIFI_MISC_IOCTL_WAKEUP_THREAD, soc_dsp_wakeup_read_thread),
	msg_func(HIFI_MISC_IOCTL_WAKEUP_PCM_READ_THREAD, soc_dsp_wakeup_pcm_read_thread),
	msg_func(HIFI_MISC_IOCTL_WAKEUP_PARA_WRITE_THREAD, soc_dsp_wakeup_para_write_thread),
#ifdef CONFIG_USB_AUDIO_DSP
	msg_func(HIFI_MISC_IOCTL_USBAUDIO, usbaudio_cmd),
#endif
	msg_func(HIFI_MISC_IOCTL_SMARTPA_PARAMS, write_smartpa_param),
	msg_func(HIFI_MISC_IOCTL_PSZ_SCENE_PARAMS, write_psz_scene_param),
	msg_func(HIFI_MISC_IOCTL_PSZ_CALIB_PARAMS, write_psz_calib_param),
#ifdef CONFIG_SOUND_TRIGGER
	msg_func(HIFI_MISC_IOCTL_SOUNDTRIGGER, soundtrigger_cmd),
#endif
	msg_func(HIFI_MISC_IOCTL_AUDIO_EFFECT_HISTEN_PARAMS, write_audio_effect_histen_param),
	msg_func(HIFI_MISC_IOCTL_GET_DSP_TIME_INFO, get_dsp_time),
};

long dsp_msg_process_cmd(unsigned int cmd, uintptr_t data32)
{
	long ret = ERROR;
	unsigned int i;

	if (ARRAY_SIZE(cmd_func) == 0) {
		loge("cmd proc is not register\n");
		return ret;
	}

	for (i = 0; i < ARRAY_SIZE(cmd_func); i++) {
		if (cmd == cmd_func[i].id) {
			logi("ioctl: %s\n", cmd_func[i].name);
			ret = cmd_func[i].func(data32);

			return ret;
		}
	}

	return ret;
}

void dsp_msg_init(struct dsp_misc_proc *misc_proc, unsigned char *base_virt,
	unsigned char *base_phy)
{
	memset(&g_msg_priv, 0, sizeof(g_msg_priv));

	spin_lock_init(&g_msg_priv.recv_sync_lock);
	init_completion(&g_msg_priv.completion);

	g_msg_priv.sn = 0;
	g_msg_priv.base_virt = base_virt;
	g_msg_priv.base_phy = base_phy;

	misc_proc->sync_msg_proc = sync_cmd_with_sem;
}

void dsp_msg_deinit(void)
{
	logi("msg deinit\n");
}

