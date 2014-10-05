#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
	char buff[1024], *json = NULL, *ret;
	int len, jlen=0;
	FILE *f;
	
	fail_unless((f = fopen("./test/test.json","r")) != NULL);
	while((len = fread(buff,1,1024,f)) > 0)
	{
		json = realloc(json,jlen+len);
		memcpy(json+jlen,buff,len);
		jlen+=len;
	}
	fclose(f);
	
	ret = js0n("test",0,json,jlen,&len);
//	printf("got %s %d\n",ret,len);
	fail_unless(strncmp("value",ret,len) == 0);


//  printf("j0g_val 'key' val offset %d\n", j0g_val("key",(char*)json,res));
//  printf("j0g_str 'key' val '%s'\n", j0g_str("key",(char*)json,res));
//  printf("j0g_str 'num' val '%0.2f'\n", (j0g_str("num",(char*)json,res)!=NULL)?strtof(j0g_str("num",(char*)json,res), NULL):0);
//  printf("j0g_test obj->true %d\n", j0g_test("true",j0g(j0g_str("obj",(char*)json,res),res,16),res));
	return 0;
}

