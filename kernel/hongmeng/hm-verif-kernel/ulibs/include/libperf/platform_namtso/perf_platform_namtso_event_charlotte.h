/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: add Namtso PMU list for charlotte platform
 * Author: HISI_DRV
 * Create: Sat May 18 10:30:43 2024
 */

#ifdef ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF
#define DSU_PMU_CHAIN						0x001e
#define DSU_PMU_BUS_ACCESS_RD				0x0060
#define DSU_PMU_BUS_ACCESS_WR				0x0061
#define DSU_PMU_BUS_ACCESS_SHARED			0x0062
#define DSU_PMU_BUS_ACCESS_NOTSHARED		0x0063
#define DSU_PMU_BUS_ACCESS_NORMAL			0x0064
#define DSU_PMU_BUS_ACCESS_PERIPH			0x0065
#define DSU_PMU_L3D_CACHE_REFILL_C0			0x00b1
#define DSU_PMU_L3D_CACHE_C0				0x00b2
#define DSU_PMU_L3D_CACHE_REFILL_C1			0x00b3
#define DSU_PMU_L3D_CACHE_C1				0x00b4
#define DSU_PMU_L3D_CACHE_REFILL_C2			0x00b5
#define DSU_PMU_L3D_CACHE_C2				0x00b6
#define DSU_PMU_L3D_CACHE_REFILL_C3			0x00b7
#define DSU_PMU_L3D_CACHE_C3				0x00b8
#define DSU_PMU_L3D_CACHE_REFILL_C4			0x00b9
#define DSU_PMU_L3D_CACHE_C4				0x00ba
#define DSU_PMU_L3D_CACHE_REFILL_C5			0x00bb
#define DSU_PMU_L3D_CACHE_C5				0x00bc
#define DSU_PMU_L3D_CACHE_REFILL_C5_TH0		0x00bd
#define DSU_PMU_L3D_CACHE_REFILL_C5_TH1		0x00be
#define DSU_PMU_L3D_CACHE_C5_TH0			0x00bf
#define DSU_PMU_L3D_CACHE_C5_TH1			0x00b0
#define DSU_PMU_L3D_CACHE_RD				0x00a0
#define DSU_PMU_L3D_CACHE_WR				0x00a1
#define DSU_PMU_L3D_CACHE_REFILL_RD			0x00a2
#define DSU_PMU_L3D_CACHE_REFILL_WR			0x00a3
#define DSU_PMU_L3D_CACHE_RD_C0				0x00a4
#define DSU_PMU_L3D_CACHE_WR_C0       		0x00a5
#define DSU_PMU_L3D_CACHE_REFILL_RD_C0		0x00a6
#define DSU_PMU_L3D_CACHE_REFILL_WR_C0		0x00a7
#define DSU_PMU_L3D_CACHE_RD_C1				0x00a8
#define DSU_PMU_L3D_CACHE_WR_C1				0x00a9
#define DSU_PMU_L3D_CACHE_REFILL_RD_C1		0x00aa
#define DSU_PMU_L3D_CACHE_REFILL_WR_C1		0x00ab
#define DSU_PMU_L3D_CACHE_RD_C2				0x00ac
#define DSU_PMU_L3D_CACHE_WR_C2				0x00ad
#define DSU_PMU_L3D_CACHE_REFILL_RD_C2		0x00ae
#define DSU_PMU_L3D_CACHE_REFILL_WR_C2		0x00af
/* ACP */
#define DSU_PMU_ACP_CYCLES					0x011d
#define DSU_PMU_ACP_ACCESS					0x0119
#define DSU_PMU_ACP_ACCESS_RD				0x0160
#define DSU_PMU_ACP_ACCESS_WR				0x0161
/* PP */
#define DSU_PMU_PPT_CYCLES					0x021d
#define DSU_PMU_PPT_ACCESS					0x0219
#define DSU_PMU_PPT_ACCESS_RD				0x0260
#define DSU_PMU_PPT_ACCESS_WR				0x0261
/* snp */
#define DSU_PMU_SCU_SNP_ACCESS				0x00c0
#define DSU_PMU_SCU_SNP_EVICT				0x00c1
#define DSU_PMU_SCU_SNP_NO_CPU_SNP			0x00c2
/* pftch */
#define DSU_PMU_SCU_PFTCH_CPU_ACCESS		0x0300
#define DSU_PMU_SCU_PFTCH_CPU_MISS			0x0301
#define DSU_PMU_SCU_PFTCH_CPU_HIT			0x0302
#define DSU_PMU_SCU_PFTCH_CPU_MATCH			0x0303
#define DSU_PMU_SCU_PFTCH_CPU_KILL			0x0304
#define DSU_PMU_SCU_PFTCH_CPU_DROPPED		0x0305
/* stach */
#define DSU_PMU_SCU_STASH_ICN_ACCESS		0x0310
#define DSU_PMU_SCU_STASH_ICN_MISS			0x0311
#define DSU_PMU_SCU_STASH_ICN_HIT			0x0312
#define DSU_PMU_SCU_STASH_ICN_MATCH			0x0313
#define DSU_PMU_SCU_STASH_ICN_KILL			0x0314
#define DSU_PMU_SCU_STASH_ICN_DROPPED		0x0315
#define DSU_PMU_SCU_STASH_ACP_ACCESS		0x0320
#define DSU_PMU_SCU_STASH_ACP_MISS			0x0321
#define DSU_PMU_SCU_STASH_ACP_HIT			0x0322
#define DSU_PMU_SCU_STASH_ACP_MATCH			0x0323
#define DSU_PMU_SCU_STASH_ACP_KILL			0x0324
#define DSU_PMU_SCU_STASH_ACP_DROPPED		0x0325
/* hazard */
#define DSU_PMU_SCU_HZD_WAYLOCK				0x0400
#define DSU_PMU_SCU_HZD_HIT_IN_USE			0x0401
#define DSU_PMU_SCU_HZD_ADDRESS				0x0402
#define DSU_PMU_SCU_HZD_STRUCT_AFB			0x0403
#define DSU_PMU_SCU_HZD_STRUCT_BDQ			0x0404
#define DSU_PMU_SCU_HZD_STRUCT_BIB			0x0405
/* others */
#define DSU_PMU_SCU_BIB_ACCESS				0x0406
#define DSU_PMU_SCU_FID_LOCKED				0x0407
#define DSU_PMU_SCU_SAFE_MODE_CYCLES		0x0408
#define DSU_PMU_CFRQ_ENQ_STALL				0x0410
#define DSU_PMU_FDQ_ENQ_STALL				0x0411
#define DSU_PMU_RXDAT_STALL					0x0412
#define DSU_PMU_CFRQ_LOW_OCC				0x0420
#define DSU_PMU_BDQ_EXIT_STALL				0x0423
#define DSU_PMU_RXSNPSHARED_C0				0x0430
#define DSU_PMU_RXSNPSHARED_C1				0x0431
#define DSU_PMU_RXSNPSHARED_C2				0x0432
#define DSU_PMU_RXSNPSHARED_C3				0x0433
#define DSU_PMU_RXSNPSHARED_C4				0x0434
#define DSU_PMU_RXSNPSHARED_C5				0x0435
#define DSU_PMU_RXSNPUNIQUE_C0				0x0436
#define DSU_PMU_RXSNPUNIQUE_C1				0x0437
#define DSU_PMU_RXSNPUNIQUE_C2				0x0438
#define DSU_PMU_RXSNPUNIQUE_C3				0x0439
#define DSU_PMU_RXSNPUNIQUE_C4				0x043a
#define DSU_PMU_RXSNPUNIQUE_C5				0x043b
#define DSU_PMU_SCU_STASH_PF_ACCESS			0x0330
#define DSU_PMU_SCU_STASH_PF_MISS			0x0331
#define DSU_PMU_SCU_STASH_PF_HIT			0x0332
#define DSU_PMU_SCU_STASH_PF_MATCH			0x0333
#define DSU_PMU_SCU_STASH_PF_KILL			0x0334
#define DSU_PMU_SCU_STASH_PF_DROPPED		0x0335
#define DSU_PMU_L3D_CACHE_REFILL_C5_NO_PFT	0x0066
#endif /* ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF */

