#include "goudy9.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Goudy9_glyphdata[349] = {
/* '!' */	0xFA00,
/* '"' */	0xB680,0x0000,0x0000,
/* '#' */	0x02BE,0xAA7A,0x8000,0x0000,
/* '$' */	0x23EB,0x4716,0xBE20,0x0000,
/* '%' */	0x42A4,0xA8EA,0x1525,0x4700,0x0000,0x0000,
/* '&' */	0x7040,0x406C,0xB48C,0x7300,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x5AA5,0x0000,
/* ')' */	0xA55A,0x0000,
/* '*' */	0xEA80,0x0000,0x0000,
/* '+' */	0x0109,0xF210,0x0000,0x0000,
/* ',' */	0x0009,0x8000,
/* '-' */	0x0060,0x0000,
/* '.' */	0x0200,
/* '/' */	0x0884,0x4221,0x1000,0x0000,
/* '0' */	0x6999,0x9D60,0x0000,
/* '1' */	0x5924,0xB800,0x0000,
/* '2' */	0x6A24,0x49E0,0x0000,
/* '3' */	0x7532,0x7000,0x0000,
/* '4' */	0x026A,0xF222,0x0000,
/* '5' */	0x3471,0x13E0,0x0000,
/* '6' */	0x164E,0x9960,0x0000,
/* '7' */	0xF922,0x4444,0x0000,
/* '8' */	0x7996,0x9960,0x0000,
/* '9' */	0x6997,0x36C0,0x0000,
/* ':' */	0x1200,
/* ';' */	0x010D,0x8000,
/* '<' */	0x012C,0x6100,0x0000,
/* '=' */	0x00F0,0xF000,0x0000,
/* '>' */	0x0861,0x6800,0x0000,
/* '?' */	0xE4A4,0x1000,0x0000,
/* '@' */	0x3C8E,0x6D9B,0xC80F,0x0000,0x0000,0x0000,
/* 'A' */	0x1050,0xA1C4,0x48BB,0x8000,0x0000,0x0000,
/* 'B' */	0xF294,0xE4A7,0xC000,0x0000,
/* 'C' */	0x3938,0x6081,0x1780,0x0000,0x0000,
/* 'D' */	0xF889,0x0A14,0x28BE,0x0000,0x0000,0x0000,
/* 'E' */	0xFA54,0xE527,0xC000,0x0000,
/* 'F' */	0xFA54,0xE523,0x8000,0x0000,
/* 'G' */	0x3C8A,0x0478,0x589E,0x0000,0x0000,0x0000,
/* 'H' */	0xEE89,0x13E4,0x48BB,0x8000,0x0000,0x0000,
/* 'I' */	0xE924,0xB800,0x0000,
/* 'J' */	0xE924,0x9280,0x0000,
/* 'K' */	0xEE99,0x6384,0x88B9,0x8000,0x0000,0x0000,
/* 'L' */	0xE210,0x8425,0xE000,0x0000,
/* 'M' */	0xC1A0,0x98CC,0xA552,0xABAE,0x0000,0x0000,0x0000,
/* 'N' */	0x8E89,0x9325,0x49B9,0x0000,0x0000,0x0000,
/* 'O' */	0x388A,0x0C18,0x288E,0x0000,0x0000,0x0000,
/* 'P' */	0xF252,0xB423,0x8000,0x0000,
/* 'Q' */	0x388A,0x0C18,0x389E,0x2406,0x0000,0x0000,
/* 'R' */	0xF924,0x9C59,0x2E40,0x0000,0x0000,
/* 'S' */	0x7986,0x19E0,0x0000,
/* 'T' */	0xFF24,0x4081,0x020E,0x0000,0x0000,0x0000,
/* 'U' */	0xE742,0x4242,0x4246,0x3C00,0x0000,0x0000,
/* 'V' */	0xEE89,0x9142,0x8604,0x0000,0x0000,0x0000,
/* 'W' */	0xFFAA,0x9649,0x42A1,0xB090,0x0000,0x0000,0x0000,
/* 'X' */	0xDD23,0x0851,0x2EC0,0x0000,0x0000,
/* 'Y' */	0x9E25,0x0820,0x8700,0x0000,0x0000,
/* 'Z' */	0xF924,0x49F0,0x0000,
/* '[' */	0xEAAB,0x0000,
/* '\' */	0x8444,0x2221,0x0000,
/* ']' */	0xD557,0x0000,
/* '^' */	0x2315,0x1000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x1F00,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x000E,0x2EF0,0x0000,
/* 'b' */	0x888F,0x99E0,0x0000,
/* 'c' */	0x003B,0x3800,0x0000,
/* 'd' */	0x3084,0xE949,0xE000,0x0000,
/* 'e' */	0x0006,0xF870,0x0000,
/* 'f' */	0x354E,0x44E0,0x0000,
/* 'g' */	0x000F,0xAE86,0xE000,
/* 'h' */	0xC104,0x1E49,0x2EC0,0x0000,0x0000,
/* 'i' */	0x082C,0xB800,0x0000,
/* 'j' */	0x11D5,0x8000,
/* 'k' */	0xC210,0xB63B,0xE000,0x0000,
/* 'l' */	0x5924,0xB800,0x0000,
/* 'm' */	0x0000,0x07E5,0x4ABF,0x8000,0x0000,0x0000,
/* 'n' */	0x0000,0x3E49,0x2EC0,0x0000,0x0000,
/* 'o' */	0x0006,0x9960,0x0000,
/* 'p' */	0x000F,0x99E8,0xC000,
/* 'q' */	0x0004,0xE94B,0xC238,0x0000,
/* 'r' */	0x0074,0xB800,0x0000,
/* 's' */	0x007C,0xF800,0x0000,
/* 't' */	0x0174,0x9800,0x0000,
/* 'u' */	0x0001,0xA529,0xE000,0x0000,
/* 'v' */	0x000F,0xAA40,0x0000,
/* 'w' */	0x0000,0x2BAA,0xA500,0x0000,0x0000,
/* 'x' */	0x0001,0xB73B,0x6000,0x0000,
/* 'y' */	0x000B,0xAA4C,0x8000,
/* 'z' */	0x0075,0x3800,0x0000,
/* '{' */	0x6944,0x9300,0x0000,
/* '|' */	0xFF80,
/* '}' */	0xC914,0x9600,0x0000,
/* '~' */	0xDB00,0x0000,0x0000,
/* 0x7F */	0xF999,0x9999,0xF000,
};

