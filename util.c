#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>
#include <ctype.h>
#include <unistd.h>
#include <libgen.h>

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

int r_rmdir(const char *prefix_path, char *rm_path)
{
	int r;
	char abs_path[PATH_MAX];

	if (!strcmp(rm_path, "."))
		return 0;

	r = snprintf(abs_path, sizeof(abs_path), "%s/%s", prefix_path, rm_path);
	if (r > (int) sizeof(abs_path))
		err_die(1, "path exceeded PATH_MAX");

	r = rmdir(abs_path);
	if (r && errno != EEXIST && errno != ENOTEMPTY)
		err_die(1, "%s", strerror(errno));

	return r_rmdir(prefix_path, dirname(rm_path));
}

void util_strtrim(char *s)
{
	char *rend;

	for (rend = s; *s; ++s)
		if (!isspace(*s))
			rend = s;

	rend[1] = '\0';
}
