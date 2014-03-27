#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "js0n.h"
#include "j0g.h"

// dumbed down parser, expects null terminated json, if fails the *index is 0 (and safe to pass into j0g_*)
char *j0g(const char *json, unsigned short *index, int ilen)
{
  int ret, len;
  if(!json) return NULL;
  len = strlen(json);
  ret = js0n((unsigned char*)json, len, index, ilen);
  if(ret) *index = 0;
  return (char*)json;
}

// return the null-terminated string value matching the given key
char *j0g_str(const char *key, char *json, const unsigned short *index)
{
  int val = j0g_val(key, json, index);
  if(!val) return NULL;
  return j0g_safe(val, json, index);
}

// null terminate and unescape any string at this value
char *j0g_safe(int val, char *json, const unsigned short *index)
{
  char *str, *cursor;
  *(json+(index[val]+index[val+1])) = 0; // null terminate
  // unescape stuff
  for(cursor=str=json+index[val]; *cursor; cursor++,str++)
  {
    if(*cursor == '\\' && *(cursor+1) == 'n')
    {
      *str = '\n';
      cursor++;
    }else if(*cursor == '\\' && *(cursor+1) == '"'){
      *str = '"';
      cursor++;
    }else{
      *str = *cursor;
    }
  }
  *str = *cursor; // copy null terminator too
  return (char*)json+index[val];
}

// return 1 if the value is the bool value true, number 1, or even the string "true", false otherwise
int j0g_test(const char *key, char *json, const unsigned short *index)
{
  char *val = j0g_str(key, json, index);
  if(!val) return 0;
  if(strcmp(val, "true") == 0) return 1;
  if(strcmp(val, "1") == 0) return 1;
  return 0;
}

// return the index offset of the value matching the given key
int j0g_val(const char *key, char *json, const unsigned short *index)
{
  if(!key || !json) return 0;
  int i, klen = strlen(key);
	for(i=0;index[i];i+=4)
	{
    if(klen == index[i+1] && strncmp(key,(char*)json+index[i],klen) == 0) return i+2;
	}
  return 0;
}

