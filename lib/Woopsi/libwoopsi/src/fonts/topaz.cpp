#include "topaz.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Topaz_glyphdata[484] = {
/* 'A' */	0x0006,0x3180,
/* 'B' */	0x0006,0xDB00,
/* 'C' */	0x0001,0xB36F,0xE000,
/* 'D' */	0x0000,0x61F6,0x0000,
/* 'E' */	0x0000,0x033A,0xC000,
/* 'F' */	0x0003,0x9B68,
/* 'G' */	0x0006,0x3300,
/* 'H' */	0x0000,0xC630,
/* 'I' */	0x0003,0x060C,
/* 'J' */	0x0000,0x0333,0xC000,
/* 'K' */	0x0000,0x3180,
/* 'L' */	0x0000,
/* 'M' */	0x0000,
/* 'N' */	0x0000,
/* 'O' */	0x0000,0x0306,0x0C00,
/* 'P' */	0x0000,0xF336,0xE000,
/* 'Q' */	0x0006,0x7780,
/* 'R' */	0x0000,0xF330,0x6000,
/* 'S' */	0x0000,0xF330,0x6000,
/* 'T' */	0x0001,0xCF6C,
/* 'U' */	0x0001,0xFB07,0xC000,
/* 'V' */	0x0001,0xCC60,
/* 'W' */	0x0001,0xF830,0x6000,
/* 'X' */	0x0000,0xF336,0x6000,
/* 'Y' */	0x0000,0xF336,0x6000,
/* 'Z' */	0x0000,0x3180,
/* '[' */	0x0000,0x3180,
/* '\' */	0x0000,0x0031,0x8000,
/* ']' */	0x0000,0x0007,0xE000,
/* '^' */	0x0000,0xC180,
/* '_' */	0x0000,0xF330,0x6000,
/* '`' */	0x0000,
/* 'a' */	0x18C0,0x3000,
/* 'b' */	0x0000,
/* 'c' */	0x6DFD,0xB360,0x0000,
/* 'd' */	0x3C0D,0xF0C0,0x0000,
/* 'e' */	0xD86D,0xAE60,0x0000,
/* 'f' */	0x76DC,0xCE7B,0x0000,
/* 'g' */	0x0000,
/* 'h' */	0x30C1,0x8300,
/* 'i' */	0x0C31,0x8C00,
/* 'j' */	0xFF3C,0x6600,0x0000,
/* 'k' */	0x7E30,0x6000,0x0000,
/* 'l' */	0x0006,0x3300,
/* 'm' */	0x7E00,0x0000,0x0000,
/* 'n' */	0x0006,0x3000,
/* 'o' */	0x1999,0x8000,
/* 'p' */	0x7EED,0x99E0,0x0000,
/* 'q' */	0x18C6,0x3000,
/* 'r' */	0x0C30,0xC3F0,0x0000,
/* 's' */	0x1C0D,0x99E0,0x0000,
/* 't' */	0xCDFC,0x3060,0x0000,
/* 'u' */	0x060D,0x99E0,0x0000,
/* 'v' */	0x7CCD,0x99E0,0x0000,
/* 'w' */	0x0C61,0x8600,
/* 'x' */	0x3CCD,0x99E0,0x0000,
/* 'y' */	0x3E0C,0x31C0,0x0000,
/* 'z' */	0x0006,0x3000,
/* '{' */	0x0006,0x3300,
/* '|' */	0x6030,0x1800,0x0000,
/* '}' */	0x00FC,0x0000,0x0000,
/* '~' */	0x0631,0x8000,0x0000,
/* 0x7F */	0x0C60,0x0600,
/* 0x80 */	0x0001,0xF63D,0xE000,
/* 0x81 */	0x0000,0xF336,0x6000,
/* 0x82 */	0x0001,0xF336,0x6000,
/* 0x83 */	0x0000,0x7986,0x0000,
/* 0x84 */	0x0001,0xE366,0x6000,
/* 0x85 */	0x0001,0xFB06,0x0000,
/* 0x86 */	0x0001,0xFB06,0x0000,
/* 0x87 */	0x0000,0xF336,0x0000,
/* 0x88 */	0x0001,0x9B36,0x6000,
/* 0x89 */	0x0003,0xC618,
/* 0x8A */	0x0000,0x1830,0x6000,
/* 0x8B */	0x0003,0x1E6D,0x8000,
/* 0x8C */	0x01B6,
/* 0x8D */	0x0003,0x1F7F,0xE000,
/* 0x8E */	0x0003,0x1F3F,0x6000,
/* 0x8F */	0x0000,0xF336,0x6000,
/* 0x90 */	0x0001,0xF336,0x6000,
/* 0x91 */	0x0007,0xB3CC,
/* 0x92 */	0x0001,0xF336,0x6000,
/* 0x93 */	0x0000,0xF337,0x0000,
/* 0x94 */	0x0001,0xF8C1,0x8000,
/* 0x95 */	0x0001,0x9B36,0x6000,
/* 0x96 */	0x0001,0x9B36,0x6000,
/* 0x97 */	0x0003,0x1E3C,0x6000,
/* 0x98 */	0x0000,0xC366,0x3C00,
/* 0x99 */	0x0000,0xC366,0x3C00,
/* 0x9A */	0x0003,0xF861,0x8000,
/* 0x9B */	0x0003,0xCC30,
/* 0x9C */	0x00C6,0x3000,
/* 0x9D */	0x0003,0xC30C,
/* 0x9E */	0x0001,0x0E6C,
/* 0x9F */	0x0000,
/* 0xA0 */	0xD7BF,0x03C0,0x0000,
/* 0xA1 */	0x7ECD,0x9B30,0x0000,
/* 0xA2 */	0x7CCD,0x9BE0,0x0000,
/* 0xA3 */	0x60C0,0xC0F0,0x0000,
/* 0xA4 */	0x66CD,0xB3C0,0x0000,
/* 0xA5 */	0x78C1,0x83F0,0x0000,
/* 0xA6 */	0x7B18,0xC000,
/* 0xA7 */	0x6ECD,0x99F0,0x0000,
/* 0xA8 */	0x7ECD,0x9B30,0x0000,
/* 0xA9 */	0x1861,0x8F00,
/* 0xAA */	0x060D,0x99E0,0x0000,
/* 0xAB */	0xF1B3,0x3630,0x0000,
/* 0xAC */	0x60C1,0x83F0,0x0000,
/* 0xAD */	0xD78F,0x1E30,0x0000,
/* 0xAE */	0xDF9F,0x1E30,0x0000,
/* 0xAF */	0x66CD,0x99E0,0x0000,
/* 0xB0 */	0x7D86,0x1800,
/* 0xB1 */	0xCD9B,0x73F0,0x0000,
/* 0xB2 */	0x7CD9,0x9B30,0x0000,
/* 0xB3 */	0x3C1D,0x99E0,0x0000,
/* 0xB4 */	0x18C6,0x3000,
/* 0xB5 */	0x66CD,0x99E0,0x0000,
/* 0xB6 */	0x6678,0xF0C0,0x0000,
/* 0xB7 */	0xD7FF,0xBE30,0x0000,
/* 0xB8 */	0x183C,0x66C3,0x0000,
/* 0xB9 */	0x18C6,0x3000,
/* 0xBA */	0x30C3,0x07F0,0x0000,
/* 0xBB */	0x30C3,0x0F00,
/* 0xBC */	0x180C,0x0603,0x0000,
/* 0xBD */	0x0C30,0xCF00,
/* 0xBE */	0xC600,0x0000,0x0000,
/* 0xBF */	0x0000,0x000F,0xE000,
/* 0xC0 */	0x0001,0x860C,
/* 0xC1 */	0x0000,0x003C,
/* 0xC2 */	0x0006,0x187C,
/* 0xC3 */	0x0000,0x003C,
/* 0xC4 */	0x0000,0x1833,0xE000,
/* 0xC5 */	0x0000,0x003C,
/* 0xC6 */	0x0001,0xCC7C,
/* 0xC7 */	0x0000,0x0003,0xE000,
/* 0xC8 */	0x0006,0x187C,
/* 0xC9 */	0x0006,0x0180,
/* 0xCA */	0x0000,0xC00C,
/* 0xCB */	0x0001,0x8306,0x6000,
/* 0xCC */	0x0006,0x3180,
/* 0xCD */	0x0000,0x00EC,
/* 0xCE */	0x0000,0x007C,
/* 0xCF */	0x0000,0x003C,
/* 0xD0 */	0x0000,0x007C,
/* 0xD1 */	0x0000,0x0003,0xE000,
/* 0xD2 */	0x0000,0x007C,
/* 0xD3 */	0x0000,0x003C,
/* 0xD4 */	0x0003,0x0C7C,
/* 0xD5 */	0x0000,0x0006,0x6000,
/* 0xD6 */	0x0000,0x0006,0x6000,
/* 0xD7 */	0x0000,0x000C,0x6000,
/* 0xD8 */	0x0000,0x000C,0x6000,
/* 0xD9 */	0x0000,0x0006,0x6000,
/* 0xDA */	0x0000,0x0007,0xE000,
/* 0xDB */	0x0000,0x38C1,0x8000,
/* 0xDC */	0x0006,0x3180,
/* 0xDD */	0x001C,0x3180,
/* 0xDE */	0x0001,0xCCE0,0x0000,
/* 0xDF */	0x0000,
/* 0xE0 */	0x0000,
/* 0xE1 */	0x067D,0x99F0,0x0000,
/* 0xE2 */	0x66CD,0x9BE0,0x0000,
/* 0xE3 */	0x6186,0x0F00,
/* 0xE4 */	0x66CD,0x99F0,0x0000,
/* 0xE5 */	0x66FD,0x81E0,0x0000,
/* 0xE6 */	0x3333,0x0000,
/* 0xE7 */	0x66CC,0xF833,0xC000,
/* 0xE8 */	0x66CD,0x9B30,0x0000,
/* 0xE9 */	0x1861,0x8300,
/* 0xEA */	0x0C30,0xC378,
/* 0xEB */	0x6CF1,0xB330,0x0000,
/* 0xEC */	0x1861,0x8300,
/* 0xED */	0xFFAF,0x1E30,0x0000,
/* 0xEE */	0x66CD,0x9B30,0x0000,
/* 0xEF */	0x66CD,0x99E0,0x0000,
/* 0xF0 */	0x66CD,0xF306,0x0000,
/* 0xF1 */	0x66CC,0xF830,0x6000,
/* 0xF2 */	0x66C1,0x8300,0x0000,
/* 0xF3 */	0x6078,0x1BE0,0x0000,
/* 0xF4 */	0x30C3,0x0700,
/* 0xF5 */	0x66CD,0x99F0,0x0000,
/* 0xF6 */	0x66CC,0xF0C0,0x0000,
/* 0xF7 */	0xC7AF,0xFB60,0x0000,
/* 0xF8 */	0x6C71,0xB630,0x0000,
/* 0xF9 */	0x66CC,0xF0C3,0x0000,
/* 0xFA */	0x0C30,0xC3F0,0x0000,
/* 0xFB */	0x7030,0x6070,0x0000,
/* 0xFC */	0x18C6,0x3000,
/* 0xFD */	0x0E30,0x6380,0x0000,
};

