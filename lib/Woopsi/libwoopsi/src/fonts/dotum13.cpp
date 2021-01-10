#include "dotum13.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Dotum13_glyphdata[561] = {
/* '!' */	0xFE60,
/* '"' */	0xB680,0x0000,0x0000,
/* '#' */	0x1428,0x53F2,0x850A,0x7E50,0xA140,0x0000,0x0000,
/* '$' */	0x1071,0x5495,0x0705,0x0992,0xA8E0,0x8000,0x0000,
/* '%' */	0x0031,0x2492,0x8640,0x4020,0x2614,0x9248,0xC000,0x0000,
/* '&' */	0x3091,0x2243,0x0A22,0xC585,0x19C8,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x1244,0x8888,0x8442,0x1000,
/* ')' */	0x8422,0x1111,0x1224,0x8000,
/* '*' */	0x0000,0x4F91,0x4000,0x0000,0x0000,
/* '+' */	0x0000,0x4081,0x1FC4,0x0810,0x0000,0x0000,0x0000,
/* ',' */	0x0000,0x1600,
/* '-' */	0x0000,0x0000,0x1FC0,0x0000,0x0000,0x0000,0x0000,
/* '.' */	0x0030,
/* '/' */	0x0410,0x8210,0x4208,0x4108,0x2000,0x0000,
/* '0' */	0x388A,0x0C18,0x3060,0xC182,0x88E0,0x0000,0x0000,
/* '1' */	0x3C92,0x4924,0x8000,
/* '2' */	0x388A,0x0C10,0x2086,0x1041,0x03F8,0x0000,0x0000,
/* '3' */	0x388A,0x0823,0x8080,0x8182,0x88E0,0x0000,0x0000,
/* '4' */	0x0C14,0x1424,0x2444,0x4484,0xFF04,0x0400,0x0000,
/* '5' */	0xFF02,0x040B,0x98A0,0x8103,0x09E0,0x0000,0x0000,
/* '6' */	0x388A,0x0C0B,0x98A0,0xC182,0x88E0,0x0000,0x0000,
/* '7' */	0xFE04,0x1020,0x8102,0x0810,0x2040,0x0000,0x0000,
/* '8' */	0x388A,0x0C14,0x4711,0x4182,0x88E0,0x0000,0x0000,
/* '9' */	0x388A,0x0C18,0x28CE,0x8182,0x88E0,0x0000,0x0000,
/* ':' */	0x30C0,
/* ';' */	0x0500,0x5800,
/* '<' */	0x0421,0x0842,0x0408,0x1020,0x4000,0x0000,
/* '=' */	0x0000,0x000F,0xE000,0x7F00,0x0000,0x0000,0x0000,
/* '>' */	0x8102,0x0408,0x1084,0x2108,0x0000,0x0000,
/* '?' */	0x7A18,0x4108,0x4104,0x0041,0x0000,0x0000,
/* '@' */	0x1F84,0x091A,0xC4D9,0x1322,0x64CC,0x6680,0x0810,0xFC00,0x0000,0x0000,
/* 'A' */	0x1818,0x2424,0x2442,0x427E,0x8181,0x8100,0x0000,
/* 'B' */	0xF884,0x8282,0x84FC,0x8281,0x8182,0xFC00,0x0000,
/* 'C' */	0x3C42,0x8180,0x8080,0x8080,0x8142,0x3C00,0x0000,
/* 'D' */	0xF884,0x8281,0x8181,0x8181,0x8284,0xF800,0x0000,
/* 'E' */	0xFF02,0x0408,0x1FA0,0x4081,0x03F8,0x0000,0x0000,
/* 'F' */	0xFF02,0x0408,0x1FA0,0x4081,0x0200,0x0000,0x0000,
/* 'G' */	0x1E10,0x9030,0x0804,0x3E03,0x0140,0x90C7,0xA000,0x0000,
/* 'H' */	0x8306,0x0C18,0x3FE0,0xC183,0x0608,0x0000,0x0000,
/* 'I' */	0xFFE0,
/* 'J' */	0x0842,0x1084,0x218C,0x5C00,0x0000,
/* 'K' */	0x830A,0x248A,0x1A24,0x4485,0x0A08,0x0000,0x0000,
/* 'L' */	0x8102,0x0408,0x1020,0x4081,0x03F8,0x0000,0x0000,
/* 'M' */	0x8060,0x1C0F,0x03A1,0x6859,0x2649,0x8C63,0x1804,0x0000,0x0000,
/* 'N' */	0xC1C1,0xA1A1,0x9191,0x8989,0x8585,0x8300,0x0000,
/* 'O' */	0x1E08,0x440A,0x0180,0x6018,0x0601,0x4088,0x41E0,0x0000,0x0000,
/* 'P' */	0xFC82,0x8181,0x82FC,0x8080,0x8080,0x8000,0x0000,
/* 'Q' */	0x1E08,0x440A,0x0180,0x6018,0x0609,0x4188,0x61E4,0x0000,0x0000,
/* 'R' */	0xFC82,0x8181,0x82FC,0x8281,0x8181,0x8100,0x0000,
/* 'S' */	0x3C42,0x8140,0x300C,0x0201,0x8142,0x3C00,0x0000,
/* 'T' */	0xFE20,0x4081,0x0204,0x0810,0x2040,0x0000,0x0000,
/* 'U' */	0x8181,0x8181,0x8181,0x8181,0x8142,0x3C00,0x0000,
/* 'V' */	0x8306,0x0A24,0x488A,0x1428,0x2040,0x0000,0x0000,
/* 'W' */	0x820C,0x1060,0x828A,0x2451,0x2288,0xA285,0x1428,0xA082,0x0410,0x0000,0x0000,0x0000,
/* 'X' */	0x80C0,0x5044,0x4140,0x4050,0x4441,0x4060,0x2000,0x0000,
/* 'Y' */	0x80C0,0x5044,0x4220,0xA020,0x1008,0x0402,0x0000,0x0000,
/* 'Z' */	0xFE04,0x1040,0x8208,0x1041,0x03F8,0x0000,0x0000,
/* '[' */	0xF249,0x2492,0x4E00,
/* '\' */	0x4212,0x1090,0x848A,0x2FFF,0x9290,0xA285,0x1418,0xC082,0x0410,0x0000,0x0000,0x0000,
/* ']' */	0xE492,0x4924,0x9E00,
/* '^' */	0x22A2,0x0000,0x0000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0x000F,0xC000,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x0000,0x03C8,0x409F,0x4285,0x09E8,0x0000,0x0000,
/* 'b' */	0x8208,0x2CCA,0x1861,0x872B,0x0000,0x0000,
/* 'c' */	0x0000,0x0E46,0x0820,0x8113,0x8000,0x0000,
/* 'd' */	0x0410,0x4D4E,0x1861,0x8533,0x4000,0x0000,
/* 'e' */	0x0000,0x1E86,0x1FE0,0x8217,0x8000,0x0000,
/* 'f' */	0x344F,0x4444,0x4440,0x0000,
/* 'g' */	0x0000,0x1F86,0x1861,0x85F0,0x6178,0x0000,
/* 'h' */	0x8208,0x2EC6,0x1861,0x8618,0x4000,0x0000,
/* 'i' */	0xDFE0,
/* 'j' */	0x5155,0x5600,
/* 'k' */	0x8208,0x218A,0x4A38,0x9228,0x4000,0x0000,
/* 'l' */	0xFFE0,
/* 'm' */	0x0000,0x0016,0x6CCC,0x4623,0x1188,0xC462,0x2000,0x0000,
/* 'n' */	0x0000,0x2EC6,0x1861,0x8618,0x4000,0x0000,
/* 'o' */	0x0000,0x01C4,0x5060,0xC182,0x88E0,0x0000,0x0000,
/* 'p' */	0x0000,0x2CCA,0x1861,0x872B,0x2080,0x0000,
/* 'q' */	0x0000,0x0D4E,0x1861,0x8533,0x4104,0x0000,
/* 'r' */	0x005D,0x2492,0x0000,
/* 's' */	0x0000,0x1E86,0x0606,0x0617,0x8000,0x0000,
/* 't' */	0x444F,0x4444,0x4430,0x0000,
/* 'u' */	0x0000,0x2186,0x1861,0x8637,0x4000,0x0000,
/* 'v' */	0x0000,0x0418,0x2891,0x1428,0x2040,0x0000,0x0000,
/* 'w' */	0x0000,0x0011,0x188C,0x4554,0xAA55,0x1108,0x8000,0x0000,
/* 'x' */	0x0000,0x0414,0x4504,0x0828,0x8A08,0x0000,0x0000,
/* 'y' */	0x0000,0x0418,0x2891,0x1428,0x2041,0x0C00,0x0000,
/* 'z' */	0x0000,0x3F04,0x2108,0x420F,0xC000,0x0000,
/* '{' */	0x1908,0x4213,0x0421,0x0841,0x8000,
/* '|' */	0xFFF8,
/* '}' */	0xC108,0x4210,0x6421,0x084C,0x0000,
/* '~' */	0x0000,0x0000,0x0003,0xC61C,0x0000,0x0000,0x0000,0x0000,
/* 0x7F */	0x07E3,0x18C6,0x318C,0x7E00,0x0000,
};

