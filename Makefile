
all: test example

test.c: test/test.c src/js0n.c
	gcc -Wall -Wextra -Wno-unused-parameter -o test/test test/test.c src/js0n.c

test: test.c
	@if ./test/test ; then \
		rm -f ./test/test; \
		echo "TESTS PASSED"; \
	else \
		rm -f ./test/test; \
		echo "TESTS FAILED"; exit 1; \
	fi; \

example: test/example.c src/js0n.c
	gcc -Wall -o example test/example.c src/js0n.c

clean:
	rm -f example test/test

.PHONY: all test clean
