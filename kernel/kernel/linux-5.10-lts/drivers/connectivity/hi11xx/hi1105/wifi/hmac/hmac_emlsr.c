/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : UAPSD hmac层处理
 * 作    者 :
 * 创建日期 : 2013年9月18日
 */
#ifdef _PRE_WLAN_FEATURE_EMLSR

#include "mac_vap.h"
#include "mac_frame.h"
#include "dpe_wlan_vap.h"
#include "hmac_config.h"
#include "hd_event_emlsr.h"
#include "hmac_chan_mgmt.h"
#include "hmac_emlsr.h"
#include "hmac_fsm.h"
#include "mac_frame_inl.h"
#include "hmac_mld_ext.h"
#ifdef _PRE_WLAN_FEATURE_WUR
#include "hmac_wur.h"
#endif
#include "hmac_11ax.h"
#include "hmac_roam_alg.h"
#include "hmac_roam_main.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID      OAM_FILE_ID_HMAC_EMLSR_C
emlsr_share_ram_stru g_hmac_share_ram = {
{0x8c, 0x000001fa,
    0x90, 0x0000017e,
    0x94, 0x00001f55,
    0x98, 0x0112e007,
    0xa8, 0x0030007f,
    0xac, 0x0000003f,
    0x51074, 0x00001006,
    0x51060, 0x00000003,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x8c, 0x000001ff,
    0x90, 0x000001ff,
    0x94, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff,
},
{0x8c, 0x000001fa,
    0x90, 0x0000017e,
    0x94, 0x00001f55,
    0x98, 0x0112e007,
    0xa8, 0x0030007f,
    0xac, 0x0000003f,
    0x51074, 0x00001006,
    0x51060, 0x00000003,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x8c, 0x000001ff,
    0x90, 0x000001ff,
    0x94, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff,
},
{0x8c, 0x000001fa,
    0x90, 0x0000017e,
    0x94, 0x00001f55,
    0x98, 0x0112e007,
    0xa8, 0x0030007f,
    0xac, 0x0000003f,
    0x51074, 0x0000100e,
    0x51060, 0x00000013,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x8c, 0x000001ff,
    0x90, 0x000001ff,
    0x94, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff,
},
{0x8c, 0x000001fa,
    0x90, 0x0000017e,
    0x94, 0x00001f55,
    0x98, 0x0112e007,
    0xa8, 0x0030007f,
    0xac, 0x0000003f,
    0x51074, 0x0000100e,
    0x51060, 0x00000013,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x8c, 0x000001ff,
    0x90, 0x000001ff,
    0x94, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff,
}};
emlsr_trx_time_ram_stru g_hmac_trx_time_ram = {
{0x10868, 0x00000001,
    0x60868, 0x00000001,
    0x10824, 0x00000031,
    0x6080c, 0x0000f070,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x62a84, 0x00020011,
    0x12814, 0x000000d1,
    0x12818, 0x00000000,
    0x12848, 0x007d30f6,
    0x12870, 0x03f003e0,
    0x12874, 0x000003dc,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x129d4, 0x00001a20,
    0x12a88, 0x02005068,
    0x62a8c, 0x003438e4,
    0x62a90, 0x00840074,
    0x62a94, 0x00600046,
    0x62a98, 0x00250010,
    0x62a9c, 0x01fb01e5,
    0x62aa0, 0x000000ec,
    0x62ab8, 0x00000000,
    0x62abc, 0x00000000,
    0x62ac0, 0x00000000,
    0x62ac4, 0x00000000,
    0x62ac8, 0x00000000,
    0x62acc, 0x00000000,
    0x62ad0, 0x00000000,
    0x62ad4, 0x00000000,
    0x12ae0, 0x02005068,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x632dc, 0x00800000,
    0x632e0, 0x00800080,
    0x632ec, 0x80808080,
    0x632f0, 0x80808080,
    0x632f4, 0x80808080,
    0x632f8, 0x80808080,
    0x64ab0, 0x80808080,
    0x64ab4, 0x80808080,
    0x64ab8, 0x80808080,
    0x64abc, 0x80808080,
    0x64ac0, 0x80808080,
    0x64ac4, 0x80808080,
    0x64ac8, 0x80808080,
    0x64acc, 0x80808080,
    0x64af0, 0x80808080,
    0x64af4, 0x80808080,
    0x64b1c, 0x00000000,
    0x64b20, 0x00000000,
    0x64b24, 0x00000000,
    0x64b28, 0x00000000,
    0x64b44, 0x00000000,
    0x64b48, 0x00000000,
    0x64b4c, 0x00000000,
    0x64b50, 0x00000000,
    0x64b54, 0x00000000,
    0x64b58, 0x00000000,
    0x64b5c, 0x00000000,
    0x64b60, 0x00000000,
    0x64b64, 0x00000000,
    0x64b68, 0x00000000,
    0x64b6c, 0x00000000,
    0x64b70, 0x00000000,
    0x64b74, 0x00000000,
    0x64b78, 0x00000000,
    0x64b7c, 0x00000000,
    0x64b80, 0x00000000,
    0x64b84, 0x00000000,
    0x64b88, 0x00000000,
    0x64b8c, 0x00000000,
    0x64b90, 0x00000000,
    0x64b94, 0x00000000,
    0x64b98, 0x00000000,
    0x64b9c, 0x00000000,
    0x64ba0, 0x00000000,
    0x64ba4, 0x00000000,
    0x64ba8, 0x00000000,
    0x64bac, 0x00000000,
    0x64bb0, 0x00000000,
    0x64bb4, 0x00000000,
    0x64bb8, 0x00000000,
    0x64bbc, 0x00000000,
    0x64bc0, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x6112c, 0x00000000,
    0x61130, 0x00000000,
    0x61134, 0x00000000,
    0x61138, 0x00000000,
    0x6113c, 0x00000000,
    0x61140, 0x00000000,
    0x61144, 0x00000000,
    0x61148, 0x00000000,
    0x611d8, 0x00000000,
    0x611dc, 0x00000000,
    0x611e0, 0x00000000,
    0x611e4, 0x00000000,
    0x611e8, 0x00000000,
    0x611ec, 0x00000000,
    0x611f0, 0x00000000,
    0x611f4, 0x00000000,
    0x61174, 0x00000000,
    0x6119c, 0x00000000,
    0x11288, 0x00000000,
    0x11128, 0x00000001,
    0x11170, 0x00000001,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x1114c, 0x00000001,
    0x11178, 0x00000001,
    0x1129c, 0x00000090,
    0x1121c, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x6394c, 0x00000000,
    0x63950, 0x00000000,
    0x63954, 0x00000000,
    0x63958, 0x00000800,
    0x6395c, 0x00000000,
    0x63960, 0x00000000,
    0x63964, 0x00000000,
    0x63968, 0x00000000,
    0x6396c, 0x00000000,
    0x63970, 0x00000000,
    0x63974, 0x00000800,
    0x63978, 0x00000000,
    0x6397c, 0x00000000,
    0x63980, 0x00000000,
    0x639cc, 0x00080008,
    0x639d0, 0x00010004,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x639c8, 0x00000004,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x618cc, 0x00000000,
    0x618d0, 0x00000000,
    0x618d4, 0x00000000,
    0x618d8, 0x00000000,
    0x618dc, 0x00000000,
    0x618e0, 0x00000000,
    0x618e4, 0x00000000,
    0x618e8, 0x00000000,
    0x618ec, 0x00000000,
    0x618f0, 0x00000000,
    0x618f4, 0x00000000,
    0x618f8, 0x00000000,
    0x618fc, 0x00000000,
    0x61900, 0x00000000,
    0x61904, 0x00000000,
    0x61908, 0x00000000,
    0x11824, 0x00000000,
    0x11828, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000000a,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000003,
    0x60868, 0x00000003,
},
{0x10868, 0x00000001,
    0x60868, 0x00000001,
    0x10824, 0x00000031,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12818, 0x00000000,
    0x12848, 0x007d30f6,
    0x12870, 0x03f003e0,
    0x12874, 0x000003dc,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x129d4, 0x00001a20,
    0x12a88, 0x02005068,
    0x12ae0, 0x02005068,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16010, 0x821a1000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16140, 0x001a00e0,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x11288, 0x00000000,
    0x11128, 0x00000001,
    0x11170, 0x00000001,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x1114c, 0x00000001,
    0x11178, 0x00000001,
    0x1129c, 0x00000090,
    0x1121c, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11824, 0x00000000,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000000a,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000003,
    0x60868, 0x00000003,
},
{0x10868, 0x00000001,
    0x60868, 0x00000001,
    0x10824, 0x00000031,
    0x6080c, 0x0000f070,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x62a84, 0x00020011,
    0x12814, 0x000000d1,
    0x12818, 0x00000000,
    0x12848, 0x007d30f6,
    0x12870, 0x03f003e0,
    0x12874, 0x000003dc,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x129d4, 0x00001a20,
    0x12a88, 0x02005068,
    0x62a8c, 0x003438e4,
    0x62a90, 0x00840074,
    0x62a94, 0x00600046,
    0x62a98, 0x00250010,
    0x62a9c, 0x01fb01e5,
    0x62aa0, 0x000000ec,
    0x62ab8, 0x00000000,
    0x62abc, 0x00000000,
    0x62ac0, 0x00000000,
    0x62ac4, 0x00000000,
    0x62ac8, 0x00000000,
    0x62acc, 0x00000000,
    0x62ad0, 0x00000000,
    0x62ad4, 0x00000000,
    0x12ae0, 0x02005068,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x632dc, 0x00800000,
    0x632e0, 0x00800080,
    0x632ec, 0x80808080,
    0x632f0, 0x80808080,
    0x632f4, 0x80808080,
    0x632f8, 0x80808080,
    0x64ab0, 0x80808080,
    0x64ab4, 0x80808080,
    0x64ab8, 0x80808080,
    0x64abc, 0x80808080,
    0x64ac0, 0x80808080,
    0x64ac4, 0x80808080,
    0x64ac8, 0x80808080,
    0x64acc, 0x80808080,
    0x64af0, 0x80808080,
    0x64af4, 0x80808080,
    0x64b1c, 0x00000000,
    0x64b20, 0x00000000,
    0x64b24, 0x00000000,
    0x64b28, 0x00000000,
    0x64b44, 0x00000000,
    0x64b48, 0x00000000,
    0x64b4c, 0x00000000,
    0x64b50, 0x00000000,
    0x64b54, 0x00000000,
    0x64b58, 0x00000000,
    0x64b5c, 0x00000000,
    0x64b60, 0x00000000,
    0x64b64, 0x00000000,
    0x64b68, 0x00000000,
    0x64b6c, 0x00000000,
    0x64b70, 0x00000000,
    0x64b74, 0x00000000,
    0x64b78, 0x00000000,
    0x64b7c, 0x00000000,
    0x64b80, 0x00000000,
    0x64b84, 0x00000000,
    0x64b88, 0x00000000,
    0x64b8c, 0x00000000,
    0x64b90, 0x00000000,
    0x64b94, 0x00000000,
    0x64b98, 0x00000000,
    0x64b9c, 0x00000000,
    0x64ba0, 0x00000000,
    0x64ba4, 0x00000000,
    0x64ba8, 0x00000000,
    0x64bac, 0x00000000,
    0x64bb0, 0x00000000,
    0x64bb4, 0x00000000,
    0x64bb8, 0x00000000,
    0x64bbc, 0x00000000,
    0x64bc0, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x6112c, 0x00000000,
    0x61130, 0x00000000,
    0x61134, 0x00000000,
    0x61138, 0x00000000,
    0x6113c, 0x00000000,
    0x61140, 0x00000000,
    0x61144, 0x00000000,
    0x61148, 0x00000000,
    0x611d8, 0x00000000,
    0x611dc, 0x00000000,
    0x611e0, 0x00000000,
    0x611e4, 0x00000000,
    0x611e8, 0x00000000,
    0x611ec, 0x00000000,
    0x611f0, 0x00000000,
    0x611f4, 0x00000000,
    0x61174, 0x00000000,
    0x6119c, 0x00000000,
    0x11288, 0x00000000,
    0x11128, 0x00000001,
    0x11170, 0x00000001,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x1114c, 0x00000001,
    0x11178, 0x00000001,
    0x1129c, 0x00000090,
    0x1121c, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x6394c, 0x00000000,
    0x63950, 0x00000000,
    0x63954, 0x00000000,
    0x63958, 0x00000800,
    0x6395c, 0x00000000,
    0x63960, 0x00000000,
    0x63964, 0x00000000,
    0x63968, 0x00000000,
    0x6396c, 0x00000000,
    0x63970, 0x00000000,
    0x63974, 0x00000800,
    0x63978, 0x00000000,
    0x6397c, 0x00000000,
    0x63980, 0x00000000,
    0x639cc, 0x00080008,
    0x639d0, 0x00010004,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x639c8, 0x00000004,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x618cc, 0x00000000,
    0x618d0, 0x00000000,
    0x618d4, 0x00000000,
    0x618d8, 0x00000000,
    0x618dc, 0x00000000,
    0x618e0, 0x00000000,
    0x618e4, 0x00000000,
    0x618e8, 0x00000000,
    0x618ec, 0x00000000,
    0x618f0, 0x00000000,
    0x618f4, 0x00000000,
    0x618f8, 0x00000000,
    0x618fc, 0x00000000,
    0x61900, 0x00000000,
    0x61904, 0x00000000,
    0x61908, 0x00000000,
    0x11824, 0x00000000,
    0x11828, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000000a,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000003,
    0x60868, 0x00000003,
},
{0x10868, 0x00000001,
    0x60868, 0x00000001,
    0x10824, 0x00000031,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12818, 0x00000000,
    0x12848, 0x007d30f6,
    0x12870, 0x03f003e0,
    0x12874, 0x000003dc,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x129d4, 0x00001a20,
    0x12a88, 0x02005068,
    0x12ae0, 0x02005068,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16010, 0x821a1000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16140, 0x001a00e0,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x11288, 0x00000000,
    0x11128, 0x00000001,
    0x11170, 0x00000001,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x1114c, 0x00000001,
    0x11178, 0x00000001,
    0x1129c, 0x00000090,
    0x1121c, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11824, 0x00000000,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000000a,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000003,
    0x60868, 0x00000003,
}};
emlsr_tx_freq_ram_stru g_hmac_tx_freq_ram = {
{0x4c114, 0x40026400,
    0x4c03c, 0x000000140,
},
{0x4c114, 0x40026400,
    0x4c03c, 0x000000140,
},
{0x4c114, 0x40026400,
    0x4c03c, 0x000000140,
},
{0x4c114, 0x40026400,
    0x4c03c, 0x000000140,
}};
emlsr_rx_freq_ram_stru g_hmac_rx_freq_ram = {
{0x40900, 0x20026400,
    0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000000,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x0cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000000,
    0x4281c, 0x54324130,
    0x42860, 0x01101101,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000016,
    0x42884, 0x086455a1,
    0x428b4, 0x00000304,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010648,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff2,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01860320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400180,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c1,
    0x42b08, 0x000000c1,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a,
},
{0x40900, 0x20026400,
    0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000000,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x0cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000000,
    0x4281c, 0x54324130,
    0x42860, 0x01101101,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000016,
    0x42884, 0x086455a1,
    0x428b4, 0x00000304,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010648,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff2,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01860320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400180,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c1,
    0x42b08, 0x000000c1,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a,
},
{0x40900, 0x20026400,
    0x40980, 0x00000000,
    0x40984, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x0cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000000,
    0x4281c, 0x54324130,
    0x42860, 0x01101101,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000016,
    0x42884, 0x086455a1,
    0x428b4, 0x00000304,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010648,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff2,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01860320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400180,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c1,
    0x42b08, 0x000000c1,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a,
},
{0x40900, 0x20026400,
    0x40980, 0x00000000,
    0x40984, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x0cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000000,
    0x4281c, 0x54324130,
    0x42860, 0x01101101,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000016,
    0x42884, 0x086455a1,
    0x428b4, 0x00000304,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010648,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff2,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01860320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400180,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c1,
    0x42b08, 0x000000c1,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a,
}};
/* FPGA配置，不再维护 */
emlsr_share_ram_stru g_hmac_share_ram_fpga = {
{0x0008c, 0x000001fa,
    0x00090, 0x000001fe,
    0x00094, 0x00001f55,
    0x000a8, 0x0030007f,
    0x00098, 0x01120007,
    0x000ac, 0x0000003f,
    0x51074, 0x00001006,
    0x51060, 0x00000003,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x0008c, 0x000001ff,
    0x00090, 0x000001ff,
    0x00094, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff
},
{0x0008c, 0x000001fa,
    0x00090, 0x000001fe,
    0x00094, 0x00001f55,
    0x000a8, 0x0030007f,
    0x00098, 0x01120007,
    0x000ac, 0x0000003f,
    0x51074, 0x00001006,
    0x51060, 0x00000003,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x0008c, 0x000001ff,
    0x00090, 0x000001ff,
    0x00094, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff
},
{0x0008c, 0x000001fa,
    0x00090, 0x000001fe,
    0x00094, 0x00001f55,
    0x000a8, 0x0031017f,
    0x00098, 0x01120007,
    0x000ac, 0x0000003f,
    0x51074, 0x0000100e,
    0x51060, 0x0000000b,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x0008c, 0x000001ff,
    0x00090, 0x000001ff,
    0x00094, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff
},
{0x0008c, 0x000001fa,
    0x00090, 0x000001fe,
    0x00094, 0x00001f55,
    0x000a8, 0x0031017f,
    0x00098, 0x01120007,
    0x000ac, 0x0000003f,
    0x51074, 0x0000100e,
    0x51060, 0x0000000b,
    0x51000, 0x0000000a,
    0x51004, 0x0000000a,
    0x51014, 0x00000006,
    0x51018, 0x00000006,
    0x5101c, 0x00000050,
    0x51020, 0x00000005,
    0x51024, 0x0000000a,
    0x51028, 0x0000000a,
    0x5102c, 0x00000006,
    0x51030, 0x00000006,
    0x51058, 0x00000fff,
    0x5106c, 0x00000000,
    0x51070, 0x00000006,
    0x51078, 0x00000000,
    0x51080, 0x00000001,
    0x50014, 0x00510080,
    0x5006c, 0x00005000,
    0x500ec, 0x00000000,
    0x50814, 0x00510080,
    0x0008c, 0x000001ff,
    0x00090, 0x000001ff,
    0x00094, 0x00001fff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000f0f,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0xfffff, 0xffffffff,
    0x51058, 0x00000fff}
    };
