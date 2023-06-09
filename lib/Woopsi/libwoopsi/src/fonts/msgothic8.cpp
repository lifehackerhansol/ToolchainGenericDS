#include "msgothic8.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 MSGothic8_glyphdata[191] = {
/* '!' */	0xAA8C,
/* '"' */	0xB680,0x0000,
/* '#' */	0x57D4,0xAFA9,0x4000,
/* '$' */	0x23AB,0x4716,0xAE00,
/* '%' */	0xCEB4,0x45AE,0x6000,
/* '&' */	0x2288,0xC9C9,0xA000,
/* ''' */	0x5800,
/* '(' */	0x2A49,0x2200,
/* ')' */	0x8892,0x4A00,
/* '*' */	0x255C,0xEA90,0x0000,
/* '+' */	0x0974,0x8000,
/* ',' */	0x00D8,
/* '-' */	0x000F,0x0000,
/* '.' */	0x000C,
/* '/' */	0x1122,0x4488,
/* '0' */	0x6999,0x9960,
/* '1' */	0x7554,
/* '2' */	0x6912,0x48F0,
/* '3' */	0x6916,0x1960,
/* '4' */	0x3194,0xA97C,0x4000,
/* '5' */	0xF8E1,0x1960,
/* '6' */	0x698E,0x9960,
/* '7' */	0xF122,0x4440,
/* '8' */	0x6996,0x9960,
/* '9' */	0x6997,0x1960,
/* ':' */	0x300C,
/* ';' */	0xC0D8,
/* '<' */	0x1248,0x4210,
/* '=' */	0x00F0,0xF000,
/* '>' */	0x8421,0x2480,
/* '?' */	0x6912,0x2022,
/* '@' */	0x746F,0x5BC1,0xC000,
/* 'A' */	0x6699,0xF990,
/* 'B' */	0xE99E,0x99E0,
/* 'C' */	0x6988,0x8960,
/* 'D' */	0xE999,0x99E0,
/* 'E' */	0xF88E,0x88F0,
/* 'F' */	0xF88E,0x8880,
/* 'G' */	0x698B,0x9970,
/* 'H' */	0x999F,0x9990,
/* 'I' */	0xFE00,
/* 'J' */	0x1111,0x1960,
/* 'K' */	0x99AC,0xA990,
/* 'L' */	0x8888,0x88F0,
/* 'M' */	0x8C77,0xBAD6,0xA000,
/* 'N' */	0x99DD,0xBB90,
/* 'O' */	0x6999,0x9960,
/* 'P' */	0xE999,0xE880,
/* 'Q' */	0x6999,0xDA50,
/* 'R' */	0xE99E,0x9990,
/* 'S' */	0x6986,0x1960,
/* 'T' */	0xF444,0x4440,
/* 'U' */	0x9999,0x9960,
/* 'V' */	0x8C62,0xA510,0x8000,
/* 'W' */	0xAD6B,0x5529,0x4000,
/* 'X' */	0x9966,0x6990,
/* 'Y' */	0x8C54,0x4210,0x8000,
/* 'Z' */	0xF122,0x48F0,
/* '[' */	0xEAAA,
/* '\' */	0x8A89,0xE278,0x8000,
/* ']' */	0xD555,
/* '^' */	0x5400,0x0000,
/* '_' */	0x0000,0x0003,0xE000,
/* '`' */	0x9000,
/* 'a' */	0x00E1,0x7970,
/* 'b' */	0x88E9,0x99E0,
/* 'c' */	0x0069,0x8960,
/* 'd' */	0x1179,0x9970,
/* 'e' */	0x0069,0xF870,
/* 'f' */	0x64F4,0x4440,
/* 'g' */	0x5AA4,0xB960,
/* 'h' */	0x88E9,0x9990,
/* 'i' */	0xBE00,
/* 'j' */	0x2092,0x4F00,
/* 'k' */	0x889A,0xE990,
/* 'l' */	0xFE00,
/* 'm' */	0x0035,0x5AD6,0xA000,
/* 'n' */	0x00E9,0x9990,
/* 'o' */	0x0069,0x9960,
/* 'p' */	0x0E99,0xE880,
/* 'q' */	0x0799,0x7110,
/* 'r' */	0x02E9,0x2000,
/* 's' */	0x0078,0x61E0,
/* 't' */	0x44F4,0x4460,
/* 'u' */	0x0099,0x9960,
/* 'v' */	0x0099,0x9660,
/* 'w' */	0x002B,0x5AA9,0x4000,
/* 'x' */	0x0096,0x6990,
/* 'y' */	0x0995,0x2240,
/* 'z' */	0x00F2,0x48F0,
/* '{' */	0x6928,0x9200,
/* '|' */	0xFF00,
/* '}' */	0xC922,0x9200,
/* '~' */	0x5A00,0x0000,
/* 0x7F */	0x0F99,0x99F0,
};

static const u16 MSGothic8_offset[95] = {
    0,    1,    3,    6,    9,   12,   15,   16,   18,   20,   23,   25,   26,   28,   29,   31,
   33,   34,   36,   38,   41,   43,   45,   47,   49,   51,   52,   53,   55,   57,   59,   61,
   64,   66,   68,   70,   72,   74,   76,   78,   80,   81,   83,   85,   87,   90,   92,   94,
   96,   98,  100,  102,  104,  106,  109,  112,  114,  117,  119,  120,  123,  124,  126,  129,
  130,  132,  134,  136,  138,  140,  142,  144,  146,  147,  149,  151,  152,  155,  157,  159,
  161,  163,  165,  167,  169,  171,  173,  176,  178,  180,  182,  184,  185,  187,  189
};

static const u8 MSGothic8_width[95] = {
 2, 3, 5, 5, 5, 5, 2, 3, 3, 5, 3, 2, 4, 2, 4, 4,
 2, 4, 4, 5, 4, 4, 4, 4, 4, 2, 2, 4, 4, 4, 4, 5,
 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 4, 4, 5, 4, 4, 4,
 4, 4, 4, 4, 4, 5, 5, 4, 5, 4, 2, 5, 2, 3, 5, 2,
 4, 4, 4, 4, 4, 4, 4, 4, 1, 3, 4, 1, 5, 4, 4, 4,
 4, 3, 4, 4, 4, 4, 5, 4, 4, 4, 3, 1, 3, 4, 4
};

MSGothic8::MSGothic8(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	MSGothic8_glyphdata,
	MSGothic8_offset,
	MSGothic8_width,
	8,
	2,
	5) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
