// by jeremie miller - 2014
// public domain or MIT license, contributions/improvements welcome via github at https://github.com/quartzjer/js0n

#include "js0n.h"
#include <string.h> // one strncmp() is used to do key comparison, and a strlen(key) if no len passed in
#include <assert.h>
#ifdef DEBUG
#include <stdio.h>
#endif

// only at depth 1, track start pointers to match key/value
#define PUSH(i) if(depth == 1) { if(!index) { val = cur+i; }else{ if(klen && index == 1) start = cur+i; else index--; } }

// determine if key matches or value is complete
#define CAP(i) if(depth == 1) { if(val && !index) {*vlen = (size_t)((cur+i+1) - val); return val;}; if(klen && start) {index = (klen == (size_t)(cur-start) && strncmp(key,start,klen)==0) ? 0 : 2; start = 0;} }


enum state {
	S_STRUCT,
	S_BARE,
	S_STRING,
	S_UTF8,
	S_ESC,
};

#define range(x,s,e) ((x) >= (s) && (x) <= (e))

// this makes a single pass across the json bytes, using each byte as an index into a jump table to build an index and transition state
const char *js0n(const char *key, size_t klen,
				 const char *json, size_t jlen, size_t *vlen)
{
	const char *val = 0;
	const char *cur, *end, *start;
	size_t index = 1;
	int depth = 0;
	int utf8_remain = 0;
	enum state state = S_STRUCT;

	if(!json || jlen <= 0 || !vlen) return 0;
	*vlen = 0;

	// no key is array mode, klen provides requested index
	if(!key)
	{
		index = klen;
		klen = 0;
	}else{
		if(klen <= 0) klen = strlen(key); // convenience
	}

	for(start=cur=json,end=cur+jlen; cur<end; cur++)
	{
	  again:
		switch (state) {
		case S_STRUCT:
			switch(*cur) {
			case '\t':
			case ' ':
			case '\r':
			case '\n':
			case ':':
			case ',':
				continue;

			case '"': goto l_qup;
			case '[': goto l_up;
			case ']': goto l_down;
			case '{': goto l_up;
			case '}': goto l_down;

			case '-': goto l_bare;
			default: {
				if (range(*cur, '0', '9') ||
				    range(*cur, 'A', 'Z') ||
				    range(*cur, 'a', 'z'))
					goto l_bare;
				else
					goto l_bad;
			}
			}
			assert(0);
		case S_BARE:
			switch (*cur) {
			case '\t':
			case ' ':
			case '\r':
			case '\n':
			case ',':
			case ']':	// correct? not [ ?
			case '}':	// correct? not { ?
			case ':':
				goto l_unbare;
			default:  {
				// could be more pedantic/validation-checking
				if (range(*cur, 32, 126))
					continue;
				goto l_bad;
			}
			}
			assert(0);
		case S_STRING:
			if (*cur == '\\') {
				state = S_ESC;
				continue;
			}
			if (*cur == '"')
				goto l_qdown;
			if (range(*cur, 32, 126))
				continue;
			if ((*cur & 224) == 192) { // range(*cur, 192, 223))
				state = S_UTF8;
				utf8_remain = 1;
				continue;
			}
			if ((*cur & 240) == 224) { // range(*cur, 224, 239)
				state = S_UTF8;
				utf8_remain = 2;
				continue;
			}
			if ((*cur & 248) == 240) { // range(*cur, 240, 247)
				state = S_UTF8;
				utf8_remain = 3;
				continue;
			}
			goto l_bad;
		// XXX no utf8 outside strings?
		case S_UTF8:
			if ((*cur & 192) == 128) { // range(*cur, 128, 191)
				if (!--utf8_remain)
					state = S_STRING;
				continue;
			}
			goto l_bad;
		case S_ESC:
			switch (*cur) {
			case '"':
			case '\\':
			case '/':
			case 'b':
			case 'f':
			case 'n':
			case 'r':
			case 't':
			case 'u':
				state = S_STRING;
				continue;
			default:
				goto l_bad;
			}
		}
		assert (0);
	  l_bad:
		*vlen = cur - json; // where error'd
#ifdef DEBUG
		fprintf(stderr, "bad at >%.5s< state %d depth %d\n",
		    cur, state, depth);
#endif
		return 0;

	  l_up:
		PUSH(0);
		++depth;
		continue;

	  l_down:
		--depth;
		CAP(0);
		continue;

	  l_qup:
		PUSH(1);
		state = S_STRING;
		continue;

	  l_qdown:
		CAP(-1);
		state = S_STRUCT;
		continue;

	  l_bare:
		PUSH(0);
		state = S_BARE;
		continue;

	  l_unbare:
		CAP(-1);
		state = S_STRUCT;
		goto again;

	}

	if (depth) {
		*vlen = jlen; // incomplete
	}
	return 0;

}
