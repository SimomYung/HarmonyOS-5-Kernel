/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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
 */
#include "pcs_rrc_msg_proc.h"

#include <linux/list.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/in.h>
#include <linux/vmalloc.h>

#include "securec.h"

#include "rrc_pcs_interface.h"
#include "pcs_bc_cap_get_dir.h"
#include "pcs_bc_cap_dir.h"
#include "pcs_log.h"

struct pcs_rrc_list_s {
	struct list_head list;
	u32 modem_id;
	u32 rcv_pid;
	u32 op_id;
	u32 ind_rslt;
	u32 data_len;
	u8 *data;
};

typedef void (*pcs_rrc_msg_proc_func)(void *data, u32 len);

struct pcs_rrc_msg_proc_tbl {
	u32 msg_type;
	pcs_rrc_msg_proc_func func;
};

struct pcs_rrc_ind_rslt_trans_tbl_s {
	int rslt;
	u32 ind_rslt;
};

struct pcs_rrc_read_diag_info {
	ps_log_third_head_t head;
	enum pcs_diag_cause cause;
	unsigned int msg_type;
	unsigned int modem_id;
	unsigned int pid;
	unsigned int op_id;
};

#define PCS_DIR_MAX_LEN 256
#define PCS_RRC_PATH_MAX_LEN (PCS_DIR_MAX_LEN + RRC_PCS_MAX_FILE_DIR_LEN)

#define PCS_RRC_NODE_MAX_NUM 1000

LIST_HEAD(pcs_rrc_list);

STATIC void pcs_show_rrc_req(struct rrc_pcs_read_file_req *req)
{
	PCS_LOGI("msg_type: %d, modem_id: %d, send_pid: %d, "
		  "op_id: %d, dir_len: %d",
		  req->msg_type, req->modem_id, req->send_pid,
		  req->op_id, req->dir_len);
}

STATIC void pcs_show_rrc_ind(struct pcs_rrc_read_file_ind *ind)
{
	PCS_LOGI("msg_type: %d, modem_id: %d, rcv_pid: %d, op_id: %d, "
		  "ind_rslt: %d, data_len: %d",
		  ind->msg_type, ind->modem_id, ind->rcv_pid, ind->op_id,
		  ind->ind_rslt, ind->data_len);
}

STATIC void pcs_show_rrc_ack(struct rrc_pcs_read_file_ack *ack)
{
	PCS_LOGI("msg_type: %d, modem_id: %d, send_pid: %d, "
		  "op_id: %d, ack_rslt: %d",
		  ack->msg_type, ack->modem_id, ack->send_pid,
		  ack->op_id, ack->ack_rslt);
}

STATIC void pcs_show_rrc_list(void)
{
	struct pcs_rrc_list_s *node = NULL;
	struct list_head *pos = NULL;

	PCS_LOGI("entry");
	list_for_each(pos, &pcs_rrc_list) {
		node = list_entry(pos, struct pcs_rrc_list_s, list);
		PCS_LOGI("modem_id: %d, rcv_pid: %d, op_id: %d, "
			  "ind_rslt: %d, data_len: %d",
			  node->modem_id, node->rcv_pid, node->op_id,
			  node->ind_rslt, node->data_len);
	}
}

STATIC void pcs_rrc_read_req_trans(struct rrc_pcs_read_file_req *req, enum pcs_diag_cause cause)
{
	struct pcs_rrc_read_diag_info info = {0};
	info.head.snd_pid = PCS_FIX_PID;
	info.cause = cause;

	if (req != NULL) {
		info.msg_type = req->msg_type;
		info.modem_id = req->modem_id;
		info.pid = req->send_pid;
		info.op_id = req->op_id;
	}

	pcs_trans_report(ID_DIAG_PCS_RRC_READ_INFO, &info, sizeof(info));
}

STATIC void pcs_rrc_read_ack_trans(struct rrc_pcs_read_file_ack *ack, enum pcs_diag_cause cause)
{
	struct pcs_rrc_read_diag_info info = {0};
	info.head.snd_pid = PCS_FIX_PID;
	info.cause = cause;

	if (ack != NULL) {
		info.msg_type = ack->msg_type;
		info.modem_id = ack->modem_id;
		info.pid = ack->send_pid;
		info.op_id = ack->op_id;
	}

	pcs_trans_report(ID_DIAG_PCS_RRC_READ_INFO, &info, sizeof(info));
}

