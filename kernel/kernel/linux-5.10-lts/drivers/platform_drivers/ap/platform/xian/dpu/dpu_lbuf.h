/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2023-2023. All rights reserved.
 * Description: define dpu operator struct
 * Create: 2023-09
 */

#ifndef DPU_LBUF_H
#define DPU_LBUF_H

#include "soc_dpu_interface.h"
#include "soc_dpu_define.h"

#define DPU_DBUF_UNIT 16 // 16 pixels (640 bits)
#define DPU_DBUF_DEPTH 90 // units per dbuf
#define DPU_DBUF_REG_NUM 6 // reg number need config in pan display
#define DPU_DBUF_LEVEL_NUM 2 // dbuf level number

enum dbuf_thd_type {
	DBUF_THD_RQOS_IN = 0,
	DBUF_THD_RQOS_OUT,
	DBUF_THD_FLUX_REQ_BEF_IN,
	DBUF_THD_FLUX_REQ_BEF_OUT,
	DBUF_THD_TYPE_MAX
};

struct dpu_lbuf_dbuf0_reg {
	DPU_LBUF_DBUF0_CTRL_UNION ctrl;
	DPU_LBUF_DBUF0_STATE_UNION state;
	DPU_LBUF_DBUF0_THD_RQOS_UNION thd_rqos;
	DPU_LBUF_DBUF0_THD_DFS_OK_UNION thd_dfs_ok;
	DPU_LBUF_DBUF0_THD_FLUX_REQ_BEF_UNION thd_flux_req_bef;
	DPU_LBUF_DBUF0_THD_FLUX_REQ_AFT_UNION thd_flux_req_aft;
	DPU_LBUF_DBUF0_ONLINE_FILL_LEVEL_UNION online_fill_level;
	DPU_LBUF_DBUF0_THD_FILL_LEV0_UNION thd_fill_lev0;
	DPU_LBUF_DBUF0_PRS_SCN0_UNION prs_scn0;
};

struct dpu_lbuf_dbuf1_reg {
	DPU_LBUF_DBUF1_CTRL_UNION ctrl;
	DPU_LBUF_DBUF1_STATE_UNION state;
	DPU_LBUF_DBUF1_THD_RQOS_UNION thd_rqos;
	DPU_LBUF_DBUF1_THD_DFS_OK_UNION thd_dfs_ok;
	DPU_LBUF_DBUF1_THD_FLUX_REQ_BEF_UNION thd_flux_req_bef;
	DPU_LBUF_DBUF1_THD_FLUX_REQ_AFT_UNION thd_flux_req_aft;
	DPU_LBUF_DBUF1_ONLINE_FILL_LEVEL_UNION online_fill_level;
	DPU_LBUF_DBUF1_THD_FILL_LEV0_UNION thd_fill_lev0;
	DPU_LBUF_DBUF1_PRS_SCN1_UNION prs_scn1;
};

struct dpu_lbuf_dbuf2_reg {
	DPU_LBUF_DBUF2_CTRL_UNION ctrl;
	DPU_LBUF_DBUF2_STATE_UNION state;
	DPU_LBUF_DBUF2_THD_RQOS_UNION thd_rqos;
	DPU_LBUF_DBUF2_ONLINE_FILL_LEVEL_UNION online_fill_level;
	DPU_LBUF_DBUF2_THD_FILL_LEV0_UNION thd_fill_lev0;
	DPU_LBUF_DBUF2_PRS_SCN2_UNION prs_scn2;
};
struct dpu_lbuf_params {
	struct dpu_lbuf_dbuf0_reg dbuf0_reg;
	struct dpu_lbuf_dbuf1_reg dbuf1_reg;
	struct dpu_lbuf_dbuf2_reg dbuf2_reg;
};
#endif