#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "pass_util.h"
#include "util.h"
#include "gpg.h"

#define DEF_PASS_DIR "pass"

static char pass_dir[PATH_MAX] = {0};
static char pass_out[PASS_MAX] = {0};

int set_pass_dir(void);

int set_pass_dir(void) {
	const char *env;

	env = getenv("PASSWORD_STORE_DIR");
	if (env) {
		strncpy(pass_dir, env, sizeof(pass_dir) - 1);
		return 0;
	}

	env = getenv("XDG_DATA_HOME");
	if (env) {
		snprintf(pass_dir, sizeof(pass_dir) - 1, "%s/%s", env,
			DEF_PASS_DIR);
		return 0;
	}

	env = getenv("HOME");
	if (env) {
		snprintf(pass_dir, sizeof(pass_dir) - 1, "%s/%s/%s", env,
			".local/share", DEF_PASS_DIR);
		return 0;
	}

	return 1;
}

int pass_init(const char *fpr)
{
	int r;
	char gpg_id_path[PATH_MAX];
	FILE *gpg_id;

	r = set_pass_dir();
	if (r)
		err_die(1, "PASSWORD_STORE_DIR not set");

	r = gpg_key_validate(fpr);
	if (r)
		err_die(1, "key not usable, try gpg --full-generate-key");


	r = r_mkdir(pass_dir, S_IRWXU);
	if (r)
		err_die(1, "%s %s", pass_dir, strerror(errno));

	r = snprintf(gpg_id_path, sizeof(gpg_id_path), "%s/%s", pass_dir, ".gpg-id");
	if (r > (int) sizeof(gpg_id_path))
		err_die(1, "path exceeded PATH_MAX");

	gpg_id = fopen(gpg_id_path, "w");
	if (!gpg_id)
		err_die(1, "%s %s", gpg_id_path, strerror(errno));

	r = fwrite(fpr, strlen(fpr), 1,gpg_id);
	fclose(gpg_id);
	if (!r)
		err_die(1, "write failed");

	return 0;
}

const char *pass_cat(const char *path)
{
	int r;
	char *rc;
	char gpg_id_path[PATH_MAX], fpr[FPR_MAX], pass_path[PATH_MAX];
	FILE *gpg_id;

	r = set_pass_dir();
	if (r)
		err_die(NULL, "PASSWORD_STORE_DIR not set");

	r = snprintf(gpg_id_path, sizeof(gpg_id_path), "%s/%s", pass_dir, ".gpg-id");
	if (r > (int) sizeof(gpg_id_path))
		err_die(NULL, "path exceeded PATH_MAX");

	gpg_id = fopen(gpg_id_path, "r");
	if (!gpg_id)
		err_die(NULL, "%s %s", gpg_id_path, strerror(errno));

	rc = fgets(fpr, sizeof(fpr), gpg_id);
	if (!rc)
		err_die(NULL, "failed to read %s", gpg_id_path);

	fclose(gpg_id);

	r = gpg_key_validate(fpr);
	if (r)
		err_die(NULL, "key not usable, try gpg --list-keys");

	r = snprintf(pass_path, sizeof(pass_path), "%s/%s.gpg", pass_dir, path);
	if (r > (int) sizeof(gpg_id_path))
		err_die(NULL, "path exceeded PATH_MAX");

	r = gpg_decrypt(fpr, pass_path, pass_out, sizeof(pass_out));
	return r ? NULL : pass_out;
}