STATIC void pcs_rrc_clear_ctx_node(struct pcs_rrc_list_s *node)
{
	if (node == NULL)
		return;

	list_del(&node->list);
	if (node->data != NULL)
		vfree(node->data);

	vfree(node);
	node = NULL;
}

STATIC struct pcs_rrc_list_s *pcs_rrc_get_ctx_node(u32 modem_id, u32 send_pid,
						   u32 op_id)
{
	struct list_head *pos = NULL;
	struct pcs_rrc_list_s *node = NULL;

	list_for_each(pos, &pcs_rrc_list) {
		node = list_entry(pos, struct pcs_rrc_list_s, list);
		if (node->modem_id == modem_id && node->rcv_pid == send_pid &&
		    node->op_id == op_id)
			break;
	}

	if (pos == &pcs_rrc_list)
		return NULL;

	return node;
}

STATIC int pcs_rrc_get_ind(struct pcs_rrc_read_file_ind **ind, u32 *ind_len,
		       struct pcs_rrc_list_s *node)
{
	u32 ind_data_len = node->data_len + 1;

	*ind_len = sizeof(**ind) + ind_data_len;
	*ind = vzalloc(*ind_len);
	if (*ind == NULL) {
		PCS_DIAG_LOGE("vzalloc fail");
		return -ENOMEM;
	}

	(*ind)->msg_type = PCS_RRC_READ_FILE_IND;
	(*ind)->modem_id = node->modem_id;
	(*ind)->rcv_pid = node->rcv_pid;
	(*ind)->op_id = node->op_id;
	(*ind)->ind_rslt = node->ind_rslt;
	(*ind)->data_len = ind_data_len;
	if ((*ind)->ind_rslt != PCS_RRC_READ_FILE_IND_SUCC)
		return 0;

	if (memcpy_s((*ind)->data, ind_data_len, node->data, node->data_len)) {
		PCS_DIAG_LOGE("memcpy fail");
		vfree(*ind);
		*ind = NULL;
		return -EINVAL;
	}

	return 0;
}

STATIC void pcs_rrc_send_msg(struct pcs_rrc_list_s *node)
{
	struct pcs_rrc_read_file_ind *ind = NULL;
	u32 ind_len;

	if (pcs_rrc_get_ind(&ind, &ind_len, node))
		return;

	pcs_show_rrc_ind(ind);
	pcs_show_rrc_list();

	if (pcs_send_msg((struct pcs_msg_head *)ind, ind_len))
		PCS_DIAG_LOGE("send_msg fail");

	vfree(ind);
}

STATIC int pcs_rrc_get_file_dir(struct rrc_pcs_read_file_req *req,
				char *dir, u32 dir_len)
{
	if (memcpy_s(dir, dir_len, req->dir, req->dir_len)) {
		PCS_DIAG_LOGE("memcpy_s fail, dir = %s", dir);
		return -ENOENT;
	}

	return 0;
}

STATIC int pcs_rrc_get_path(char *path, u32 path_len,
			    struct rrc_pcs_read_file_req *req)
{
	char dir[PCS_DIR_MAX_LEN] = {0};
	char file_dir[RRC_PCS_MAX_FILE_DIR_LEN] = {0};
	int dir_len;
	int ret;

	dir_len = pcs_get_bc_cap_dir(dir, sizeof(dir));
	ret = pcs_rrc_get_file_dir(req, file_dir, sizeof(file_dir));
	if (dir_len <= 0 || ret != 0)
		return -ENOENT;

	(void)memset_s(path, path_len, 0, path_len);

	ret |= memcpy_s(path, path_len, dir, dir_len);
	ret |= strncat_s(path, path_len, file_dir, strlen(file_dir));
	if (ret) {
		PCS_DIAG_LOGE("path cat fail");
		return -ENOENT;
	}

	return 0;
}

STATIC int pcs_rrc_read_data_to_node(struct file *fp, struct pcs_rrc_list_s *node,
				     struct rrc_pcs_read_file_req *req)
{
	u8 **data = &node->data;
	loff_t size;
	loff_t pos = 0;
	u32 read_size;

