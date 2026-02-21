/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "lbuf_mgr_v8xx.h"

#define DISP_LOG_TAG "RES_MGR"

static uint32_t get_lb_static_reserved_num(int32_t part_id)
{
	uint32_t index;
	uint32_t lb_num = 0;
	struct lbuf_static_reserved_config *lb_static_cfg;

	struct lbuf_static_reserved_configs *lb_static_reserved_cfgs = dpu_get_lb_static_reserved_config();
	if (!lb_static_reserved_cfgs) {
		dpu_pr_warn("[LB] lb_static_reserved_config is nullptr");
		return lb_num;
	}

	for (index = 0; index < lb_static_reserved_cfgs->config_num; index++) {
		lb_static_cfg = &(lb_static_reserved_cfgs->lb_static_reserved_cfg[index]);
		if (lb_static_cfg->part_id != part_id)
			continue;

		lb_num += lb_static_cfg->reserved_lb;
	}

	return lb_num;
}

static void init_lbuf_node(const struct lbuf_part_res_config_info *lb_cfg_info,
	struct lbuf_node_info *lb_part_info)
{
	uint32_t reserve_lbnum = 0;

	sema_init(&lb_part_info->sem, 1);
	lb_part_info->part_id = (uint32_t)lb_cfg_info->part_id;
	lb_part_info->used_lb = 0;
	reserve_lbnum = get_lb_static_reserved_num(lb_cfg_info->part_id);
	lb_part_info->max_lb = lb_cfg_info->max_lb - reserve_lbnum;
	INIT_LIST_HEAD(&lb_part_info->used_node_head);
	INIT_LIST_HEAD(&lb_part_info->reserved_node_head);

	dpu_pr_info("[LB] init lbuf part_id=%d node succ: max_lb=%u, static reserve num=%u",
		lb_cfg_info->part_id, lb_cfg_info->max_lb, reserve_lbnum);
}

static int init_lbuf_pool(struct dpu_lbuf_mgr *lbuf_mgr)
{
	uint32_t index;
	struct lbuf_part_node_config_info *lb_part_node_config_info = dpu_get_lb_part_node_config_info();

	if (!lb_part_node_config_info) {
		dpu_pr_err("[LB] lb_part_node_config_info is nullptr");
		return -1;
	}

	lbuf_mgr->part_num = lb_part_node_config_info->part_num;
	if (lbuf_mgr->part_num == 0) {
		dpu_pr_err("[LB] part_num's value cannot be zero!\n");
		return -1;
	}

	lbuf_mgr->lb_part_info = kzalloc(sizeof(struct lbuf_node_info) * lbuf_mgr->part_num, GFP_KERNEL);
	if (!lbuf_mgr->lb_part_info) {
		dpu_pr_err("[LB] kzalloc lb pool info fail");
		return -ENOMEM;
	}

	for (index = 0; index < lbuf_mgr->part_num; ++index) {
		init_lbuf_node(&lb_part_node_config_info->lb_cfg_info[index],
			&lbuf_mgr->lb_part_info[index]);
		dpu_pr_info("[LB] init lbuf part %u succ", index);
	}

	return 0;
}

void *lbuf_mgr_init(struct dpu_res_data *rm_data)
{
	struct dpu_lbuf_mgr *lbuf_mgr = kzalloc(sizeof(struct dpu_lbuf_mgr), GFP_KERNEL);
	if (!lbuf_mgr)
		return NULL;

	if (init_lbuf_pool(lbuf_mgr)) {
		kfree(lbuf_mgr);
		return NULL;
	}

	sema_init(&lbuf_mgr->sem, 1);
	lbuf_mgr->has_turn_to_client = false;

	return lbuf_mgr;
}