static const u16 Topaz_offset[189] = {
    0,    2,    4,    7,   10,   13,   15,   17,   19,   21,   24,   26,   27,   28,   29,   32,
   35,   37,   40,   43,   45,   48,   50,   53,   56,   59,   61,   63,   66,   69,   71,   74,
   75,   77,   78,   81,   84,   87,   90,   91,   93,   95,   98,  101,  103,  106,  108,  110,
  113,  115,  118,  121,  124,  127,  130,  132,  135,  138,  140,  142,  145,  148,  151,  153,
  156,  159,  162,  165,  168,  171,  174,  177,  180,  182,  185,  188,  189,  192,  195,  198,
  201,  203,  206,  209,  212,  215,  218,  221,  224,  227,  230,  232,  234,  236,  238,  239,
  242,  245,  248,  251,  254,  257,  259,  262,  265,  267,  270,  273,  276,  279,  282,  285,
  287,  290,  293,  296,  298,  301,  304,  307,  310,  312,  315,  317,  320,  322,  325,  328,
  330,  332,  334,  336,  339,  341,  343,  346,  348,  350,  352,  355,  357,  359,  361,  363,
  365,  368,  370,  372,  374,  377,  380,  383,  386,  389,  392,  395,  397,  399,  402,  403,
  404,  407,  410,  412,  415,  418,  420,  423,  426,  428,  430,  433,  435,  438,  441,  444,
  447,  450,  453,  456,  458,  461,  464,  467,  470,  473,  476,  479,  481
};