	size = i_size_read(file_inode(fp));
	if (size == 0) {
		PCS_DIAG_LOGE("size invalid");
		pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_READ_FILE_FAILED);
		return -EIO;
	}

	if (size > PCS_RRC_IND_DATA_MAX_LEN) {
		PCS_DIAG_LOGE("size too large");
		pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_READ_FILE_FAILED);
		return -EFBIG;
	}

	node->data_len = size;
	*data = vzalloc(size);
	if (*data == NULL) {
		PCS_DIAG_LOGE("vzalloc fail");
		pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_ALLOC_BUFF_FAILED);
		return -ENOMEM;
	}

	read_size = kernel_read(fp, *data, size, &pos);
	if (read_size != size) {
		PCS_DIAG_LOGE("kernel_read fail");
		pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_READ_FILE_FAILED);
		vfree(*data);
		*data = NULL;
		return -EIO;
	}

	return 0;
}

STATIC int pcs_rrc_get_data(struct pcs_rrc_list_s *node, struct rrc_pcs_read_file_req *req)
{
	struct file *fp = NULL;
	char path[PCS_RRC_PATH_MAX_LEN] = {0};
	int ret;
	int close_ret;

	ret = pcs_rrc_get_path(path, sizeof(path), req);
	if (ret)
		return ret;

	if (!pcs_bc_cap_verify_dir(path, sizeof(path)))
		return -ENOENT;

	fp = filp_open(path, O_RDONLY, 0);
	if (IS_ERR_OR_NULL(fp)) {
		PCS_DIAG_LOGE("open file fail, path =  %s", path);
		return -ENOENT;
	}

	ret = pcs_rrc_read_data_to_node(fp, node, req);
	close_ret = filp_close(fp, NULL);
	if (close_ret)
		PCS_DIAG_LOGE("filp_close fail, ret = %d", close_ret);

	return ret;
}

STATIC int pcs_rrc_init_node(struct rrc_pcs_read_file_req *req,
				struct pcs_rrc_list_s *node)
{
	if (node->data != NULL) {
		vfree(node->data);
		node->data = NULL;
	}

	node->modem_id = req->modem_id;
	node->rcv_pid = req->send_pid;
	node->op_id = req->op_id;
	node->ind_rslt = PCS_RRC_READ_FILE_IND_SUCC;
	node->data_len = 0;

	return pcs_rrc_get_data(node, req);
}

STATIC struct pcs_rrc_ind_rslt_trans_tbl_s pcs_rrc_ind_rslt_trans_tbl[PCS_RRC_READ_FILE_IND_BUTT] = {
	{ 0, PCS_RRC_READ_FILE_IND_SUCC },
	{ -ENOMEM, PCS_RRC_READ_FILE_IND_NO_MEM },
	{ -EFBIG, PCS_RRC_READ_FILE_IND_TOO_LARGE },
	{ -EIO, PCS_RRC_READ_FILE_IND_READ_FAILED },
	{ -ENOENT, PCS_RRC_READ_FILE_IND_PATH_INVALID },
};

STATIC u32 pcs_rrc_trans_ind_rslt(int rslt)
{
	int i;
	for (i = 0; i < PCS_RRC_READ_FILE_IND_BUTT; i++) {
		if (pcs_rrc_ind_rslt_trans_tbl[i].rslt == rslt)
			return pcs_rrc_ind_rslt_trans_tbl[i].ind_rslt;
	}

	PCS_DIAG_LOGE("invalid rslt = %d", rslt);
	return PCS_RRC_READ_FILE_IND_BUTT;
}

STATIC void pcs_rrc_send_err_msg(u32 modem_id, u32 send_pid,
				 u32 op_id, u32 ind_rslt)
{
	struct pcs_rrc_read_file_ind ind = {0};

	ind.msg_type = PCS_RRC_READ_FILE_IND;
	ind.modem_id = modem_id;
	ind.rcv_pid = send_pid;
	ind.op_id = op_id;
	ind.ind_rslt = pcs_rrc_trans_ind_rslt(ind_rslt);
	ind.data_len = 0;

	pcs_show_rrc_ind(&ind);

	if (pcs_send_msg((struct pcs_msg_head *)(&ind), sizeof(ind)))
		PCS_DIAG_LOGE("send_msg fail");
}

STATIC void pcs_rrc_del_first_node(void)
{
	struct pcs_rrc_list_s *node = NULL;

	pcs_show_rrc_list();

	node = list_first_entry(&pcs_rrc_list, struct pcs_rrc_list_s, list);
	pcs_rrc_clear_ctx_node(node);
}

