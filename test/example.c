#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"

void ex1(void)
{
	size_t vlen = 0;

	char *json = "{\"foo\":\"bar\",\"barbar\":[1,2,3],\"obj\":{\"a\":\"b\"}}";
	printf("parsing '%s'\n", json);
	const char *val = js0n("barbar", 0, json, strlen(json), &vlen);
	printf("returned %.*s\n",(int)vlen,val);

	char *array = "[\"foo\",\"bar\",[1,2,3],{\"a\":\"b\"},42]";
	printf("parsing '%s'\n", array);
	val = js0n(0, 3, array, strlen(array), &vlen);
	printf("returned %.*s\n",(int)vlen,val);

}

int main(int argc, char **argv)
{
	ex1();
	return 0;
}

