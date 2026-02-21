/* SPDX-License-Identifier: GPL-2.0 */

/* DECLARE_HOOK and DECLARE_RESTRICTED_HOOK depends on hck_vendor_hooks.h */
#include <trace/hooks/hck_hook/hck_vendor_hooks.h>

#if defined(CONFIG_TRACEPOINTS) && defined(CONFIG_VENDOR_HOOKS)

#define DECLARE_HOOK DECLARE_HCK_HOOK
#define DECLARE_RESTRICTED_HOOK DECLARE_HCK_RESTRICTED_HOOK

#else /* !CONFIG_TRACEPOINTS || !CONFIG_VENDOR_HOOKS */

#define DECLARE_HOOK DECLARE_EVENT_NOP
#define DECLARE_RESTRICTED_HOOK(name, proto, args, cond)                \
	DECLARE_EVENT_NOP(name, PARAMS(proto), PARAMS(args))
#endif /* CONFIG_TRACEPOINTS && CONFIG_VENDOR_HOOKS */
