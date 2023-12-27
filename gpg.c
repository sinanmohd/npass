#include <stdio.h>
#include <locale.h>

#include <gpgme.h>

#include "gpg.h"

#define fail_if_err(err) \
	if (err) { \
		fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, \
		      gpgme_strsource(err), gpgme_strerror(err)); \
		gpg_cleanup(); \
		return 1; \
	}

static gpgme_ctx_t ctx = NULL;
static gpgme_key_t key = NULL;

int gpg_init(void);
void gpg_cleanup(void);

int gpg_init(void)
{
	gpgme_error_t err;
	const char *local = setlocale(LC_ALL, "");
	
	gpgme_check_version(NULL);
	gpgme_set_locale(NULL, LC_CTYPE, local);
#ifdef LC_MESSAGES
	gpgme_set_locale(NULL, LC_MESSAGES, local);
#endif

	err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
	fail_if_err(err);
	err = gpgme_new(&ctx);
	fail_if_err(err);

	return 0;
}

void gpg_cleanup(void)
{
	if (ctx)
		gpgme_key_release(key);
	if (key)
		gpgme_release(ctx);
}

int gpg_key_validate(const char *fpr)
{
	int r;
	gpgme_error_t err;

	r = gpg_init();
	if (r)
		return r;

	err = gpgme_get_key(ctx, fpr, &key, 1);
	fail_if_err(err);

	gpg_cleanup();

	return 0;
}
