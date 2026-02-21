#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

struct path {
	size_t p, q, l, l0, cnt, nup;
	int check_dir;
};

static size_t slash_len(const char *s)
{
	const char *s0 = s;
	while (*s == '/') s++;
	return s-s0;
}

char *realpath(const char *restrict filename, char *restrict resolved)
{
	char stack[PATH_MAX+1];
	char output[PATH_MAX];
	size_t q_resolved = 0;
	int path_not_exist = 0;
	struct path path;
	path.cnt = 0;
	path.nup = 0;
	path.check_dir = 0;

	if (!filename) {
		errno = EINVAL;
		return 0;
	}
	path.l = strnlen(filename, sizeof stack);
	if (!path.l) {
		errno = ENOENT;
		return 0;
	}
	if (path.l >= PATH_MAX) goto toolong;
	path.p = sizeof stack - path.l - 1;
	path.q = 0;
	memcpy(stack+path.p, filename, path.l+1);

	/* Main loop. Each iteration pops the next part from stack of
	 * remaining path components and consumes any slashes that follow.
	 * If not a link, it's moved to output; if a link, contents are
	 * pushed to the stack. */
restart:
	for (; ; path.p+=slash_len(stack+path.p)) {
		/* If stack starts with /, the whole component is / or //
		 * and the output state must be reset. */
		if (stack[path.p] == '/') {
			path.check_dir=0;
			path.nup=0;
			path.q=0;
			output[path.q++] = '/';
			path.p++;
			/* Initial // is special. */
			if (stack[path.p] == '/' && stack[path.p+1] != '/')
				output[path.q++] = '/';
			continue;
		}

		char *z = __strchrnul(stack+path.p, '/');
		path.l0 = path.l = z-(stack+path.p);

		if (!path.l && !path.check_dir) break;

		/* Skip any . component but preserve check_dir status. */
		if (path.l==1 && stack[path.p]=='.') {
			path.p += path.l;
			continue;
		}

		/* Copy next component onto output at least temporarily, to
		 * call readlink, but wait to advance output position until
		 * determining it's not a link. */
		if (path.q && output[path.q-1] != '/') {
			if (!path.p) goto toolong;
			stack[--path.p] = '/';
			path.l++;
		}
		if (path.q+path.l >= PATH_MAX) goto toolong;
		memcpy(output+path.q, stack+path.p, path.l);
		output[path.q+path.l] = 0;
		path.p += path.l;

		int up = 0;
		if (path.l0==2 && stack[path.p-2]=='.' && stack[path.p-1]=='.') {
			up = 1;
			/* Any non-.. path components we could cancel start
			 * after nup repetitions of the 3-byte string "../";
			 * if there are none, accumulate .. components to
			 * later apply to cwd, if needed. */
			if (path.q <= 3*path.nup) {
				path.nup++;
				path.q += path.l;
				continue;
			}
			/* When previous components are already known to be
			 * directories, processing .. can skip readlink. */
			if (!path.check_dir) goto skip_readlink;
		}
		ssize_t k = readlink(output, stack, path.p);
		if (k==path.p) goto toolong;
		if (!k) {
			errno = ENOENT;
			return 0;
		}
		if (k<0) {
			if (errno != EINVAL && errno != ENOENT) return 0;
			if (errno == ENOENT && !path_not_exist) {
				/* Record the location where the path does not
			     * exist for the first time. */
				q_resolved = path.q + path.l + 1;
				path_not_exist = 1;
			}
skip_readlink:
			path.check_dir = 0;
			if (up) {
				while(path.q && output[path.q-1]!='/') path.q--;
				if (path.q>1 && (path.q>2 || output[0]!='/')) path.q--;
				continue;
			}
			if (path.l0) path.q += path.l;
			path.check_dir = stack[path.p];
			continue;
		}
		if (++path.cnt == SYMLOOP_MAX) {
			errno = ELOOP;
			return 0;
		}

		/* If link contents end in /, strip any slashes already on
		 * stack to avoid /->// or //->/// or spurious toolong. */
		if (stack[k-1]=='/') while (stack[path.p]=='/') path.p++;
		path.p -= k;
		memmove(stack+path.p, stack, k);

		/* Skip the stack advancement in case we have a new
		 * absolute base path. */
		goto restart;
	}

 	output[path.q] = 0;

	if (output[0] != '/') {
		if (!getcwd(stack, sizeof stack)) return 0;
		path.l = strlen(stack);
		/* Cancel any initial .. components. */
		path.p = 0;
		while (path.nup--) {
			while(path.l>1 && stack[path.l-1]!='/') path.l--;
			if (path.l>1) path.l--;
			path.p += 2;
			if (path.p<path.q) path.p++;
		}
		if (path.q-path.p && stack[path.l-1]!='/') stack[path.l++] = '/';
		if (path.l + (path.q-path.p) + 1 >= PATH_MAX) goto toolong;
		memmove(output + path.l, output + path.p, path.q - path.p + 1);
		memcpy(output, stack, path.l);
		path.q = path.l + path.q-path.p;
		q_resolved = path.l + q_resolved - path.p;
	}

	int i;
	if (output[0] == '/' && output[1] == '/') {
		for (i = 0; i < path.q; i++) output[i] = output[i + 1];
		output[path.q] = 0;
		path.q--;
		q_resolved--;
	}

    if (path_not_exist) {
		if (resolved) for (i = 0; i < path.q + 1; i++) resolved[i] = output[i];
		else return 0;
		if (resolved[q_resolved - 1] == '/') q_resolved--;
		resolved[q_resolved] = 0;
	    return 0;
	}

	if (resolved) return memcpy(resolved, output, path.q+1);
	else return strdup(output);

toolong:
	errno = ENAMETOOLONG;
	return 0;
}
