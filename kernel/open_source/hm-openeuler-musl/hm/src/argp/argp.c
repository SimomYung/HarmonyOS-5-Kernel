/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of argp
 * Author: Huawei RTOS
 * Create: Wed Oct 27 09:55:49 2021
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <argp.h>
const char *argp_program_version = "1.0";
typedef enum {
	MLINE_EVT, // mid line '-'
	EQUL_EVT,
	NARG_EVT, // next arg char
	EARG_EVT, // end of arg char
	NARGS_EVT // next args MAX_EVT
}evt_type_t;

typedef enum {
	INIT_STATE,
	SOPTION_STATE,
	SPARAM_STATE,
	LOPTION_STATE,
	LPARAM_STATE,
	MAX_STATE
}state_type_t;

typedef struct {
	evt_type_t type;
	void *data;
}evt_t;

#define LEN 64
#define MAX_ARG_LEN ((LEN) - 2)
#define MAX_LKEY_LEN ((LEN) - 2)
typedef struct {
	state_type_t cur_state;
	struct argp_state state;
	int *end_index;
	const struct argp **argps;
	int num_argps;
	const struct argp *key_argp;
	int arg_idx;
	/* init data */
	int skey;
	char skeys[LEN];
	char lkey[LEN];
	char arg[LEN];
	int err;
}argp_sm_t;

typedef enum {
	NO_PARAM,
	NEED_PARAM,
	INVAL_KEY
}key_type_t;


