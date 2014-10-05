// pass it a raw json string, length, and key and it will return a pointer to the start of the value
// if an arg is given, it will set it to the length, otherwise it will null terminate it
// to get an item from an array, pass a null key and the array offset in the arg
// returns 0 if not found or any error
char *js0n(char *key, char *json, unsigned int len, unsigned int *arg);