emlsr_trx_time_ram_stru g_hmac_trx_time_ram_fpga = {
{0x10868, 0x00000005,
    0x60868, 0x00000005,
    0x10824, 0x00000031,
    0x6080c, 0x0000f070,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12848, 0x007d30f6,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x62a84, 0x00020011,
    0x62a8c, 0x003438e4,
    0x62a90, 0x00840074,
    0x62a94, 0x00600046,
    0x62a98, 0x00250010,
    0x62a9c, 0x01fb01e5,
    0x62aa0, 0x000000ec,
    0x62ab8, 0x00000000,
    0x62abc, 0x00000000,
    0x62ac0, 0x00000000,
    0x62ac4, 0x00000000,
    0x62ac8, 0x00000000,
    0x62acc, 0x00000000,
    0x62ad0, 0x00000000,
    0x62ad4, 0x00000000,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x632dc, 0x30cc0080,
    0x632e0, 0x00800080,
    0x632ec, 0x80808080,
    0x632f0, 0x80808080,
    0x632f4, 0x80808080,
    0x632f8, 0x80808080,
    0x64ab0, 0x80808080,
    0x64ab4, 0x80808080,
    0x64ab8, 0x80808080,
    0x64abc, 0x80808080,
    0x64ac0, 0x80808080,
    0x64ac4, 0x80808080,
    0x64ac8, 0x80808080,
    0x64acc, 0x80808080,
    0x64af0, 0x80808080,
    0x64af4, 0x80808080,
    0x64b1c, 0x0004000c,
    0x64b20, 0x00000000,
    0x64b24, 0x00000000,
    0x64b28, 0x00000000,
    0x64b44, 0x000000eb,
    0x64b48, 0x00000084,
    0x64b4c, 0x0000004a,
    0x64b50, 0x0000002a,
    0x64b54, 0x00000018,
    0x64b58, 0x0000000d,
    0x64b5c, 0x00000000,
    0x64b60, 0x00000000,
    0x64b64, 0x00000000,
    0x64b68, 0x00000000,
    0x64b6c, 0x00000000,
    0x64b70, 0x00000000,
    0x64b74, 0x00000000,
    0x64b78, 0x00000000,
    0x64b7c, 0x00000000,
    0x64b80, 0x00000000,
    0x64b84, 0x00000000,
    0x64b88, 0x00000000,
    0x64b8c, 0x00000000,
    0x64b90, 0x00000000,
    0x64b94, 0x00000000,
    0x64b98, 0x00000000,
    0x64b9c, 0x00000000,
    0x64ba0, 0x00000000,
    0x64ba4, 0x00000000,
    0x64ba8, 0x00000000,
    0x64bac, 0x00000000,
    0x64bb0, 0x00000000,
    0x64bb4, 0x00000000,
    0x64bb8, 0x00000000,
    0x64bbc, 0x00000000,
    0x64bc0, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x00000000,
    0x16038, 0x0002b03a,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x6112c, 0x00000000,
    0x61130, 0x00000000,
    0x61134, 0x00000000,
    0x61138, 0x00000000,
    0x6113c, 0x00000000,
    0x61140, 0x00000000,
    0x61144, 0x00000000,
    0x61148, 0x00000000,
    0x611d8, 0x00000000,
    0x611dc, 0x00000000,
    0x611e0, 0x00000000,
    0x611e4, 0x00000000,
    0x611e8, 0x00000000,
    0x611ec, 0x00000000,
    0x611f0, 0x00000000,
    0x611f4, 0x00000000,
    0x61174, 0x00000000,
    0x6119c, 0x00000000,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11128, 0x00000001,
    0x1114c, 0x00000001,
    0x11170, 0x00000001,
    0x11178, 0x00000001,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x1121c, 0x00000011,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x11288, 0x00000000,
    0x1129c, 0x00000011,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x6394c, 0x00000000,
    0x63950, 0x00000000,
    0x63954, 0x00000000,
    0x63958, 0x00000800,
    0x6395c, 0x00000000,
    0x63960, 0x00000000,
    0x63964, 0x00000000,
    0x63968, 0x00000000,
    0x6396c, 0x00000000,
    0x63970, 0x00000000,
    0x63974, 0x00000800,
    0x63978, 0x00000000,
    0x6397c, 0x00000000,
    0x63980, 0x00000000,
    0x639c8, 0x00000004,
    0x639cc, 0x00080008,
    0x639d0, 0x00010004,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x618cc, 0x00000000,
    0x618d0, 0x00000000,
    0x618d4, 0x00000000,
    0x618d8, 0x00000000,
    0x618dc, 0x00000000,
    0x618e0, 0x00000000,
    0x618e4, 0x00000000,
    0x618e8, 0x00000000,
    0x618ec, 0x00000000,
    0x618f0, 0x00000000,
    0x618f4, 0x00000000,
    0x618f8, 0x00000000,
    0x618fc, 0x00000000,
    0x61900, 0x00000000,
    0x61904, 0x00000000,
    0x61908, 0x00000000,
    0x11824, 0x00000001,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x20000000,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000007,
    0x60868, 0x00000007
},
{0x10868, 0x00000005,
    0x60868, 0x00000005,
    0x10824, 0x00000031,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12848, 0x007d30f6,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11128, 0x00000001,
    0x1114c, 0x00000001,
    0x11170, 0x00000001,
    0x11178, 0x00000001,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11218, 0x00000011,
    0x1121c, 0x00000011,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x11288, 0x00000000,
    0x11298, 0x00000011,
    0x1129c, 0x00000011,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11824, 0x00000001,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x00000028,
    0x14028, 0x00000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000400a,
    0x14034, 0x00000000,
    0x10868, 0x00000007,
    0x60868, 0x00000007
},
{0x10868, 0x00000005,
    0x60868, 0x00000005,
    0x10824, 0x00000011,
    0x6080c, 0x0000f070,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12848, 0x007d30f6,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x62a84, 0x00020011,
    0x62a8c, 0x003438e4,
    0x62a90, 0x00840074,
    0x62a94, 0x00600046,
    0x62a98, 0x00250010,
    0x62a9c, 0x01fb01e5,
    0x62aa0, 0x000000ec,
    0x62ab8, 0x00000000,
    0x62abc, 0x00000000,
    0x62ac0, 0x00000000,
    0x62ac4, 0x00000000,
    0x62ac8, 0x00000000,
    0x62acc, 0x00000000,
    0x62ad0, 0x00000000,
    0x62ad4, 0x00000000,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x632dc, 0x30cc0080,
    0x632e0, 0x00800080,
    0x632ec, 0x80808080,
    0x632f0, 0x80808080,
    0x632f4, 0x80808080,
    0x632f8, 0x80808080,
    0x64ab0, 0x80808080,
    0x64ab4, 0x80808080,
    0x64ab8, 0x80808080,
    0x64abc, 0x80808080,
    0x64ac0, 0x80808080,
    0x64ac4, 0x80808080,
    0x64ac8, 0x80808080,
    0x64acc, 0x80808080,
    0x64af0, 0x80808080,
    0x64af4, 0x80808080,
    0x64b1c, 0x0004000c,
    0x64b20, 0x00000000,
    0x64b24, 0x00000000,
    0x64b28, 0x00000000,
    0x64b44, 0x000000eb,
    0x64b48, 0x00000084,
    0x64b4c, 0x0000004a,
    0x64b50, 0x0000002a,
    0x64b54, 0x00000018,
    0x64b58, 0x0000000d,
    0x64b5c, 0x00000000,
    0x64b60, 0x00000000,
    0x64b64, 0x00000000,
    0x64b68, 0x00000000,
    0x64b6c, 0x00000000,
    0x64b70, 0x00000000,
    0x64b74, 0x00000000,
    0x64b78, 0x00000000,
    0x64b7c, 0x00000000,
    0x64b80, 0x00000000,
    0x64b84, 0x00000000,
    0x64b88, 0x00000000,
    0x64b8c, 0x00000000,
    0x64b90, 0x00000000,
    0x64b94, 0x00000000,
    0x64b98, 0x00000000,
    0x64b9c, 0x00000000,
    0x64ba0, 0x00000000,
    0x64ba4, 0x00000000,
    0x64ba8, 0x00000000,
    0x64bac, 0x00000000,
    0x64bb0, 0x00000000,
    0x64bb4, 0x00000000,
    0x64bb8, 0x00000000,
    0x64bbc, 0x00000000,
    0x64bc0, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x6112c, 0x00000000,
    0x61130, 0x00000000,
    0x61134, 0x00000000,
    0x61138, 0x00000000,
    0x6113c, 0x00000000,
    0x61140, 0x00000000,
    0x61144, 0x00000000,
    0x61148, 0x00000000,
    0x611d8, 0x00000000,
    0x611dc, 0x00000000,
    0x611e0, 0x00000000,
    0x611e4, 0x00000000,
    0x611e8, 0x00000000,
    0x611ec, 0x00000000,
    0x611f0, 0x00000000,
    0x611f4, 0x00000000,
    0x61174, 0x00000000,
    0x6119c, 0x00000000,
    0x11298, 0x00000090,
    0x11218, 0x00000030,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11128, 0x00000001,
    0x1114c, 0x00000001,
    0x11170, 0x00000001,
    0x11178, 0x00000001,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x1121c, 0x00000011,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x11288, 0x00000000,
    0x1129c, 0x00000011,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x6394c, 0x00000000,
    0x63950, 0x00000000,
    0x63954, 0x00000000,
    0x63958, 0x00000800,
    0x6395c, 0x00000000,
    0x63960, 0x00000000,
    0x63964, 0x00000000,
    0x63968, 0x00000000,
    0x6396c, 0x00000000,
    0x63970, 0x00000000,
    0x63974, 0x00000800,
    0x63978, 0x00000000,
    0x6397c, 0x00000000,
    0x63980, 0x00000000,
    0x639c8, 0x00000004,
    0x639cc, 0x00080008,
    0x639d0, 0x00010004,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x618cc, 0x00000000,
    0x618d0, 0x00000000,
    0x618d4, 0x00000000,
    0x618d8, 0x00000000,
    0x618dc, 0x00000000,
    0x618e0, 0x00000000,
    0x618e4, 0x00000000,
    0x618e8, 0x00000000,
    0x618ec, 0x00000000,
    0x618f0, 0x00000000,
    0x618f4, 0x00000000,
    0x618f8, 0x00000000,
    0x618fc, 0x00000000,
    0x61900, 0x00000000,
    0x61904, 0x00000000,
    0x61908, 0x00000000,
    0x11824, 0x00000001,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x20000000,
    0x1402c, 0xffffffff,
    0x14030, 0x20000000,
    0x14028, 0x00000000,
    0x14034, 0x00000000,
    0x10868, 0x00000007,
    0x60868, 0x00000007
},
{0x10868, 0x00000005,
    0x60868, 0x00000005,
    0x10824, 0x00000011,
    0x10010, 0x0000001e,
    0x12368, 0x00000008,
    0x1236c, 0x03026040,
    0x12498, 0x01ee4fc4,
    0x12570, 0x0001ffff,
    0x12590, 0x00000000,
    0x12594, 0x00000000,
    0x1259c, 0x00000004,
    0x125a0, 0x00000021,
    0x125a4, 0x000021c0,
    0x125a8, 0x00005020,
    0x125ac, 0x00004000,
    0x125b0, 0x00000000,
    0x12814, 0x000000d1,
    0x12848, 0x007d30f6,
    0x1290c, 0x00133110,
    0x1293c, 0x0000012c,
    0x12954, 0x09844026,
    0x125b4, 0x20026400,
    0x12048, 0x03000309,
    0x1204c, 0x007f0309,
    0x120c8, 0x01386658,
    0x120f4, 0x00000488,
    0x13018, 0x00000000,
    0x1321c, 0x00000000,
    0x1324c, 0x00000000,
    0x14b00, 0x00000000,
    0x14b04, 0x00000000,
    0x16034, 0x3da01004,
    0x16038, 0x0003ac26,
    0x16044, 0x000007f4,
    0x16170, 0x3da01004,
    0x16174, 0x0003ac26,
    0x16180, 0x0007f430,
    0x16010, 0x821a1000,
    0x16140, 0x001a00e0,
    0x110a4, 0x00000000,
    0x110a8, 0x00000000,
    0x11124, 0x00000000,
    0x11128, 0x00000001,
    0x1114c, 0x00000001,
    0x11170, 0x00000001,
    0x11178, 0x00000001,
    0x11190, 0x00000001,
    0x11198, 0x00000001,
    0x111a0, 0x00000001,
    0x111a8, 0x00000001,
    0x111b0, 0x00000001,
    0x11218, 0x00000011,
    0x1121c, 0x00000011,
    0x11220, 0x00000030,
    0x11224, 0x00000030,
    0x11228, 0x00000003,
    0x11230, 0x00000000,
    0x11288, 0x00000000,
    0x11298, 0x00000011,
    0x1129c, 0x00000011,
    0x112a0, 0x00000090,
    0x112a4, 0x00000090,
    0x112a8, 0x00001111,
    0x112ac, 0x01000100,
    0x112b0, 0x00001000,
    0x112b4, 0x00000006,
    0x112b8, 0x00000000,
    0x112bc, 0x00000001,
    0x112c0, 0x00000000,
    0x13820, 0x0000000a,
    0x13824, 0x0000000a,
    0x13828, 0x0000000a,
    0x1382c, 0x0000000a,
    0x13830, 0x0000000a,
    0x13834, 0x0000000a,
    0x13838, 0x00000004,
    0x1383c, 0x00000004,
    0x1386c, 0x00001fff,
    0x13870, 0x00000000,
    0x13874, 0x00000000,
    0x13878, 0x00000004,
    0x1387c, 0x00000004,
    0x138a0, 0x00000005,
    0x138a4, 0x00000000,
    0x138b4, 0x00000001,
    0x138bc, 0x00001110,
    0x138f4, 0x00000005,
    0x138f8, 0x00000000,
    0x13908, 0x00000001,
    0x13910, 0x00001110,
    0x13940, 0x0000007f,
    0x13948, 0x00000000,
    0x13984, 0x00000000,
    0x16860, 0x0000000a,
    0x16864, 0x0000000a,
    0x16868, 0x0000000a,
    0x1686c, 0x0000000a,
    0x16870, 0x0000000a,
    0x16874, 0x0000000a,
    0x168a4, 0x00000000,
    0x168a8, 0x00000000,
    0x168b4, 0x00000007,
    0x168b8, 0x00000007,
    0x11824, 0x00000001,
    0x11828, 0x00000000,
    0x11840, 0x00000000,
    0x11844, 0x00000000,
    0x11848, 0x00000000,
    0x1184c, 0x00000000,
    0x11878, 0x00000000,
    0x11928, 0x00000000,
    0x1192c, 0x00000000,
    0x11930, 0x00000000,
    0x11934, 0x00000000,
    0x11960, 0x00000000,
    0x14020, 0xffffffff,
    0x14024, 0x00000028,
    0x14028, 0x00000000,
    0x1402c, 0xffffffff,
    0x14030, 0x0000400a,
    0x14034, 0x00000000,
    0x10868, 0x00000007,
    0x60868, 0x00000007}
    };