static const u16 Goudy9_offset[95] = {
    0,    1,    4,    8,   12,   18,   24,   25,   27,   29,   32,   36,   38,   40,   41,   45,
   48,   51,   54,   57,   60,   63,   66,   69,   72,   75,   76,   78,   81,   84,   87,   90,
   96,  102,  106,  111,  117,  121,  125,  131,  137,  140,  143,  149,  153,  160,  166,  172,
  176,  182,  187,  190,  196,  202,  208,  215,  220,  225,  228,  230,  233,  235,  239,  243,
  245,  248,  251,  254,  258,  261,  264,  267,  272,  275,  277,  281,  284,  290,  295,  298,
  301,  305,  308,  311,  314,  318,  321,  326,  330,  333,  336,  339,  340,  343,  346
};

static const u8 Goudy9_width[95] = {
 1, 3, 5, 5, 8, 8, 1, 2, 2, 3, 5, 2, 2, 1, 5, 4,
 3, 4, 3, 4, 4, 4, 4, 4, 4, 1, 2, 4, 4, 4, 3, 7,
 7, 5, 6, 7, 5, 5, 7, 7, 3, 3, 7, 5, 9, 7, 7, 5,
 7, 6, 4, 7, 8, 7, 9, 6, 6, 4, 2, 4, 2, 5, 5, 2,
 4, 4, 3, 5, 4, 4, 4, 6, 3, 2, 5, 3, 7, 6, 4, 4,
 5, 3, 3, 3, 5, 4, 6, 5, 4, 3, 3, 1, 3, 4, 4
};

Goudy9::Goudy9(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Goudy9_glyphdata,
	Goudy9_offset,
	Goudy9_width,
	12,
	3,
	9) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
