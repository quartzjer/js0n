#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"

void ex1(void)
{
	char *json = "{\"foo\":\"bar\",\"barbar\":[1,2,3],\"obj\":{\"a\":\"b\"}}";

	printf("parsing '%s'\n", json);

	char *val = js0n("barbar", json, strlen(json), 0);

	printf("returned %s\n",val);

}

int main(int argc, char **argv)
{
	ex1();
}

