// key = string to match or null
// len = pointer to key length or array offset to match
// json = json object or array
// jlen = length of json
// returns pointer to value and sets len to value length, or 0 if not found or any error
char *js0n(char *key, unsigned int *len, char *json, unsigned int jlen);