static const u16 Dotum13_offset[95] = {
    0,    1,    4,   11,   18,   26,   33,   34,   38,   42,   47,   54,   56,   63,   64,   70,
   77,   80,   87,   94,  101,  108,  115,  122,  129,  136,  137,  139,  145,  152,  158,  164,
  174,  181,  188,  195,  202,  209,  216,  224,  231,  232,  237,  244,  251,  260,  267,  276,
  283,  292,  299,  306,  313,  320,  327,  339,  347,  355,  362,  365,  377,  380,  385,  391,
  393,  400,  406,  412,  418,  424,  428,  434,  440,  441,  443,  449,  450,  458,  464,  471,
  477,  483,  486,  492,  496,  502,  509,  517,  524,  531,  537,  542,  543,  548,  556
};

static const u8 Dotum13_width[95] = {
 1, 3, 7, 7, 9, 7, 1, 4, 4, 5, 7, 2, 7, 1, 6, 7,
 3, 7, 7, 8, 7, 7, 7, 7, 7, 1, 2, 6, 7, 6, 6,11,
 8, 8, 8, 8, 7, 7, 9, 7, 1, 5, 7, 7,10, 8,10, 8,
10, 8, 8, 7, 8, 7,13, 9, 9, 7, 3,13, 3, 5, 6, 2,
 7, 6, 6, 6, 6, 4, 6, 6, 1, 2, 6, 1, 9, 6, 7, 6,
 6, 3, 6, 4, 6, 7, 9, 7, 7, 6, 5, 1, 5, 9, 5
};

Dotum13::Dotum13(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Dotum13_glyphdata,
	Dotum13_offset,
	Dotum13_width,
	14,
	4,
	13) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
