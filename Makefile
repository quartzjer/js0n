MANDIR=/usr/share/man/man3

all: js0n_test example

js0n_test: test/js0n_test.c js0n.c j0g.c
	gcc -Wall -o js0n_test test/js0n_test.c js0n.c j0g.c

example: test/example.c js0n.c j0g.c
	gcc -Wall -o example test/example.c js0n.c j0g.c

clean:
	rm -f example js0n_test


man: ${MANDIR}/js0n.3

${MANDIR}/js0n.3: js0n.3
	sudo cp $? $@
