/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:oal_sdio.c header file
 * Author: @CompanyNameTag
 */

#ifndef OAL_SDIO_H
#define OAL_SDIO_H

#define HISDIO_VENDOR_ID_MP13    0x12D1 /* VENDOR mp13 */
#define HISDIO_VENDOR_ID_MP15    0x12D1 /* VENDOR mp15 */
#define HISDIO_VENDOR_ID_HISI    0x12D1 /* VENDOR HISI */
#define MSDIO_VENDOR_ID_MPXX     0xED2E

#define HISDIO_PRODUCT_ID_HISI          0x5347 /* Product mpxx */
#define HISDIO_PRODUCT_ID_MP15_HISI     0x5349 /* Product mp15 */
#define HISDIO_PRODUCT_ID_MP16_HISI     0x534b /* Product mp16 */
#define HISDIO_PRODUCT_ID_MP16C         0xb001 /* Product mp16c */
#define MSDIO_PRODUCT_ID_MP17C          0xEEF8
#define MSDIO_PRODUCT_ID_MP12           0xEEED  /* Product mp12 */

#define SDIO_MAX_CONTINUS_RX_COUNT 64 /* sdio max continus rx cnt */

/* sdio extend function, add 64B register for hcc */
#define HISDIO_FUNC1_EXTEND_REG_LEN  64

#define HISDIO_FUNC1_INT_DREADY (1 << 0) /* data ready interrupt */
#define HISDIO_FUNC1_INT_RERROR (1 << 1) /* data read error interrupt */
#define HISDIO_FUNC1_INT_MFARM  (1 << 2) /* ARM Msg interrupt */
#define HISDIO_FUNC1_INT_ACK    (1 << 3) /* ACK interrupt */

#define HISDIO_FUNC1_INT_MASK (HISDIO_FUNC1_INT_DREADY | HISDIO_FUNC1_INT_RERROR | HISDIO_FUNC1_INT_MFARM)

#endif /* end of oal_sdio.h */