STATIC void pcs_rrc_req_proc(void *data, u32 len)
{
	struct rrc_pcs_read_file_req *req = (struct rrc_pcs_read_file_req *)data;
	struct pcs_rrc_list_s *node = NULL;
	struct list_head *pos = NULL;
	u32 cnt = 0;
	u32 ret;

	if (len < sizeof(*req)) {
		pcs_rrc_read_req_trans(NULL, PCS_DIAG_CAUSE_RCV_LEN_INVALID);
		return;
	}

	if (req->dir_len > PCS_DIR_MAX_LEN ||
	    len != sizeof(*req) + req->dir_len ||
	    req->dir_len != (strnlen(req->dir, len - sizeof(*req)) + 1)) {
		pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_RCV_DIR_LEN_INVALID);
		return;
	}

	pcs_show_rrc_req(req);
	pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_RCV_ENTRY);

	node = pcs_rrc_get_ctx_node(req->modem_id, req->send_pid, req->op_id);
	/*
	 * before the current data transmission is complete,
	 * If a new request is received with the same
	 * modem_id & send_pid & op_id,
	 * the req information is assigned to the node with the same ID
	 * instead of vzalloc a new node.
	 */
	if (node == NULL) {
		list_for_each(pos, &pcs_rrc_list)
			cnt++;

		if (cnt >= PCS_RRC_NODE_MAX_NUM) {
			pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_TOO_MANY_NODE);
			pcs_rrc_del_first_node();
		}

		node = vzalloc(sizeof(*node));
		if (node == NULL) {
			pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_ALLOC_NODE_FAILED);
			ret = -ENOMEM;
			goto err;
		}

		INIT_LIST_HEAD(&node->list);
		list_add_tail(&node->list, &pcs_rrc_list);
	}

	ret = pcs_rrc_init_node(req, node);
	if (ret)
		goto init_node_err;

	pcs_rrc_send_msg(node);
	pcs_rrc_read_req_trans(req, PCS_DIAG_CAUSE_SEND_ENTRY);
	return;

init_node_err:
	pcs_rrc_clear_ctx_node(node);

err:
	pcs_rrc_send_err_msg(req->modem_id, req->send_pid, req->op_id, ret);
}

STATIC void pcs_rrc_ack_proc(void *data, u32 len)
{
	struct rrc_pcs_read_file_ack *ack = (struct rrc_pcs_read_file_ack *)data;
	struct pcs_rrc_list_s *node = NULL;

	if (len != sizeof(*ack)) {
		pcs_rrc_read_ack_trans(NULL, PCS_DIAG_CAUSE_RCV_LEN_INVALID);
		return;
	}

	pcs_show_rrc_ack(ack);
	pcs_rrc_read_ack_trans(ack, PCS_DIAG_CAUSE_RCV_ENTRY);

	node = pcs_rrc_get_ctx_node(ack->modem_id, ack->send_pid, ack->op_id);
	if (node == NULL)
		return;

	if (ack->ack_rslt != RRC_PCS_READ_FILE_ACK_SUCC) {
		pcs_rrc_read_ack_trans(ack, PCS_DIAG_CAUSE_ACK_PARAM_ABNORMAL);
	}

	pcs_rrc_clear_ctx_node(node);
}

struct pcs_rrc_msg_proc_tbl pcs_rrc_proc_tbl[] = {
	{ RRC_PCS_READ_FILE_REQ, pcs_rrc_req_proc },
	{ RRC_PCS_READ_FILE_ACK, pcs_rrc_ack_proc },
};

void pcs_rrc_msg_proc(struct pcs_msg_head *head, u32 len)
{
	u32 i;
	for (i = 0; i < ARRAY_SIZE(pcs_rrc_proc_tbl); i++) {
		if (head->msg_type == pcs_rrc_proc_tbl[i].msg_type)
			return pcs_rrc_proc_tbl[i].func(head, len);
	}

	PCS_DIAG_LOGE("msg type[%d] invalid", head->msg_type);
	return;
}

int pcs_rrc_init(void)
{
	int ret = pcs_bc_cap_init_get_dir();
	if (!ret)
		INIT_LIST_HEAD(&pcs_rrc_list);

	return ret;
}

void pcs_rrc_deinit(void)
{
	struct pcs_rrc_list_s *node = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;

	list_for_each_safe(pos, n, &pcs_rrc_list) {
		node = list_entry(pos, struct pcs_rrc_list_s, list);
		list_del(&node->list);
		if (node->data != NULL)
			vfree(node->data);

		vfree(node);
		node = NULL;
	}
}