static const u8 Topaz_width[189] = {
 5, 6, 7, 7, 7, 6, 5, 6, 6, 7, 5, 0, 0, 0, 8, 7,
 5, 7, 7, 6, 7, 6, 7, 7, 7, 5, 5, 7, 7, 5, 7, 0,
 5, 0, 7, 7, 7, 8, 0, 6, 6, 8, 7, 5, 7, 5, 5, 7,
 5, 7, 7, 7, 7, 7, 6, 7, 7, 5, 5, 7, 7, 7, 6, 7,
 7, 7, 7, 7, 7, 7, 7, 7, 6, 7, 7, 3, 7, 7, 7, 7,
 6, 7, 7, 7, 7, 7, 7, 8, 8, 7, 6, 4, 6, 6, 0, 7,
 7, 7, 7, 7, 7, 5, 7, 7, 6, 7, 7, 7, 7, 7, 7, 6,
 7, 7, 7, 5, 7, 7, 7, 8, 5, 7, 6, 8, 6, 7, 7, 6,
 6, 6, 6, 7, 6, 6, 7, 6, 5, 6, 7, 5, 6, 6, 6, 6,
 7, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 0, 0,
 7, 7, 6, 7, 7, 4, 7, 7, 6, 6, 7, 6, 7, 7, 7, 7,
 7, 7, 7, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7
};

Topaz::Topaz(u8 fixedWidth) : PackedFont1 (
	65,
	253,
	Topaz_glyphdata,
	Topaz_offset,
	Topaz_width,
	5,
	2,
	8) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