emlsr_tx_freq_ram_stru g_hmac_tx_freq_ram_fpga = {
{0x4c114, 0x40006400},
    {0x4c114, 0x40006400},
    {0x4c114, 0x40006400},
    {0x4c114, 0x40006400},
    };
emlsr_rx_freq_ram_stru g_hmac_rx_freq_ram_fpga = {
{   0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40900, 0x20006400,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000003,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x1cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000001,
    0x4281c, 0x54324130,
    0x42860, 0x01101001,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000014,
    0x42884, 0x086454a1,
    0x428b4, 0x00000300,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010448,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff3,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01840320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400080,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c0,
    0x42b08, 0x000000c0,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a
},
{0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40900, 0x20006400,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000003,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x1cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000001,
    0x4281c, 0x54324130,
    0x42860, 0x01101001,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000014,
    0x42884, 0x086454a1,
    0x428b4, 0x00000300,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010448,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff3,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01840320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400080,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c0,
    0x42b08, 0x000000c0,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a
},
{0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40900, 0x20006400,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000003,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x1cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000001,
    0x4281c, 0x54324130,
    0x42860, 0x01101001,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000014,
    0x42884, 0x086454a1,
    0x428b4, 0x00000300,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010448,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff3,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01840320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400080,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c0,
    0x42b08, 0x000000c0,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a
},
{0x40980, 0x00000000,
    0x40878, 0x0009e283,
    0x4087c, 0x010710c0,
    0x408a0, 0x00000000,
    0x40900, 0x20006400,
    0x40904, 0x079f1414,
    0x40924, 0x00000000,
    0x4092c, 0x00000000,
    0x40930, 0x0a30f815,
    0x4093c, 0x00258019,
    0x40944, 0x00000000,
    0x40948, 0x7f7f7f7f,
    0x4094c, 0x00000000,
    0x40950, 0x08888888,
    0x40954, 0x000a287f,
    0x40964, 0x00000000,
    0x40984, 0x00000003,
    0x40994, 0x00030005,
    0x40998, 0x0bb811f4,
    0x4099c, 0x07d02134,
    0x409a0, 0x0000000a,
    0x40a58, 0x00000001,
    0x42000, 0x1cf60528,
    0x42324, 0x00000000,
    0x42328, 0x00000000,
    0x42384, 0x00000000,
    0x42388, 0x00000001,
    0x4281c, 0x54324130,
    0x42860, 0x01101001,
    0x42864, 0x73bf7318,
    0x42874, 0x03323200,
    0x42878, 0x00000014,
    0x42884, 0x086454a1,
    0x428b4, 0x00000300,
    0x428c0, 0x0023001e,
    0x428c4, 0x001c520d,
    0x428e0, 0x00000728,
    0x428e4, 0x00015555,
    0x428e8, 0x07770007,
    0x428ec, 0x0e131414,
    0x428f8, 0x0105140a,
    0x428fc, 0x000003db,
    0x42918, 0x000000c3,
    0x42920, 0x00000641,
    0x42924, 0x10010448,
    0x42928, 0x00002160,
    0x4292c, 0x42000014,
    0x42930, 0x00ff0ff3,
    0x42994, 0x101010e1,
    0x429d0, 0x00224db4,
    0x42a08, 0x01840320,
    0x42a30, 0x11111151,
    0x42a40, 0x00000000,
    0x42a44, 0x00000f00,
    0x42a5c, 0x0c400080,
    0x42a68, 0x00000001,
    0x42ab8, 0x000000c0,
    0x42b08, 0x000000c0,
    0x42b28, 0x00000001,
    0x42d94, 0x000a0007,
    0x42dac, 0x01322814,
    0x42db0, 0x01009d4b,
    0x42db8, 0x26440f9f,
    0x42dd0, 0x00010442,
    0x42dd4, 0x023ffe0c,
    0x42de0, 0x001c275a}
    };

emlsr_share_ram_stru *g_hmac_emlsr_share_ram = NULL;
emlsr_trx_time_ram_stru *g_hmac_emlsr_trx_time_ram = NULL;
emlsr_tx_freq_ram_stru *g_hmac_emlsr_tx_freq_ram = NULL;
emlsr_rx_freq_ram_stru *g_hmac_emlsr_rx_freq_ram = NULL;
#define EMLSR_ENTER_RSSI_THRES      (-60)
uint32_t g_emlsr_disable_bitmap = 0x100000; /* bitmap有置位不进入emlsr，wur_notify_event_enum */
uint32_t g_emlsr_disable_mvap_id_bitmap = 0;
/*
 * 函 数 名  : hmac_post_emlsr_ram_data_event
 * 功能描述  : 校准数据下发抛事件
 */
static uint32_t hmac_post_emlsr_ram_data_event(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf,
    uint8_t *emlsr_ram_data, int32_t *remain_len, h2d_emlsr_ram_trans_stru *h2d_emlsr_trans)
{
    frw_event_mem_stru *evt_mem = NULL;
    frw_event_stru *evt = NULL;
    dmac_tx_event_stru *emlsr_ram_data_evt = NULL;
    uint32_t ret;
    uint16_t trans_len;

    evt_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));
    if (evt_mem == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_EMLSR, "{hmac_post_emlsr_ram_data_event::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    evt = frw_get_event_stru(evt_mem);
    frw_event_hdr_init(&(evt->st_event_hdr),
        FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_EMLSR_HMAC2DMAC,
        sizeof(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);

    memset_s(oal_netbuf_cb(netbuf), OAL_TX_CB_LEN, 0, OAL_TX_CB_LEN);

    /* netbuf len 不能超过1544字节 */
    trans_len = (uint16_t)oal_min(WLAN_LARGE_NETBUF_SIZE - HMAC_NETBUF_REMAIN_MAX_DIFF_LEN,
        *remain_len + sizeof(h2d_emlsr_ram_trans_stru));
    h2d_emlsr_trans->curr_send_data_len = (int32_t)trans_len - (int32_t)sizeof(h2d_emlsr_ram_trans_stru); // 实际数据的长度

    ret = (uint32_t)memcpy_s(oal_netbuf_data(netbuf), WLAN_LARGE_NETBUF_SIZE,
        h2d_emlsr_trans, sizeof(h2d_emlsr_ram_trans_stru));
    ret += (uint32_t)memcpy_s(oal_netbuf_data(netbuf) + sizeof(h2d_emlsr_ram_trans_stru),
        WLAN_LARGE_NETBUF_SIZE - sizeof(h2d_emlsr_ram_trans_stru),
        emlsr_ram_data - *remain_len, (uint32_t)h2d_emlsr_trans->curr_send_data_len);
    if (ret != EOK) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_EMLSR, "hmac_post_emlsr_ram_data_event::memcpy fail!");
        frw_event_free_m(evt_mem);
        return OAL_FAIL;
    }

    emlsr_ram_data_evt = (dmac_tx_event_stru *)evt->auc_event_data;
    emlsr_ram_data_evt->pst_netbuf = netbuf;
    emlsr_ram_data_evt->us_frame_len = trans_len;
    emlsr_ram_data_evt->us_remain = (uint16_t)*remain_len;
    *remain_len -= h2d_emlsr_trans->curr_send_data_len;

    if (*remain_len <= 0) {
        h2d_emlsr_trans->finish = OAL_TRUE;
    }
    ret = frw_event_dispatch_event(evt_mem);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_error_log3(mac_vap->uc_vap_id, OAM_SF_EMLSR,
            "{hmac_post_emlsr_ram_data_event::dispatch event fail ret[%d], remain_len[%d] trans_len[%d].}",
            ret, *remain_len, trans_len);
    }

    frw_event_free_m(evt_mem);
    return ret;
}

/*
 * 函 数 名  : hmac_get_emlsr_data_send_addr
 * 功能描述  : 获取下发数据类型
 */
uint8_t *hmac_get_emlsr_data_send_addr(h2d_emlsr_ram_trans_stru *h2d_emlsr_trans)
{
    uint8_t *data_addr = NULL;

    switch (h2d_emlsr_trans->emlsr_ram_type) {
        case H2D_EMLSR_SHARE_RAM_TYPE:
            data_addr = (uint8_t *)(g_hmac_emlsr_share_ram);
            break;
        case H2D_EMLSR_TRX_TIME_RAM_TYPE:
            data_addr = (uint8_t *)(g_hmac_emlsr_trx_time_ram);
            break;
        case H2D_EMLSR_TX_FREQ_RAM_TYPE:
            data_addr = (uint8_t *)(g_hmac_emlsr_tx_freq_ram);
            break;
        case H2D_EMLSR_RX_FREQ_RAM_TYPE:
            data_addr = (uint8_t *)(g_hmac_emlsr_rx_freq_ram);
            break;
        default:
            oam_error_log1(0, OAM_SF_EMLSR,
                "hmac_get_emlsr_data_send_addr fail::emlsr_data_type[%d].}", h2d_emlsr_trans->emlsr_ram_type);
            break;
    }
    return data_addr;
}
/*
 * 函 数 名  : hmac_get_emlsr_data_send_len
 * 功能描述  获取数据长度
 */
