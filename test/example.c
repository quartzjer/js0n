#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../js0n.h"
#include "../j0g.h"

void
ex1()
{
	char *s = "{\"foo\":\"bar\",\"barbar\":[1,2,3],\"obj\":{\"a\":\"b\"}}";

	// 3 keys, 3 values, each with a start and offset --> 12
	// Plus one for a terminating zero = 13.
	unsigned short kvpairs[13];

	printf("Parsing '%s'\n", s);

	int rc = js0n((unsigned char*) s, strlen(s), kvpairs, 13);

	printf("returned %d\n",rc);

	for (int i = 0; kvpairs[i]; i += 2)

		printf("%d: at %d len %d is %.*s\n", i, 
			kvpairs[i], kvpairs[i + 1], kvpairs[i + 1], s + kvpairs[i]);

}

int main(int argc, char **argv)
{
	ex1();
}