static void deinit_lbuf_pool(struct dpu_lbuf_mgr *lbuf_mgr)
{
	uint32_t index = 0;
	struct used_node_info *used_node = NULL;
	struct used_node_info *_u_node_ = NULL;
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	for (index = 0; index < lbuf_mgr->part_num; index++) {
		struct lbuf_node_info *node_info = &(lbuf_mgr->lb_part_info[index]);
		down(&node_info->sem);
		list_for_each_entry_safe(used_node, _u_node_, &node_info->used_node_head, list_node) {
			if (!used_node)
				continue;

			list_del(&used_node->list_node);
			kfree(used_node);
			used_node = NULL;
		}
		node_info->used_lb = 0;

		list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
			if (!rsv_node)
				continue;

			list_del(&rsv_node->list_node);
			kfree(rsv_node);
			rsv_node = NULL;
		}
		up(&node_info->sem);
	}

	if (lbuf_mgr->lb_part_info) {
		kfree(lbuf_mgr->lb_part_info);
		lbuf_mgr->lb_part_info = NULL;
	}
}

void lbuf_mgr_deinit(void *data)
{
	struct dpu_lbuf_mgr *lbuf_mgr = (struct dpu_lbuf_mgr *)data;
	if (!lbuf_mgr)
		return;

	deinit_lbuf_pool(lbuf_mgr);
	kfree(lbuf_mgr);
}

static void lbuf_mgr_reset_node_info(struct lbuf_node_info *node_info)
{
	struct used_node_info *used_node = NULL;
	struct used_node_info *_u_node_ = NULL;
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	down(&node_info->sem);
	list_for_each_entry_safe(used_node, _u_node_, &node_info->used_node_head, list_node) {
		if (!used_node)
			continue;

		dpu_pr_debug("[LB] node: pard_id=%u used_lb=%u max_lb=%u release: user_type=%d scene_id=%d used_lb=%u",
			node_info->part_id, node_info->used_lb, node_info->max_lb,
			used_node->user_type, used_node->scene_id, used_node->used_lb);
		list_del(&used_node->list_node);
		kfree(used_node);
		used_node = NULL;
	}
	node_info->used_lb = 0;

	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		dpu_pr_debug("[LB] release reserved node: user_type=%d reserved=%u",
			rsv_node->user_type, rsv_node->reserved_lb);
		list_del(&rsv_node->list_node);
		kfree(rsv_node);
		rsv_node = NULL;
	}
	up(&node_info->sem);
}

void lbuf_mgr_reset(void *data)
{
	struct dpu_lbuf_mgr *lbuf_mgr = (struct dpu_lbuf_mgr *)data;
	uint32_t index;

	dpu_pr_debug("[LB] reset lbuf_mgr");

	if (!lbuf_mgr)
		return;

	if (!(lbuf_mgr->lb_part_info))
		return;

	for (index = 0; index < lbuf_mgr->part_num; ++index)
		lbuf_mgr_reset_node_info(&lbuf_mgr->lb_part_info[index]);
}

static void lbuf_mgr_release_node_info(struct lbuf_node_info *node_info, int32_t tgid)
{
	struct used_node_info *used_node = NULL;
	struct used_node_info *_u_node_ = NULL;
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	down(&node_info->sem);
	list_for_each_entry_safe(used_node, _u_node_, &node_info->used_node_head, list_node) {
		if (!used_node)
			continue;

		dpu_pr_debug("[LB] used_node: tgid=%d user_type=%d scene_id=%d used_lb=%u",
			used_node->user_tgid, used_node->user_type, used_node->scene_id, used_node->used_lb);

		if (used_node->user_tgid == tgid) {
			dpu_pr_debug("[LB] node: part_id=%u used_lb=%u max_lb=%u release:user_type=%d scene_id=%d used_lb=%u",
				node_info->part_id, node_info->used_lb, node_info->max_lb,
				used_node->user_type, used_node->scene_id, used_node->used_lb);
			node_info->used_lb -= used_node->used_lb;
			list_del(&used_node->list_node);
			kfree(used_node);
			used_node = NULL;
		}
	}

	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		dpu_pr_debug("[LB] rsv_node: tgid=%d user_type=%d scene_id=%d reserved_lb=%u",
			rsv_node->user_tgid, rsv_node->user_type, rsv_node->scene_id, rsv_node->reserved_lb);

		if (rsv_node->user_tgid == tgid) {
			dpu_pr_debug("[LB] release reserved node: user_type=%d scene_id=%d reserved=%u",
				rsv_node->user_type, rsv_node->scene_id, rsv_node->reserved_lb);
			node_info->used_lb -= rsv_node->reserved_lb;
			list_del(&rsv_node->list_node);
			kfree(rsv_node);
			rsv_node = NULL;
		}
	}
	up(&node_info->sem);
}