int32_t hmac_get_emlsr_data_send_len(h2d_emlsr_ram_trans_stru *h2d_emlsr_trans)
{
    uint32_t data_len = 0;

    switch (h2d_emlsr_trans->emlsr_ram_type) {
        case H2D_EMLSR_SHARE_RAM_TYPE:
            data_len =  sizeof(emlsr_share_ram_stru);
            break;
        case H2D_EMLSR_TRX_TIME_RAM_TYPE:
            data_len =  sizeof(emlsr_trx_time_ram_stru);
            break;
        case H2D_EMLSR_TX_FREQ_RAM_TYPE:
            data_len =  sizeof(emlsr_tx_freq_ram_stru);
            break;
        case H2D_EMLSR_RX_FREQ_RAM_TYPE:
            data_len =  sizeof(emlsr_rx_freq_ram_stru);
            break;

        default:
             oam_error_log1(0, OAM_SF_EMLSR,
                 "hmac_get_emlsr_data_send_len fail::emlsr_data_type[%d].}", h2d_emlsr_trans->emlsr_ram_type);
            break;
    }
    return (int32_t)data_len;
}
uint32_t hmac_send_emlsr_ram_data(mac_vap_stru *mac_vap, h2d_emlsr_ram_trans_stru *h2d_emlsr_trans)
{
    oal_netbuf_stru            *netbuf = NULL;
    uint8_t                    *param = NULL;
    uint32_t                   ret;
    int32_t                    remain_len;

    param = hmac_get_emlsr_data_send_addr(h2d_emlsr_trans);
    remain_len = hmac_get_emlsr_data_send_len(h2d_emlsr_trans);
    if (param == NULL || remain_len == 0 || mac_vap == NULL) {
        return OAL_FAIL;
    }

    h2d_emlsr_trans->data_total_len = remain_len;
    h2d_emlsr_trans->netbuf_idx = 0;
    param += remain_len;

    netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_LARGE_NETBUF_SIZE, OAL_NETBUF_PRIORITY_MID);
    if (netbuf == NULL) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_EMLSR,
            "{hmac_send_emlsr_ram_data::pst_netbuf alloc null SIZE[%d].}", WLAN_LARGE_NETBUF_SIZE);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    while (remain_len > 0) {
        ret = hmac_post_emlsr_ram_data_event(mac_vap, netbuf, param, &remain_len, h2d_emlsr_trans);
        if (ret != OAL_SUCC) {
            oal_netbuf_free(netbuf);
            return ret;
        }
        h2d_emlsr_trans->netbuf_idx++;
    }

    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_EMLSR,
        "{hmac_send_emlsr_ram_data:: data type %d len %d succ!}", h2d_emlsr_trans->emlsr_ram_type,
        h2d_emlsr_trans->data_total_len);
    oal_netbuf_free(netbuf);
    return OAL_SUCC;
}

void hmac_emlsr_ram_init(mac_vap_stru *mac_vap)
{
    h2d_emlsr_ram_trans_stru emlsr_share_ram_trans = {0};
    h2d_emlsr_ram_trans_stru emlsr_trx_time_trans = {0};
    h2d_emlsr_ram_trans_stru emlsr_tx_time_trans = {0};
    h2d_emlsr_ram_trans_stru emlsr_rx_time_trans = {0};

    emlsr_share_ram_trans.emlsr_ram_type = H2D_EMLSR_SHARE_RAM_TYPE;
    emlsr_trx_time_trans.emlsr_ram_type = H2D_EMLSR_TRX_TIME_RAM_TYPE;
    emlsr_tx_time_trans.emlsr_ram_type = H2D_EMLSR_TX_FREQ_RAM_TYPE;
    emlsr_rx_time_trans.emlsr_ram_type = H2D_EMLSR_RX_FREQ_RAM_TYPE;

    if (mpxx_is_asic()) {
        g_hmac_emlsr_share_ram = &g_hmac_share_ram;
        g_hmac_emlsr_trx_time_ram = &g_hmac_trx_time_ram;
        g_hmac_emlsr_tx_freq_ram = &g_hmac_tx_freq_ram;
        g_hmac_emlsr_rx_freq_ram = &g_hmac_rx_freq_ram;
    } else {
        g_hmac_emlsr_share_ram = &g_hmac_share_ram_fpga;
        g_hmac_emlsr_trx_time_ram = &g_hmac_trx_time_ram_fpga;
        g_hmac_emlsr_tx_freq_ram = &g_hmac_tx_freq_ram_fpga;
        g_hmac_emlsr_rx_freq_ram = &g_hmac_rx_freq_ram_fpga;
    }

    // share ram
    hmac_send_emlsr_ram_data(mac_vap, &emlsr_share_ram_trans);
    // trx_ram
    hmac_send_emlsr_ram_data(mac_vap, &emlsr_trx_time_trans);
    // tx_time_ram
    hmac_send_emlsr_ram_data(mac_vap, &emlsr_tx_time_trans);
    // rx_time_ram
    hmac_send_emlsr_ram_data(mac_vap, &emlsr_rx_time_trans);
}

uint8_t hmac_check_only_mlsr_sta(mac_vap_stru *link_down_vap, mac_vap_stru *link_up_vap)
{
    mac_device_stru *mac_device = mac_res_get_mac_dev();
    mac_vap_stru *up_vap_array[WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE] = {NULL};
    uint32_t up_vap_num;

    up_vap_num = mac_device_find_up_vaps(mac_device, up_vap_array, WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE);
    /* 将link_down_vap 从up_vap 数组中删除 */
    up_vap_num = hmac_sub_vap_from_up_vap_array(link_down_vap, up_vap_array, up_vap_num);
    /* 将link_up_vap 从up_vap 数组中删除 */
    up_vap_num = hmac_sub_vap_from_up_vap_array(link_up_vap, up_vap_array, up_vap_num);

    return (up_vap_num == 0 ? OAL_TRUE : OAL_FALSE);
}

uint8_t hmac_check_emlsr_enter_is_allowed(void)
{
    hmac_vap_stru *hmac_link_vap_work = NULL;
    hmac_vap_stru *hmac_link_vap_down = NULL;
    uint8_t mld_assoc_mode;
    uint32_t ret;

    if (g_wlan_spec_cfg->feature_ml_is_supported != OAL_TRUE) {
        return OAL_FALSE;
    }

    ret = hmac_mld_get_mlsr_link_sta(&hmac_link_vap_work, &hmac_link_vap_down);
    if (ret != OAL_SUCC) {
        return OAL_FALSE;
    }

    mld_assoc_mode = hmac_get_mld_assoc_mode(hmac_link_vap_work);
    if (mld_assoc_mode != MLD_ASSOC_MODE_MLSR) {
        oam_warning_log1(hmac_link_vap_work->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_check_emlsr_enter_is_allowed::mld_assoc_mode[%d] not open emlsr}", mld_assoc_mode);
        return OAL_FALSE;
    }

    if (hmac_link_vap_work->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP ||
        hmac_link_vap_down->st_vap_base_info.en_vap_state != MAC_VAP_STATE_STA_LINK_DOWN) {
        oam_warning_log4(0, OAM_SF_EMLSR,
            "{hmac_check_emlsr_enter_is_allowed::vap[%d] state[%d]  vap[%d] state[%d] is invalid!}",
            hmac_link_vap_work->st_vap_base_info.uc_vap_id, hmac_link_vap_work->st_vap_base_info.en_vap_state,
            hmac_link_vap_down->st_vap_base_info.uc_vap_id, hmac_link_vap_down->st_vap_base_info.en_vap_state);
        return OAL_FALSE;
    }

    if (mac_mib_get_EHTEMLSROptionActivated(&(hmac_link_vap_work->st_vap_base_info)) == OAL_FALSE ||
        mac_mib_get_EHTEMLSROptionActivated(&(hmac_link_vap_down->st_vap_base_info)) == OAL_FALSE) {
        oam_warning_log0(hmac_link_vap_work->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_check_emlsr_enter_is_allowed::emlsr mib is false!}");
        return OAL_FALSE;
    }

    /* 检查是否只存在mlsr的2个vap，则可以进入emlsr */
    return hmac_check_only_mlsr_sta(&(hmac_link_vap_work->st_vap_base_info), &(hmac_link_vap_down->st_vap_base_info));
}

uint32_t hmac_emlsr_default_enter_process(void)
{
    hmac_vap_stru *hmac_link_vap_work = NULL;
    hmac_vap_stru *hmac_link_vap_down = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint32_t ret;

    if (hmac_mld_get_mlsr_link_sta(&hmac_link_vap_work, &hmac_link_vap_down) != OAL_SUCC) {
        return OAL_FAIL;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_link_vap_work->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(hmac_link_vap_work->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_default_enter_process::hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_vap_manage = hmac_link_vap_work->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_link_vap_work->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_default_enter_process::mld_vap_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_vap_manage->emlsr_record_info.emlsr_param.emlsr_mode = OAL_TRUE;

    ret = hmac_emlsr_enter_check_device_state_allowed(hmac_link_vap_work, hmac_user);
    if (ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

void hmac_wlan_vap_emlsr_state_sync_event(hmac_mld_manage_stru *mld_vap_manage)
{
    uint8_t loop;
    hmac_vap_stru *hmac_vap;

    if (mld_vap_manage == NULL) {
        return;
    }

    for (loop = 0; loop < mld_vap_manage->mld_link_cnt; loop++) {
        hmac_vap = mld_vap_manage->link_vap[loop];
        if (hmac_vap == NULL) {
            continue;
        }
        dpe_wlan_vap_emlsr_state_sync(hmac_vap->st_vap_base_info.uc_vap_id,
            mld_vap_manage->emlsr_record_info.emlsr_state);
    }
}

OAL_STATIC uint8_t hmac_emlsr_switch_event_valid_check(hmac_vap_stru *hmac_vap, hmac_mld_manage_stru *mld_vap_manage,
    emlsr_vap_rpt_stru *emlsr_rpt_info)
{
    /* omn发送成功后，更新padding delay、transtion delay（未同步device），不执行emlsr open流程 */
    mld_vap_manage->emlsr_cap.emlsr_padding_delay = mld_vap_manage->emlsr_record_info.emlsr_param.emlsr_padding_delay;
    mld_vap_manage->emlsr_cap.emlsr_transition_delay =
        mld_vap_manage->emlsr_record_info.emlsr_param.emlsr_transition_delay;

    if (mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_PARAM_UPDATE) {
        mld_vap_manage->emlsr_record_info.emlsr_state = EMLSR_OPENED;
        return OAL_FALSE;
    }

    /* host侧emlsr状态为wait aopen等待omn帧发送结果，device上报host不允许进入emlsr，则将emlsr状态切回closed */
    if (emlsr_rpt_info->emlsr_mode == OAL_FALSE &&
        (mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_WAIT_OPEN ||
        mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_OPENED)) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_event_process::device not allow emlsr_open}");
        hmac_emlsr_force_close_process(hmac_vap, EMLSR_CLOSE_FOR_DEVICE_REJECT);
        return OAL_FALSE;
    }

    /* 非wait open、wait close状态，不处理device抛上来的 emlsr进入/退出事件 */
    if (mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_WAIT_OPEN &&
        mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_WAIT_CLOSE) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_event_process::cur emlsr_state[%d] can't open/close}",
            mld_vap_manage->emlsr_record_info.emlsr_state);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

static void hmac_emlsr_enter_reset_roam_invalid_scan_cnt(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *hmac_another_link = NULL;
    hmac_roam_info_stru *roam_info = NULL;

    if (hmac_check_work_in_emlsr(hmac_vap) != OAL_TRUE) {
        return;
    }

    roam_info = (hmac_roam_info_stru *)hmac_vap->pul_roam_info;
    if (roam_info == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_emlsr_enter_reset_roam_invalid_scan_cnt::roam info null!}");
        return;
    }
    roam_info->st_alg.uc_invalid_scan_cnt = 0;

    hmac_another_link = hmac_mld_get_another_vap(hmac_vap);
    if (hmac_another_link && hmac_another_link->pul_roam_info) {
        roam_info = (hmac_roam_info_stru *)hmac_another_link->pul_roam_info;
        roam_info->st_alg.uc_invalid_scan_cnt = 0;
    }
}

static void hmac_emlsr_destroy_emlsr_open_timer(hmac_mld_manage_stru *mld_vap_manage)
{
    if (mld_vap_manage != NULL &&
        mld_vap_manage->emlsr_record_info.emlsr_open_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&mld_vap_manage->emlsr_record_info.emlsr_open_timer);
    }
}

uint32_t hmac_emlsr_enter_event_process(frw_event_mem_stru *event_mem)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    frw_event_stru *event = NULL; /* 事件结构体 */
    uint8_t emlsr_enter_allowed;
    uint32_t ret;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_enter_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    emlsr_enter_allowed = *(uint8_t *)event->auc_event_data;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_enter_event_process::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_enter_event_process::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* device状态不支持进入emlsr，不发送omn帧 */
    if (emlsr_enter_allowed == OAL_FALSE) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_emlsr_enter_event_process::device not allow emlsr_enter}");
        return OAL_SUCC;
    }

    ret = hmac_emlsr_enter_tx_omn_action(hmac_vap, hmac_user);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_emlsr_enter_event_process::enter emlsr fail!}");
    }

    return ret;
}

