#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "locale_impl.h"
#include "libc.h"
#include "lock.h"
#include "fork_impl.h"

const char *__lctrans_impl(const char *msg, const struct __locale_map *lm)
{
	const char *trans = 0;
	if (lm) trans = __mo_lookup(lm->map, lm->map_size, msg);
	return trans ? trans : msg;
}

static const char envvars[][12] = {
	"LC_CTYPE",
	"LC_NUMERIC",
	"LC_TIME",
	"LC_COLLATE",
	"LC_MONETARY",
	"LC_MESSAGES",
};

volatile int __locale_lock[1];
volatile int *const __locale_lockptr = __locale_lock;

const struct __locale_map *__get_locale(int cat, const char *val)
{
	static void *volatile loc_head;
	const struct __locale_map *p;
	struct __locale_map *new = 0;
	const char *path = 0, *z;
	char buf[256];
	size_t l, n;

	if (!*val) {
		(val = getenv("LC_ALL")) && *val ||
		(val = getenv(envvars[cat])) && *val ||
		(val = getenv("LANG")) && *val ||
		(val = "C.UTF-8");
	}

	/* Limit name length and forbid leading dot or any slashes. */
	for (n=0; n<LOCALE_NAME_MAX && val[n] && val[n]!='/'; n++);
	if (val[0]=='.' || val[n]) val = "C.UTF-8";
	int builtin = (val[0]=='C' && !val[1])
		|| !strcmp(val, "C.UTF-8")
		|| !strcmp(val, "POSIX");

	if (builtin) {
		if (val[1]=='.')
			return (void *)&__c_dot_utf8;
		return 0;
	}

	for (p=loc_head; p; p=p->next)
		if (!strcmp(val, p->name)) return p;

	char *codeset = __strchrnul(val, '.');
	/* We only support UTF-8 now and return failed otherwise */
	if (*codeset && strcmp(codeset + 1, "UTF-8")) {
		return LOC_MAP_FAILED;
	}
	int name_len = codeset - val;

	if (!libc.secure) path = getenv("MUSL_LOCPATH");
	/* FIXME: add a default path? */

	if (path) for (; *path; path=z+!!*z) {
		z = __strchrnul(path, ':');
		l = z - path;
		if (l >= sizeof buf - name_len - 2) continue;
		memcpy(buf, path, l);
		buf[l] = '/';
		memcpy(buf+l+1, val, name_len);
		buf[l+1+name_len] = 0;
		size_t map_size;
		const void *map = __map_file(buf, &map_size);
		if (map) {
			new = malloc(sizeof *new);
			if (!new) {
				__munmap((void *)map, map_size);
				break;
			}
			new->map = map;
			new->map_size = map_size;
			memcpy(new->name, val, n);
			new->name[n] = 0;
			new->next = loc_head;
			loc_head = new;
			break;
		}
		/* Check if the locale existing in the path specified by MUSL_LOCPATH and
		 * make a locale map object if so.*/
		else if (!access(buf, F_OK)) {
			new = malloc(sizeof *new);
			if (!new) {
				break;
			}
			new->map = __c_dot_utf8.map;
			new->map_size = __c_dot_utf8.map_size;
			memcpy(new->name, val, n);
			new->name[n] = 0;
			new->next = loc_head;
			loc_head = new;
			break;
		}
	}

	return new ? new : LOC_MAP_FAILED;
}