void lbuf_mgr_release(void *data)
{
	struct dpu_lbuf_mgr *lbuf_mgr = (struct dpu_lbuf_mgr *)data;
	uint32_t index;
	int32_t tgid = task_tgid_vnr(current);

	dpu_pr_debug("[LB] release lbuf_mgr tgid=%d", tgid);

	if (!lbuf_mgr)
		return;

	if (!(lbuf_mgr->lb_part_info))
		return;

	for (index = 0; index < lbuf_mgr->part_num; ++index)
			lbuf_mgr_release_node_info(&lbuf_mgr->lb_part_info[index], tgid);
}

static bool lbuf_mgr_is_reserve_lb_satisfy(const struct lbuf_node_info *node_info)
{
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		if (rsv_node->reserved_lb < rsv_node->reserve_lb) {
			dpu_pr_info("[LB] current node: part_id=%u used_lb=%u max_lb=%u cannot request, "
				"reserve node: user_type=%d user_tgid=%d reserved_lb=%u reserve_lb=%u",
				node_info->part_id, node_info->used_lb, node_info->max_lb,
				rsv_node->user_type, rsv_node->user_tgid, rsv_node->reserved_lb, rsv_node->reserve_lb);
			return false;
		}
	}

	return true;
}

static uint32_t lbuf_mgr_get_node_need_reserved_lb(const struct lbuf_node_info *node_info)
{
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;
	uint32_t need_reserved_lb = 0;

	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		need_reserved_lb += rsv_node->reserve_lb - rsv_node->reserved_lb;
	}

	return need_reserved_lb;
}

static void lbuf_mgr_dec_used_node_lb(struct lbuf_node_info *node_info, const struct req_lbuf_info *lb_info,
	uint32_t used_lb)
{
	struct used_node_info *used_node = NULL;
	struct used_node_info *_u_node_ = NULL;

	list_for_each_entry_safe(used_node, _u_node_, &node_info->used_node_head, list_node) {
		if (!used_node)
			continue;

		if ((used_node->user_type == lb_info->user_type) &&
			(used_node->scene_id == lb_info->scene_id)) {
			dpu_pr_debug("[LB] node: part_id=%u total used_lb=%u max_lb=%u, used node: user_type=%d scene_id=%d"
				" used_lb=%u, dec_value=%u", node_info->part_id, node_info->used_lb, node_info->max_lb,
				used_node->user_type, used_node->scene_id, used_node->used_lb, used_lb);

			if (used_lb > used_node->used_lb) {
				dpu_pr_err("[LB] dec_value=%u is greater than current used_lb=%u", used_lb, used_node->used_lb);
				return; // user of same scene only exists once in one node
			}
			used_node->used_lb -= used_lb;
			return; // user of same scene only exists once in one node
		}
	}

	dpu_pr_err("[LB] not found: user_type=%d scene_id=%d part_id=%d used_lb=%u",
		lb_info->user_type, lb_info->scene_id, node_info->part_id,
		used_lb);
}