uint32_t hmac_emlsr_switch_event(frw_event_mem_stru *event_mem)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    frw_event_stru *event = NULL; /* 事件结构体 */
    emlsr_vap_rpt_stru *emlsr_rpt_info = NULL;
    uint32_t next_mld_mode;
    uint32_t ret = OAL_SUCC;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_switch_event::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    emlsr_rpt_info = (emlsr_vap_rpt_stru *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(emlsr_rpt_info->curr_vap_id);
    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_switch_event::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    hmac_emlsr_destroy_emlsr_open_timer(mld_vap_manage);

    /* 有omn重传定时器则为emlsr退出发送的omn帧成功事件，不需要执行emlsr退出流程 */
    if (mld_vap_manage->emlsr_record_info.omn_retry_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&mld_vap_manage->emlsr_record_info.omn_retry_timer);
        mld_vap_manage->emlsr_record_info.omn_retry_cnt = 0;
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_event_process::destroy omn_retry_timer,close emlsr emlsr_state[%d])}",
            mld_vap_manage->emlsr_record_info.emlsr_state);
        return OAL_SUCC;
    }

    if (hmac_emlsr_switch_event_valid_check(hmac_vap, mld_vap_manage, emlsr_rpt_info) == OAL_FALSE) {
        /* 进emlsr fail，通知其他模块(wur) */
        hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
        hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_EMLSR_EVENT, COFEATURE_ENTER_FAIL);
        hmac_wlan_vap_emlsr_state_sync_event(mld_vap_manage);
        return OAL_SUCC;
    }

    /* 更新host侧emlsr工作状态 */
    mld_vap_manage->emlsr_record_info.emlsr_state = EMLSR_OPENED;

    hmac_emlsr_enter_reset_roam_invalid_scan_cnt(hmac_vap);

    /* MLD 模式切换:
     * 从MLSR -> EMLSR，vap_state 配置为UP;
     * 从EMLSR -> MLSR，vap_state 配置为LINK_DOWN;
     */
    next_mld_mode = (mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_OPENED) ?
                    MLD_MODE_SWITCH_EMLSR : MLD_MODE_SWITCH_MLSR;
    oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
        "{hmac_emlsr_event_process::emlsr event is[%d](1:to mlsr 2:to emlsr)}", next_mld_mode);

    ret = hmac_config_mld_mode_switch(hmac_vap, next_mld_mode, 0xff);
    if (ret != OAL_SUCC) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR, "{hmac_emlsr_event_process::ret[%d]}", ret);
    }
    hmac_wlan_vap_emlsr_state_sync_event(mld_vap_manage);

    hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_MLO, MLO_WORK_STATE_EMLSR, 0);

    return ret;
}

/* 函数功能：根据emlsr参数，填写omn帧 帧体内容 */
OAL_STATIC uint16_t hmac_encap_omn_action_config_param(mac_emlsr_omn_stru *emlsr, hmac_vap_stru *hmac_vap,
    mac_cfg_emlsr_action_param_stru *action_param)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint16_t link_index;
    uint16_t len = 0;

    emlsr->emlsr_mode = action_param->emlsr_mode;
    emlsr->emlmr_mode = 0; /* fixed */
    emlsr->emlsr_param_update_control = 0;
    emlsr->indevice_coex_activities = 0; /* 根据蓝牙情况填写, 可填默认值0 */
    emlsr->reserved = 0;
    len++;

    /* 如果有emlsr参数需要更新，则需携带EMLSR Parameter Update字段 */
    if ((emlsr->emlsr_mode == 1) && (action_param->emlsr_param_update_control == 1)) {
        emlsr->emlsr_param_update_control = 1;
        emlsr->emlsr_padding_delay = action_param->emlsr_padding_delay;
        emlsr->emlsr_transition_delay = action_param->emlsr_transition_delay;
        emlsr->reserved1 = 0;
        len++;
    }

    /* emlsr进入，omn帧需要携带link bitmap字段，退出时不携带 */
    if (emlsr->emlsr_mode == 0) {
        return len;
    }

    /* set EMLSR Link Bitmap */
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{dmac_config_encap_emlsr_action::mld_vap_manage is null!}");
        return len;
    }

    for (link_index = 0; link_index < mld_vap_manage->mld_link_cnt; link_index++) {
        emlsr->emlsr_emlmr_link_bitmap |= (1 << link_index);
    }
    len += EMLSR_OMN_LINK_BITMAP_LEN;

    return len;
}

/* 函数功能：设置action的mac头信息，封装omn帧帧体 */
OAL_STATIC uint16_t hmac_encap_omn_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_emlsr_action_param_stru *action_param, oal_netbuf_stru *netbuf)
{
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    mac_ieee80211_frame_stru *mac_header = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);
    uint8_t *payload_addr = mac_get_80211_mgmt_payload(netbuf);
    mac_emlsr_omn_stru *emlsr = NULL;
    uint8_t *mac_addr = NULL;
    uint16_t index = 0;

    mac_addr = mac_mib_get_StationID(mac_vap);
    if (mac_addr == NULL || mac_header == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_encap_omn_action:mac_addr or mac_header is NULL!}");
        return 0;
    }
    /*************************************************************************/
    /* Management Frame Format */
    /* -------------------------------------------------------------------- */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS| */
    /* -------------------------------------------------------------------- */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  | */
    /* -------------------------------------------------------------------- */
    /*************************************************************************/
    /*************************************************************************/
    /* Set the fields in the frame header */
    /*************************************************************************/
    /* 设置 Frame Control field */
    /* 帧控制字段全为0，除了type和subtype */
    mac_hdr_set_frame_control((uint8_t *)mac_header,
        WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number((uint8_t *)mac_header, 0);

    /* 设置地址1，与STA连接的AP MAC地址 */
    oal_set_mac_addr((uint8_t *)mac_header + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);

    /* 设置地址2为自己的MAC地址 */
    oal_set_mac_addr((uint8_t *)mac_header + WLAN_HDR_ADDR2_OFFSET, mac_addr);

    /* 设置地址3，为VAP的MAC地址 */
    oal_set_mac_addr((uint8_t *)mac_header + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);
    /*************************************************************************/
    /* Set the fields in the frame body */
    /*************************************************************************/
    /*************************************************************************/
    /* ttlm request frame - Frame Body */
    /* --------------------------------------------------------------------- */
    /* |Category  |Protected EHT Action |Dialog Token |TTLM element | */
    /* --------------------------------------------------------------------- */
    /* |1         |1                    |1            |Variable    |       */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    /* 设置Category */
    payload_addr[index++] = MAC_ACTION_CATEGORY_PROTECTED_EHT;

    /* 设置Action */
    /* 参见mac_protected_eht_action_type_enum枚举类型定义 */
    payload_addr[index++] = MAC_PROTECTED_EHT_ACTION_EML_OPERATING_MODE_NOTIFICATION;

    /* 设置Dialog Token */
    payload_addr[index++] = 1;

    if (action_param == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_encap_omn_action:action_param is NULL!}");
        return 0;
    }
    mac_header->st_frame_control.bit_power_mgmt = action_param->emlsr_pm;

    /* 设置帧内容 */
    emlsr = (mac_emlsr_omn_stru *)&payload_addr[index];
    memset_s(emlsr, sizeof(mac_emlsr_omn_stru), 0, sizeof(mac_emlsr_omn_stru));

    index += hmac_encap_omn_action_config_param(emlsr, hmac_vap, action_param);
    return (uint16_t)(MAC_80211_FRAME_LEN + index);
}

/* 函数功能：组omn action帧的cb */
OAL_STATIC void hmac_tx_omn_cb_init(oal_netbuf_stru *buffer, hmac_user_stru *hmac_user,
    uint16_t frame_len, wlan_cb_frame_subtype_enum_uint8 cb_frame_subtype)
{
    mac_tx_ctl_stru *tx_ctl = NULL;

    memset_s(oal_netbuf_cb(buffer), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(buffer);
    if (tx_ctl == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_tx_omn_cb_init::tx_ctl is null}");
        return;
    }
    mac_get_cb_tx_user_idx(tx_ctl) = hmac_user->st_user_base_info.us_assoc_id;
    mac_get_cb_mpdu_len(tx_ctl) = frame_len;
    mac_get_cb_frame_type(tx_ctl) = WLAN_CB_FRAME_TYPE_MGMT;
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_AC_MGMT;
    mac_get_cb_frame_subtype(tx_ctl) = cb_frame_subtype;
    oal_netbuf_put(buffer, frame_len);
}

/* 函数功能：申请action帧内存，组omn帧后抛事件到device发送 */
uint32_t hmac_tx_omn_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_emlsr_action_param_stru *emlsr_action)
{
    mac_vap_stru *mac_vap = NULL;
    oal_netbuf_stru *mgmt_buf = NULL;
    uint16_t mgmt_len;
    uint32_t ret;

    mac_vap = &hmac_vap->st_vap_base_info;
    if (mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_tx_omn_action::mac_vap is null}");
        return OAL_FAIL;
    }

    /* 申请管理帧内存 */
    mgmt_buf = oal_mem_netbuf_alloc(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (mgmt_buf == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_EMLSR, "{hmac_tx_omn_action::pst_mgmt_buf null.}");
        return OAL_FAIL;
    }

    mgmt_len = hmac_encap_omn_action(hmac_vap, hmac_user, emlsr_action, mgmt_buf);
    if (mgmt_len > WLAN_MGMT_NETBUF_SIZE) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_EMLSR, "{hmac_tx_omn_action:encap presence rsp len[%d] too large}",
                       mgmt_len);
    }
    if (mgmt_len == 0) {
        oal_netbuf_free(mgmt_buf);
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_tx_omn_action::mgmt len = 0}");
        return OAL_FAIL;
    }
    /* 填写netbuf的cb字段，供发送管理帧和发送完成接口使用 */
    hmac_tx_omn_cb_init(mgmt_buf, hmac_user, mgmt_len, WLAN_ACTION_EMLSR_OMN);

    /* 调用发送管理帧接口 */
    ret = hmac_tx_mgmt_send_event(mac_vap, mgmt_buf, mgmt_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(mgmt_buf);
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_EMLSR, "{dmac_send_emlsr_action::tx link meas rpt action failed.}");
        return ret;
    }

    return OAL_SUCC;
}

/* 函数功能：检查MLSR工作状态是否支持emlsr */
OAL_STATIC uint8_t hmac_check_mlsr_support_emlsr(hmac_vap_stru *hmac_vap, hmac_mld_manage_stru *mld_vap_manage,
    hmac_mld_user_manage_stru *mld_user_manage, mac_cfg_emlsr_action_param_stru *emlsr_param)
{
    hmac_vap_stru *another_vap = NULL;

    /* 当且仅当MLSR入网且双link，才支持emlsr */
    if (mld_vap_manage->mld_assoc_mode != MLD_ASSOC_MODE_MLSR ||
        mld_vap_manage->mld_link_cnt != WLAN_MAX_ML_LINK_NUM) {
        oam_warning_log2(0, OAM_SF_EMLSR,
            "{hmac_check_mlsr_support_emlsr::mld_assoc_mode[%d] link_cnt[%d] not support emlsr}",
            mld_vap_manage->mld_assoc_mode, mld_vap_manage->mld_link_cnt);
        return OAL_FALSE;
    }

    another_vap = hmac_mld_get_another_vap(hmac_vap);
    /* MLSR双link同频段时不支持EMLSR */
    if (another_vap == NULL ||
        hmac_vap->st_vap_base_info.st_channel.en_band == another_vap->st_vap_base_info.st_channel.en_band) {
        oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_check_mlsr_support_emlsr::equal band, not support emlsr}");
        return OAL_FALSE;
    }

    if (mac_mib_get_EHTEMLSROptionActivated(&hmac_vap->st_vap_base_info) == OAL_FALSE ||
        mac_mib_get_EHTEMLSROptionActivated(&another_vap->st_vap_base_info) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_check_mlsr_support_emlsr::emlsr mib is false}");
        return OAL_FALSE;
    }

    /* 如果对端MLD AP不支持emlsr，则不进入emlsr */
    if (mld_user_manage->eml_hdl.emlsr_capable != OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_check_mlsr_support_emlsr::user not support emlsr}");
        return OAL_FALSE;
    }

    /* emlsr进入时，发omn帧的vap state一定是UP，若为其他状态如roaming则不能进入emlsr */
    if (emlsr_param->emlsr_mode == OAL_TRUE && hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_check_mlsr_support_emlsr::vap_state[%d]}", hmac_vap->st_vap_base_info.en_vap_state);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/* 函数功能：检查是否存在多vap场景不支持emlsr进入 */
