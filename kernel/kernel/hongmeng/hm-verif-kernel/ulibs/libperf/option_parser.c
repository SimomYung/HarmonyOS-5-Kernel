/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: wrap getopt option and usage
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 24 19:01:57 2022
 */

#include <stdio.h>
#include <string.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libperf/option_parser.h>
#include <libhmlog/hmlog.h>

/* option processing stops as soon as a nonoption argument is encountered when using this prefix */
#define SHORT_OPTIONS_PREFIX "+"
#define SHORT_OPT_REQUIRE_ARG ':'

#define OPTPARSE_NEWLINE_SEP "\n"
#define OPTPARSE_SPACE_PAD 2
#define OPTPARSE_LONG_SPACE_PRE 8
#define OPTPARSE_SHORT_SPACE_PRE 4
#define OPTPARSE_LEN_SHORT_NAME 2
#define OPTPARSE_LEN_SEP 2
#define OPTPARSE_LONG_NAME_COST 2
#define OPTPARSE_DESC_COST 3
#define OPTPARSE_SHORT_OPT_MAX_LEN 3
#define OPTPARSE_SHORT_OPTS_PREFIX_LEN 1
#define OPTPARSE_TAIL_ZERO_LEN 1
#define OPTPARSE_TEMP_OPT_LEN (OPTPARSE_SHORT_OPT_MAX_LEN + OPTPARSE_TAIL_ZERO_LEN)

static size_t opt_size_exclude_group(const struct option_parser *parser)
{
	struct perf_option* options;
	size_t count = 0;

	options = parser->options;
	for (; options->type != OPTPARSE_END; options++) {
		if (options->type == OPTPARSE_GROUP_OPTION) {
			continue;
		} else {
			++count;
		}
	}

	/* include opt end */
	++count;

	return count;
}

static void compelete_simple_opts(struct option_parser *parser)
{
	struct perf_option* options;

	options = parser->options;
	for (; options->type != OPTPARSE_END; options++) {
		if (options->type == OPTPARSE_SIMPLE_OPTION && options->long_option.has_arg == required_argument) {
			options->input_desc = options->long_option.name;
		}
	}
}

static int gen_short_opt_from_name(const struct perf_option* option, char *short_opt, size_t opt_len)
{
	if (opt_len < OPTPARSE_TEMP_OPT_LEN) {
		return E_HM_INVAL;
	}
	int count = 0;

	if (option->short_name != 0) {
		short_opt[count++] = option->short_name;
		if (option->long_option.has_arg == required_argument) {
			short_opt[count++] = SHORT_OPT_REQUIRE_ARG;
		} else if (option->long_option.has_arg == optional_argument) {
			short_opt[count++] = SHORT_OPT_REQUIRE_ARG;
			short_opt[count++] = SHORT_OPT_REQUIRE_ARG;
		}
	}
	short_opt[count] = '\0';

	return E_HM_OK;
}

static int gen_short_options(struct option_parser *parser, size_t len)
{
	int err;
	/* each options could take up at most 3 chracters in shor_option string.
	 * then, plus one prefix character and one tailing zero. */
	size_t max_len = len * OPTPARSE_SHORT_OPT_MAX_LEN + OPTPARSE_SHORT_OPTS_PREFIX_LEN
			 + OPTPARSE_TAIL_ZERO_LEN;
	size_t temp_opt_len = OPTPARSE_TEMP_OPT_LEN;
	char temp_opt[OPTPARSE_TEMP_OPT_LEN] = {0};
	char *short_opts = NULL;
	struct perf_option* options = parser->options;

	short_opts = (char *)malloc(sizeof(char) * max_len);
	if (short_opts == NULL) {
		return E_HM_NOMEM;
	}
	err = memset_s(short_opts, max_len, 0, max_len);
	if (err != 0) {
		free(short_opts);
		return E_HM_RESFAULT;
	}
	err = strcat_s(short_opts, max_len, SHORT_OPTIONS_PREFIX);
	if (err != 0) {
		free(short_opts);
		return E_HM_RESFAULT;
	}

	for (; options->type != OPTPARSE_END; options++) {
		if (options->type == OPTPARSE_GROUP_OPTION) {
			continue;
		}
		if ((err = gen_short_opt_from_name(options, temp_opt, temp_opt_len)) == E_HM_OK) {
			/* no short name for current option */
			if (temp_opt[0] == 0)
				continue;
			err = strcat_s(short_opts, max_len, temp_opt);
			if (err != 0) {
				free(short_opts);
				return E_HM_RESFAULT;
			}
		} else {
			free(short_opts);
			return err;
		}
	}

	parser->short_options = short_opts;
	return E_HM_OK;
}

/* generate struct option which used in getopt */
static int gen_long_opts(struct option_parser *parser, size_t len)
{
	size_t count = 0;
	struct perf_option* options = parser->options;
	struct option* long_opts = NULL;

	long_opts = (struct option*) malloc(sizeof(struct option) * len);
	if (long_opts == NULL) {
		return E_HM_NOMEM;
	}

	for (; options->type != OPTPARSE_END; options++) {
		if (options->type == OPTPARSE_GROUP_OPTION) {
			/* do not copy group option */
			continue;
		} else {
			long_opts[count].name = options->long_option.name;
			long_opts[count].has_arg = options->long_option.has_arg;
			long_opts[count].flag = options->long_option.flag;
			long_opts[count].val = options->long_option.val;
			++count;
		}
	}

	/* copy option end */
	long_opts[count].name = NULL;
	long_opts[count].has_arg = no_argument;
	long_opts[count].flag = NULL;
	long_opts[count].val = 0;

	parser->getopt_options = long_opts;

	return 0;
}