static void lbuf_mgr_inc_used_node_lb(struct lbuf_node_info *node_info, const struct req_lbuf_info *lb_info,
	uint32_t used_lb)
{
	struct used_node_info *used_node = NULL;
	struct used_node_info *_u_node_ = NULL;

	list_for_each_entry_safe(used_node, _u_node_, &node_info->used_node_head, list_node) {
		if (!used_node)
			continue;

		if ((used_node->user_type == lb_info->user_type) &&
			(used_node->scene_id == lb_info->scene_id)) {
			dpu_pr_debug("[LB] node: part_id=%u total used_lb=%u max_lb=%u, used node: user_type=%d scene_id=%d"
				" used_lb=%u, inc_value=%u", node_info->part_id, node_info->used_lb, node_info->max_lb,
				used_node->user_type, used_node->scene_id, used_node->used_lb, used_lb);

			used_node->used_lb += used_lb;
			return; // user of same scene only exists once in one node
		}
	}

	// create a new used_node when there is no node in list of used_node_head
	used_node = kzalloc(sizeof(struct used_node_info), GFP_KERNEL);
	if (!used_node) {
		dpu_pr_err("[LB] used_node is nullptr");
		return;
	}

	used_node->user_type = lb_info->user_type;
	used_node->scene_id = lb_info->scene_id;
	used_node->user_tgid = task_tgid_vnr(current);
	used_node->used_lb = used_lb;
	dpu_pr_debug("[LB] create used_node: user_tgid=%d user_type=%d scene_id=%d, used_lb=%u", used_node->user_tgid,
		used_node->user_type, used_node->scene_id, used_node->used_lb);

	list_add_tail(&used_node->list_node, &node_info->used_node_head);
	return;
}

static void lbuf_mgr_request_node(struct dpu_lbuf_mgr *lbuf_mgr, const  struct req_lbuf_info *lb_info,
	int32_t part_id, uint32_t *request_lb)
{
	uint32_t index;
	struct lbuf_node_info *node_info = NULL;
	uint32_t remained_lb = 0;
	uint32_t need_reserved_lb = 0;

	for (index = 0; index < lbuf_mgr->part_num; ++index) {
		node_info = &lbuf_mgr->lb_part_info[index];
		if (node_info->part_id != (uint32_t)part_id) {
			continue;
		}

		down(&node_info->sem);
		dpu_pr_debug("[LB] index=%u part_id=%d used_lb=%u max_lb=%u request_lb=%u",
			index, part_id, node_info->used_lb, node_info->max_lb, *request_lb);

		if (!lbuf_mgr_is_reserve_lb_satisfy(node_info)) {
			dpu_pr_warn("[LB] request node: stop request cause reserve is not satisfied");
			up(&node_info->sem);
			return;
		}

		remained_lb = node_info->max_lb - node_info->used_lb;
		need_reserved_lb = lbuf_mgr_get_node_need_reserved_lb(node_info);
		if (need_reserved_lb > remained_lb) {
			dpu_pr_warn("[LB] request node: stop request cause need_reserved_lb=%u is greater than "
				"remained=%u: max_lb=%u used_lb=%u",
				need_reserved_lb, remained_lb, node_info->max_lb, node_info->used_lb);
			up(&node_info->sem);
			return;
		}

		remained_lb -= need_reserved_lb;
		if (*request_lb <= remained_lb) {
			dpu_pr_debug("[LB] request node: succ-> request_lb=%u remained_lb=%u", *request_lb, remained_lb);
			node_info->used_lb += *request_lb;
			up(&node_info->sem);
			*request_lb = 0; // set to 0 for successfully stop traverse
			return;
		}

		up(&node_info->sem);
		dpu_pr_warn("[LB] remained lb[%u] is not satisfied, request lb[%u], part_id=%d, max_lb=%u used_lb=%u, "
			"need_reserved_lb=%u", remained_lb, *request_lb, part_id, node_info->max_lb, node_info->used_lb,
			need_reserved_lb);
		return;
	}
}

static void lbuf_mgr_turn_to_client(struct dpu_lbuf_mgr *lbuf_mgr,
	const struct req_lbuf_info *lb_info)
{
	if (lb_info->user_type != SCENE_USER_MDC)
		return;

	down(&lbuf_mgr->sem);
	dpu_res_send_uevent(RES_EVENT_TURN_TO_CLIENT);
	lbuf_mgr->has_turn_to_client = true;
	dpu_pr_info("[LB] mdc request lb fail, need turn to client");
	up(&lbuf_mgr->sem);
}

