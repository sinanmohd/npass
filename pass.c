#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pass_util.h"
#include "util.h"

void print_usage(void);

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

int main(int argc, char *argv[])
{
	int r = 0;
	const char *s = NULL;

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

		s = pass_cat(argv[1]);
		r = !(s == NULL);
		if (s)
			fputs(s, stdout);
	}

	return r;
}
