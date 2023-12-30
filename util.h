#ifndef UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define err_die(r, ...) {\
	fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
	return r; \
}

int r_mkdir(const char *path, mode_t mode);
int r_rmdir(const char *prefix_path, char *rm_path);
void util_strtrim(char *s);

#define UTIL_H
#endif
