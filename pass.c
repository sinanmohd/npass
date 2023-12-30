#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "pass_util.h"
#include "util.h"

void print_usage(void);
int cat(const char *path);
int add(const char *path);

void print_usage(void)
{
	printf("Usage: pass COMMAND\n\n"

	       "Commands:\n"
	       "  init key-id/fingerprint\n"
	       "	Initialize new password storage\n"
	       "  ls   [ pass-path ]\n"
	       "	List passwords\n"
	       "  rm   pass-name\n"
	       "	Remove password\n"
	       "  add  pass-name\n"
	       "	Add new password\n"
	       "  gen  pass-name\n"
	       "	Generate new password\n"
	       "  cat  pass-name\n"
	       "	Show encrypted password\n"
	       "  help\n"
	       "	Show this help\n");
}

int cat(const char *path)
{
	const char *s = NULL;

	s = pass_cat(path);
	if (s)
		fputs(s, stdout);

	return  (s == NULL);
}

int add(const char *path)
{
	char *p1 = NULL, *p2 = NULL;
	FILE *in;
	size_t n;
	int r;

	in = fopen("/dev/tty", "r");
	if (!in)
		in = stdin;

	fputs("Password: ", stdout);
	r = pass_getpass(&p1, &n, in);
	if (r < 0) {
		if (in != stdin)
			fclose(in);
		if (p1)
			free(p1);
		err_die(1, "%d:%s:", errno, strerror(errno));
	}

	fputs("\nRetype password: ", stdout);
	r = pass_getpass(&p2, &n, in);
	putc('\n', stdout);
	if (r < 0) {
		if (in != stdin)
			fclose(in);
		if (p2)
			free(p1);
		if (p2)
			free(p2);
		err_die(1, "%d:%s:", errno, strerror(errno));
	}

	if (in != stdin)
		fclose(in);

	if (n > PASS_MAX - 1) /* TODO: get rid of the limit */
		err_die(1, "password must not exceed %d characters", PASS_MAX);

	if (strcmp(p1, p2)) {
		free(p1);
		free(p2);
		err_die(1, "Sorry, passwords do not match");
	}

	free(p1);
	r = pass_add(path, p2, n);

	free(p2);
	return r;
}

int main(int argc, char *argv[])
{
	int r = 0;

	if (!--argc) {
		print_usage();
		exit(EXIT_FAILURE);
	}
	++argv;


	if (!strcmp("help", *argv)) {
		print_usage();
	} else if (!strcmp("init", *argv)) {
		if (!argv[1])
			err_die(1, "invalid usage, try pass help");

		r = pass_init(argv[1]);
	} else if (!strcmp("cat", *argv)) {
		if (!argv[1])
			err_die(1, "invalid usage, try pass help");

		r = cat(argv[1]);
	} else if (!strcmp("add", *argv)) {
		if (!argv[1])
			err_die(1, "invalid usage, try pass help");

		r = add(argv[1]);
	} else {
		r = cat(*argv);
	}

	if (r)
		err_die(r, "Command failed");

	return 0;
}
