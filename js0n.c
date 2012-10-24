#include "js0n.h"

/* this code is based on the work by jeremie miller, which is part of the public domain
 * git://github.com/quartzjer/js0n.git */

#define CASES_a_TO_z \
     'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':\
case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':\
case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':\
case 'y': case 'z'

#define CASES_A_TO_Z \
     'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':\
case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':\
case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':\
case 'Y': case 'Z'

#define CASES_0_TO_9 \
     '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':\
case '8': case '9'

#define CASES_ALL_SYMBOLS \
     ' ': case '!': case '"': case '#': case '$': case '%': case '&': case '\'':\
case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/':\
case ':': case ';': case '<': case '=': case '>': case '?': case '@': case '[':\
case '\\': case ']': case '^': case '_': case '{': case '|': case '}': case '~'

#define CASES_VALID_STRING_SYMBOLS \
     ' ': case '!': /*case '"':*/ case '#': case '$': case '%': case '&': case '\'':\
case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/':\
case ':': case ';': case '<': case '=': case '>': case '?': case '@': case '[':\
/*case '\\':*/ case ']': case '^': case '_': case '{': case '|': case '}': case '~'

#define CASES_VALID_BARE_SYMBOLS \
    '!': /*case ' ':*/ case '"': case '#': case '$': case '%': case '&': case '\'':\
case '(': case ')': case '*': case '+': /*case ',':*/ case '-': case '.': case '/':\
case ':': case ';': case '<': case '=': case '>': case '?': case '@': case '[':\
case '\\': /*case ']':*/ case '^': case '_': case '{': case '|': /*case '}':*/ case '~'

#define CASES_CONTROL_CHARACTERS \
     0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:\
case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:\
case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:\
case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31

#define CASES_UTF8_1 \
     128: case 129: case 130: case 131: case 132: case 133: case 134: case 135:\
case 136: case 137: case 138: case 139: case 140: case 141: case 142: case 143:\
case 144: case 145: case 146: case 147: case 148: case 149: case 150: case 151:\
case 152: case 153: case 154: case 155: case 156: case 157: case 158: case 159:\
case 160: case 161: case 162: case 163: case 164: case 165: case 166: case 167:\
case 168: case 169: case 170: case 171: case 172: case 173: case 174: case 175:\
case 176: case 177: case 178: case 179: case 180: case 181: case 182: case 183:\
case 184: case 185: case 186: case 187: case 188: case 189: case 190: case 191

#define CASES_UTF8_2 \
     192: case 193: case 194: case 195: case 196: case 197: case 198: case 199:\
case 200: case 201: case 202: case 203: case 204: case 205: case 206: case 207:\
case 208: case 209: case 210: case 211: case 212: case 213: case 214: case 215:\
case 216: case 217: case 218: case 219: case 220: case 221: case 222: case 223

#define CASES_UTF8_3 \
     224: case 225: case 226: case 227: case 228: case 229: case 230: case 231:\
case 232: case 233: case 234: case 235: case 236: case 237: case 238: case 239

#define CASES_UTF8_4 \
     240: case 241: case 242: case 243: case 244: case 245: case 246: case 247

typedef enum
{
    JS0N_STATE_GOSTRUCT         = 0,
    JS0N_STATE_GOBARE           = 1,
    JS0N_STATE_GOSTRING         = 2,
    JS0N_STATE_GOUTF8_CONTINUE  = 3,
    JS0N_STATE_GOESC            = 4,
} js0n_gostate_t;

void PUSH ( js0n_parser_t * parser, int refpos )
{
    if ( parser->depth == 1 )
    {
        parser->mark = ( parser->cursor + refpos );
    }
}

void CAP ( js0n_parser_t * parser, int refpos )
{
    uint16_t length;

    if ( parser->depth == 1 )
    {
        length = ( ( ( parser->cursor + refpos ) - parser->mark ) + 1 );

        /* notify the user */
        parser->user_cb ( parser->mark, length );
        
        parser->mark = ( parser->cursor + refpos );
    }
}

int l_loop ( js0n_parser_t * parser )
{
     ++parser->cursor;

    return 0;
}

int l_bad ( js0n_parser_t * parser )
{
    return 1;
}

int l_up ( js0n_parser_t * parser )
{
    PUSH ( parser, 0 );
    ++parser->depth;
    ++parser->cursor;

    return 0;
}

int l_down ( js0n_parser_t * parser )
{
    --parser->depth;
    CAP ( parser, 0 );
    ++parser->cursor;

    return 0;
}

int l_qup ( js0n_parser_t * parser )
{
    PUSH ( parser, 1 );
    parser->gostate = JS0N_STATE_GOSTRING;
    ++parser->cursor;

    return 0;
}

