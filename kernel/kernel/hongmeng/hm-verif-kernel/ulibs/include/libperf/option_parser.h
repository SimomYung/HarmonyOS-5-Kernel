/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: wrap getopt option and usage
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 21 11:44:25 2022
 */

#ifndef __PERF_OPTION_H
#define __PERF_OPTION_H

#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

enum perf_option_type {
	OPTPARSE_OPTION,
	OPTPARSE_SIMPLE_OPTION,
	OPTPARSE_GROUP_OPTION,
	OPTPARSE_OPTIONAL_OPTION,
	OPTPARSE_END,
};

/*
 * indicate the end of options, this macro must be contained in the end.
 */
#define optparse_end() \
	{\
		.short_name = 0, \
		.long_option = {NULL, no_argument, NULL, 0}, \
		.input_desc = NULL, \
		.help_msg = NULL, \
		.type = OPTPARSE_END, \
		.check_func = NULL, \
	}

/*
 * s_name: a string for option short name
 * long_name: getopt patameter, a string for option long name
 * has_arg: getopt patameter, can be set to no_argument/required_argument/optional_argument
 * flag: getopt patameter, specifies how results are returned for a long option.
 * val: getopt patameter, all getopt patameter can refert to getopt doc:
 * 	https://man7.org/linux/man-pages/man3/getopt.3.html to see it's usage.
 * in_desc: input description if option has argument, these description will be wraped in <> when printed.
 * help_info: help information for this option. charaters after '\n' in help_info is aligned with precious lines.
 * check_func: call back function when printing, see macro optparse_optional_option.
 */
#define optparse_option(s_name, long_name, has_arg, flag, val, in_desc, help_info) \
	{\
		.short_name = (s_name), \
		.long_option = { long_name, has_arg, flag, val }, \
		.input_desc = (char *)(in_desc), \
		.help_msg = (char *)(help_info), \
		.type = OPTPARSE_OPTION, \
		.check_func = NULL, \
	}

/*
 * when check_func returns true, this option will be printed,
 * other parameters stay the same with optparse_option
 */
#define optparse_optional_option(s_name, long_name, has_arg, flag, val, in_desc, help_info, func) \
	{\
		.short_name = (s_name), \
		.long_option = { long_name, has_arg, flag, val }, \
		.input_desc = (char *)(in_desc), \
		.help_msg = (char *)(help_info), \
		.type = OPTPARSE_OPTIONAL_OPTION, \
		.check_func = (bool (*)())(func), \
	}

/*
 * a common used command setting.
 * input desc is eqivalent to long_name, if has_arg is set to require_argument
 * flag is set to NULL
 */
#define optparse_simple_option(s_name, long_name, has_arg, val, help_info) \
	{\
		.short_name = (s_name), \
		.long_option = { long_name, has_arg, NULL, val }, \
		.input_desc = NULL, \
		.help_msg = (char *)(help_info), \
		.type = OPTPARSE_SIMPLE_OPTION, \
		.check_func = NULL, \
	}

/*
 * options below this macro will be printed in a new block of lines.
 */
#define optparse_group_option(help_info) \
	{\
		.short_name = 0, \
		.long_option = {NULL, no_argument, NULL, 0}, \
		.input_desc = NULL, \
		.help_msg = (char *)(help_info), \
		.type = OPTPARSE_GROUP_OPTION, \
		.check_func = NULL, \
	}

struct perf_option {
	const char short_name;
	/* struct option is used in getopt */
	const struct option long_option;
	const char *input_desc;
	const char *help_msg;
	enum perf_option_type type;
	bool (*check_func)();
};

struct option_parser {
	/* usages ends with a NULL potinter */
	const char *const *usages;
	/* a description after usage */
	const char *description;
	/* a description at the end */
	const char *epilog;
	struct perf_option *options;
	/* short_options and getopt_options are used in getopt */
	char *short_options;
	struct option *getopt_options;
};

int parser_init(struct option_parser *parser, const char *const *usages, const char *description,
		const char *epilog, struct perf_option* options);
void parser_free(struct option_parser *parser);
void option_usage(const struct option_parser *parser);

#endif