OAL_STATIC uint8_t hmac_emlsr_switch_multi_vap_valid(hmac_vap_stru *hmac_vap, hmac_mld_manage_stru *mld_vap_manage,
    uint8_t emlsr_mode)
{
    mac_device_stru *mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    mac_vap_stru *up_vap_array[WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE] = {NULL};
    uint32_t up_vap_num;
    uint8_t vap_index;

    if (emlsr_mode == OAL_FALSE) {
        return OAL_TRUE;
    }

    up_vap_num = mac_device_find_up_vaps(mac_device, up_vap_array, WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE);
    for (vap_index = 0; vap_index < up_vap_num; vap_index++) {
        if (up_vap_array[vap_index]->uc_vap_id != mld_vap_manage->link_vap[0]->st_vap_base_info.uc_vap_id &&
            up_vap_array[vap_index]->uc_vap_id != mld_vap_manage->link_vap[1]->st_vap_base_info.uc_vap_id) {
            oam_warning_log0(up_vap_array[vap_index]->uc_vap_id, OAM_SF_EMLSR,
                "{hmac_emlsr_switch_multi_vap_valid::vap_mode}");
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}

static uint8_t hmac_emlsr_check_omn_allowed(hmac_mld_manage_stru *mld_vap_manage,
    uint8_t emlsr_mode, uint8_t emlsr_param_update)
{
    uint8_t ret = OAL_FALSE;

    switch (mld_vap_manage->emlsr_record_info.emlsr_state) {
        case EMLSR_WAIT_OPEN:
        case EMLSR_WAIT_CLOSE:
        case EMLSR_PARAM_UPDATE:
        case EMLSR_CLOSED: /* emlsr强制退出时会有重传omn帧，需要在emlsr已退出状态下发送omn帧 */
            ret = OAL_TRUE;
            break;

        case EMLSR_OPENED:
            /* emlsr open状态下，只能发 退出emlsr 或 emlsr参数更新 的omn帧 */
            if ((emlsr_mode == OAL_FALSE) || (emlsr_mode == OAL_TRUE && emlsr_param_update == OAL_TRUE)) {
                ret = OAL_TRUE;
            }
            break;

        default:
            break;
    }

    return ret;
}

OAL_STATIC void hmac_emlsr_open_failed_handle(hmac_mld_manage_stru *mld_vap_manage)
{
    hmac_user_stru *hmac_user = NULL;

    /* emlsr进入 omn帧发送失败，状态退回closed */
    mld_vap_manage->emlsr_record_info.emlsr_state = EMLSR_CLOSED;

    hmac_wlan_vap_emlsr_state_sync_event(mld_vap_manage);

    if (mld_vap_manage->mld_vap == NULL) {
        return;
    }
    hmac_user = mac_res_get_hmac_user(mld_vap_manage->mld_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        return;
    }
    /* 通知其他特性emlsr进入失败 */
    hmac_vap_state_change_notify_feature(mld_vap_manage->mld_vap, hmac_user,
        COFEATURE_EMLSR_EVENT, COFEATURE_ENTER_FAIL);
}

uint32_t hmac_emlsr_open_timeout(void *arg)
{
    hmac_mld_manage_stru *mld_vap_manage = (hmac_mld_manage_stru *)arg;

    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_open_timeout::mld_vap_manage is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* emlsr open定时器只在emlsr进入 / 参数更新时启动，其他状态为异常 */
    if (mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_WAIT_OPEN &&
        mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_PARAM_UPDATE) {
        oam_error_log1(0, OAM_SF_EMLSR, "{hmac_emlsr_open_timeout::send omn timeout, invalid emlsr_state[%d]!}",
            mld_vap_manage->emlsr_record_info.emlsr_state);
    }

    oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_open_timeout::emlsr_open, send omn timeout}");

    hmac_emlsr_open_failed_handle(mld_vap_manage);

    return OAL_SUCC;
}

static uint8_t hmac_emlsr_tx_omn_check_is_valid(hmac_mld_manage_stru *mld_vap_manage, hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, mac_cfg_emlsr_action_param_stru *emlsr_param)
{
    hmac_mld_user_manage_stru *mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;

    if (mld_user_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_tx_omn_check_is_valid::mld_user_mgmt is null!}");
        return OAL_FALSE;
    }

    if (hmac_check_mlsr_support_emlsr(hmac_vap, mld_vap_manage, mld_user_manage, emlsr_param) == OAL_FALSE) {
        return OAL_FALSE;
    }

    if (hmac_mlsr_is_link_switching() == OAL_TRUE) {
        /* 有link切换正在执行，不处理本次漫游扫描结果，不执行后续漫游流程 */
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_emlsr_tx_omn_check_is_valid::is link switching!}");
        return OAL_FALSE;
    }

    /* 检查其他vap是否处于不允许emlsr进入/退出的状态 */
    if (hmac_check_all_vap_state_allow_mld_feature(&hmac_vap->st_vap_base_info) != OAL_SUCC) {
        return OAL_FALSE;
    }

    if (hmac_emlsr_switch_multi_vap_valid(hmac_vap, mld_vap_manage, emlsr_param->emlsr_mode) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_EMLSR,
            "{hmac_emlsr_tx_omn_check_is_valid::other vap is up, emlsr switch is invalid}");
        return OAL_FALSE;
    }

    /* 当前emlsr状态下，是否支持发送omn帧 */
    if (hmac_emlsr_check_omn_allowed(mld_vap_manage,
        emlsr_param->emlsr_mode, emlsr_param->emlsr_param_update_control) == OAL_FALSE) {
        oam_warning_log1(0, OAM_SF_EMLSR, "{hmac_emlsr_tx_omn_check_is_valid::emlsr state[%d], can't tx omn}",
            mld_vap_manage->emlsr_record_info.emlsr_state);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

OAL_STATIC void hmac_ml_emlsr_enable_check_affected_feature(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    /* 进emlsr，通知其他模块(wur) */
    hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_EMLSR_EVENT, COFEATURE_ENTER);

#ifdef _PRE_WLAN_FEATURE_11AX
    /* 进emlsr，退出omi */
    hmac_omi_force_close_process();
#endif
}

void hmac_emlsr_disable_bitmap_set(uint8_t event_type)
{
    g_emlsr_disable_bitmap |= BIT(event_type);
}

void hmac_emlsr_disable_bitmap_clear(uint8_t event_type)
{
    g_emlsr_disable_bitmap &= ~BIT(event_type);
}

uint8_t hmac_emlsr_check_bitmap_allowed_enter(void)
{
    return (g_emlsr_disable_bitmap == 0) ? OAL_TRUE : OAL_FALSE;
}

void hmac_emlsr_print_info(void)
{
    oam_warning_log2(0, OAM_SF_EMLSR,
        "{hmac_emlsr_print_info::emlsr_disable_bitmap[0x%x] emlsr_mvap_disable_bitmap[0x%x]}",
        g_emlsr_disable_bitmap, g_emlsr_disable_mvap_id_bitmap);
}

static uint32_t hmac_emlsr_enter_check_state_valid(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_mld_manage_stru *mld_vap_manage, mac_cfg_emlsr_action_param_stru *emlsr_param)
{
    if (hmac_emlsr_check_bitmap_allowed_enter() != OAL_TRUE) {
        hmac_emlsr_print_info();
        return OAL_FAIL;
    }

    /* 从上层指定VAP发送OMN帧 */
    if (is_legacy_sta(&hmac_vap->st_vap_base_info) == OAL_FALSE ||
        hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::not legact_sta or mld_vap!}");
        return OAL_FAIL;
    }

    if (hmac_emlsr_tx_omn_check_is_valid(mld_vap_manage, hmac_vap, hmac_user, emlsr_param) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::cur state not allow emlsr_enter}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t hmac_emlsr_enter_check_device_state_allowed(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    mac_cfg_emlsr_action_param_stru *emlsr_param = NULL;
    uint8_t ped = 0;
    uint32_t ret;

    if (hmac_vap == NULL || hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::hmac_vap or hmac_user is null!}");
        return OAL_FAIL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::mld_vap_mgmt is null!}");
        return OAL_FAIL;
    }

    emlsr_param = &mld_vap_manage->emlsr_record_info.emlsr_param;
    if (hmac_emlsr_enter_check_state_valid(hmac_vap, hmac_user, mld_vap_manage, emlsr_param) == OAL_FAIL) {
        return OAL_FAIL;
    }

    ret = hmac_config_send_event(&(hmac_vap->st_vap_base_info), WLAN_CFGID_EMLSR_ENTER_CHECK_DEVICE_STATE,
        sizeof(uint8_t), (uint8_t *)&ped);
    return ret;
}

/* 函数功能：判断是否可以进入/退出emlsr，并发送omn帧 */
uint32_t hmac_emlsr_enter_tx_omn_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    mac_cfg_emlsr_action_param_stru *emlsr_param = NULL;
    uint32_t ret;

    if (hmac_vap == NULL || hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::hmac_vap or hmac_user is null!}");
        return OAL_FAIL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::mld_vap_mgmt is null!}");
        return OAL_FAIL;
    }

    emlsr_param = &mld_vap_manage->emlsr_record_info.emlsr_param;
    if (hmac_emlsr_enter_check_state_valid(hmac_vap, hmac_user, mld_vap_manage, emlsr_param) == OAL_FAIL) {
        return OAL_FAIL;
    }

    /* 进emlsr前关闭受影响的特性 */
    hmac_ml_emlsr_enable_check_affected_feature(hmac_vap, hmac_user);

    /* 确定需要从mlsr进入emlsr，配置寄存器；emslr工作状态下，更新omn参数时不配置寄存器 */
    if (emlsr_param->emlsr_mode == OAL_TRUE && mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_CLOSED) {
        /* emlsr初始化 */
        hmac_emlsr_ram_init(&hmac_vap->st_vap_base_info);
    }

    /* host侧记录当前emlsr状态，和emlsr的padding delay、transtion delay参数 */
    mld_vap_manage->emlsr_record_info.emlsr_param.emlsr_padding_delay = emlsr_param->emlsr_padding_delay;
    mld_vap_manage->emlsr_record_info.emlsr_param.emlsr_transition_delay = emlsr_param->emlsr_transition_delay;
    mld_vap_manage->emlsr_record_info.emlsr_state = (emlsr_param->emlsr_param_update_control == OAL_TRUE &&
            mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_OPENED) ? EMLSR_PARAM_UPDATE : EMLSR_WAIT_OPEN;
    oam_warning_log1(0, OAM_SF_EMLSR, "{hmac_emlsr_enter_tx_omn_action::emlsr_state[%d]}",
        mld_vap_manage->emlsr_record_info.emlsr_state);
    hmac_wlan_vap_emlsr_state_sync_event(mld_vap_manage);
    ret = hmac_tx_omn_action(hmac_vap, hmac_user, emlsr_param);
    if (ret != OAL_SUCC) {
        hmac_emlsr_open_failed_handle(mld_vap_manage);
        return ret;
    }

    frw_timer_create_timer_m(&(mld_vap_manage->emlsr_record_info.emlsr_open_timer), hmac_emlsr_open_timeout,
        EMLSR_OPEN_TIMEOUT, (void *)mld_vap_manage,  OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);

    return ret;
}

/* 函数功能：接收device侧触发的emlsr进入/退出事件，发送omn帧 */
uint32_t hmac_emlsr_event_process(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL; /* 事件结构体 */
    emlsr_switch_event_stru *emlsr_event = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    mac_cfg_emlsr_action_param_stru *emlsr_param = NULL;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    emlsr_event = (emlsr_switch_event_stru *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(emlsr_event->vap_id);
    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id,
            OAM_SF_EMLSR, "{hmac_emlsr_event_process::dmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    emlsr_param = &mld_vap_manage->emlsr_record_info.emlsr_param;
    emlsr_param->emlsr_mode = (emlsr_event->emlsr_switch_event == EMLSR_OPEN_EVENT) ? OAL_TRUE : OAL_FALSE;
    emlsr_param->emlsr_pm = emlsr_event->emlsr_pm;
    /* 如果device不更新emlsr参数，默认填mld_vap_manage下保存的参数 */
    emlsr_param->emlsr_padding_delay = mld_vap_manage->emlsr_cap.emlsr_padding_delay;
    emlsr_param->emlsr_transition_delay = mld_vap_manage->emlsr_cap.emlsr_transition_delay;

    hmac_vap_state_change_notify_emlsr(hmac_vap, hmac_user, COFEATURE_BT_EVENT, emlsr_param->emlsr_mode);

    return OAL_SUCC;
}

static hmac_vap_stru *hmac_select_same_chan_left_vap(hmac_vap_stru *hmac_ml_vap, hmac_vap_stru *hmac_link_vap,
    uint8_t band, uint8_t chan_number)
{
    if (hmac_ml_vap->st_vap_base_info.st_channel.uc_chan_number == chan_number) {
        return hmac_ml_vap;
    }

    if (hmac_link_vap->st_vap_base_info.st_channel.uc_chan_number == chan_number) {
        return hmac_link_vap;
    }

    return NULL;
}

/* 函数功能：检查指定的emlsr退出后工作vap是否合法 */
static uint8_t hmac_emlsr_close_check_expect_vap_is_valid(const hmac_vap_stru *hmac_ml_vap,
    const hmac_vap_stru *hmac_link_vap, const hmac_vap_stru *expect_vap)
{
    if (expect_vap == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_close_check_expect_vap_is_valid::expect_vap is null!}");
        return OAL_FALSE;
    }

    /* 指定的工作vap必须为当前MLSR关联的2个vap之一 */
    if (expect_vap->st_vap_base_info.uc_vap_id != hmac_ml_vap->st_vap_base_info.uc_vap_id &&
        expect_vap->st_vap_base_info.uc_vap_id != hmac_link_vap->st_vap_base_info.uc_vap_id) {
        oam_error_log3(0, OAM_SF_EMLSR,
            "{hmac_emlsr_close_check_expect_vap_is_valid::expect_vap[%d] ml_vap[%d] link_vap[%d]}",
            expect_vap->st_vap_base_info.uc_vap_id,
            hmac_ml_vap->st_vap_base_info.uc_vap_id,
            hmac_link_vap->st_vap_base_info.uc_vap_id);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/* 函数功能：检查emlsr close原因，是否需要指定工作vap，除多vap入网退出emlsr外均需要指定退出后的工作vap */
static uint8_t hmac_emlsr_close_reason_need_indicate_work_vap(uint8_t reason)
{
    uint8_t res = OAL_FALSE;

    switch (reason) {
        case EMLSR_CLOSE_FOR_HIPRIV:
        case EMLSR_CLOSE_FOR_IOCTL:
        case EMLSR_CLOSE_FOR_ROAM:
        case EMLSR_CLOSE_FOR_LINKLOSS:
        case EMLSR_CLOSE_FOR_BT:
        case EMLSR_CLOSE_FOR_RECONFIG:
        case EMLSR_CLOSE_FOR_TTLM:
        case EMLSR_CLOSE_FOR_RECONNECT:
            res = OAL_TRUE;
            break;

        case EMLSR_CLOSE_FOR_MVAP:
            res = OAL_FALSE;
            break;

        default:
            oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_close_reason_need_indicate_work_vap::invalid reason}");
            break;
    }

    return res;
}

/* 函数功能：根据信道，决定退出EMLSR后，工作在ml_vap还是link_vap
 * 返回值：退出EMLSR状态后继续工作的VAP
 */
static hmac_vap_stru *hmac_emlsr_close_work_vap_decision(hmac_vap_stru *hmac_ml_vap, hmac_vap_stru *hmac_link_vap,
    const mac_channel_stru *new_channel_info)
{
    hmac_vap_stru *expect_work_vap = NULL;
    uint8_t chan_number;
    uint8_t band;

    /* 多vap入网退出emlsr，新入网的vap信息传入异常，默认退回主link */
    if (new_channel_info == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_close_work_vap_decision::new_channel_info is null!}");
        return hmac_ml_vap;
    }

    band = new_channel_info->en_band;
    chan_number = new_channel_info->uc_chan_number;
    oam_warning_log2(0, OAM_SF_EMLSR,
        "{hmac_emlsr_close_work_vap_decision::new_vap band[%d] chan_id[%d]}", band, chan_number);

    /* 如果新入网vap为2g频段，则优选emlsr退回5g vap组成DBDC，优于2g DCHN */
    if (band == WLAN_BAND_2G &&
        hmac_ml_vap->st_vap_base_info.st_channel.en_band != hmac_link_vap->st_vap_base_info.st_channel.en_band) {
        return (hmac_ml_vap->st_vap_base_info.st_channel.en_band == band) ? hmac_link_vap : hmac_ml_vap;
    }

    /* 检查emlsr是否有link vap与新入网vap同信道，有则保留该vap与新入网vap组成DCHN */
    expect_work_vap = hmac_select_same_chan_left_vap(hmac_ml_vap, hmac_link_vap, band, chan_number);
    if (expect_work_vap != NULL) {
        return expect_work_vap;
    }

    if (hmac_ml_vap->st_vap_base_info.st_channel.en_band != hmac_link_vap->st_vap_base_info.st_channel.en_band) {
        /* 如果emlsr的2个link不同频段，则保留与新入网vap不同频段的link vap，组成DBDC */
        expect_work_vap = (hmac_ml_vap->st_vap_base_info.st_channel.en_band == band) ? hmac_link_vap : hmac_ml_vap;
    } else {
        /* 如果emlsr的2个link相同频段，保留当前mld_mode为ML_LINK的vap */
        expect_work_vap = hmac_ml_vap;
    }

    return expect_work_vap;
}

