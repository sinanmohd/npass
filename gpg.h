#include <stdio.h>
#include <sys/types.h>

int gpg_key_validate(const char *fpr);
int gpg_decrypt(const char *path, char *pass_out, size_t n);
int gpg_encrypt(FILE *stream, const char *fpr, const char *pass, size_t n);
