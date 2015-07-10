#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
	char buff[1024], *json = NULL;
	const char *ret;
	size_t len, len2, jlen=0;
	FILE *f;
	
	fail_unless((f = fopen("./test/test.json","r")) != NULL);
	while((len = fread(buff,1,1024,f)) > 0)
	{
		json = realloc(json,jlen+len);
		memcpy(json+jlen,buff,len);
		jlen+=len;
	}
	fclose(f);
	
	fail_unless((ret = js0n("test",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("value",ret,len) == 0);

	fail_unless((ret = js0n("foo",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("b\\\"a and \\\\ r",ret,len) == 0);

	fail_unless((ret = js0n("array",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("[1,2,\"three\",4]",ret,len) == 0);

	fail_unless((ret = js0n(0,2,ret,len,&len2)));
	fail_unless(len);
	fail_unless(strncmp("three",ret,len2) == 0);

	fail_unless((ret = js0n("baz",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("{\"a\":\"b\"}",ret,len) == 0);

	fail_unless((ret = js0n("num",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("123.45",ret,len) == 0);

	fail_unless((ret = js0n("bool",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("true",ret,len) == 0);

	fail_unless((ret = js0n("bare",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("yep",ret,len) == 0);

	fail_unless((ret = js0n("utf8",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("$¢€𤪤",ret,len) == 0);

	fail_unless((ret = js0n("key",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("value\\n\\\"newline\\\"",ret,len) == 0);

	fail_unless((ret = js0n("obj",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("{\"true\":true}",ret,len) == 0);

	fail_unless((ret = js0n("value",0,json,jlen,&len)));
	fail_unless(len);
	fail_unless(strncmp("real",ret,len) == 0);

	// test parse errors

	fail_unless(js0n("x",0,"{}",2,&len) == 0);
	fail_unless(len == 0);

	fail_unless(js0n("x",0,"{",1,&len) == 0);
	fail_unless(len == 1);

	fail_unless(js0n("x",0,"{\0}",3,&len) == 0);
	fail_unless(len == 1);

	return 0;
}