/* 函数功能：封装退出emlsr时 omn帧发送流程，参数当前为默认 */
static uint32_t hmac_emlsr_force_close_send_omn(hmac_mld_manage_stru *mld_vap_manage,
    hmac_vap_stru *expect_work_vap, hmac_user_stru *hmac_user)
{
    mac_cfg_emlsr_action_param_stru *emlsr_param = &mld_vap_manage->emlsr_record_info.emlsr_param;
    uint32_t ret;

    emlsr_param = &mld_vap_manage->emlsr_record_info.emlsr_param;
    emlsr_param->emlsr_mode = OAL_FALSE;
    emlsr_param->emlsr_pm = OAL_TRUE;
    emlsr_param->emlsr_padding_delay = mld_vap_manage->emlsr_cap.emlsr_padding_delay;
    emlsr_param->emlsr_transition_delay = mld_vap_manage->emlsr_cap.emlsr_transition_delay;

    ret = hmac_tx_omn_action(expect_work_vap, hmac_user, emlsr_param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_error_log1(expect_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
                       "{hmac_emlsr_force_close_send_omn::hmac_tx_omn_action failed[%d].}", ret);
    }

    return ret;
}


/* 函数功能：emlsr退出时，omn帧重传处理 */
uint32_t hmac_omn_retry_timeout(void *arg)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)arg;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_user_stru *hmac_user = NULL;

    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_omn_retry_timeout::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR, "{hmac_omn_retry_timeout::hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_omn_retry_timeout::mld_vap_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 重传次数超过上限，销毁定时器，不再重传 */
    if (mld_vap_manage->emlsr_record_info.omn_retry_cnt > EMLSR_CLOSE_OMN_RETRY_LIMIT) {
        mld_vap_manage->emlsr_record_info.omn_retry_cnt = 0;
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id,
            OAM_SF_EMLSR, "{hmac_omn_retry_timeout::omn fail, retry_cnt is over!}");
        return OAL_SUCC;
    }

    /* 重启定时器，重传次数+1 */
    frw_timer_create_timer_m(&(mld_vap_manage->emlsr_record_info.omn_retry_timer),
        hmac_omn_retry_timeout, EMLSR_OMN_RETRY_TIME, (void *)hmac_vap, OAL_FALSE,
        OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    mld_vap_manage->emlsr_record_info.omn_retry_cnt++;
    oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
        "{hmac_config_ml_emlsr_enable::omn retry_cnt[%d].}", mld_vap_manage->emlsr_record_info.omn_retry_cnt);

    return hmac_emlsr_force_close_send_omn(mld_vap_manage, hmac_vap, hmac_user);
}

static void hmac_emlsr_exit_recover_roam_trigger_type(hmac_vap_stru *hmac_vap)
{
    uint8_t vap_id = hmac_vap->st_vap_base_info.uc_vap_id;
    hmac_vap_stru *hmac_another_link = NULL;
    hmac_roam_info_stru *roam_info = NULL;

    roam_info = (hmac_roam_info_stru *)hmac_vap->pul_roam_info;
    if (roam_info == NULL) {
        oam_error_log0(vap_id, OAM_SF_ROAM, "{hmac_emlsr_exit_recover_roam_trigger_type::roam info null!}");
        return;
    }

    if (roam_info->uc_rssi_type != ROAM_ENV_LINKLOSS ||
        hmac_check_work_in_emlsr(hmac_vap) != OAL_TRUE) {
        return;
    }

    /* 退出EMLSR，恢复漫游门限为默认值 */
    hmac_roam_rssi_trigger_type_single_link(hmac_vap, ROAM_ENV_SPARSE_AP_TH2);
    roam_info->st_alg.uc_invalid_scan_cnt = 0;

    hmac_another_link = hmac_mld_get_another_vap(hmac_vap);
    if (hmac_another_link != NULL) {
        /* 退出EMLSR，需要更新另外一个link漫游门限 */
        hmac_roam_rssi_trigger_type_single_link(hmac_another_link, ROAM_ENV_SPARSE_AP_TH2);
        roam_info = (hmac_roam_info_stru *)hmac_another_link->pul_roam_info;
        if (roam_info) {
            roam_info->st_alg.uc_invalid_scan_cnt = 0;
        }
    }
}

static uint32_t hmac_emlsr_force_close_check_params(hmac_vap_stru *expect_work_vap,
    hmac_user_stru **hmac_user, hmac_mld_manage_stru **mld_vap_manage)
{
    hmac_user_stru *hmac_user_tmp = NULL;
    hmac_user_tmp = (hmac_user_stru *)mac_res_get_hmac_user(expect_work_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user_tmp == NULL || hmac_user_tmp->mld_user_link_info.mld_user_manage == NULL) {
        oam_error_log1(expect_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_force_close_process::user %d or mld_user_manage is null.}",
            expect_work_vap->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    *hmac_user = hmac_user_tmp;

    *mld_vap_manage = expect_work_vap->mld_vap_link_info.mld_vap_manage;
    if (*mld_vap_manage == NULL) {
        oam_error_log0(expect_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_force_close_process::mld_vap_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return OAL_SUCC;
}

/* 函数功能：多vap入网、漫游等场景，强制退出emlsr */
uint32_t hmac_emlsr_force_close_process(hmac_vap_stru *expect_work_vap, uint8_t reason)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint32_t ret;
    hmac_vap_stru *hmac_ml_vap = NULL;
    hmac_vap_stru *hmac_link_vap = NULL;
    hmac_user_stru *hmac_ml_user = NULL;
    uint8_t emlsr_pre_state;

    if (hmac_emlsr_force_close_check_params(expect_work_vap, &hmac_user, &mld_vap_manage) != OAL_SUCC) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 非emlsr开启状态，不执行后续流程 */
    emlsr_pre_state = mld_vap_manage->emlsr_record_info.emlsr_state;
    if (mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_OPENED &&
        mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_WAIT_OPEN) {
        oam_warning_log1(expect_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR,
            "{hmac_emlsr_force_close_process::emlsr_state[%d]}", mld_vap_manage->emlsr_record_info.emlsr_state);
        return OAL_SUCC;
    }

    /* 退出EMLSR模式，恢复漫游门限 */
    hmac_emlsr_exit_recover_roam_trigger_type(expect_work_vap);

    if (hmac_mld_get_mlsr_link_sta(&hmac_ml_vap, &hmac_link_vap) != OAL_SUCC) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 退出emlsr的工作link不是当前的ml vap，通知wur模块link切换成功 */
    if (expect_work_vap != hmac_ml_vap) {
        hmac_ml_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_ml_vap->st_vap_base_info.us_assoc_vap_id);
        if (hmac_user == NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }
        hmac_vap_state_change_notify_feature(hmac_ml_vap, hmac_ml_user,
            COFEATURE_LINK_SWITCH_EVENT, COFEATURE_ENTER_SUCC);
    }

    frw_timer_create_timer_m(&(mld_vap_manage->emlsr_record_info.omn_retry_timer),
        hmac_omn_retry_timeout, EMLSR_OMN_RETRY_TIME, (void *)expect_work_vap, OAL_FALSE,
        OAM_MODULE_ID_HMAC, expect_work_vap->st_vap_base_info.core_id);

    /* 组omn帧发送 */
    hmac_emlsr_force_close_send_omn(mld_vap_manage, expect_work_vap, hmac_user);

    oam_warning_log1(expect_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_EMLSR, "{hmac_emlsr_force_close_process:: \
        close reason[%d] (1:hipriv 2:ioctl 3:mvap 4:roam 5:linkloss 6:bt 7:reconfig 8:ttlm)}", reason);

    /* 更新host侧emlsr工作状态 */
    mld_vap_manage->emlsr_record_info.emlsr_state = EMLSR_CLOSED;
    hmac_wlan_vap_emlsr_state_sync_event(mld_vap_manage);

    if (emlsr_pre_state == EMLSR_WAIT_OPEN) {
        return OAL_SUCC;
    }
    /* MLD 模式切换:从EMLSR -> MLSR，vap_state 配置为LINK_DOWN */
    ret = hmac_config_mld_mode_switch(expect_work_vap, MLD_MODE_SWITCH_MLSR, reason);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_EMLSR, "{hmac_emlsr_event_process::ret[%d]}", ret);
    }

    hmac_cofeture_state_change_notify_sta(expect_work_vap, COFEATURE_TYPE_MLO, MLO_WORK_STATE_MLSR, reason);
    /* 通知上层mlsr工作link变化 */
    hmac_mld_work_link_change_notify(expect_work_vap);

    hmac_vap_state_change_notify_feature(expect_work_vap, hmac_user, COFEATURE_EMLSR_EVENT, COFEATURE_EXIT);

    return ret;
}

void hmac_emlsr_mvap_disable_bitmap_set(uint8_t vap_id)
{
    g_emlsr_disable_mvap_id_bitmap |= BIT(vap_id);
}

void hmac_emlsr_mvap_disable_bitmap_clear(uint8_t vap_id)
{
    g_emlsr_disable_mvap_id_bitmap &= ~BIT(vap_id);
}

uint8_t hmac_emlsr_check_mvap_disable_bitmap_allowed(void)
{
    hmac_vap_stru *hmac_ml_vap = NULL;
    hmac_vap_stru *hmac_link_vap = NULL;
    uint32_t bitmap = g_emlsr_disable_mvap_id_bitmap;

    if (hmac_mld_get_mlsr_link_sta(&hmac_ml_vap, &hmac_link_vap) != OAL_SUCC) {
        return OAL_FALSE;
    }

    bitmap &= ~BIT(hmac_ml_vap->st_vap_base_info.uc_vap_id);
    bitmap &= ~BIT(hmac_link_vap->st_vap_base_info.uc_vap_id);

    return (bitmap == 0 ? OAL_TRUE : OAL_FALSE);
}

