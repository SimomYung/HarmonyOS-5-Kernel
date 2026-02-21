// SPDX-License-Identifier: GPL-2.0-only
/*
 * Helpers for category Liblinux API and linker scripts
 */

#ifndef __LIBLINUX_API_H__
#define __LIBLINUX_API_H__

#define LIBLINUX_TEXT \
	ALIGN_FUNCTION(); \
	*(.__liblinux_unimpl) \
	*(.text.override) \
	ALIGN_FUNCTION(); \
	__liblinux_kapi_text_start = .; \
	*(.liblinux.kapi.text) \
	__liblinux_kapi_text_end = .; \
	ALIGN_FUNCTION(); \
	__liblinux_kapi_diff_text_start = .; \
	*(.liblinux.kapi.diff.text) \
	__liblinux_kapi_diff_text_end = .; \
	ALIGN_FUNCTION(); \
	__liblinux_kapi_unimpl_text_start = .; \
	*(.liblinux.unimpl.text) \
	__liblinux_kapi_unimpl_text_end = .; \

#define LIBLINUX_KAPI __attribute__((section(".liblinux.kapi.text")))
#define LIBLINUX_KAPI_DIFF __attribute__((section(".liblinux.kapi.diff.text")))
#define LIBLINUX_KAPI_UNIMPL __attribute__((section(".liblinux.kapi.unimpl.text")))

#ifndef __ASSEMBLY__

extern char __liblinux_kapi_text_start[];
extern char __liblinux_kapi_text_end[];

extern char __liblinux_kapi_diff_text_start[];
extern char __liblinux_kapi_diff_text_end[];

extern char __liblinux_kapi_unimpl_text_start[];
extern char __liblinux_kapi_unimpl_text_end[];

#endif

#endif /* __LIBLINUX_API_H__ */
