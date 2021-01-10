#include "tunga8.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Tunga8_glyphdata[210] = {
/* '!' */	0xFA00,
/* '"' */	0xB400,0x0000,
/* '#' */	0x55F5,0xFAA0,
/* '$' */	0x7568,0xE2D5,0xC400,
/* '%' */	0x4AC5,0x0428,0xD280,
/* '&' */	0x3924,0x9C96,0x2740,
/* ''' */	0xC000,
/* '(' */	0x2A49,0x2200,
/* ')' */	0x8892,0x4A00,
/* '*' */	0xEA80,0x0000,
/* '+' */	0x0008,0x4F90,0x8000,
/* ',' */	0x0600,
/* '-' */	0x00C0,
/* '.' */	0x0200,
/* '/' */	0x2525,0x2000,
/* '0' */	0x6999,0x99E0,
/* '1' */	0x7554,
/* '2' */	0x6912,0x24F0,
/* '3' */	0x6912,0x1960,
/* '4' */	0x1195,0x2F88,0x4000,
/* '5' */	0x74E9,0x1960,
/* '6' */	0x69E9,0x9960,
/* '7' */	0xF122,0x4440,
/* '8' */	0x6996,0x9960,
/* '9' */	0x6999,0x7960,
/* ':' */	0x2200,
/* ';' */	0x4600,
/* '<' */	0x0544,0x4000,
/* '=' */	0x00F0,0xF000,
/* '>' */	0x1115,0x0000,
/* '?' */	0x7442,0x6200,0x8000,
/* '@' */	0x3C42,0x95AD,0xA9AA,0xBC43,
/* 'A' */	0x1050,0xA227,0xC8A0,0x8000,
/* 'B' */	0xE99F,0x99E0,
/* 'C' */	0x7461,0x0845,0xC000,
/* 'D' */	0xF463,0x18C7,0xC000,
/* 'E' */	0xFC21,0xF843,0xE000,
/* 'F' */	0xF88E,0x8880,
/* 'G' */	0x7461,0x38C5,0xC000,
/* 'H' */	0x8C63,0xF8C6,0x2000,
/* 'I' */	0xFE00,
/* 'J' */	0x1111,0x19E0,
/* 'K' */	0x8CA9,0x8A4A,0x2000,
/* 'L' */	0x8888,0x88F0,
/* 'M' */	0x838F,0x1D5A,0xB564,0x8000,
/* 'N' */	0x8E73,0x59CE,0x2000,
/* 'O' */	0x7463,0x18C5,0xC000,
/* 'P' */	0xF99F,0x8880,
/* 'Q' */	0x7463,0x18C9,0xE000,
/* 'R' */	0xF463,0xE946,0x2000,
/* 'S' */	0x6986,0x1960,
/* 'T' */	0xF908,0x4210,0x8000,
/* 'U' */	0x8C63,0x18C5,0xC000,
/* 'V' */	0x8305,0x1222,0x8504,0x0000,
/* 'W' */	0x8894,0x5555,0x5555,0x2200,
/* 'X' */	0x8A94,0x452A,0x2000,
/* 'Y' */	0x8A94,0x4210,0x8000,
/* 'Z' */	0xFC21,0x0842,0x0FC0,
/* '[' */	0xEAAA,
/* '\' */	0x9124,0x4800,
/* ']' */	0xD555,
/* '^' */	0x5680,0x0000,
/* '_' */	0x0000,0x0003,0xE000,
/* '`' */	0x9000,
/* 'a' */	0x00F1,0x79F0,
/* 'b' */	0x88E9,0x99E0,
/* 'c' */	0x0069,0x8960,
/* 'd' */	0x1179,0x9970,
/* 'e' */	0x0069,0xF870,
/* 'f' */	0x2BA4,0x9000,
/* 'g' */	0x7999,0x7960,
/* 'h' */	0x88E9,0x9990,
/* 'i' */	0xBE00,
/* 'j' */	0xBF00,
/* 'k' */	0x889A,0xEA90,
/* 'l' */	0xFE00,
/* 'm' */	0x0003,0xF499,0x3264,0x8000,
/* 'n' */	0x00E9,0x9990,
/* 'o' */	0x0069,0x9960,
/* 'p' */	0xE999,0xE880,
/* 'q' */	0x7999,0x7110,
/* 'r' */	0x03C9,0x2000,
/* 's' */	0x0078,0x61E0,
/* 't' */	0x2EAC,
/* 'u' */	0x0099,0x9970,
/* 'v' */	0x0022,0xA528,0x8000,
/* 'w' */	0x002B,0x5AD5,0x4000,
/* 'x' */	0x0096,0x6690,
/* 'y' */	0x8A94,0xA211,0x0000,
/* 'z' */	0x0395,0x3800,
/* '{' */	0x6928,0x9200,
/* '|' */	0xFF00,
/* '}' */	0xC922,0x9200,
/* '~' */	0x000D,0xB000,
/* 0x7F */	0x0F99,0x99F0,
};

static const u16 Tunga8_offset[95] = {
    0,    1,    3,    5,    8,   11,   14,   15,   17,   19,   21,   24,   25,   26,   27,   29,
   31,   32,   34,   36,   39,   41,   43,   45,   47,   49,   50,   51,   53,   55,   57,   60,
   64,   68,   70,   73,   76,   79,   81,   84,   87,   88,   90,   93,   95,   99,  102,  105,
  107,  110,  113,  115,  118,  121,  125,  129,  132,  135,  138,  139,  141,  142,  144,  147,
  148,  150,  152,  154,  156,  158,  160,  162,  164,  165,  166,  168,  169,  173,  175,  177,
  179,  181,  183,  185,  186,  188,  191,  194,  196,  199,  201,  203,  204,  206,  208
};

static const u8 Tunga8_width[95] = {
 1, 3, 4, 5, 6, 6, 1, 3, 3, 3, 5, 1, 2, 1, 3, 4,
 2, 4, 4, 5, 4, 4, 4, 4, 4, 1, 1, 3, 4, 3, 5, 8,
 7, 4, 5, 5, 5, 4, 5, 5, 1, 4, 5, 4, 7, 5, 5, 4,
 5, 5, 4, 5, 5, 7, 8, 5, 5, 6, 2, 3, 2, 3, 5, 2,
 4, 4, 4, 4, 4, 3, 4, 4, 1, 1, 4, 1, 7, 4, 4, 4,
 4, 3, 4, 2, 4, 5, 5, 4, 5, 3, 3, 1, 3, 4, 4
};

Tunga8::Tunga8(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Tunga8_glyphdata,
	Tunga8_offset,
	Tunga8_width,
	8,
	2,
	8) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