/* 函数功能：非mld vap 关联/创建GO调用，退出EMLSR */
uint32_t hmac_emlsr_close_for_mvap(mac_vap_stru *mac_vap, mac_channel_stru *new_channel_info, uint8_t event_type)
{
    hmac_vap_stru *hmac_ml_vap = NULL;
    hmac_vap_stru *hmac_link_vap = NULL;
    hmac_vap_stru *expect_work_vap = NULL;

    if (mac_vap == NULL || new_channel_info == NULL) {
        return OAL_FAIL;
    }

    hmac_emlsr_mvap_disable_bitmap_set(mac_vap->uc_vap_id);

    if (hmac_mld_get_mlsr_link_sta(&hmac_ml_vap, &hmac_link_vap) != OAL_SUCC) {
        return OAL_SUCC;
    }

    /* 决策emlsr退出后的工作link */
    expect_work_vap = hmac_emlsr_close_work_vap_decision(hmac_ml_vap, hmac_link_vap, new_channel_info);
    if (expect_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR, "{hmac_emlsr_close_for_mvap::new_vap channel invalid}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap_state_change_notify_emlsr(expect_work_vap, NULL, COFEATURE_MVAP_EVENT, OAL_FALSE);
    return OAL_SUCC;
}

static uint32_t hmac_emlsr_close_indicate_work_vap(hmac_vap_stru *expect_work_vap, uint8_t reason)
{
    hmac_vap_stru *hmac_ml_vap = NULL;
    hmac_vap_stru *hmac_link_vap = NULL;

    if (hmac_mld_get_mlsr_link_sta(&hmac_ml_vap, &hmac_link_vap) != OAL_SUCC) {
        return OAL_SUCC;
    }

    /* 退出原因不合法 或 期望退出后的工作vap不合法，则默认退回主link */
    if (hmac_emlsr_close_reason_need_indicate_work_vap(reason) == OAL_FALSE ||
        hmac_emlsr_close_check_expect_vap_is_valid(hmac_ml_vap, hmac_link_vap, expect_work_vap) == OAL_FALSE) {
        expect_work_vap = hmac_ml_vap;
    }

    return hmac_emlsr_force_close_process(expect_work_vap, reason);
}

uint32_t hmac_emlsr_force_close_event_process(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL; /* 事件结构体 */
    emlsr_close_event_stru *emlsr_close_event = NULL;
    hmac_vap_stru *hmac_vap = NULL;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_emlsr_force_close_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    emlsr_close_event = (emlsr_close_event_stru *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(emlsr_close_event->vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(emlsr_close_event->vap_id, OAM_SF_EMLSR, "{hmac_emlsr_force_close_event_process::vap null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap_state_change_notify_emlsr(hmac_vap, NULL, COFEATURE_BT_EVENT, OAL_FALSE);

    return OAL_SUCC;
}

OAL_STATIC void hmac_emlsr_bitmap_sync_notify(hmac_vap_stru *hmac_vap, uint8_t event, uint32_t ori_bitamp)
{
    /* bitmap没变化不同步到device */
    if (g_emlsr_disable_bitmap == ori_bitamp) {
        return;
    }

    oam_warning_log3(0, OAM_SF_ANY,
        "{hmac_emlsr_bitmap_sync_notify::event[%d], old_emlsr_disable_bitmap[0x%x], new_emlsr_disable_bitmap[0x%x]}",
        event, ori_bitamp, g_emlsr_disable_bitmap);

    if (hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_EMLSR_BITMAP_SYNC,
        sizeof(uint32_t), (uint8_t *)&g_emlsr_disable_bitmap) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_emlsr_bitmap_sync_notify::send event failed}");
    }
}

OAL_STATIC void hmac_emlsr_cfgvendor_event_handle(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, uint8_t is_enter_event)
{
    if (is_enter_event == OAL_TRUE) {
        hmac_emlsr_disable_bitmap_clear(EMLSR_CFGVENDOR_EVENT); /* 上层ioctl命令触发进入emlsr，清掉bit位 */
        hmac_emlsr_enter_check_device_state_allowed(hmac_vap, hmac_user);
    } else {
        hmac_emlsr_disable_bitmap_set(EMLSR_CFGVENDOR_EVENT); /* 命令触发退出emlsr，置bit位 */
        hmac_emlsr_force_close_process(hmac_vap, EMLSR_CLOSE_FOR_IOCTL);
    }
}

OAL_STATIC void hmac_emlsr_mvap_event_handle(hmac_vap_stru *hmac_vap, uint8_t is_enter_event)
{
    if (is_enter_event == OAL_TRUE) {
        /* mvap退网，进入emlsr */
        hmac_emlsr_mvap_disable_bitmap_clear(hmac_vap->st_vap_base_info.uc_vap_id);
        if (g_emlsr_disable_mvap_id_bitmap == 0) {
            /* 没有多vap限制禁止进入EMLSR，清除emlsr_disable_bitmap.COFEATURE_MVAP bit */
            hmac_emlsr_disable_bitmap_clear(COFEATURE_MVAP_EVENT);
        }
        if (hmac_emlsr_check_mvap_disable_bitmap_allowed() != OAL_TRUE) {
            hmac_emlsr_print_info();
            return;
        }
        hmac_emlsr_disable_bitmap_clear(COFEATURE_MVAP_EVENT);
        hmac_emlsr_default_enter_process();
    } else {
        /* 退出EMLSR */
        if (g_emlsr_disable_mvap_id_bitmap != 0) {
            /* 非MLD VAP触发的禁止EMLSR,才置位多vap禁止标志; 是MLD VAP自己触发的禁止EMLSR,不用置位多VAP禁止标志 */
            hmac_emlsr_disable_bitmap_set(COFEATURE_MVAP_EVENT); /* mvap入网，退出emlsr */
        }
        hmac_emlsr_force_close_process(hmac_vap, EMLSR_CLOSE_FOR_MVAP);
    }
}

OAL_STATIC void hmac_emlsr_reconfig_event_handle(hmac_vap_stru *hmac_vap, uint8_t is_enter_event)
{
    if (is_enter_event == OAL_FALSE) {
        hmac_emlsr_disable_bitmap_set(COFEATURE_RECONFIG_EVENT);
        hmac_emlsr_close_indicate_work_vap(hmac_vap, EMLSR_CLOSE_FOR_RECONFIG);
    }
}

OAL_STATIC void hmac_emlsr_ttlm_event_handle(hmac_vap_stru *hmac_vap, uint8_t is_enter_event)
{
    if (is_enter_event == OAL_TRUE) {
        hmac_emlsr_disable_bitmap_clear(COFEATURE_TTLM_EVENT);
        /* 补充:ttlm解禁自动进入emlsr */
    } else {
        hmac_emlsr_disable_bitmap_set(COFEATURE_TTLM_EVENT);
        hmac_emlsr_close_indicate_work_vap(hmac_vap, EMLSR_CLOSE_FOR_RECONFIG);
    }
}

OAL_STATIC void hmac_emlsr_bt_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t is_enter_event)
{
    if (is_enter_event == OAL_TRUE) {
        /* 检查是否支持进入emlsr */
        if (hmac_check_emlsr_enter_is_allowed() == OAL_FALSE) {
            oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                "{hmac_emlsr_bt_event_handle::not allow emlsr}");
            return;
        }
        hmac_emlsr_enter_tx_omn_action(hmac_vap, hmac_user);
    } else {
        hmac_emlsr_close_indicate_work_vap(hmac_vap, EMLSR_CLOSE_FOR_BT);
    }
}

OAL_STATIC void hmac_emlsr_link_switch_event_handle(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, uint8_t is_enter_event)
{
    if (hmac_vap == NULL || hmac_user == NULL) {
        return;
    }

    if (is_enter_event != OAL_FALSE) {
        return;
    }

    if (hmac_check_emlsr_enter_is_allowed() == OAL_FALSE) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{hmac_emlsr_bt_event_handle::not allow emlsr}");
        return;
    }

    hmac_emlsr_enter_check_device_state_allowed(hmac_vap, hmac_user);
}

OAL_STATIC void hmac_mld_deinit_emlsr_exit(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *hmac_link_vap_work = NULL;
    hmac_vap_stru *hmac_link_vap_down = NULL;
    uint32_t ret;

    if (g_wlan_spec_cfg->feature_ml_is_supported != OAL_TRUE) {
        return;
    }

    ret = hmac_mld_get_mlsr_link_sta(&hmac_link_vap_work, &hmac_link_vap_down);
    if (ret != OAL_SUCC) {
        return;
    }

    if (hmac_vap != hmac_link_vap_work && hmac_vap != hmac_link_vap_down) {
        return;
    }

    hmac_emlsr_disable_bitmap_set(EMLSR_CFGVENDOR_EVENT);
    hmac_emlsr_disable_bitmap_clear(COFEATURE_RECONFIG_EVENT);
    hmac_emlsr_disable_bitmap_clear(COFEATURE_TTLM_EVENT);
    /* 退出MLSR模式，清除MLSR emlsr_mvap_disable_bitmap */
    hmac_emlsr_mvap_disable_bitmap_clear(hmac_link_vap_work->st_vap_base_info.uc_vap_id);
    hmac_emlsr_mvap_disable_bitmap_clear(hmac_link_vap_down->st_vap_base_info.uc_vap_id);

    /* emlsr状态去关联 通知wur模块退出emlsr */
    hmac_vap_state_change_notify_feature(hmac_link_vap_work, NULL, COFEATURE_EMLSR_EVENT, COFEATURE_EXIT);
}

void hmac_vap_state_change_notify_emlsr(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t is_enter_event)
{
    uint32_t ori_bitamp = g_emlsr_disable_bitmap;

    if (hmac_vap == NULL) {
        oam_error_log2(0, OAM_SF_EMLSR,
            "{hmac_vap_state_change_notify_emlsr::hmac_vap null! event[%d] enter_emlsr[%d]}",
            event_type, is_enter_event);
        return;
    }

    /* emlsr定制化关闭，不处理通知emlsr的事件 */

    switch (event_type) {
        case EMLSR_CFGVENDOR_EVENT:
            hmac_emlsr_cfgvendor_event_handle(hmac_vap, hmac_user, is_enter_event);
            break;
        case COFEATURE_MVAP_EVENT:
            hmac_emlsr_mvap_event_handle(hmac_vap, is_enter_event);
            break;
        case COFEATURE_ROAM_EVENT:
        case COFEATURE_RECONNECT_EVENT:
            /* 漫游/自愈重关联退出emlsr，通知上层已退出，置位上层关闭emlsr bit位，需上层重新触发emlsr */
            if (is_enter_event == OAL_FALSE) {
                hmac_emlsr_disable_bitmap_set(EMLSR_CFGVENDOR_EVENT);
            }
            hmac_emlsr_close_indicate_work_vap(hmac_vap, EMLSR_CLOSE_FOR_ROAM);
            break;
        case COFEATURE_RECONFIG_EVENT:
            hmac_emlsr_reconfig_event_handle(hmac_vap, is_enter_event);
            break;
        case COFEATURE_TTLM_EVENT:
            hmac_emlsr_ttlm_event_handle(hmac_vap, is_enter_event);
            break;
        case COFEATURE_BT_EVENT:
            hmac_emlsr_bt_event_handle(hmac_vap, hmac_user, is_enter_event);
            break;
        case COFEATURE_MLD_DEINIT_EVENT:
            hmac_mld_deinit_emlsr_exit(hmac_vap);
            break;
        default:
            oam_warning_log0(0, OAM_SF_ANY, "{hmac_vap_state_change_notify_wur::unknow event type}");
            break;
    }

    /* bitmap同步到device */
    hmac_emlsr_bitmap_sync_notify(hmac_vap, event_type, ori_bitamp);
}

void hmac_vap_state_change_notify_emlsr_common_proc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype)
{
    uint32_t ori_bitamp = g_emlsr_disable_bitmap;

    if (hmac_vap == NULL) {
        oam_error_log2(0, OAM_SF_EMLSR,
            "{hmac_vap_state_change_notify_emlsr::hmac_vap null! event[%d] enter_emlsr[%d]}",
            event_type, event_subtype);
        return;
    }

    /* emlsr定制化关闭，不处理通知emlsr的事件 */
    switch (event_type) {
        case COFEATURE_LINK_SWITCH_EVENT:
            hmac_emlsr_link_switch_event_handle(hmac_vap, hmac_user, event_subtype);
            break;
        default:
            break;
    }

    /* bitmap同步到device */
    hmac_emlsr_bitmap_sync_notify(hmac_vap, event_type, ori_bitamp);
}

#define SIGNAL_LEVEL_NUM 4
struct rssi_to_signal_level {
    uint32_t level;
    int8_t rssi[WLAN_BAND_BUTT];
};

struct rssi_to_signal_level g_rssi_to_signal_level[SIGNAL_LEVEL_NUM] = {
    { 4,  {-65, -65}, }, // 四格
    { 3,  {-75, -72}, }, // 三格
    { 2,  {-82, -79}, }, // 两格
    { 1,  {-88, -85}, }, // 一格
    // 零格
};

uint32_t hmac_rssi_to_signal_level(int8_t rssi, wlan_channel_band_enum_uint8 band)
{
    uint32_t i = 0;
    if (band >= WLAN_BAND_BUTT) {
        /* 非法频段，返回最小信号格数 */
        oam_error_log1(0, OAM_SF_EMLSR, "{hmac_rssi_to_signal_level::invalid band [%d]}", band);
        return 0;
    }

    for (i = 0; i < SIGNAL_LEVEL_NUM; i++) {
        if (rssi >= g_rssi_to_signal_level[i].rssi[band]) {
            return g_rssi_to_signal_level[i].level;
        }
    }
    return 0;
}

hmac_vap_stru *hmac_emlsr_exit_choose_strong_rssi_link(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *next_work_vap = NULL;
    hmac_vap_stru *hmac_ml_vap = NULL;
    hmac_vap_stru *hmac_link_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint32_t signal_level_ml = 0;
    uint32_t signal_level_link = 0;

    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_EMLSR,
            "{hmac_emlsr_exit_choose_strong_rssi_link::hmac_vap null!}");
        return NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return hmac_vap;
    }

    if (mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_OPENED) {
        return hmac_vap;
    }

    if (hmac_mld_get_mlsr_link_sta(&hmac_ml_vap, &hmac_link_vap) != OAL_SUCC) {
        return hmac_vap;
    }
    signal_level_ml = hmac_rssi_to_signal_level(hmac_ml_vap->station_info.signal,
        hmac_ml_vap->st_vap_base_info.st_channel.en_band);
    signal_level_link = hmac_rssi_to_signal_level(hmac_link_vap->station_info.signal,
        hmac_ml_vap->st_vap_base_info.st_channel.en_band);
    /* 为避免退出EMLSR信号跳变:
     * 1. 选择RSSI强的VAP为工作VAP
     * 2. RSSI信号格数相同，优选5G工作
     */
    if (signal_level_ml > signal_level_link) {
        next_work_vap = hmac_ml_vap;
    } else if (signal_level_ml < signal_level_link) {
        next_work_vap = hmac_link_vap;
    } else {
        next_work_vap = (hmac_ml_vap->st_vap_base_info.st_channel.en_band == WLAN_BAND_5G) ?
            hmac_ml_vap : hmac_link_vap;
    }

    return next_work_vap;
}

void hmac_config_d2h_emlsr_pm_info_sync(mac_vap_stru *mac_vap, uint8_t pm_state)
{
    uint32_t ret;
    mac_cfg_ps_mode_param_stru ps_mode = { 0 };
    mac_cfg_ps_open_stru sta_pm_open = { 0 };
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }
    ps_mode.uc_vap_ps_mode = (pm_state != NO_POWERSAVE) ? hmac_vap->uc_ps_mode : NO_POWERSAVE;
    sta_pm_open.uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_HOST;
    sta_pm_open.uc_pm_enable = pm_state;
    ret = hmac_config_sync_set_sta_pm_on(mac_vap, &ps_mode, sizeof(mac_cfg_ps_open_stru), (uint8_t *)&sta_pm_open);
    if (ret != OAL_SUCC) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_d2h_emlsr_pm_info_sync::hmac_config_set_pm_by_module failed[%d].}", ret);
    }
}

uint32_t hmac_config_d2h_emlsr_info_syn(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    mac_emlsr_info_sync_stru *emlsr_info = (mac_emlsr_info_sync_stru *)param;
    hmac_config_d2h_emlsr_pm_info_sync(mac_vap, emlsr_info->pm_state);
    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_CFG, "hmac_config_d2h_emlsr_info_syn::emlsr[%d] pm[%d]",
        emlsr_info->emlsr_state, emlsr_info->pm_state);
    return OAL_SUCC;
}

#endif