#ifdef DEBUG
#define debug(format, ...) \
	do { \
	    (void)fprintf(stderr, "[%s] " format "\n", \
	        __func__, ##__VA_ARGS__ ); \
	} while(0)
#else
#define debug(format, ...)
#endif

static const struct argp_option argp_def_options[] = {
  { "help", 'h', 0, 0, "show help info", -1 },
  { "version",'v', 0, 0, "show version info", -1 },
  { 0, 0, 0, 0, 0, 0 }
};

static void argp_show_options(const struct argp *argp)
{
	int i;
	const struct argp_option *option = NULL;
	const char *name = NULL;
	int key = 0;
	const char *doc = NULL;

	debug("argp:%p\n", argp);
	for (i = 0;;i++) {
		option = &argp->options[i];
		name = option->name;
		key = option->key;
		doc = option->doc;
		debug("name:%p key:%d doc:%p\n", name, key, doc);
		if (!name && key == 0 && !doc) {
			break;
		}

		if (name) {
			(void)printf("--%s\t", name);
		}

		if (isalpha(key) != 0) {
			(void)printf("-%c\t", (char)key);
		}

		if (doc) {
			(void)printf("%s\n", doc);
		}
	}
}

static void argp_def_help(struct argp_state *state)
{
	int i;

	if (state->root_argp->doc) {
		(void)printf("%s\n", state->root_argp->doc);
	}

	struct argp **argps = (struct argp**)state->pstate;
	for (i =0; argps[i] != NULL; i++) {
		argp_show_options(argps[i]);
	}
}

static int argp_def_parser(int key, __attribute__((unused)) char *arg,
		struct argp_state *state)
{
	switch (key) {
	case 'h':
		debug("info:h");
		argp_def_help (state);
		exit(0);
	case 'v':
		(void)printf("version: %s\n", argp_program_version);
		exit(0);
	default:
		break;
	}

	return 0;
}

static const struct argp def_argp = {
	argp_def_options, argp_def_parser, NULL, NULL, NULL, NULL, "libc"
};

static void evt_ctor(evt_t *evt, evt_type_t type, void *data)
{
	evt->type = type;
	evt->data = data;
	debug("evt->data:%ld\n", (long)evt->data);
}

static void argp_sm_tran(argp_sm_t *argp_sm, state_type_t state)
{
	debug("state tran from %u to %u\n", argp_sm->cur_state, state);
	argp_sm->cur_state = state;
}

static void argp_sm_init_data(argp_sm_t *argp_sm)
{
	argp_sm->key_argp = NULL;
	argp_sm->skey = 0;
	(void)memset(argp_sm->skeys, 0, sizeof(argp_sm->skeys));
	(void)memset(argp_sm->lkey, 0, sizeof(argp_sm->lkey));
	(void)memset(argp_sm->arg, 0, sizeof(argp_sm->arg));
}

static void argp_sm_ctor(argp_sm_t *argp_sm, const struct argp *argp)
{
	const struct argp_child *p = NULL;
	int i = 0;

	argp_sm_init_data(argp_sm);

	size_t argp_size = 0;
	for (p = argp->children; p && p->argp; p++) {
		++argp_size;
	}
	argp_sm->argps = calloc(argp_size + 1, sizeof(const struct argp *));
	if (argp_sm->argps == NULL) {
		exit(-1);
	}
	argp_sm->argps[i++] = argp;
	debug("argp:%p, argp->children:%p\n", argp, argp->children);
	for (p = argp->children; p && p->argp; p++) {
		debug("argp:%p, p:%p\n", argp, p);
		argp_sm->argps[i++] = p->argp;
	}

	argp_sm->argps[i] = &def_argp;
	argp_sm->num_argps = i;
	debug("num_argps:%d\n", argp_sm->num_argps);
	argp_sm->state.root_argp = argp;
	argp_sm->state.hook = NULL;
	argp_sm->state.pstate = argp_sm->argps;
	argp_sm->state.child_inputs = malloc((unsigned int)(argp_sm->num_argps * sizeof(void*)));
	if (!argp_sm->state.child_inputs) {
		free(argp_sm->argps);
		exit(-1);
	}

	(void)memset(argp_sm->state.child_inputs, 0, (unsigned int)(argp_sm->num_argps * sizeof(void*)));
}

static void argp_sm_init(argp_sm_t *argp_sm, const evt_t *evt)
{
	switch (evt->type) {
	case MLINE_EVT:
		argp_sm_tran(argp_sm, SOPTION_STATE);
		break;
	default:
		argp_sm_init_data(argp_sm);
		break;
	}
}

static key_type_t argp_parse_skey(argp_sm_t *argp_sm, long skey, const struct argp *argp)
{
	unsigned int j = 0;
	for (j = 0; ; j++) {
		if (argp->options[j].name == NULL && argp->options[j].key == 0
				&& argp->options[j].doc == NULL) {
			break;
		}
		debug("key:%d,skey:%ld", argp->options[j].key, skey);
		if (argp->options[j].key == skey) {
			argp_sm->key_argp = argp;
			argp_sm->skey = argp->options[j].key;
			if (argp->options[j].name) {
				(void)strncpy(argp_sm->lkey, argp->options[j].name, sizeof(argp_sm->lkey) - 1);
			}
			if (argp->options[j].arg != NULL) {
				return NEED_PARAM;
			}

			return NO_PARAM;
		}
	}

	return INVAL_KEY;
}

static key_type_t argp_sm_parse_skey(argp_sm_t *argp_sm, long skey)
{
	int i;

	struct argp **argps = (struct argp**)argp_sm->state.pstate;
	debug("sm argps:%p pstate%p\n, argps:%p", argp_sm->argps, argp_sm->state.pstate, argps);
	for (i =0; i < argp_sm->num_argps; i++) {
		key_type_t ret = argp_parse_skey(argp_sm, skey, argps[i]);
		if (ret != INVAL_KEY) {
			return ret;
		}
	}

	return INVAL_KEY;
}

static key_type_t argp_parse_lkey(argp_sm_t *argp_sm, const char *lkey, const struct argp *argp)
{
	const char *name = NULL;
	unsigned int j = 0;
	for (j = 0; ; j++) {
		if (argp->options[j].name == NULL && argp->options[j].key == 0
				&& argp->options[j].doc == NULL) {
			break;
		}

		debug("name:%s, lkey:%s\n", argp->options[j].name, lkey);
		name = argp->options[j].name;
		if (name && strcmp(name,lkey) == 0) {
			argp_sm->key_argp = argp;
			argp_sm->skey = argp->options[j].key;
			debug("skey:%d key:%d\n", argp_sm->skey, argp->options[j].key);
			(void)strncpy(argp_sm->lkey, argp->options[j].name, sizeof(argp_sm->lkey) - 1);
			if (argp->options[j].arg != NULL) {
				return NEED_PARAM;
			}

			return NO_PARAM;
		}
	}

	return INVAL_KEY;
}

static key_type_t argp_sm_parse_lkey(argp_sm_t *argp_sm, const char *lkey)
{
	int i;

	struct argp **argps = (struct argp**)argp_sm->state.pstate;
	for (i =0; i < argp_sm->num_argps; i++) {
		key_type_t ret = argp_parse_lkey(argp_sm, lkey, argps[i]);
		if (ret != INVAL_KEY) {
			return ret;
		}
	}

	return INVAL_KEY;
}

static void argp_sm_parser_start(argp_sm_t *argp_sm)
{
	int i;

	const struct argp *argp = NULL;
	struct argp **argps = (struct argp**)argp_sm->state.pstate;
	for (i =0; i < argp_sm->num_argps; i++) {
		argp = argps[i];
		if (argp->parser == NULL) {
			break;
		}
		debug("state hook:%p\n", argp_sm->state.hook);
		argp->parser(ARGP_KEY_INIT, NULL, &argp_sm->state);
	}
}

static void argp_sm_parser_end(argp_sm_t *argp_sm)
{
	int i;

	const struct argp *argp = NULL;
	struct argp **argps = (struct argp**)argp_sm->state.pstate;
	for (i =0; i < argp_sm->num_argps; i++) {
		argp = argps[i];
		if (argp->parser == NULL) {
			break;
		}

		debug("ARGP_KEY_END");
		argp->parser(ARGP_KEY_END, NULL, &argp_sm->state);
		debug("ARGP_KEY_SUCCESS");
		argp->parser(ARGP_KEY_SUCCESS, NULL, &argp_sm->state);
	}
}
static void argp_sm_parse_noopt(argp_sm_t *argp_sm)
{
	const struct argp *key_argp = argp_sm->state.root_argp;
	debug("key_arpg:%p, arpg_sm:%p", key_argp, argp_sm);
	if (key_argp->parser) {
		argp_sm_parser_start(argp_sm);
		key_argp->parser(ARGP_KEY_NO_ARGS, NULL, &argp_sm->state);
		argp_sm_parser_end(argp_sm);
	}
}

static void argp_sm_parse_opt(argp_sm_t *argp_sm, int key, char *arg, struct argp_state *argp_state)
{
	const struct argp *key_argp = argp_sm->key_argp;
	debug("root_argp:%p, key_argp:%p, skey:%d, key:%d, arg:%s\n",
			argp_sm->state.root_argp, key_argp, argp_sm->skey, key, arg);

	if (key_argp->parser == NULL) {
		return;
	}

	argp_sm_parser_start(argp_sm);
	key_argp->parser(ARGP_KEY_ARG, arg, argp_state);
	key_argp->parser(key, arg, argp_state);
	argp_sm_parser_end(argp_sm);
}

static void argp_sm_soption(argp_sm_t *argp_sm, const evt_t *evt)
{
	char arg[2] = {0};
	key_type_t key_type;

	switch (evt->type) {
	case NARG_EVT:
		arg[0] = (char)(long)evt->data;
		debug("argp_sm->arg:%s, arg:%c\n", argp_sm->arg, arg[0]);
		if (strlen(argp_sm->skeys) < 1) {
			(void)strncat(argp_sm->skeys, arg, 1);
		} else {
			(void)printf("skey more than 1 %s\n", argp_sm->skeys);
			argp_sm->err = 1;
			argp_sm_tran(argp_sm, INIT_STATE);
		}

		break;
	case EARG_EVT:
		argp_sm->state.next = argp_sm->arg_idx + 1;
		debug("argp_sm skeys:%s\n", argp_sm->skeys);
		if (strlen(argp_sm->skeys) != 1) {
			(void)printf("Error short key %s\n", argp_sm->skeys);
			argp_sm->err = 1;
			argp_sm_tran(argp_sm, INIT_STATE);
			break;
		}

		argp_sm->skey = argp_sm->skeys[0];
		key_type = argp_sm_parse_skey(argp_sm, argp_sm->skey);
		if (key_type == NO_PARAM) {
			argp_sm_parse_opt(argp_sm, argp_sm->skey, NULL,
					&argp_sm->state);
			argp_sm_tran(argp_sm, INIT_STATE);
		} else if (key_type == NEED_PARAM) {
			argp_sm_tran(argp_sm, SPARAM_STATE);
		} else {
			argp_sm->err = 1;
			(void)printf("Error short key %s\n", argp_sm->skeys);
			argp_sm_tran(argp_sm, INIT_STATE);
		}

		break;
	case MLINE_EVT:
		argp_sm_tran(argp_sm, LOPTION_STATE);
		break;
	default:
		debug("unknow evt:%u\n", evt->type);
		break;
	}
}

static void argp_sm_param_earg_evt(argp_sm_t *argp_sm)
{
		argp_sm->state.next = argp_sm->arg_idx + 1;
		argp_sm_parse_opt(argp_sm, argp_sm->skey, argp_sm->arg,
				&argp_sm->state);
		argp_sm_tran(argp_sm, INIT_STATE);
}

static void argp_sm_sparam(argp_sm_t *argp_sm, const evt_t *evt)
{
	char arg[2] = {0};

	debug("sparam state\n");
	switch (evt->type) {
	case NARG_EVT:
		arg[0] = (char)(long)evt->data;
		debug("argp_sm->arg:%s, arg:%c\n", argp_sm->arg, arg[0]);
		if (strlen(argp_sm->arg) < MAX_ARG_LEN) {
			(void)strncat(argp_sm->arg, arg, 1);
		} else {
			(void)printf("sparam overflow\n");
			argp_sm->err = 1;
			argp_sm_tran(argp_sm, INIT_STATE);
		}

		break;
	case EARG_EVT:
		argp_sm_param_earg_evt(argp_sm);
		break;
	default:
		debug("sparam state unkown evt:%u\n", evt->type);
		break;
	}
}

static void argp_sm_loption_narg_evt(argp_sm_t *argp_sm, const evt_t *evt)
{
	char arg[2] = {0};
	arg[0] = (char)(long)evt->data;
	debug("argp_sm->arg:%s, arg:%c\n", argp_sm->arg, arg[0]);
	if (strlen(argp_sm->lkey) < MAX_LKEY_LEN) {
		(void)strncat(argp_sm->lkey, arg, 1);
	} else {
		(void)printf("lkey overflow\n");
		argp_sm->err = 1;
		argp_sm_tran(argp_sm, INIT_STATE);
	}
}

static void argp_sm_loption_equl_evt(argp_sm_t *argp_sm)
{
	debug("lkey:%s skey:%d\n", argp_sm->lkey,argp_sm->skey);
	argp_sm->state.next = argp_sm->arg_idx + 1;
	key_type_t key_type = argp_sm_parse_lkey(argp_sm, argp_sm->lkey);
	if (key_type == NEED_PARAM) {
		debug("lkey:%s skey:%d\n", argp_sm->lkey,argp_sm->skey);
		argp_sm_tran(argp_sm, LPARAM_STATE);
	} else {
		(void)printf("Error long option %s\n", argp_sm->lkey);
		argp_sm->err = 1;
		argp_sm_tran(argp_sm, INIT_STATE);
	}
}

static void argp_sm_loption_earg_evt(argp_sm_t *argp_sm)
{
	debug("lkey:%s skey:%d\n", argp_sm->lkey,argp_sm->skey);
	argp_sm->state.next = argp_sm->arg_idx + 1;
	key_type_t key_type = argp_sm_parse_lkey(argp_sm, argp_sm->lkey);
	if (key_type == NO_PARAM) {
		debug("lkey:%s skey:%d\n", argp_sm->lkey,argp_sm->skey);
		argp_sm_parse_opt(argp_sm, argp_sm->skey, NULL, &argp_sm->state);
		argp_sm_tran(argp_sm, INIT_STATE);
	} else if (key_type == NEED_PARAM) {
		debug("lkey:%s skey:%d\n", argp_sm->lkey,argp_sm->skey);
		argp_sm_tran(argp_sm, LPARAM_STATE);
	} else {
		(void)printf("Err long option %s\n", argp_sm->lkey);
		argp_sm->err = 1;
		argp_sm_tran(argp_sm, INIT_STATE);
	}
}

static void argp_sm_loption(argp_sm_t *argp_sm, const evt_t *evt)
{
	debug("argp_sm->arg:%s, arg:%c\n", argp_sm->arg, arg[0]);
	switch (evt->type) {
	case NARG_EVT:
		argp_sm_loption_narg_evt(argp_sm, evt);
		break;
	case EQUL_EVT:
		argp_sm_loption_equl_evt(argp_sm);
		break;
	case EARG_EVT:
		argp_sm_loption_earg_evt(argp_sm);
		break;
	default:
		debug("sparam state unkown evt:%u\n", evt->type);
		break;
	}
}

static void argp_sm_lparam(argp_sm_t *argp_sm, const evt_t *evt)
{
	char arg[2] = {0};

	debug("evt type:%u data:%ld\n", evt->type, (long)evt->data);
	switch (evt->type) {
	case NARG_EVT:
		arg[0] = (char)(long)evt->data;
		if (strlen(argp_sm->arg) < MAX_ARG_LEN) {
			(void)strncat(argp_sm->arg, arg, 1);
		} else {
			(void)printf("lparam overflow\n");
			argp_sm->err = 1;
			argp_sm_tran(argp_sm, INIT_STATE);
		}
		debug("argp_sm->arg:%s\n", argp_sm->arg);
		break;
	case EARG_EVT:
		argp_sm_param_earg_evt(argp_sm);
		break;
	default:
		debug("sparam state unkown evt:%u\n", evt->type);
		break;
	}
}

typedef void (*argp_evt_func)(argp_sm_t *argp_sm, const evt_t *evt);
static argp_evt_func argp_state_table[(int)MAX_STATE] = {
	argp_sm_init, argp_sm_soption, argp_sm_sparam, argp_sm_loption, argp_sm_lparam
};

static void argp_sm_dispatch(argp_sm_t *argp_sm, evt_t *evt)
{
	debug("cur_state:%u evt type:%u data:%ld\n", argp_sm->cur_state, evt->type, (long)evt->data);
	argp_state_table[argp_sm->cur_state](argp_sm, evt);
}

static void do_argp_parse(argp_sm_t *argp_sm, evt_t *evt, char *arg)
{
	unsigned int j = 0;
	for (j = 0; j <= strlen(arg); j++) {
		debug("%c\n", arg[j]);
		switch (arg[j]) {
		case '-':
			if (j == 0 || (j == 1 && arg[0] == '-')) {
				evt_ctor(evt, MLINE_EVT, NULL);
				break;
			} else {
				evt_ctor(evt, NARG_EVT, (void*)(long)arg[j]);
				break;
			}
		case '\0':
			evt_ctor(evt, EARG_EVT, NULL);
			break;
		case '=':
			evt_ctor(evt, EQUL_EVT, NULL);
			break;
		default:
			evt_ctor(evt, NARG_EVT, (void*)(long)arg[j]);
			break;
		}
		debug("evt type:%u,data:%p\n", evt.type, evt.data);
		argp_sm_dispatch(argp_sm, evt);
	}
}

int argp_parse (const struct argp *__restrict argp,
			   int argc, char **__restrict argv,
			   unsigned flags, int *__restrict end_index,
			   void *__restrict input)
{
	argp_sm_t argp_sm;
	evt_t evt;
	if (argp == NULL || argv == NULL || argc < 1) {
		return -1;
	}

	argp_sm.state.argc = argc;
	argp_sm.state.argv = argv;
	argp_sm.state.next = 1;
	argp_sm.state.flags = flags;
	argp_sm.end_index = end_index;
	argp_sm.state.input = input;
	argp_sm.cur_state = INIT_STATE;
	argp_sm.num_argps = 0;
	argp_sm.arg_idx = 0;
	argp_sm.err = 0;
	argp_sm_ctor(&argp_sm, argp);

	if (argc == 1) {
		argp_sm_parse_noopt(&argp_sm);
		goto out;
	}
	for (int i = 1; i < argc; i++) {
		argp_sm.arg_idx = i;
		evt_ctor(&evt, NARGS_EVT, NULL);
		argp_sm_dispatch(&argp_sm, &evt);
		debug("%s\n", argv[i]);
		do_argp_parse(&argp_sm, &evt, argv[i]);
	}
out:
	if (end_index) {
		*end_index = argp_sm.state.next;
		debug("end_index:%d\n", *end_index);
	}
	if (argp_sm.err != 0) {
		debug("argp.err:%d\n", argp_sm.err);
		argp_def_help(&argp_sm.state);
		exit(argp_sm.err);
	}
	if (argp_sm.argps != NULL) {
		free(argp_sm.argps);
		argp_sm.argps = NULL;
	}
	if (argp_sm.state.child_inputs != NULL) {
		free(argp_sm.state.child_inputs);
		argp_sm.state.child_inputs = NULL;
	}
	return 0;
}

void argp_help (const struct argp *__restrict argp,
		       FILE *__restrict stream,
		       unsigned flags, char *__restrict name)
{
	(void)stream;

	char *argv[] = {name, "-h"};
	(void)argp_parse(argp, 2, argv, flags, NULL, NULL); // argc is 2
}

void argp_failure (const struct argp_state *__restrict state,
			  int status, int errnum,
			  const char *__restrict fmt, ...)
{
	(void)state;
	if (fmt) {
		va_list ap;
		va_start(ap, fmt);
		(void)vfprintf(stderr, fmt, ap);
		va_end(ap);
	}

	if (errnum != 0) {
		(void)fprintf(stderr, "errnum:%d\n", errnum);
	}

	if (status != 0) {
		exit(status);
	}
}

void argp_error (const struct argp_state *__restrict state,
			const char *__restrict fmt, ...)
{
	if (fmt) {
		va_list ap;
		va_start(ap, fmt);
		(void)vfprintf(stderr, fmt, ap);
		va_end(ap);
	}

	if (!state || !(state->flags & ARGP_NO_ERRS) || !(state->flags & ARGP_NO_EXIT)) {
		exit (0);
	}
}