static void lbuf_mgr_turn_to_device(struct dpu_lbuf_mgr *lbuf_mgr,
	const struct req_lbuf_info *lb_info)
{
	if (lb_info->user_type != SCENE_USER_MDC)
		return;

	down(&lbuf_mgr->sem);
	if (lbuf_mgr->has_turn_to_client) {
		dpu_res_send_uevent(RES_EVENT_TURN_TO_DEVICE);
		lbuf_mgr->has_turn_to_client = false;
		dpu_pr_info("[LB] mdc release lb succ, need turn to device");
	}
	up(&lbuf_mgr->sem);
}

static void lbuf_mgr_revert_lb(struct dpu_lbuf_mgr *lbuf_mgr, uint32_t req_success_num,
	struct lbuf_part_info *req_success_part_info)
{
	uint32_t succ_index;
	uint32_t index;
	struct lbuf_part_info *part_info;
	struct lbuf_node_info *node_info;

	for (succ_index = 0; succ_index < req_success_num; ++succ_index) {
		part_info = &req_success_part_info[succ_index];
		for (index = 0; index < lbuf_mgr->part_num; ++index) {
			node_info = &lbuf_mgr->lb_part_info[index];
			if (node_info->part_id != (uint32_t)(part_info->part_id)) {
				continue;
			}
			down(&node_info->sem);
			node_info->used_lb -= part_info->used_lb;
			up(&node_info->sem);
			break;
		}
	}
}

static void lbuf_mgr_confirm_lb(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info,
	uint32_t req_success_num, struct lbuf_part_info *req_success_part_info)
{
	uint32_t succ_index;
	uint32_t index;
	struct lbuf_part_info *part_info;
	struct lbuf_node_info *node_info;

	for (succ_index = 0; succ_index < req_success_num; ++succ_index) {
		part_info = &req_success_part_info[succ_index];
		for (index = 0; index < lbuf_mgr->part_num; ++index) {
			node_info = &lbuf_mgr->lb_part_info[index];
			if (node_info->part_id != (uint32_t)(part_info->part_id)) {
				continue;
			}
			dpu_pr_debug("[LB] req node: part_id=%d total used_lb=%u",
				node_info->part_id, part_info->used_lb);

			down(&node_info->sem);
			lbuf_mgr_inc_used_node_lb(node_info, lb_info, part_info->used_lb);
			up(&node_info->sem);
			break;
		}
	}

	return;
}

/*
 * When user invoke request operation, lbuf_mgr will do these things to handle its demand
 * 1、check request information valid
 * 2、max_lb dec used_lb of current node, which is denoted as remained_lb,
 * can satisfy current reserve demand in reserved_node_head, remained_lb will dec reserve lb size
 * 3、create a new node of used type if there is none, or use a created one otherwise
 * 4、if remained_lb can satisfy request used lb size, node will update to used_node.
 * if remained_lb cannot satisfy, continue search for next pool, if fail, clear lb info of requested, and return -1.
 */
static int32_t lbuf_mgr_request_lb(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info)
{
	int32_t ret = 0;
	uint32_t req_index;
	const struct lbuf_part_info *req_part_info = NULL;
	uint32_t request_lb = 0;
	uint32_t req_success_num = 0;
	struct lbuf_part_info req_success_part_info[LBUF_PART_NUM_MAX] = {0};

	for (req_index = 0; req_index < lb_info->part_num; ++req_index) {
		req_part_info = &lb_info->req_part_info[req_index];
		dpu_pr_debug("[LB] req_index=%u: scene_id=%d part_id=%d used_lb=%u", req_index,
			lb_info->scene_id, req_part_info->part_id, req_part_info->used_lb);

		request_lb = req_part_info->used_lb;
		lbuf_mgr_request_node(lbuf_mgr, lb_info, req_part_info->part_id, &request_lb);
		if (request_lb > 0) {
			dpu_pr_warn("[LB] request node: stop request part id=%d used_lb=%u", req_part_info->part_id,
				req_part_info->used_lb);
			ret = -1;
			break;
		}

		req_success_part_info[req_success_num].part_id = req_part_info->part_id;
		req_success_part_info[req_success_num].used_lb = req_part_info->used_lb;
		req_success_num++;
	}

	if (ret) {
		lbuf_mgr_revert_lb(lbuf_mgr, req_success_num, req_success_part_info);
		lbuf_mgr_turn_to_client(lbuf_mgr, lb_info);

		return -1;
	}

	lbuf_mgr_confirm_lb(lbuf_mgr, lb_info, req_success_num, req_success_part_info);
	lbuf_mgr_turn_to_device(lbuf_mgr, lb_info);

	return 0;
}

