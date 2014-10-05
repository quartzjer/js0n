// pass it a raw json string, length, and key and it will return a pointer to the start of the value
// if a vlen is given, it will set it to the length, otherwise it will null terminate it
// returns 0 if not found or any error
char *js0n(char *key, char *json, unsigned int len, unsigned int *vlen);
