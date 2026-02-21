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

#ifndef VOICE_PROXY_INTERFACE_H
#define VOICE_PROXY_INTERFACE_H

#include <linux/types.h>

#define PROXY_VOICE_CODEC_MAX_DATA_LEN 32       /* 16 bit */

/* the MsgID define between PROXY and Voice */
enum voice_proxy_voice_msg_id {
	ID_PROXY_VOICE_LTE_RX_NTF  = 0xDDF0,
	ID_VOICE_PROXY_LTE_RX_NTF  = 0xDDF2,
	ID_VOICE_PROXY_LTE_TX_NTF  = 0xDDF4,
	ID_PROXY_VOICE_LTE_TX_NTF  = 0xDDF6,

	ID_PROXY_VOICE_ENCRYPT_KEY_BEGIN = 0xDDFD,
	ID_PROXY_VOICE_ENCRYPT_KEY_END   = 0xDDFE,

	ID_PROXY_VOICE_RTT_TX_NTF = 0xDFD1,
	ID_VOICE_PROXY_RTT_RX_IND = 0xDFD2,
	ID_PROXY_VOICE_DATA_MSGID_BUT
};

/*
 * describe: the struct of the Rx request between PROXY and modem_voice by lte
 * size of voice_proxy_lte_rx_notify should be equal to ps_unpacked_rx_data
 */
struct voice_proxy_lte_rx_notify {
	uint16_t msg_id;
	uint16_t reverse1;
	uint16_t codec_type;
	uint16_t frame_type;
	uint16_t data[PROXY_VOICE_CODEC_MAX_DATA_LEN];
	uint16_t para_len;
	uint16_t reverse2;
	uint8_t  para[0];
};

/* describe: the struct of the Tx request between Voice-Proxy and modem_voice by lte */
struct voice_proxy_lte_tx_notify {
	uint16_t msg_id;
	uint16_t codec_type;
	uint16_t amr_type;
	uint16_t frame_type;
	uint16_t data[PROXY_VOICE_CODEC_MAX_DATA_LEN];
	uint16_t para_len;
	uint16_t reverse1;
	uint8_t  para[0];
};

struct voice_proxy_voice_encrypt_key_end {
	uint16_t msg_id;
	uint16_t reserved;
	uint8_t  encrypt_negotiation_result;
	uint8_t  reserved2[3];
};

struct proxy_voice_rtt_tx_ntf {
    uint16_t msg_id;
    uint16_t channel_id;
    uint32_t data_len;
    uint8_t  data[0];
};
 
struct voice_proxy_rtt_rx_ind {
    uint16_t msg_id;
    uint16_t channel_id;
    uint32_t data_len;
    uint8_t  data[0];
};

#endif /* VOICE_PROXY_INTERFACE_H */