#ifdef PERF_PLATFORM_NAMTSO_ENUM
    HM_HW_DSU_CHAIN,
    HM_HW_DSU_BUS_ACCESS_RD,
    HM_HW_DSU_BUS_ACCESS_WR,
    HM_HW_DSU_BUS_ACCESS_SHARED,
    HM_HW_DSU_BUS_ACCESS_NOTSHARED,
    HM_HW_DSU_BUS_ACCESS_NORMAL,
    HM_HW_DSU_BUS_ACCESS_PERIPH,
    HM_HW_DSU_L3D_CACHE_REFILL_C0,
    HM_HW_DSU_L3D_CACHE_C0,
    HM_HW_DSU_L3D_CACHE_REFILL_C1,
    HM_HW_DSU_L3D_CACHE_C1,
    HM_HW_DSU_L3D_CACHE_REFILL_C2,
    HM_HW_DSU_L3D_CACHE_C2,
    HM_HW_DSU_L3D_CACHE_REFILL_C3,
    HM_HW_DSU_L3D_CACHE_C3,
    HM_HW_DSU_L3D_CACHE_REFILL_C4,
    HM_HW_DSU_L3D_CACHE_C4,
    HM_HW_DSU_L3D_CACHE_REFILL_C5,
    HM_HW_DSU_L3D_CACHE_C5,
    HM_HW_DSU_L3D_CACHE_REFILL_C5_TH0,
    HM_HW_DSU_L3D_CACHE_REFILL_C5_TH1,
    HM_HW_DSU_L3D_CACHE_C5_TH0,
    HM_HW_DSU_L3D_CACHE_C5_TH1,
    HM_HW_DSU_L3D_CACHE_RD,
    HM_HW_DSU_L3D_CACHE_WR,
    HM_HW_DSU_L3D_CACHE_REFILL_RD,
    HM_HW_DSU_L3D_CACHE_REFILL_WR,
    HM_HW_DSU_L3D_CACHE_RD_C0,
    HM_HW_DSU_L3D_CACHE_WR_C0,
    HM_HW_DSU_L3D_CACHE_REFILL_RD_C0,
    HM_HW_DSU_L3D_CACHE_REFILL_WR_C0,
    HM_HW_DSU_L3D_CACHE_RD_C1,
    HM_HW_DSU_L3D_CACHE_WR_C1,
    HM_HW_DSU_L3D_CACHE_REFILL_RD_C1,
    HM_HW_DSU_L3D_CACHE_REFILL_WR_C1,
    HM_HW_DSU_L3D_CACHE_RD_C2,
    HM_HW_DSU_L3D_CACHE_WR_C2,
    HM_HW_DSU_L3D_CACHE_REFILL_RD_C2,
    HM_HW_DSU_L3D_CACHE_REFILL_WR_C2,
    /* ACP */
    HM_HW_DSU_ACP_CYCLES,
    HM_HW_DSU_ACP_ACCESS,
    HM_HW_DSU_ACP_ACCESS_RD,
    HM_HW_DSU_ACP_ACCESS_WR,
    /* PP */
    HM_HW_DSU_PPT_CYCLES,
    HM_HW_DSU_PPT_ACCESS,
    HM_HW_DSU_PPT_ACCESS_RD,
    HM_HW_DSU_PPT_ACCESS_WR,
    /* snp */
    HM_HW_DSU_SCU_SNP_ACCESS,
    HM_HW_DSU_SCU_SNP_EVICT,
    HM_HW_DSU_SCU_SNP_NO_CPU_SNP,
    /* pftch */
    HM_HW_DSU_SCU_PFTCH_CPU_ACCESS,
    HM_HW_DSU_SCU_PFTCH_CPU_MISS,
    HM_HW_DSU_SCU_PFTCH_CPU_HIT,
    HM_HW_DSU_SCU_PFTCH_CPU_MATCH,
    HM_HW_DSU_SCU_PFTCH_CPU_KILL,
    HM_HW_DSU_SCU_PFTCH_CPU_DROPPED,
    /* stach */
    HM_HW_DSU_SCU_STASH_ICN_ACCESS,
    HM_HW_DSU_SCU_STASH_ICN_MISS,
    HM_HW_DSU_SCU_STASH_ICN_HIT,
    HM_HW_DSU_SCU_STASH_ICN_MATCH,
    HM_HW_DSU_SCU_STASH_ICN_KILL,
    HM_HW_DSU_SCU_STASH_ICN_DROPPED,
    HM_HW_DSU_SCU_STASH_ACP_ACCESS,
    HM_HW_DSU_SCU_STASH_ACP_MISS,
    HM_HW_DSU_SCU_STASH_ACP_HIT,
    HM_HW_DSU_SCU_STASH_ACP_MATCH,
    HM_HW_DSU_SCU_STASH_ACP_KILL,
    HM_HW_DSU_SCU_STASH_ACP_DROPPED,
    /* hazard */
    HM_HW_DSU_SCU_HZD_WAYLOCK,
    HM_HW_DSU_SCU_HZD_HIT_IN_USE,
    HM_HW_DSU_SCU_HZD_ADDRESS,
    HM_HW_DSU_SCU_HZD_STRUCT_AFB,
    HM_HW_DSU_SCU_HZD_STRUCT_BDQ,
    HM_HW_DSU_SCU_HZD_STRUCT_BIB,
    /* others */
    HM_HW_DSU_SCU_BIB_ACCESS,
    HM_HW_DSU_SCU_FID_LOCKED,
    HM_HW_DSU_SCU_SAFE_MODE_CYCLES,
    HM_HW_DSU_CFRQ_ENQ_STALL,
    HM_HW_DSU_FDQ_ENQ_STALL,
    HM_HW_DSU_RXDAT_STALL,
    HM_HW_DSU_CFRQ_LOW_OCC,
    HM_HW_DSU_BDQ_EXIT_STALL,
    HM_HW_DSU_RXSNPSHARED_C0,
    HM_HW_DSU_RXSNPSHARED_C1,
    HM_HW_DSU_RXSNPSHARED_C2,
    HM_HW_DSU_RXSNPSHARED_C3,
    HM_HW_DSU_RXSNPSHARED_C4,
    HM_HW_DSU_RXSNPSHARED_C5,
    HM_HW_DSU_RXSNPUNIQUE_C0,
    HM_HW_DSU_RXSNPUNIQUE_C1,
    HM_HW_DSU_RXSNPUNIQUE_C2,
    HM_HW_DSU_RXSNPUNIQUE_C3,
    HM_HW_DSU_RXSNPUNIQUE_C4,
    HM_HW_DSU_RXSNPUNIQUE_C5,
    HM_HW_DSU_SCU_STASH_PF_ACCESS,
    HM_HW_DSU_SCU_STASH_PF_MISS,
    HM_HW_DSU_SCU_STASH_PF_HIT,
    HM_HW_DSU_SCU_STASH_PF_MATCH,
    HM_HW_DSU_SCU_STASH_PF_KILL,
    HM_HW_DSU_SCU_STASH_PF_DROPPED,
    HM_HW_DSU_L3D_CACHE_REFILL_C5_NO_PFT,