int l_qdown ( js0n_parser_t * parser )
{
    CAP ( parser, -1 );
    parser->gostate = JS0N_STATE_GOSTRUCT;
    ++parser->cursor;

    return 0;
}

int l_esc ( js0n_parser_t * parser )
{
    parser->gostate = JS0N_STATE_GOESC;
    ++parser->cursor;

    return 0;
}

int l_unesc ( js0n_parser_t * parser )
{
    parser->gostate = JS0N_STATE_GOSTRING;
    ++parser->cursor;

    return 0;
}

int l_bare ( js0n_parser_t * parser )
{
    PUSH ( parser, 0 );
    parser->gostate = JS0N_STATE_GOBARE;
    ++parser->cursor;

    return 0;
}

int l_unbare ( js0n_parser_t * parser )
{
    CAP ( parser, -1 );
    parser->gostate = JS0N_STATE_GOSTRUCT;
    
    return 0;
}

int l_utf8_2 ( js0n_parser_t * parser )
{
    parser->gostate = JS0N_STATE_GOUTF8_CONTINUE;
    parser->utf8_remain = 1;
    ++parser->cursor;

    return 0;
}

int l_utf8_3 ( js0n_parser_t * parser )
{
    parser->gostate = JS0N_STATE_GOUTF8_CONTINUE;
    parser->utf8_remain = 2;
    ++parser->cursor;

    return 0;
}

int l_utf8_4 ( js0n_parser_t * parser )
{
    parser->gostate = JS0N_STATE_GOUTF8_CONTINUE;
    parser->utf8_remain = 3;
    ++parser->cursor;

    return 0;
}

int l_utf_continue ( js0n_parser_t * parser )
{
    if ( !--parser->utf8_remain )
    {
        parser->gostate = JS0N_STATE_GOSTRING;
    }

    ++parser->cursor;

    return 0;
}

int js0n_parse ( js0n_parser_t * parser )
{
    int ret;

    parser->mark = 0;
    parser->depth = 0;
    parser->utf8_remain = 0;
    parser->cursor = parser->js;
    parser->gostate = JS0N_STATE_GOSTRUCT;

    while ( parser->cursor <= ( parser->js + parser->length ) )
    {
        switch ( parser->gostate )
        {
        case JS0N_STATE_GOSTRUCT:
            switch ( *parser->cursor )
            {
            case '\t':
            case ' ':
            case '\r':
            case '\n':
            case ':':
            case ',':
                ret = l_loop ( parser );
                break;

            case '"':
                ret = l_qup ( parser );
                break;
           
            case '[':
            case '{':
                ret = l_up ( parser );
                break;

            case ']':
            case '}':
                ret = l_down ( parser );
                break;

            case '-':
            case 't':
            case 'f':
            case 'n':
            case CASES_0_TO_9:
                ret = l_bare ( parser );
                break;

            default:
                ret = l_bad ( parser );
                break;
            }
            break;

        case JS0N_STATE_GOBARE:
            switch ( *parser->cursor )
            {
            case CASES_a_TO_z:
            case CASES_A_TO_Z:
            case CASES_0_TO_9:
            case CASES_VALID_BARE_SYMBOLS:
                ret = l_loop ( parser );
                break;

            case '\t':
            case ' ':
            case '\r':
            case '\n':
            case ',':
            case ']':
            case '}':
                ret = l_unbare ( parser );
                break;

            default:
                ret = l_bad ( parser );
                break;
            }
            break;

        case JS0N_STATE_GOSTRING:
            switch ( *parser->cursor )
            {
            case CASES_a_TO_z:
            case CASES_A_TO_Z:
            case CASES_0_TO_9:
            case CASES_VALID_STRING_SYMBOLS:
                ret = l_loop ( parser );
                break;

            case '\\':
                ret = l_esc ( parser );
                break;

            case '"':
                ret = l_qdown ( parser );
                break;

            case CASES_UTF8_2:
                ret = l_utf8_2 ( parser );
                break;

            case CASES_UTF8_3:
                ret = l_utf8_3 ( parser );
                break;

            case CASES_UTF8_4:
                ret = l_utf8_4 ( parser );
                break;

            default:
                ret = l_bad ( parser );
                break;
            }
            break;

        case JS0N_STATE_GOUTF8_CONTINUE:
            switch ( *parser->cursor )
            {
            case CASES_UTF8_1:
                ret = l_utf_continue ( parser );
                break;

            default:
                ret = l_bad ( parser );
                break;
            }

        case JS0N_STATE_GOESC:
            switch ( *parser->cursor )
            {
            case '"':
            case '\\':
            case '/':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
            case 'u':
                ret = l_unesc ( parser );
                break;

            default:
                ret = l_bad ( parser );
                break;
            }
            break;
        }

        if ( ret ) return -1;
    }

    return parser->depth; // 0 if successful full parse, >0 for incomplete data
}
