/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Fault event lib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 11 16:19:37 2023
 */

#ifndef ULIBS_LIBHM_FAULT_EVENT_H
#define ULIBS_LIBHM_FAULT_EVENT_H

#include <stdint.h>
#include <hongmeng/macro.h>
#include <libstrict/strict.h>

#include <libhmfaultevt/fault_event_ids.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FEVT_EVENT_ID_TO_MODULE(event_id)	((event_id) >> FEVT_EVENT_ID_SHIFT)

#define FEVT_EVENT_ID_TO_IDX(event_id, module_id)		\
	((event_id) - (FEVT_EVENT_ID_BEGIN(module_id) + 1U))

#define FEVT_EVENT_ID_TO_IDX_RAW(module_name, event_name)	\
	((FEVT_EVENT_##module_name##_##event_name) - 		\
	 (FEVT_EVENT_ID_BEGIN_RAW(module_name) + 1U))

#define __fevt_kv_fmt(n, key, value)						\
__builtin_choose_expr(								\
	(__builtin_types_compatible_p(typeof(value), char) 			\
	 || __builtin_types_compatible_p(typeof(value), signed char)		\
	 || __builtin_types_compatible_p(typeof(value), unsigned char)),	\
		key "=%c",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), short),			\
		key "=%hd",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), unsigned short),		\
		key "=%hu",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), int),			\
		key "=%d",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), unsigned),			\
		key "=%u",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), long),			\
		key "=%ld",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), unsigned long),		\
		key "=%lu",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), long long),			\
		key "=%lld",							\
__builtin_choose_expr(								\
	__builtin_types_compatible_p(typeof(value), unsigned long long),	\
		key "=%llu",							\
__builtin_choose_expr(								\
	(__builtin_types_compatible_p(typeof(value), char *)			\
	 || __builtin_types_compatible_p(typeof(value), char [])		\
	 || __builtin_types_compatible_p(typeof(value), const char *)),		\
		key "=\"%s\"",							\
	/* void expression results in a compile-time error */			\
	(void)0)))))))))), (value)

#define __FEVT_KV_MAP0(n, ...)		(n)
#define __FEVT_KV_MAP2(n, ...)		__SC_MAP1(n, __VA_ARGS__)
#define __FEVT_KV_MAP4(n, ...)		__SC_MAP2(n, __VA_ARGS__)
#define __FEVT_KV_MAP6(n, ...)		__SC_MAP3(n, __VA_ARGS__)
#define __FEVT_KV_MAP8(n, ...)		__SC_MAP4(n, __VA_ARGS__)
#define __FEVT_KV_MAP10(n, ...)		__SC_MAP5(n, __VA_ARGS__)
#define __FEVT_KV_MAP12(n, ...)		__SC_MAP6(n, __VA_ARGS__)
#define __FEVT_KV_MAP14(n, ...)		__SC_MAP7(n, __VA_ARGS__)
#define __FEVT_KV_MAP16(n, ...)		__SC_MAP8(n, __VA_ARGS__)

/* Two parameters form a key-value pair, so divide by 2 */
#define FEVT_KV_MAP(n, ...)		__FEVT_KV_MAP##n(((n) / 2), __VA_ARGS__)
#define fevt_kv_pairs(n, ...)		((n) / 2), FEVT_KV_MAP(n, __fevt_kv_fmt, __VA_ARGS__)

enum fevt_event_level {
	FEVT_LEVEL_DEBUG,
	FEVT_LEVEL_INFO,
	FEVT_LEVEL_GENERAL,
	FEVT_LEVEL_IMPORTANT,
	FEVT_LEVEL_CRITICAL,
	FEVT_LEVEL_NR,
};

enum fevt_event_type {
	FEVT_TYPE_FAULT,
	FEVT_TYPE_STATICS,
	FEVT_TYPE_NR,
};

struct fevt_event {
	const enum fevt_event_level level;
	const enum fevt_event_type type;
	const char *desc;
};

/*
 * Report a fault event
 *
 * @param event_id      [IN] event id of fault event
 *
 * @param kv_nr         [IN] number of key-value pairs
 *
 * @param ...(va_args)  [IN] optional varargs.
 *                           The number of parameters is 0, or an integer multiple of two.
 *
 * @return              [OUT] NA
 *
 * @note                [DESC] Do not call this function directly, but use hm_fevt_report()
 *                             unless you confirm:
 *                      (1) the number of parameters (@va_args) passed in meets the
 *                          needs of @kv_nr (A key-value pair requires two parameters);
 *                      (2) the incoming "key" must be a string in the form of "key=%FORMAT"
 *                          and cannot be a variable. "FORMAT" must be the format character
 *                          corresponding to the type of "value".
 *                       For example:
 *                         hm_fevt_report_raw(event_id, 0);
 *                         hm_fevt_report_raw(event_id, 1, "key=%d", 1);
 *                         hm_fevt_report_raw(event_id, 2, "key1=%d", 1, "key2=%s", "value");
 */
void hm_fevt_report_raw(uint32_t event_id, uint32_t kv_nr, ...);
int hm_fevt_module_register(enum fevt_module_id module_id, const struct fevt_event *events, uint32_t evt_nr);

#ifdef __HOST_UT__
#define UT_FEVT_TEST_MODULE	"UT_TEST_MODULE"
void ut_fevt_set_test_event(const struct fevt_event *event);
#endif

/*
 * The macro hm_fevt_report is a layer of encapsulation of the function hm_fevt_report_raw(),
 * and this macro is provided for the convenience of the caller.
 *
 * This macro mainly does two things. One is to calculate the value of @kv_nr when compiling
 * with the macro VA_ARGS_NUM, and the other is to add "format character" to the "key"
 * according to the type of "value".
 */
#define hm_fevt_report(event_name, ...)						\
	hm_fevt_report_raw((uint32_t)FEVT_EVENT_##event_name,			\
			   fevt_kv_pairs(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif
