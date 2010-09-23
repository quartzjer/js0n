
// pass it a raw json string and length, it will return an array of unsigned shorts, see js0n.c for more
int js0n(unsigned char *js, unsigned int len, unsigned short *out);

// TODO: decode a string value, copy into new while unescaping things and unicode stuffs
// int js0n(unsigned char *in, unsigned int len, unsigned char *out); 
