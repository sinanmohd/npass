#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <ctype.h>

#include "util.h"

int r_mkdir(const char *path, mode_t mode)
{
	int r;
	size_t len;
	char *p;
	char tmp[NAME_MAX + 1];

	strncpy(tmp, path, sizeof(tmp) - 1);
	len = strlen(tmp);
	if(tmp[len - 1] == '/')
		tmp[len - 1] = '\0';

	for (p = tmp + 1; *p; ++p) {
		if (*p == '/') {
			*p = '\0';

			r = mkdir(tmp, mode);
			if (r && !(errno & EEXIST))
				return r;

			*p = '/';
		}
	}

	return mkdir(path, mode);
}

void util_strtrim(char *s)
{
	char *rend;

	for (rend = s; *s; ++s)
		if (!isspace(*s))
			rend = s;

	rend[1] = '\0';
}
