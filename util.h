#ifndef UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define err_die(...) {\
	fprintf(stderr, "err: %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
	return 1; \
}

int r_mkdir(const char *path, mode_t mode);

#define UTIL_H
#endif
