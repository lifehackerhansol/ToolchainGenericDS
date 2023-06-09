#include "mssans9b.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 MSSans9b_glyphdata[383] = {
/* '!' */	0xFFCC,0x0000,
/* '"' */	0xFF00,0x0000,0x0000,
/* '#' */	0x7BFF,0xBFFB,0xC000,0x0000,
/* '$' */	0x33FD,0xE73D,0xFE60,0x0000,
/* '%' */	0x67F9,0xE183,0xCFF3,0x0000,0x0000,0x0000,
/* '&' */	0x67BC,0xCFF9,0xE000,0x0000,
/* ''' */	0xF000,0x0000,
/* '(' */	0x7B6D,0xB660,0x0000,
/* ')' */	0xCDB6,0xDBC0,0x0000,
/* '*' */	0xF6F0,0x0000,0x0000,
/* '+' */	0x00C3,0x3F30,0xC000,0x0000,0x0000,
/* ',' */	0x0000,0x1E00,0x0000,
/* '-' */	0x0000,0xF000,0x0000,
/* '.' */	0x000C,0x0000,
/* '/' */	0x3366,0x6CC0,0x0000,
/* '0' */	0x76F7,0xBDED,0xC000,0x0000,
/* '1' */	0x7DB6,0xD800,0x0000,
/* '2' */	0x76C6,0x6663,0xE000,0x0000,
/* '3' */	0x76C6,0x61ED,0xC000,0x0000,
/* '4' */	0x18E7,0xB6FC,0x6180,0x0000,0x0000,
/* '5' */	0xFE3C,0x31ED,0xC000,0x0000,
/* '6' */	0x76F1,0xEDED,0xC000,0x0000,
/* '7' */	0xF8CC,0x6331,0x8000,0x0000,
/* '8' */	0x76F6,0xEDED,0xC000,0x0000,
/* '9' */	0x76F6,0xF1ED,0xC000,0x0000,
/* ':' */	0x0C0C,0x0000,
/* ';' */	0x0180,0x1E00,0x0000,
/* '<' */	0x00DD,0x870C,0x0000,0x0000,
/* '=' */	0x003E,0x0F80,0x0000,0x0000,
/* '>' */	0x061C,0x3760,0x0000,0x0000,
/* '?' */	0x76C6,0x6300,0xC000,0x0000,
/* '@' */	0x3E31,0xB7FE,0xFF7E,0xF980,0x7E00,0x0000,0x0000,
/* 'A' */	0x183C,0x3C66,0x7EC3,0xC300,0x0000,0x0000,
/* 'B' */	0xF6F7,0xEDEF,0xC000,0x0000,
/* 'C' */	0x7B3C,0x30C3,0x3780,0x0000,0x0000,
/* 'D' */	0xF36C,0xF3CF,0x6F00,0x0000,0x0000,
/* 'E' */	0xFE31,0xFC63,0xE000,0x0000,
/* 'F' */	0xFE31,0xFC63,0x0000,0x0000,
/* 'G' */	0x7B3C,0x37CF,0x37C0,0x0000,0x0000,
/* 'H' */	0xCF3C,0xFFCF,0x3CC0,0x0000,0x0000,
/* 'I' */	0xFFFC,0x0000,
/* 'J' */	0x18C6,0x31ED,0xC000,0x0000,
/* 'K' */	0xCF6F,0x38F3,0x6CC0,0x0000,0x0000,
/* 'L' */	0xC631,0x8C63,0xE000,0x0000,
/* 'M' */	0xC3C3,0xE7FF,0xFFDB,0xC300,0x0000,0x0000,
/* 'N' */	0xCFBE,0xFFDF,0x7CC0,0x0000,0x0000,
/* 'O' */	0x7B3C,0xF3CF,0x3780,0x0000,0x0000,
/* 'P' */	0xFB3C,0xFEC3,0x0C00,0x0000,0x0000,
/* 'Q' */	0x7B3C,0xF3CF,0x7783,0x0000,0x0000,
/* 'R' */	0xFB3C,0xFEDB,0x3CC0,0x0000,0x0000,
/* 'S' */	0x76F0,0xE1ED,0xC000,0x0000,
/* 'T' */	0xFCC3,0x0C30,0xC300,0x0000,0x0000,
/* 'U' */	0xCF3C,0xF3CF,0x3780,0x0000,0x0000,
/* 'V' */	0xC3C3,0x6666,0x3C3C,0x1800,0x0000,0x0000,
/* 'W' */	0xCCF3,0x37F9,0xFE33,0x0CC3,0x3000,0x0000,0x0000,0x0000,
/* 'X' */	0xC6D8,0xE1C3,0x8DB1,0x8000,0x0000,0x0000,
/* 'Y' */	0xCF37,0x8C30,0xC300,0x0000,0x0000,
/* 'Z' */	0xF8CC,0xC663,0xE000,0x0000,
/* '[' */	0xFB6D,0xB6E0,0x0000,
/* '\' */	0xCC66,0x6330,0x0000,
/* ']' */	0xEDB6,0xDBE0,0x0000,
/* '^' */	0x31EC,0xC000,0x0000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0xFC00,0x0000,
/* '`' */	0xCC00,0x0000,0x0000,
/* 'a' */	0x003C,0x37ED,0xE000,0x0000,
/* 'b' */	0xC63D,0xBDEF,0xC000,0x0000,
/* 'c' */	0x001D,0xBC6D,0xC000,0x0000,
/* 'd' */	0x18DF,0xBDED,0xE000,0x0000,
/* 'e' */	0x001D,0xBFE1,0xE000,0x0000,
/* 'f' */	0x7BED,0xB000,0x0000,
/* 'g' */	0x001F,0xBDED,0xE3F0,0x0000,
/* 'h' */	0xC63D,0xBDEF,0x6000,0x0000,
/* 'i' */	0xCFFC,0x0000,
/* 'j' */	0xCFFF,0x8000,
/* 'k' */	0xC637,0xEE7B,0x6000,0x0000,
/* 'l' */	0xFFFC,0x0000,
/* 'm' */	0x0000,0xFEDB,0xDBDB,0xDB00,0x0000,0x0000,
/* 'n' */	0x003D,0xBDEF,0x6000,0x0000,
/* 'o' */	0x001D,0xBDED,0xC000,0x0000,
/* 'p' */	0x003D,0xBDEF,0xD8C0,0x0000,
/* 'q' */	0x001F,0xBDED,0xE318,0x0000,
/* 'r' */	0x03ED,0xB000,0x0000,
/* 's' */	0x001F,0x870F,0xC000,0x0000,
/* 't' */	0x1BED,0xB800,0x0000,
/* 'u' */	0x0037,0xBDED,0xE000,0x0000,
/* 'v' */	0x000C,0xF379,0xE300,0x0000,0x0000,
/* 'w' */	0x0000,0xDBDB,0xFF66,0x6600,0x0000,0x0000,
/* 'x' */	0x0037,0xB76F,0x6000,0x0000,
/* 'y' */	0x0037,0xBDB9,0x8CC0,0x0000,
/* 'z' */	0x003E,0x3763,0xE000,0x0000,
/* '{' */	0x3666,0xC666,0x3000,
/* '|' */	0xFFFF,0x0000,
/* '}' */	0xC666,0x3666,0xC000,
/* '~' */	0x7F80,0x0000,0x0000,0x0000,
/* 0x7F */	0x07F7,0xBDEF,0xE000,0x0000,
};

