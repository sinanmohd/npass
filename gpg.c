#include <stdio.h>
#include <errno.h>
#include <locale.h>

#include <gpgme.h>

#include "gpg.h"
#include "util.h"

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

int gpg_decrypt(const char *fpr, const char *path, char *pass_out, size_t n)
{
	int r;
	gpgme_data_t in, out;
	gpgme_error_t err;

	r = gpg_init();
	if (r)
		return r;

	err = gpgme_get_key(ctx, fpr, &key, 1);
	fail_if_err(err);

	err = gpgme_data_new_from_file(&in, path, 1);
	fail_if_err(err);
	err = gpgme_data_new(&out);
	fail_if_err(err);
	err = gpgme_op_decrypt(ctx, in, out);
	fail_if_err(err);

	r = gpgme_data_seek(out, 0, SEEK_SET);
	if (r)
		fail_if_err (gpgme_err_code_from_errno(errno));
	r = gpgme_data_read(out, pass_out, n);
	gpg_cleanup();
	if (r < 0)
		fail_if_err(gpgme_err_code_from_errno(errno));
	// if (r) // TODO: upstream: did not return 0 despite eob
	// 	err_die(r, "did not reach end of object");

	return 0;
}

int gpg_encrypt(FILE *stream, const char *fpr, const char *pass, size_t n)
{
	int r;
	char buf[BUFSIZ];
	gpgme_data_t in, out;
	gpgme_error_t err;

	r = gpg_init();
	if (r)
		return r;

	err = gpgme_get_key(ctx, fpr, &key, 1);
	fail_if_err(err);

	err = gpgme_data_new_from_mem(&in, pass, n, 0);
	fail_if_err(err);
	err = gpgme_data_new(&out);
	fail_if_err(err);
	err = gpgme_op_encrypt(ctx, &key, GPGME_ENCRYPT_ALWAYS_TRUST, in, out);
	fail_if_err(err);

	r = gpgme_data_seek(out, 0, SEEK_SET);
	if (r)
		fail_if_err (gpgme_err_code_from_errno(errno));

	while ((r = gpgme_data_read(out, buf, BUFSIZ)))
		fwrite(buf, r, 1, stream);
	gpg_cleanup();
	if (r < 0)
		fail_if_err(gpgme_err_code_from_errno(errno));

	return 0;
}
