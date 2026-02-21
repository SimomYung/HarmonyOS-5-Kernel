#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getenv(const char *name)
{
	char **env = NULL;

	if (name == NULL || name[0]  == '\0' || __environ == NULL)
		return NULL;

	if (name[1] == '\0') {
		/* fast path */
		for (env = __environ; *env != NULL; ++env) {
			if (name[0] == (*env)[0] && (*env)[1] == '=')
				return &(*env)[2];
		}
	} else {
		/* slow path */
		size_t len = strlen(name) - 2;
		for (env = __environ; *env != NULL; ++env) {
			if (name[0] == (*env)[0] && name[1] == (*env)[1]
			    && !strncmp(*env + 2, name + 2, len)
			    && (*env)[len + 2] == '=')
				return &(*env)[len + 3];
		}
	}
	return 0;
}
