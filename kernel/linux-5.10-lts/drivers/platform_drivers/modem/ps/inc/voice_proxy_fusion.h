/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef VOICE_PROXY_FUSION_H
#define VOICE_PROXY_FUSION_H

#include <linux/types.h>

#define VPEER_VOICE_RTP_MAX_DATA_LEN 1000

/* ID_PROXY_VOICE_FUSIONCALL_CTRL_IND */
struct proxy_fusion_voice_ntf {
	uint16_t msg_name; /* voice_proxy_fusioncall_msg_id */
	uint16_t reserve1;
	uint32_t msg_len;
	uint8_t msg[4];
};

/* ID_VPEER_VOICE_RX_RTP_IND */
struct vpeer_voice_rx_rtp_ntf {
	uint16_t msg_id;
	uint16_t channel_id;
	uint16_t port_type; /* Port Type: 0 RTP; 1 RTCP */
	uint16_t data_len;
	uint8_t data[0];
};

/* ID_VOICE_VPEER_TX_RTP_IND */
struct voice_vpeer_tx_rtp_ntf {
	uint16_t msg_id;
	uint16_t channel_id;
	uint16_t port_type; /* Port Type: 0 RTP; 1 RTCP */
	uint16_t data_len;
	uint8_t data[0];
};

/* fusioncall defined here */
enum voice_proxy_fusioncall_msg_id {
	ID_PROXY_VOICE_FUSIONCALL_CTRL_IND = 0xDF4E, /* FusionCall->HIFI */
	ID_VOICE_PROXY_FUSIONCALL_CTRL_IND = 0xDF4F, /* HIFI->FusionCall */
	ID_PROXY_VOICE_VPEER_CTRL_NTF = 0xDF50, /* VPEER->HIFI, rx data */
	ID_VOICE_PROXY_VPEER_CTRL_NTF = 0xDF52, /* HIFI->VPEER tx data */
};

#endif /* VOICE_PROXY_FUSION_H */
