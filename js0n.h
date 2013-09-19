// pass it a raw json string and length, and it will tag all the key/value offsets in the out argument (size it same as js to be safe)
// returns 0 if successful, >0 if not
int js0n(unsigned char *js, unsigned int len, unsigned short *out, unsigned int olen);
