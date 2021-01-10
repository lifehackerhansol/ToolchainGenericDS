#include "modern9.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Modern9_glyphdata[304] = {
/* '!' */	0xFA00,
/* '"' */	0xB400,0x0000,0x0000,
/* '#' */	0x55F5,0xFAA0,0x0000,
/* '$' */	0x7568,0xE2D5,0xC400,0x0000,
/* '%' */	0x4AC5,0x0428,0xD280,0x0000,0x0000,
/* '&' */	0x3924,0x9C96,0x2740,0x0000,0x0000,
/* ''' */	0xC000,
/* '(' */	0x2A49,0x2220,0x0000,
/* ')' */	0x8892,0x4A80,0x0000,
/* '*' */	0xEA80,0x0000,0x0000,
/* '+' */	0x0008,0x4F90,0x8000,0x0000,
/* ',' */	0x0300,
/* '-' */	0x00C0,0x0000,
/* '.' */	0x0200,
/* '/' */	0x2525,0x2000,0x0000,
/* '0' */	0x6999,0x99E0,0x0000,
/* '1' */	0x7554,0x0000,
/* '2' */	0x6912,0x24F0,0x0000,
/* '3' */	0x6912,0x1960,0x0000,
/* '4' */	0x1195,0x2F88,0x4000,0x0000,
/* '5' */	0x74E9,0x1960,0x0000,
/* '6' */	0x69E9,0x9960,0x0000,
/* '7' */	0xF122,0x4440,0x0000,
/* '8' */	0x6996,0x9960,0x0000,
/* '9' */	0x6999,0x7960,0x0000,
/* ':' */	0x2200,
/* ';' */	0x2300,
/* '<' */	0x0544,0x4000,0x0000,
/* '=' */	0x00F0,0xF000,0x0000,
/* '>' */	0x1115,0x0000,0x0000,
/* '?' */	0x7442,0x6200,0x8000,0x0000,
/* '@' */	0x3C42,0x95AD,0xA9AA,0xBC43,0x3C00,0x0000,
/* 'A' */	0x1050,0xA227,0xC8A0,0x8000,0x0000,0x0000,
/* 'B' */	0xE99F,0x99E0,0x0000,
/* 'C' */	0x7461,0x0845,0xC000,0x0000,
/* 'D' */	0xF463,0x18C7,0xC000,0x0000,
/* 'E' */	0xFC21,0xF843,0xE000,0x0000,
/* 'F' */	0xF88E,0x8880,0x0000,
/* 'G' */	0x7461,0x38C5,0xC000,0x0000,
/* 'H' */	0x8C63,0xF8C6,0x2000,0x0000,
/* 'I' */	0xFE00,
/* 'J' */	0x1111,0x19E0,0x0000,
/* 'K' */	0x8CA9,0x8A4A,0x2000,0x0000,
/* 'L' */	0x8888,0x88F0,0x0000,
/* 'M' */	0x838F,0x1D5A,0xB564,0x8000,0x0000,0x0000,
/* 'N' */	0x8E73,0x59CE,0x2000,0x0000,
/* 'O' */	0x7463,0x18C5,0xC000,0x0000,
/* 'P' */	0xF99F,0x8880,0x0000,
/* 'Q' */	0x7463,0x18C9,0xE000,0x0000,
/* 'R' */	0xF463,0xE946,0x2000,0x0000,
/* 'S' */	0x6986,0x1960,0x0000,
/* 'T' */	0xF908,0x4210,0x8000,0x0000,
/* 'U' */	0x8C63,0x18C5,0xC000,0x0000,
/* 'V' */	0x8305,0x1222,0x8504,0x0000,0x0000,0x0000,
/* 'W' */	0x88CA,0x554A,0xA552,0xA888,0x0000,0x0000,0x0000,
/* 'X' */	0x8A94,0x452A,0x2000,0x0000,
/* 'Y' */	0x8A94,0x4210,0x8000,0x0000,
/* 'Z' */	0xFC21,0x0842,0x0FC0,0x0000,0x0000,
/* '[' */	0xEAAA,0xC000,
/* '\' */	0x9124,0x4800,0x0000,
/* ']' */	0xD555,0xC000,
/* '^' */	0x5680,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x00F8,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x00F1,0x79F0,0x0000,
/* 'b' */	0x88E9,0x99E0,0x0000,
/* 'c' */	0x0069,0x8960,0x0000,
/* 'd' */	0x1179,0x9970,0x0000,
/* 'e' */	0x0069,0xF870,0x0000,
/* 'f' */	0x2BA4,0x9000,0x0000,
/* 'g' */	0x0079,0x9979,0x6000,
/* 'h' */	0x88E9,0x9990,0x0000,
/* 'i' */	0xBE00,
/* 'j' */	0xBF00,
/* 'k' */	0x889A,0xEA90,0x0000,
/* 'l' */	0xFE00,
/* 'm' */	0x0003,0xF499,0x3264,0x8000,0x0000,0x0000,
/* 'n' */	0x00E9,0x9990,0x0000,
/* 'o' */	0x0069,0x9960,0x0000,
/* 'p' */	0x00E9,0x99E8,0x8000,
/* 'q' */	0x0079,0x9971,0x1000,
/* 'r' */	0x03C9,0x2000,0x0000,
/* 's' */	0x0078,0x61E0,0x0000,
/* 't' */	0x2EAC,0x0000,
/* 'u' */	0x0099,0x9970,0x0000,
/* 'v' */	0x0022,0xA528,0x8000,0x0000,
/* 'w' */	0x002B,0x5AD5,0x4000,0x0000,
/* 'x' */	0x0096,0x6690,0x0000,
/* 'y' */	0x0022,0xA528,0x8440,0x0000,
/* 'z' */	0x0395,0x3800,0x0000,
/* '{' */	0x6928,0x9260,0x0000,
/* '|' */	0xFF00,
/* '}' */	0xC922,0x92C0,0x0000,
/* '~' */	0x000D,0xB000,0x0000,
/* 0x7F */	0x00F9,0x999F,0x0000,
};

