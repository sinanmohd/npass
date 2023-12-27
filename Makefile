OBJECTS = pass.o pass_util.o util.o gpg.o
CC = gcc
CFLAGS = -g -Wvla -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wpedantic -fsanitize=address

pass: $(OBJECTS)
	$(CC) $(CFLAGS) -o pass $(OBJECTS) -l gpgme

pass.o: pass_util.h util.h
pass_util.o: util.h gpg.h

.PHONY: clean
clean:
	rm -f pass $(OBJECTS)
