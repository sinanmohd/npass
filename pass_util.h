#include <stdio.h>

int pass_init(const char *fpr);
int pass_cat(FILE *out, const char *path);
int pass_add(const char *path, const char *pass, size_t n);
ssize_t pass_getpass(char **lineptr, size_t *n, FILE *stream);