static void print_help_msgs(const char* help_msgs, size_t pos, size_t opts_width)
{
	char *help_dup;
	char *save_str;
	char *one_msg;

	size_t pad;
	if (pos <= opts_width) {
		pad = opts_width - pos;
	} else {
		(void)printf("\n");
		pad = opts_width;
	}

	help_dup = strdup(help_msgs);
	if (help_dup == NULL) {
		hm_error("copy help messages failed\n");
		return;
	}

	one_msg = strtok_r(help_dup, OPTPARSE_NEWLINE_SEP, &save_str);

	while (one_msg != NULL) {
		(void)printf("%*s%s\n", (int)pad + OPTPARSE_SPACE_PAD, "", one_msg);
		one_msg = strtok_r(NULL, OPTPARSE_NEWLINE_SEP, &save_str);
		pad = opts_width;
	}

	free(help_dup);
}

static void print_usage(const struct option_parser *parser, size_t usage_opts_width)
{
	const struct perf_option *options;

	options = parser->options;
	for (; options->type != OPTPARSE_END; options++) {
		size_t pos = 0;
		if (options->type == OPTPARSE_GROUP_OPTION) {
			(void)printf("\n");
			(void)printf("    %s", options->help_msg);
			(void)printf("\n");
			continue;
		}

		if (options->type == OPTPARSE_OPTIONAL_OPTION) {
			if (options->check_func == NULL) {
				continue;
			}
			if (!options->check_func()) {
				continue;
			}
		}

		(void)printf("    ");
		pos = OPTPARSE_SHORT_SPACE_PRE;
		if (options->short_name != '\0') {
			(void)printf("-%c", options->short_name);
			pos += OPTPARSE_LEN_SHORT_NAME;
		}
		if (options->long_option.name && options->short_name != '\0') {
			(void)printf(", ");
			pos += OPTPARSE_LEN_SEP;
		}
		if (options->long_option.name) {
			if (options->short_name != '\0') {
				(void)printf("--%s", options->long_option.name);
				pos += OPTPARSE_LONG_NAME_COST + strlen(options->long_option.name);
			} else {
				(void)printf("    --%s", options->long_option.name);
				pos += (OPTPARSE_LONG_SPACE_PRE - OPTPARSE_SHORT_SPACE_PRE) +
					OPTPARSE_LONG_NAME_COST + strlen(options->long_option.name);
			}
		}
		if (options->input_desc) {
			(void)printf("=<%s>", options->input_desc);
			pos += OPTPARSE_DESC_COST + strlen(options->input_desc);
		}
		if (options->help_msg)
			print_help_msgs(options->help_msg, pos, usage_opts_width);
	}
}

static size_t get_usage_width(const struct option_parser *parser)
{
	const struct perf_option *options;
	size_t usage_opts_width = 0;
	size_t prefix;
	size_t len;

	options = parser->options;
	/* figure out the longest width */
	for (; options->type != OPTPARSE_END; options++) {
		len = 0;
		prefix = OPTPARSE_LONG_SPACE_PRE;
		if (options->short_name != '\0') {
			len += OPTPARSE_LEN_SHORT_NAME;
			prefix = prefix - OPTPARSE_LEN_SHORT_NAME;
		}
		if (options->short_name != '\0' && options->long_option.name) {
			/* length for separator ", " */
			len += OPTPARSE_LEN_SEP;
			prefix = prefix - OPTPARSE_LEN_SEP;
		}
		if (options->long_option.name) {
			len += strlen(options->long_option.name) + OPTPARSE_LONG_NAME_COST;
		}
		if (options->input_desc) {
			/* length for '=<input_desc>' */
			len += strlen(options->input_desc) + OPTPARSE_DESC_COST;
		}
		len += prefix;
		if (usage_opts_width < len)
			usage_opts_width = len;
	}

	return usage_opts_width;
}

int parser_init(struct option_parser *parser, const char *const *usages, const char *description,
		const char *epilog, struct perf_option* options)
{
	if (parser == NULL || options == NULL) {
		return E_HM_INVAL;
	}

	int err;
	size_t no_group_size;

	parser->usages = usages;
	parser->description = description;
	parser->epilog = epilog;
	parser->options = options;
	compelete_simple_opts(parser);

	no_group_size = opt_size_exclude_group(parser);
	err = gen_short_options(parser, no_group_size);
	if (err != 0) {
		return err;
	}

	err = gen_long_opts(parser, no_group_size);
	if (err != 0) {
		return err;
	}

	return 0;
}

void parser_free(struct option_parser *parser)
{
	if (parser->getopt_options != NULL) {
		free(parser->getopt_options);
		parser->getopt_options = NULL;
	}
	if (parser->short_options != NULL) {
		free(parser->short_options);
		parser->short_options = NULL;
	}
}

void option_usage(const struct option_parser *parser)
{
	if (parser == NULL) {
		return;
	}

	const char *const *usages = parser->usages;
	if (usages != NULL) {
		(void)printf("Usage: %s\n", *usages++);
		while (*usages != NULL && **usages != '\0')
			(void)printf("   or: %s\n", *usages++);
	} else {
		(void)printf("Usage:\n");
	}

	if (parser->description)
		(void)printf("%s\n", parser->description);

	(void)printf("\n");

	size_t usage_opts_width = get_usage_width(parser);

	print_usage(parser, usage_opts_width);

	if (parser->epilog)
		(void)printf("\n%s", parser->epilog);

	(void)printf("\n");
}
