#include <sys/types.h>

int gpg_key_validate(const char *fpr);
int gpg_decrypt(const char *fpr, const char *path, char *pass_out, size_t n);
