/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: function declaration for sending smc cmd
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TC_BITDEF_H
#define TC_BITDEF_H

#ifdef DIV_ROUND_UP
#undef DIV_ROUND_UP
#endif
#define DIV_ROUND_UP(n, d)              (((n) + (d) - 1) / (d))

#ifdef BITS_PER_BYTE
#undef BITS_PER_BYTE
#endif
#define BITS_PER_BYTE                   8U

#ifdef BITS_TO_LONGS
#undef BITS_TO_LONGS
#endif
#define BITS_TO_LONGS(nr)               DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(uint64_t))

#ifdef BIT_MASK
#undef BIT_MASK
#endif
#define BIT_MASK(nr)                    (1UL << (((uint64_t)(nr)) % (BITS_PER_BYTE * sizeof(uint64_t))))

#ifdef BIT_WORD
#undef BIT_WORD
#endif
#define BIT_WORD(nr)                    ((nr) / sizeof(uint64_t))

#ifdef DECLARE_BITMAP
#undef DECLARE_BITMAP
#endif
#define DECLARE_BITMAP(name, bits)      uint64_t name[BITS_TO_LONGS(bits)]

#endif