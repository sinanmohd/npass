#include <stdio.h>
#include <sys/types.h>

int gpg_key_validate(const char *fpr);
int gpg_decrypt(FILE *pass_out, const char *pass_path);
int gpg_encrypt(FILE *stream, const char *fpr, const char *pass, size_t n);
