
int js0n(unsigned char *js, unsigned int len, unsigned short **out)
{
	unsigned char buff[1024], *cur, *end;
	int mode=0; // 0=structures 1=strings
	int depth=0;
	int kind=0; // 0=obj 1=array
	char quote;
    static void *gostruct[] = 
    {
        [0 ... 255] = &&l_bad,
        ['\t'] = &&l_ws, [' '] = &&l_ws, ['\r'] = &&l_ws, ['\n'] = &&l_ws,
        ['"'] = &&l_quot,
        [':'] = &&l_is,[','] = &&l_more,
		['['] = &&l_ko, [']'] = &&l_kc,
		['{'] = &&l_ko, ['}'] = &&l_kc,
		['-'] = &&l_bare, [48 ... 57] = &&l_bare, // 0-9
		['t'] = &&l_bare, ['f'] = &&l_bare, ['n'] = &&l_bare // true, false, null
    };
    static void *gobare[] = 
    {
    	[0 ... 31] = &&l_bad,
        [32 ... 126] = &&l_char,
        ['\t'] = &&l_unbare, [' '] = &&l_unbare, ['\r'] = &&l_unbare, ['\n'] = &&l_unbare,
        [','] = &&l_unbare, [']'] = &&l_unbare, ['}'] = &&l_unbare,
        [127 ... 255] = &&l_bad
    };
    static void *gostring[] = 
    {
    	[0 ... 31] = &&l_bad, [127] = &&l_bad,
        [32 ... 126] = &&l_char,
        ['\\'] = &&l_esc, ['"'] = &&l_quot,
        [128 ... 255] = &&l_char
    };
    static void *goesc[] = 
    {
        [0 ... 255] = &&l_bad,
        ['"'] = &&l_unesc, ['\\'] = &&l_unesc, ['/'] = &&l_unesc, ['b'] = &&l_unesc,
		['f'] = &&l_unesc, ['n'] = &&l_unesc, ['r'] = &&l_unesc, ['t'] = &&l_unesc, ['u'] = &&l_unesc
    };
	static void **go = gostruct;
	
	for(cur=js,end=js+len; cur<end; cur++)
	{
			goto *go[*cur];
			l_loop:;
	}
	
	l_bad:
		printf("bad char %d\n",*cur);
		return 1;
	
	l_char:
		printf("%c",mode?'.':'*');
		goto l_loop;
	
	l_ko:
		printf("%d%c",++depth,*cur);
		goto l_loop;
	l_kc:
		printf("%d%c",--depth,*cur);
		goto l_loop;

	l_ws:
	l_is:
	l_more:
		printf("%c",*cur);
		goto l_loop;
	
	l_quot:
		printf("%c",*cur);
		go = mode?gostruct:gostring;
		mode^=1;
		goto l_loop;
		
	l_esc:
		go = goesc;
		goto l_loop;
		
	l_unesc:
		go = gostring;
		goto l_loop;

	l_bare:
		go = gobare;
		goto l_loop;

	l_unbare:
		go = gostruct;
		goto *go[*cur];
		
}

