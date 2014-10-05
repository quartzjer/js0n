#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/js0n.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
	unsigned char buff[1024], *json = NULL;
	int len, lastlen=0, ret, i;
	unsigned short *res;
	FILE *f;
	
	if((f = fopen(argv[1],"r")) == NULL)
	{
		printf("uhoh opening %s\n",argv[1]);
		exit(1);
	}
	while((len = fread(buff,1,1024,f)) > 0)
	{
		json = realloc(json,lastlen+len);
		memcpy(json+lastlen,buff,len);
		lastlen+=len;
	}
	fclose(f);
	printf("got[%.*s]\n",lastlen,json);
	res = malloc(lastlen); // way more than enough
	ret = js0n(json,lastlen,res,lastlen);
	printf("returned %d\n",ret);
	for(i=0;res[i];i+=2)
	{
		printf("%d: at %d len %d is %.*s\n",i,res[i],res[i+1],res[i+1],json+res[i]);
	}
	
	// j0g tests
  printf("j0g_val 'key' val offset %d\n", j0g_val("key",(char*)json,res));
  printf("j0g_str 'key' val '%s'\n", j0g_str("key",(char*)json,res));
  printf("j0g_str 'num' val '%0.2f'\n", (j0g_str("num",(char*)json,res)!=NULL)?strtof(j0g_str("num",(char*)json,res), NULL):0);
  printf("j0g_test obj->true %d\n", j0g_test("true",j0g(j0g_str("obj",(char*)json,res),res,16),res));
	return 0;
}

