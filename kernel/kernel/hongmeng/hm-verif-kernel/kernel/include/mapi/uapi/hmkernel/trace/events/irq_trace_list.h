/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Kernel irq tracepoints
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 19 14:29:17 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(irq, irq_handler_entry,
		TP_PROTO(int, irq, const char *, name),
		TP_ARGS(irq, name),
		TP_FIELD(
			__field(int, irq)
			__string(name, name)
		),
		TP_ASSIGN(
			__entry->irq = irq;
			/* keep the same format with ldk */
			__assign_str(name, name)
		),
		TP_PRINT("irq=%d name=%s", __entry->irq, __get_str(name))
)

TRACE_EVENT(irq, irq_handler_exit,
		TP_PROTO(int, irq, int, retval),
		TP_ARGS(irq, retval),
		TP_FIELD(
			__field(int, irq)
			__field(int, ret)
		),
		TP_ASSIGN(
			__entry->irq = irq;
			/* keep the same format with ldk */
			__entry->ret = retval;
		),
		TP_PRINT("irq=%d ret=%s", __entry->irq, __entry->ret ? "handled" : "unhandled")
)
