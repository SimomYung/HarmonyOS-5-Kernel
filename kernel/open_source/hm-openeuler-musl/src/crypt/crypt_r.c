#include <crypt.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

char *__crypt_r(const char *key, const char *salt, struct crypt_data *data)
{
	/* Per the crypt_r API, the caller has provided a pointer to
	 * struct crypt_data; however, this implementation does not
	 * use the structure to store any internal state, and treats
	 * it purely as a char buffer for storing the result. */
	char *output = (char *)data;
	const char *s = salt;
	if (salt[0] == '$' && salt[1] && salt[2]) {
		if (salt[1] == '1' && salt[2] == '$')
			return __crypt_md5(key, salt, output);
		if (salt[1] == '2' && salt[3] == '$')
			return __crypt_blowfish(key, salt, output);
		if (salt[1] == '5' && salt[2] == '$')
			return __crypt_sha256(key, salt, output);
		if (salt[1] == '6' && salt[2] == '$')
			return __crypt_sha512(key, salt, output);
	}
	size_t slen = strlen(salt);
	if (slen < 2) {
		errno = EINVAL;
		return NULL;
	}
	/* The first two digits of the salt value in the DES encryption
	algorithm can only be alphanumeric or '.' '/' */
	for (int i = 0; i < 2; i++) {
		if (isalnum(*s) || *s == '.' || *s == '/') {
			s++;
		} else {
			errno = EINVAL;
			return NULL;
		}
	}
	return __crypt_des(key, salt, output);
}

weak_alias(__crypt_r, crypt_r);