#endif /* PERF_PLATFORM_NAMTSO_ENUM */

#ifdef PERF_PLATFORM_NAMTSO_SYMBOL
	{ chw(DSU_CHAIN),						"dsu-chain",						13  },
	{ chw(DSU_BUS_ACCESS_RD),				"dsu-bus-access-rd",				21  },
	{ chw(DSU_BUS_ACCESS_WR),				"dsu-bus-access-wr",				21  },
	{ chw(DSU_BUS_ACCESS_SHARED),			"dsu-bus-access-shared",			25  },
	{ chw(DSU_BUS_ACCESS_NOTSHARED),		"dsu-bus-access-notshared",			28  },
	{ chw(DSU_BUS_ACCESS_NORMAL),			"dsu-bus-access-normal",			25  },
	{ chw(DSU_BUS_ACCESS_PERIPH),			"dsu-bus-access-periph",			25  },
	{ chw(DSU_L3D_CACHE_REFILL_C0),			"dsu-l3d-cache-refill-c0",			27  },
	{ chw(DSU_L3D_CACHE_C0),				"dsu-l3d-cache-c0",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C1),			"dsu-l3d-cache-refill-c1",			27  },
	{ chw(DSU_L3D_CACHE_C1),				"dsu-l3d-cache-c1",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C2),			"dsu-l3d-cache-refill-c2",			27  },
	{ chw(DSU_L3D_CACHE_C2),				"dsu-l3d-cache-c2",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C3),			"dsu-l3d-cache-refill-c3",			27  },
	{ chw(DSU_L3D_CACHE_C3),				"dsu-l3d-cache-c3",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C4),			"dsu-l3d-cache-refill-c4",			27  },
	{ chw(DSU_L3D_CACHE_C4),				"dsu-l3d-cache-c4",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C5),			"dsu-l3d-cache-refill-c5",			27  },
	{ chw(DSU_L3D_CACHE_C5),				"dsu-l3d-cache-c5",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_C5_TH0),		"dsu-l3d-cache-refill-c5-th0",		31  },
	{ chw(DSU_L3D_CACHE_REFILL_C5_TH1),		"dsu-l3d-cache-refill-c5-th1",		31  },
	{ chw(DSU_L3D_CACHE_C5_TH0),			"dsu-l3d-cache-c5-th0",				24  },
	{ chw(DSU_L3D_CACHE_C5_TH1),			"dsu-l3d-cache-c5-th1",				24  },
	{ chw(DSU_L3D_CACHE_RD),				"dsu-l3d-cache-rd",					20  },
	{ chw(DSU_L3D_CACHE_WR),				"dsu-l3d-cache-wr",					20  },
	{ chw(DSU_L3D_CACHE_REFILL_RD),			"dsu-l3d-cache-refill-rd",			27  },
	{ chw(DSU_L3D_CACHE_REFILL_WR),			"dsu-l3d-cache-refill-wr",			27  },
	{ chw(DSU_L3D_CACHE_RD_C0),				"dsu-l3d-cache-rd-c0",				23  },
	{ chw(DSU_L3D_CACHE_WR_C0),				"dsu-l3d-cache-wr-c0",				23  },
	{ chw(DSU_L3D_CACHE_REFILL_RD_C0),		"dsu-l3d-cache-refill-rd-c0",		30  },
	{ chw(DSU_L3D_CACHE_REFILL_WR_C0),		"dsu-l3d-cache-refill-wr-c0",		30  },
	{ chw(DSU_L3D_CACHE_RD_C1),				"dsu-l3d-cache-rd-c1",				23  },
	{ chw(DSU_L3D_CACHE_WR_C1),				"dsu-l3d-cache-wr-c1",				23  },
	{ chw(DSU_L3D_CACHE_REFILL_RD_C1),		"dsu-l3d-cache-refill-rd-c1",		30  },
	{ chw(DSU_L3D_CACHE_REFILL_WR_C1),		"dsu-l3d-cache-refill-wr-c1",		30  },
	{ chw(DSU_L3D_CACHE_RD_C2),				"dsu-l3d-cache-rd-c2",				23  },
	{ chw(DSU_L3D_CACHE_WR_C2),				"dsu-l3d-cache-wr-c2",				23  },
	{ chw(DSU_L3D_CACHE_REFILL_RD_C2),		"dsu-l3d-cache-refill-rd-c2",		30  },
	{ chw(DSU_L3D_CACHE_REFILL_WR_C2),		"dsu-l3d-cache-refill-wr-c2",		30  },
	{ chw(DSU_ACP_CYCLES),					"dsu-acp-cycles",					18  },
	{ chw(DSU_ACP_ACCESS),					"dsu-acp-access",					18  },
	{ chw(DSU_ACP_ACCESS_RD),				"dsu-acp-access-rd",				21  },
	{ chw(DSU_ACP_ACCESS_WR),				"dsu-acp-access-wr",				21  },
	{ chw(DSU_PPT_CYCLES),					"dsu-ppt-cycles",					18  },
	{ chw(DSU_PPT_ACCESS),					"dsu-ppt-access",					18  },
	{ chw(DSU_PPT_ACCESS_RD),				"dsu-ppt-access-rd",				21  },
	{ chw(DSU_PPT_ACCESS_WR),				"dsu-ppt-access-wr",				21  },
	{ chw(DSU_SCU_SNP_ACCESS),				"dsu-scu-snp-access",				22  },
	{ chw(DSU_SCU_SNP_EVICT),				"dsu-scu-snp-evict",				21  },
	{ chw(DSU_SCU_SNP_NO_CPU_SNP),			"dsu-scu-snp-no-cpu-snp",			26  },
	{ chw(DSU_SCU_PFTCH_CPU_ACCESS),		"dsu-scu-pftch-cpu-access",			27  },
	{ chw(DSU_SCU_PFTCH_CPU_MISS),			"dsu-scu-pftch-cpu-miss",			26  },
	{ chw(DSU_SCU_PFTCH_CPU_HIT),			"dsu-scu-pftch-cpu-hit",			25  },
	{ chw(DSU_SCU_PFTCH_CPU_MATCH),			"dsu-scu-pftch-cpu-match",			27  },
	{ chw(DSU_SCU_PFTCH_CPU_KILL),			"dsu-scu-pftch-cpu-kill",			26  },
	{ chw(DSU_SCU_PFTCH_CPU_DROPPED),		"dsu-scu-pftch-cpu-dropped",		29  },
	{ chw(DSU_SCU_STASH_ICN_ACCESS),		"dsu-scu-stash-icn-access",			27  },
	{ chw(DSU_SCU_STASH_ICN_MISS),			"dsu-scu-stash-icn-miss",			25  },
	{ chw(DSU_SCU_STASH_ICN_HIT),			"dsu-scu-stash-icn-hit",			24  },
	{ chw(DSU_SCU_STASH_ICN_MATCH),			"dsu-scu-stash-icn-match",			26  },
	{ chw(DSU_SCU_STASH_ICN_KILL),			"dsu-scu-stash-icn-kill",			25  },
	{ chw(DSU_SCU_STASH_ICN_DROPPED),		"dsu-scu-stash-icn-dropped",		29  },
	{ chw(DSU_SCU_STASH_ACP_ACCESS),		"dsu-scu-stash-acp-access",			27  },
	{ chw(DSU_SCU_STASH_ACP_MISS),			"dsu-scu-stash-acp-miss",			25  },
	{ chw(DSU_SCU_STASH_ACP_HIT),			"dsu-scu-stash-acp-hit",			24  },
	{ chw(DSU_SCU_STASH_ACP_MATCH),			"dsu-scu-stash-acp-match",			26  },
	{ chw(DSU_SCU_STASH_ACP_KILL),			"dsu-scu-stash-acp-kill",			25  },
	{ chw(DSU_SCU_STASH_ACP_DROPPED),		"dsu-scu-stash-acp-dropped",		29  },
	{ chw(DSU_SCU_HZD_WAYLOCK),				"dsu-scu-hzd-waylock",				23  },
	{ chw(DSU_SCU_HZD_HIT_IN_USE),			"dsu-scu-hzd-hit-in-use",			26  },
	{ chw(DSU_SCU_HZD_ADDRESS),				"dsu-scu-hzd-address",				23  },
	{ chw(DSU_SCU_HZD_STRUCT_AFB),			"dsu-scu-hzd-struct-afb",			26  },
	{ chw(DSU_SCU_HZD_STRUCT_BDQ),			"dsu-scu-hzd-struct-bdq",			26  },
	{ chw(DSU_SCU_HZD_STRUCT_BIB),			"dsu-scu-hzd-struct-bib",			26  },
	{ chw(DSU_SCU_BIB_ACCESS),				"dsu-scu-bib-access",				22  },
	{ chw(DSU_SCU_FID_LOCKED),				"dsu-scu-fid-locked",				22  },
	{ chw(DSU_SCU_SAFE_MODE_CYCLES),		"dsu-scu-safe-mode-cycles",			28  },
	{ chw(DSU_CFRQ_ENQ_STALL),				"dsu-cfrq-enq-stall",				22  },
	{ chw(DSU_FDQ_ENQ_STALL),				"dsu-fdq-enq-stall",				21  },
	{ chw(DSU_RXDAT_STALL),					"dsu-rxdat-stall",					19  },
	{ chw(DSU_CFRQ_LOW_OCC),				"dsu-cfrq-low-occ",					20  },
	{ chw(DSU_BDQ_EXIT_STALL),				"dsu-bdq-exit-stall",				22  },
	{ chw(DSU_RXSNPSHARED_C0),				"dsu-rxsnpshared-c0",				22  },
	{ chw(DSU_RXSNPSHARED_C1),				"dsu-rxsnpshared-c1",				22  },
	{ chw(DSU_RXSNPSHARED_C2),				"dsu-rxsnpshared-c2",				22  },
	{ chw(DSU_RXSNPSHARED_C3),				"dsu-rxsnpshared-c3",				22  },
	{ chw(DSU_RXSNPSHARED_C4),				"dsu-rxsnpshared-c4",				22  },
	{ chw(DSU_RXSNPSHARED_C5),				"dsu-rxsnpshared-c5",				22  },
	{ chw(DSU_RXSNPUNIQUE_C0),				"dsu-rxsnpunique-c0",				22  },
	{ chw(DSU_RXSNPUNIQUE_C1),				"dsu-rxsnpunique-c1",				22  },
	{ chw(DSU_RXSNPUNIQUE_C2),				"dsu-rxsnpunique-c2",				22  },
	{ chw(DSU_RXSNPUNIQUE_C3),				"dsu-rxsnpunique-c3",				22  },
	{ chw(DSU_RXSNPUNIQUE_C4),				"dsu-rxsnpunique-c4",				22  },
	{ chw(DSU_RXSNPUNIQUE_C5),				"dsu-rxsnpunique-c5",				22  },
	{ chw(DSU_SCU_STASH_PF_ACCESS),			"dsu-scu-stash-pf-access",			27  },
	{ chw(DSU_SCU_STASH_PF_MISS),			"dsu-scu-stash-pf-miss",			25  },
	{ chw(DSU_SCU_STASH_PF_HIT),			"dsu-scu-stash-pf-hit",				22  },
	{ chw(DSU_SCU_STASH_PF_MATCH),			"dsu-scu-stash-pf-match",			26  },
	{ chw(DSU_SCU_STASH_PF_KILL),			"dsu-scu-stash-pf-kill",			25  },
	{ chw(DSU_SCU_STASH_PF_DROPPED),		"dsu-scu-stash-pf-dropped",			28  },
	{ chw(DSU_L3D_CACHE_REFILL_C5_NO_PFT),	 "dsu-l3d-cache-refill-c5-no-pft",	34  },