static int32_t lbuf_mgr_release_lb(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info)
{
	uint32_t req_index;
	uint32_t index;
	const struct lbuf_part_info *req_node_info = NULL;
	struct lbuf_node_info *node_info = NULL;

	for (req_index = 0; req_index < lb_info->part_num; ++req_index) {
		req_node_info = &lb_info->req_part_info[req_index];
		for (index = 0; index < lbuf_mgr->part_num; ++index) {
			node_info = &lbuf_mgr->lb_part_info[index];
			if (node_info->part_id != req_node_info->part_id) {
				continue;
			}
			dpu_pr_debug("[LB] release node: part_id=%d used_lb=%u",
				node_info->part_id, req_node_info->used_lb);

			down(&node_info->sem);
			if (req_node_info->used_lb > node_info->used_lb) {
				dpu_pr_err("[LB] release node: skip release cause release_lb=%u is greater than used_lb=%u",
					req_node_info->used_lb, node_info->used_lb);
				up(&node_info->sem);
				break;
			}
			node_info->used_lb -= req_node_info->used_lb;

			lbuf_mgr_dec_used_node_lb(node_info, lb_info, req_node_info->used_lb);
			up(&node_info->sem);
			break;
		}
	}

	return 0;
}

static struct reserved_node_info *lbuf_mgr_get_reserved_lb_node(struct lbuf_node_info *node_info,
	const struct req_lbuf_info *lb_info)
{
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		if ((rsv_node->user_type == lb_info->user_type) &&
			(rsv_node->scene_id == lb_info->scene_id))
			return rsv_node; // user of same scene only exists once in one node
	}

	rsv_node = kzalloc(sizeof(struct reserved_node_info), GFP_KERNEL);
	if (!rsv_node) {
		dpu_pr_err("[LB] rsv_node is nullptr");
		return NULL;
	}

	rsv_node->user_type = lb_info->user_type;
	rsv_node->scene_id = lb_info->scene_id;
	rsv_node->user_tgid = task_tgid_vnr(current);
	rsv_node->reserved_lb = 0;
	rsv_node->reserve_lb = 0;

	list_add_tail(&rsv_node->list_node, &node_info->reserved_node_head);

	return rsv_node;
}

static void lbuf_mgr_clear_reserve_node_info(struct lbuf_node_info *node_info,
	const struct req_lbuf_info *lb_info)
{
	struct reserved_node_info *rsv_node = NULL;
	struct reserved_node_info *_r_node_ = NULL;

	down(&node_info->sem);
	list_for_each_entry_safe(rsv_node, _r_node_, &node_info->reserved_node_head, list_node) {
		if (!rsv_node)
			continue;

		if ((rsv_node->user_type == lb_info->user_type) &&
			(rsv_node->scene_id == lb_info->scene_id)) {
			dpu_pr_debug("[LB] clear reserved node: user_type=%d scene_id=%d reserved=%u",
				rsv_node->user_type, rsv_node->scene_id, rsv_node->reserved_lb);
			node_info->used_lb -= rsv_node->reserved_lb;
			rsv_node->reserve_lb = 0;
			rsv_node->reserved_lb = 0;
		}
	}
	up(&node_info->sem);
}

static void lbuf_mgr_clear_user_reserve_node_info(struct dpu_lbuf_mgr *lbuf_mgr,
	const struct req_lbuf_info *lb_info)
{
	uint32_t index;
	struct lbuf_node_info *lb_part_info = NULL;

	for (index = 0; index < lbuf_mgr->part_num; ++index) {
		lb_part_info = &lbuf_mgr->lb_part_info[index];
		lbuf_mgr_clear_reserve_node_info(lb_part_info, lb_info);
	}
}

