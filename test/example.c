#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"

void ex1(void)
{
	int arg = 0;

	char *json = "{\"foo\":\"bar\",\"barbar\":[1,2,3],\"obj\":{\"a\":\"b\"}}";
	printf("parsing '%s'\n", json);
	char *val = js0n("barbar", &arg, json, strlen(json));
	printf("returned %.*s\n",arg,val);

	char *array = "[\"foo\",\"bar\",[1,2,3],{\"a\":\"b\"},42]";
	printf("parsing '%s'\n", array);
	arg = 3;
	val = js0n(0, &arg, array, strlen(array));
	printf("returned %.*s\n",arg,val);

}

int main(int argc, char **argv)
{
	ex1();
}

