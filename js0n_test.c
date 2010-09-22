#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "js0n.h"

int main(int argc, char **argv)
{
	unsigned char buff[1024], *json;
	int len, lastlen=0;
	unsigned short **res;
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
	return 0;
}