static const u16 MSSans9b_offset[95] = {
    0,    2,    5,    9,   13,   19,   23,   25,   28,   31,   34,   39,   42,   45,   47,   50,
   54,   57,   61,   65,   70,   74,   78,   82,   86,   90,   92,   95,   99,  103,  107,  111,
  118,  124,  128,  133,  138,  142,  146,  151,  156,  158,  162,  167,  171,  177,  182,  187,
  192,  197,  202,  206,  211,  216,  222,  230,  236,  241,  245,  248,  251,  254,  259,  264,
  267,  271,  275,  279,  283,  287,  290,  294,  298,  300,  302,  306,  308,  314,  318,  322,
  326,  330,  333,  337,  340,  344,  349,  355,  359,  363,  367,  370,  372,  375,  379
};

static const u8 MSSans9b_width[95] = {
 2, 4, 5, 5, 7, 5, 2, 3, 3, 4, 6, 3, 4, 2, 4, 5,
 3, 5, 5, 6, 5, 5, 5, 5, 5, 2, 3, 5, 5, 5, 5, 9,
 8, 5, 6, 6, 5, 5, 6, 6, 2, 5, 6, 5, 8, 6, 6, 6,
 6, 6, 5, 6, 6, 8,10, 7, 6, 5, 3, 4, 3, 6, 6, 3,
 5, 5, 5, 5, 5, 3, 5, 5, 2, 2, 5, 2, 8, 5, 5, 5,
 5, 3, 5, 3, 5, 6, 8, 5, 5, 5, 4, 2, 4, 5, 5
};

MSSans9b::MSSans9b(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	MSSans9b_glyphdata,
	MSSans9b_offset,
	MSSans9b_width,
	12,
	3,
	10) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
