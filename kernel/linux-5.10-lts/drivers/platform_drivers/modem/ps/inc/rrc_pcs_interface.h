/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
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
 * *	notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *	notice, this list of conditions and the following disclaimer in the
 * *	documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *	be used to endorse or promote products derived from this software
 * *	without specific prior written permission.
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

#ifndef RRC_PCS_INTERFACE_H
#define RRC_PCS_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 4)

/*
 * The following is used in cross-core messages, so the bit width occupied
 * by the enumeration members in the structure must be explicitly defined,
 * the enumeration type of the member can be searched according to the name.
 */

#define RRC_PCS_MAX_FILE_DIR_LEN 255
#define PCS_RRC_IND_DATA_MAX_LEN 8000

enum rrc_pcs_msg_type {
	RRC_PCS_READ_FILE_REQ = 0,
	PCS_RRC_READ_FILE_IND,
	RRC_PCS_READ_FILE_ACK,

	RRC_PCS_MSG_BUTT,
};

enum pcs_rrc_read_file_ind_rslt {
	PCS_RRC_READ_FILE_IND_SUCC = 0,

	PCS_RRC_READ_FILE_IND_NO_MEM,
	PCS_RRC_READ_FILE_IND_TOO_LARGE,
	PCS_RRC_READ_FILE_IND_READ_FAILED,
	PCS_RRC_READ_FILE_IND_PATH_INVALID,
	PCS_RRC_READ_FILE_IND_BUTT
};

enum rrc_pcs_read_file_ack_rslt {
	RRC_PCS_READ_FILE_ACK_SUCC = 0,
	RRC_PCS_READ_FILE_ACK_FAIL,
	RRC_PCS_READ_FILE_ACK_BUTT
};

struct rrc_pcs_read_file_req {
	unsigned int msg_type;
	unsigned int modem_id;
	unsigned int send_pid;
	unsigned int op_id;
	unsigned int dir_len;
	char dir[0];
};

struct pcs_rrc_read_file_ind {
	unsigned int msg_type;
	unsigned int modem_id;
	unsigned int rcv_pid;
	unsigned int op_id;
	unsigned int ind_rslt;
	unsigned char rsv[2];
	unsigned short data_len;
	unsigned char data[0];
};

struct rrc_pcs_read_file_ack {
	unsigned int msg_type;
	unsigned int modem_id;
	unsigned int send_pid;
	unsigned int op_id;
	unsigned int ack_rslt;
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
