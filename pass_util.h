#include <stdio.h>

#define PASS_MAX 4096
#define FPR_MAX 128

int pass_init(const char *fpr);
const char *pass_cat(const char *path);
size_t pass_getpass(char **lineptr, size_t *n, FILE *stream);