static void lbuf_mgr_reserve_node(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info,
	int32_t part_id, uint32_t *reserve_lb)
{
	uint32_t index;
	struct lbuf_node_info *node_info = NULL;
	struct reserved_node_info *rsv_node = NULL;
	uint32_t remained_lb = 0;
	uint32_t need_reserved_lb = 0;

	for (index = 0; index < lbuf_mgr->part_num; ++index) {
		node_info = &(lbuf_mgr->lb_part_info[index]);
		if (node_info->part_id != (uint32_t)part_id)
			continue;

		down(&node_info->sem);
		rsv_node = lbuf_mgr_get_reserved_lb_node(node_info, lb_info);
		if (!rsv_node) {
			dpu_pr_err("[LB] reserve node: skip reserve%u cause get reserve node fail", index);
			up(&node_info->sem);
			return;
		}

		if (*reserve_lb <= rsv_node->reserved_lb) {
			dpu_pr_debug("[LB] reserve node: succ-> reserve%u reserve_lb=%u reserved_lb=%u "
				"max_lb=%u used_lb=%u", index, *reserve_lb,
				rsv_node->reserved_lb, node_info->max_lb, node_info->used_lb);

			node_info->used_lb -= (rsv_node->reserved_lb - *reserve_lb);
			rsv_node->reserve_lb = *reserve_lb;
			rsv_node->reserved_lb = *reserve_lb;
			up(&node_info->sem);
			*reserve_lb = 0; // set to 0 for successfully stop traverse
			return;
		}

		remained_lb = node_info->max_lb - node_info->used_lb;
		need_reserved_lb = *reserve_lb - rsv_node->reserved_lb;
		if (need_reserved_lb <= remained_lb) {
			dpu_pr_debug("[LB] reserve node: succ-> reserve%u reserve_lb=%u need_reserved_lb=%u "
				"remained=%u: max_lb=%u used_lb=%u", index, *reserve_lb,
				need_reserved_lb, remained_lb, node_info->max_lb, node_info->used_lb);

			node_info->used_lb += need_reserved_lb;
			rsv_node->reserve_lb = *reserve_lb;
			rsv_node->reserved_lb = *reserve_lb;
			up(&node_info->sem);
			*reserve_lb = 0; // set to 0 for successfully stop traverse
			return;
		}

		// current node lb size cannot satisfy request size, just use remained size and continue traverse
		node_info->used_lb = node_info->max_lb;
		rsv_node->reserve_lb = *reserve_lb;
		rsv_node->reserved_lb += remained_lb;
		*reserve_lb = need_reserved_lb - remained_lb;
		up(&node_info->sem);
		break;
	}
}

/*
 * When user invoke reserve operation, lbuf_mgr will do these things to handle its demand
 * 1、create a new node of reserve type if there is none, or use a created one otherwise and get its
 * already reserved lb num
 * 2、determin used_lb and max_lb of current node, which is denoted as remained_lb, plus with reserved lb num,
 * can satisfy current reserve demand, remained_lb will dec size of not reserved lb size(reserve lb num
 * dec reserved lb num), and reserved lb num is set to reserve lb num
 * 3、if remained_lb, plus with reserved lb num, cannot satisfy, reserved lb num is to inc remained_lb
 * Note: each user invoke this function will overwrite its own reserve information, return 0 as succ,
 * return 1 means user should invoke at next time, and it reserves all lbuf_mgr can provide
 * return -1 means user input parameters is invalid
 */
static int32_t lbuf_mgr_reserve_lb(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info)
{
	int32_t ret = 0;
	uint32_t req_index;
	uint32_t reserve_lb = 0;
	const struct lbuf_part_info *part_info;

	for (req_index = 0; req_index < lb_info->part_num; ++req_index) {
		part_info = &lb_info->req_part_info[req_index];

		reserve_lb = part_info->used_lb;
		lbuf_mgr_reserve_node(lbuf_mgr, lb_info, part_info->part_id, &reserve_lb);
		if (reserve_lb > 0) {
			dpu_pr_warn("[LB] reserve node: stop reserve part_id=%d used_lb=%u", part_info->part_id,
				part_info->used_lb);
			ret = -1;
		}
	}

	return ret;
}

