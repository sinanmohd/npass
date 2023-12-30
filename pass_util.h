#include <stdio.h>

#define PASS_MAX 4096

int pass_init(const char *fpr);
const char *pass_cat(const char *path);
int pass_add(const char *path, const char *pass, size_t n);
ssize_t pass_getpass(char **lineptr, size_t *n, FILE *stream);