#endif /* PERF_PLATFORM_NAMTSO_SYMBOL */

#ifdef PERF_PLATFORM_NAMTSO_MAP
	[dsu_index(HM_HW_DSU_CHAIN)]						= DSU_PMU_CHAIN,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_RD)]				= DSU_PMU_BUS_ACCESS_RD,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_WR)]				= DSU_PMU_BUS_ACCESS_WR,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_SHARED)]			= DSU_PMU_BUS_ACCESS_SHARED,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_NOTSHARED)]			= DSU_PMU_BUS_ACCESS_NOTSHARED,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_NORMAL)]			= DSU_PMU_BUS_ACCESS_NORMAL,
	[dsu_index(HM_HW_DSU_BUS_ACCESS_PERIPH)]			= DSU_PMU_BUS_ACCESS_PERIPH,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C0)]			= DSU_PMU_L3D_CACHE_REFILL_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C0)]					= DSU_PMU_L3D_CACHE_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C1)]			= DSU_PMU_L3D_CACHE_REFILL_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C1)]					= DSU_PMU_L3D_CACHE_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C2)]			= DSU_PMU_L3D_CACHE_REFILL_C2,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C2)]					= DSU_PMU_L3D_CACHE_C2,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C3)]			= DSU_PMU_L3D_CACHE_REFILL_C3,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C3)]					= DSU_PMU_L3D_CACHE_C3,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C4)]			= DSU_PMU_L3D_CACHE_REFILL_C4,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C4)]					= DSU_PMU_L3D_CACHE_C4,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C5)]			= DSU_PMU_L3D_CACHE_REFILL_C5,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C5)]					= DSU_PMU_L3D_CACHE_C5,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C5_TH0)]		= DSU_PMU_L3D_CACHE_REFILL_C5_TH0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C5_TH1)]		= DSU_PMU_L3D_CACHE_REFILL_C5_TH1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C5_TH0)]				= DSU_PMU_L3D_CACHE_C5_TH0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_C5_TH1)]				= DSU_PMU_L3D_CACHE_C5_TH1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_RD)]					= DSU_PMU_L3D_CACHE_RD,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WR)]					= DSU_PMU_L3D_CACHE_WR,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_RD)]			= DSU_PMU_L3D_CACHE_REFILL_RD,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_WR)]			= DSU_PMU_L3D_CACHE_REFILL_WR,
	[dsu_index(HM_HW_DSU_L3D_CACHE_RD_C0)]				= DSU_PMU_L3D_CACHE_RD_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WR_C0)]				= DSU_PMU_L3D_CACHE_WR_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_RD_C0)]		= DSU_PMU_L3D_CACHE_REFILL_RD_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_WR_C0)]		= DSU_PMU_L3D_CACHE_REFILL_WR_C0,
	[dsu_index(HM_HW_DSU_L3D_CACHE_RD_C1)]				= DSU_PMU_L3D_CACHE_RD_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WR_C1)]				= DSU_PMU_L3D_CACHE_WR_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_RD_C1)]		= DSU_PMU_L3D_CACHE_REFILL_RD_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_WR_C1)]		= DSU_PMU_L3D_CACHE_REFILL_WR_C1,
	[dsu_index(HM_HW_DSU_L3D_CACHE_RD_C2)]				= DSU_PMU_L3D_CACHE_RD_C2,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WR_C2)]				= DSU_PMU_L3D_CACHE_WR_C2,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_RD_C2)]		= DSU_PMU_L3D_CACHE_REFILL_RD_C2,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_WR_C2)]		= DSU_PMU_L3D_CACHE_REFILL_WR_C2,
	[dsu_index(HM_HW_DSU_ACP_CYCLES)]					= DSU_PMU_ACP_CYCLES,
	[dsu_index(HM_HW_DSU_ACP_ACCESS)]					= DSU_PMU_ACP_ACCESS,
	[dsu_index(HM_HW_DSU_ACP_ACCESS_RD)]				= DSU_PMU_ACP_ACCESS_RD,
	[dsu_index(HM_HW_DSU_ACP_ACCESS_WR)]				= DSU_PMU_ACP_ACCESS_WR,
	[dsu_index(HM_HW_DSU_PPT_CYCLES)]					= DSU_PMU_PPT_CYCLES,
	[dsu_index(HM_HW_DSU_PPT_ACCESS)]					= DSU_PMU_PPT_ACCESS,
	[dsu_index(HM_HW_DSU_PPT_ACCESS_RD)]				= DSU_PMU_PPT_ACCESS_RD,
	[dsu_index(HM_HW_DSU_PPT_ACCESS_WR)]				= DSU_PMU_PPT_ACCESS_WR,
	[dsu_index(HM_HW_DSU_SCU_SNP_ACCESS)]				= DSU_PMU_SCU_SNP_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_SNP_EVICT)]				= DSU_PMU_SCU_SNP_EVICT,
	[dsu_index(HM_HW_DSU_SCU_SNP_NO_CPU_SNP)]			= DSU_PMU_SCU_SNP_NO_CPU_SNP,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_ACCESS)]			= DSU_PMU_SCU_PFTCH_CPU_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_MISS)]			= DSU_PMU_SCU_PFTCH_CPU_MISS,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_HIT)]			= DSU_PMU_SCU_PFTCH_CPU_HIT,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_MATCH)]			= DSU_PMU_SCU_PFTCH_CPU_MATCH,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_KILL)]			= DSU_PMU_SCU_PFTCH_CPU_KILL,
	[dsu_index(HM_HW_DSU_SCU_PFTCH_CPU_DROPPED)]		= DSU_PMU_SCU_PFTCH_CPU_DROPPED,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_ACCESS)]			= DSU_PMU_SCU_STASH_ICN_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_MISS)]			= DSU_PMU_SCU_STASH_ICN_MISS,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_HIT)]			= DSU_PMU_SCU_STASH_ICN_HIT,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_MATCH)]			= DSU_PMU_SCU_STASH_ICN_MATCH,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_KILL)]			= DSU_PMU_SCU_STASH_ICN_KILL,
	[dsu_index(HM_HW_DSU_SCU_STASH_ICN_DROPPED)]		= DSU_PMU_SCU_STASH_ICN_DROPPED,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_ACCESS)]			= DSU_PMU_SCU_STASH_ACP_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_MISS)]			= DSU_PMU_SCU_STASH_ACP_MISS,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_HIT)]			= DSU_PMU_SCU_STASH_ACP_HIT,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_MATCH)]			= DSU_PMU_SCU_STASH_ACP_MATCH,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_KILL)]			= DSU_PMU_SCU_STASH_ACP_KILL,
	[dsu_index(HM_HW_DSU_SCU_STASH_ACP_DROPPED)]		= DSU_PMU_SCU_STASH_ACP_DROPPED,
	[dsu_index(HM_HW_DSU_SCU_HZD_WAYLOCK)]				= DSU_PMU_SCU_HZD_WAYLOCK,
	[dsu_index(HM_HW_DSU_SCU_HZD_HIT_IN_USE)]			= DSU_PMU_SCU_HZD_HIT_IN_USE,
	[dsu_index(HM_HW_DSU_SCU_HZD_ADDRESS)]				= DSU_PMU_SCU_HZD_ADDRESS,
	[dsu_index(HM_HW_DSU_SCU_HZD_STRUCT_AFB)]			= DSU_PMU_SCU_HZD_STRUCT_AFB,
	[dsu_index(HM_HW_DSU_SCU_HZD_STRUCT_BDQ)]			= DSU_PMU_SCU_HZD_STRUCT_BDQ,
	[dsu_index(HM_HW_DSU_SCU_HZD_STRUCT_BIB)]			= DSU_PMU_SCU_HZD_STRUCT_BIB,
	[dsu_index(HM_HW_DSU_SCU_BIB_ACCESS)]				= DSU_PMU_SCU_BIB_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_FID_LOCKED)]				= DSU_PMU_SCU_FID_LOCKED,
	[dsu_index(HM_HW_DSU_SCU_SAFE_MODE_CYCLES)]			= DSU_PMU_SCU_SAFE_MODE_CYCLES,
	[dsu_index(HM_HW_DSU_CFRQ_ENQ_STALL)]				= DSU_PMU_CFRQ_ENQ_STALL,
	[dsu_index(HM_HW_DSU_FDQ_ENQ_STALL)]				= DSU_PMU_FDQ_ENQ_STALL,
	[dsu_index(HM_HW_DSU_RXDAT_STALL)]					= DSU_PMU_RXDAT_STALL,
	[dsu_index(HM_HW_DSU_CFRQ_LOW_OCC)]					= DSU_PMU_CFRQ_LOW_OCC,
	[dsu_index(HM_HW_DSU_BDQ_EXIT_STALL)]				= DSU_PMU_BDQ_EXIT_STALL,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C0)]				= DSU_PMU_RXSNPSHARED_C0,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C1)]				= DSU_PMU_RXSNPSHARED_C1,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C2)]				= DSU_PMU_RXSNPSHARED_C2,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C3)]				= DSU_PMU_RXSNPSHARED_C3,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C4)]				= DSU_PMU_RXSNPSHARED_C4,
	[dsu_index(HM_HW_DSU_RXSNPSHARED_C5)]				= DSU_PMU_RXSNPSHARED_C5,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C0)]				= DSU_PMU_RXSNPUNIQUE_C0,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C1)]				= DSU_PMU_RXSNPUNIQUE_C1,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C2)]				= DSU_PMU_RXSNPUNIQUE_C2,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C3)]				= DSU_PMU_RXSNPUNIQUE_C3,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C4)]				= DSU_PMU_RXSNPUNIQUE_C4,
	[dsu_index(HM_HW_DSU_RXSNPUNIQUE_C5)]				= DSU_PMU_RXSNPUNIQUE_C5,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_ACCESS)]			= DSU_PMU_SCU_STASH_PF_ACCESS,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_MISS)]			= DSU_PMU_SCU_STASH_PF_MISS,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_HIT)]				= DSU_PMU_SCU_STASH_PF_HIT,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_MATCH)]			= DSU_PMU_SCU_STASH_PF_MATCH,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_KILL)]			= DSU_PMU_SCU_STASH_PF_KILL,
	[dsu_index(HM_HW_DSU_SCU_STASH_PF_DROPPED)]			= DSU_PMU_SCU_STASH_PF_DROPPED,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILL_C5_NO_PFT)] 	= DSU_PMU_L3D_CACHE_REFILL_C5_NO_PFT,
#endif /* PERF_PLATFORM_NAMTSO_MAP */