/* Currently user invoke this for free all reserved information of given user_type
 */
static int32_t lbuf_mgr_free_reserved_lb(struct dpu_lbuf_mgr *lbuf_mgr, const struct req_lbuf_info *lb_info)
{
	lbuf_mgr_clear_user_reserve_node_info(lbuf_mgr, lb_info);
	return 0;
}

int32_t lbuf_mgr_handle_lb_opt(struct dpu_lbuf_mgr *lbuf_mgr, struct req_lbuf_info *lb_info)
{
	if (unlikely(lb_info->user_type < 0 || lb_info->user_type >= SCENE_USER_MAX)) {
		dpu_pr_err("[LB] user_type=%d is invalid", lb_info->user_type);
		return -1;
	}

	if (unlikely(lb_info->scene_id < 0 || lb_info->scene_id >= DPU_SCENE_MAX)) {
		dpu_pr_err("[LB] invalid scene_id=%d", lb_info->scene_id);
		return -1;
	}

	if (unlikely(lb_info->part_num > LBUF_PART_NUM_MAX)) {
		dpu_pr_err("[LB] part_num=%u is invalid", lb_info->part_num);
		return -1;
	}

	switch (lb_info->opt) {
	case LB_REQUEST:
		return lbuf_mgr_request_lb(lbuf_mgr, lb_info);
	case LB_RELEASE:
		return lbuf_mgr_release_lb(lbuf_mgr, lb_info);
	case LB_RESERVE:
		return lbuf_mgr_reserve_lb(lbuf_mgr, lb_info);
	case LB_FREE_RESERVED:
		return lbuf_mgr_free_reserved_lb(lbuf_mgr, lb_info);
	default:
		dpu_pr_err("[LB] lbuf mgr unsupport opt = 0x%x", lb_info->opt);
		return -1;
	}
}

static long lbuf_mgr_ioctl(struct dpu_res_resouce_node *res_node, uint32_t cmd, void __user *argp)
{
	struct dpu_lbuf_mgr *lbuf_mgr = NULL;
	struct req_lbuf_info lb_info = {0};

	if (!res_node || !res_node->data || !argp) {
		dpu_pr_err("[LB] res_node or node data or argp is NULL");
		return -EINVAL;
	}

	lbuf_mgr = (struct dpu_lbuf_mgr*)res_node->data;
	switch (cmd) {
	case RES_HANDLE_LBUF_OPT:
		if (unlikely(copy_from_user(&lb_info, argp, sizeof(lb_info)) != 0)) {
			dpu_pr_err("[LB] copy from user fail");
			return -1;
		}
		return lbuf_mgr_handle_lb_opt(lbuf_mgr, &lb_info);
	default:
		dpu_pr_debug("[LB] lbuf mgr unsupport cmd, need processed by other module, cmd = 0x%x", cmd);
		return 1;
	}

	return 0;
}

void dpu_res_register_lbuf_mgr(struct list_head *resource_head)
{
	struct dpu_res_resouce_node *lbuf_mgr_node = kzalloc(sizeof(struct dpu_res_resouce_node), GFP_KERNEL);
	if (!lbuf_mgr_node)
		return;

	lbuf_mgr_node->res_type = RES_LBUF;
	atomic_set(&lbuf_mgr_node->res_ref_cnt, 0);

	lbuf_mgr_node->init = lbuf_mgr_init;
	lbuf_mgr_node->deinit = lbuf_mgr_deinit;
	lbuf_mgr_node->reset = lbuf_mgr_reset;
	lbuf_mgr_node->release = lbuf_mgr_release;
	lbuf_mgr_node->ioctl = lbuf_mgr_ioctl;

	list_add_tail(&lbuf_mgr_node->list_node, resource_head);
	dpu_pr_debug("[LB] dpu_res_register_lbuf_mgr success!");
}

