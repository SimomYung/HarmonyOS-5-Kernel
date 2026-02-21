// SPDX-License-Identifier: GPL-2.0

void liblinux_cpa_prepare_memory_for_work(struct liblinux_pal_cpa_prepare_memory_args *args)
{
	if (args->flags == 0) {
		args->flags =  LIBLINUX_PAL_CPA_PREPARE_CAN_DROP |
			LIBLINUX_PAL_CPA_PREPARE_CAN_KILL |
			LIBLINUX_PAL_CPA_PREPARE_CAN_RECLAIM |
			LIBLINUX_PAL_CPA_PREPARE_CAN_COMPAT |
			LIBLINUX_PAL_CPA_PREPARE_KILL_ZRAM |
			LIBLINUX_PAL_CPA_PREPARE_COMPAT_MODEL_1 |
			LIBLINUX_PAL_CPA_PREPARE_MODE_WORK;
	}

	liblinux_pal_cpa_prepare_memory(args);
}

void liblinux_cpa_prepare_memory_for_alloc(struct liblinux_pal_cpa_prepare_memory_args *args)
{
	if (args->flags == 0) {
		args->flags =  LIBLINUX_PAL_CPA_PREPARE_CAN_DROP |
			LIBLINUX_PAL_CPA_PREPARE_CAN_KILL |
			LIBLINUX_PAL_CPA_PREPARE_CAN_RECLAIM |
			LIBLINUX_PAL_CPA_PREPARE_CAN_COMPAT |
			LIBLINUX_PAL_CPA_PREPARE_KILL_ZRAM |
			LIBLINUX_PAL_CPA_PREPARE_COMPAT_MODEL_0 |
			LIBLINUX_PAL_CPA_PREPARE_MODE_ALLOC;
	}

	liblinux_pal_cpa_prepare_memory(args);
}

void liblinux_cpa_prepare_memory_for_remaining(struct liblinux_pal_cpa_prepare_memory_args *args)
{
	if (args->flags == 0) {
		args->flags = LIBLINUX_PAL_CPA_PREPARE_CAN_KILL |
			LIBLINUX_PAL_CPA_PREPARE_CAN_RECLAIM |
			LIBLINUX_PAL_CPA_PREPARE_CAN_COMPAT |
			LIBLINUX_PAL_CPA_PREPARE_KILL_FREEMEM |
			LIBLINUX_PAL_CPA_PREPARE_COMPAT_MODEL_1 |
			LIBLINUX_PAL_CPA_PREPARE_MODE_REMAIN;
	}

	liblinux_pal_cpa_prepare_memory(args);
}
