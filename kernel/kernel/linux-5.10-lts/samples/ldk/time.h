/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LDK_TIME_H
#define __LDK_TIME_H

struct liblinux_time_udata {
	long long offs_real;	/* real to mono offset in nanoseconds */
	long long real;		/* real time in nanoseconds */
	long long cycle_last;	/* mono time last cycle */
	long long offs_boot;	/* boot to mono offset in nanoseconds */
	long long rtc_offset;   /* offset from soc rtc to walltime */
	long long rtc_lastts;	/* last timestamp set by kernel */
	int tz_minuteswest;	/* minutes west of Greenwich */
	int tz_dsttime;		/* type of DST correction */
	bool only_update_timezone; /* only update timezone to ldk */
	unsigned long mono_sec;	/* mono time part of sec */
	unsigned long long mono_nsec;	/* mono time part of nanoseconds */
};

int liblinux_read_time_udata(struct liblinux_time_udata *data);

#endif /* __LDK_TIME_H */