static const u16 Modern9_offset[95] = {
    0,    1,    4,    7,   11,   16,   21,   22,   25,   28,   31,   35,   36,   38,   39,   42,
   45,   47,   50,   53,   57,   60,   63,   66,   69,   72,   73,   74,   77,   80,   83,   87,
   93,   99,  102,  106,  110,  114,  117,  121,  125,  126,  129,  133,  136,  142,  146,  150,
  153,  157,  161,  164,  168,  172,  178,  185,  189,  193,  198,  200,  203,  205,  208,  212,
  214,  217,  220,  223,  226,  229,  232,  235,  238,  239,  240,  243,  244,  250,  253,  256,
  259,  262,  265,  268,  270,  273,  277,  281,  284,  288,  291,  294,  295,  298,  301
};

static const u8 Modern9_width[95] = {
 1, 3, 4, 5, 6, 6, 1, 3, 3, 3, 5, 1, 2, 1, 3, 4,
 2, 4, 4, 5, 4, 4, 4, 4, 4, 1, 1, 3, 4, 3, 5, 8,
 7, 4, 5, 5, 5, 4, 5, 5, 1, 4, 5, 4, 7, 5, 5, 4,
 5, 5, 4, 5, 5, 7, 9, 5, 5, 6, 2, 3, 2, 3, 5, 2,
 4, 4, 4, 4, 4, 3, 4, 4, 1, 1, 4, 1, 7, 4, 4, 4,
 4, 3, 4, 2, 4, 5, 5, 4, 5, 3, 3, 1, 3, 4, 4
};

Modern9::Modern9(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Modern9_glyphdata,
	Modern9_offset,
	Modern9_width,
	12,
	3,
	9) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
