#include "latha10.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Latha10_glyphdata[358] = {
/* '!' */	0xFD00,
/* '"' */	0xB680,0x0000,0x0000,
/* '#' */	0x28AF,0xD453,0xEA28,0x0000,0x0000,
/* '$' */	0x7568,0xE296,0xAE20,0x0000,
/* '%' */	0x624A,0x250D,0x00B0,0xA452,0x4600,0x0000,0x0000,
/* '&' */	0x3124,0x8C52,0x389D,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x2A49,0x2444,0x0000,
/* ')' */	0x8892,0x4950,0x0000,
/* '*' */	0x5D50,0x0000,0x0000,
/* '+' */	0x0008,0x4F90,0x8000,0x0000,
/* ',' */	0x01C0,
/* '-' */	0x0001,0xC000,0x0000,
/* '.' */	0x0100,
/* '/' */	0x2524,0xA400,0x0000,
/* '0' */	0x7463,0x18C6,0x2E00,0x0000,
/* '1' */	0x2E92,0x4900,0x0000,
/* '2' */	0x7442,0x1111,0x1F00,0x0000,
/* '3' */	0x7442,0x6086,0x2E00,0x0000,
/* '4' */	0x1194,0xA97C,0x4200,0x0000,
/* '5' */	0x7A21,0xE086,0x2E00,0x0000,
/* '6' */	0x7461,0xE8C6,0x2E00,0x0000,
/* '7' */	0xF884,0x4221,0x0800,0x0000,
/* '8' */	0x7462,0xE8C6,0x2E00,0x0000,
/* '9' */	0x7463,0x1786,0x2E00,0x0000,
/* ':' */	0x2100,
/* ';' */	0x21C0,
/* '<' */	0x0002,0xE838,0x2000,0x0000,
/* '=' */	0x0001,0xF07C,0x0000,0x0000,
/* '>' */	0x0020,0xE0BA,0x0000,0x0000,
/* '?' */	0x7442,0x2210,0x0400,0x0000,
/* '@' */	0x1F18,0x24D6,0x4DA2,0x689A,0x6A6C,0x404F,0xE000,
/* 'A' */	0x1050,0xA144,0x4FA0,0xC100,0x0000,
/* 'B' */	0xFA18,0x7F86,0x187E,0x0000,0x0000,
/* 'C' */	0x3918,0x2082,0x044E,0x0000,0x0000,
/* 'D' */	0xF228,0x6186,0x18BC,0x0000,0x0000,
/* 'E' */	0xFC21,0xF842,0x1F00,0x0000,
/* 'F' */	0xFC21,0xE842,0x1000,0x0000,
/* 'G' */	0x388A,0x0C08,0xF051,0x1C00,0x0000,
/* 'H' */	0x8618,0x7F86,0x1861,0x0000,0x0000,
/* 'I' */	0xFF00,
/* 'J' */	0x1111,0x1996,0x0000,
/* 'K' */	0x8629,0x2CD2,0x28A1,0x0000,0x0000,
/* 'L' */	0x8421,0x0842,0x1F00,0x0000,
/* 'M' */	0x838F,0x1D5A,0xB564,0xC900,0x0000,
/* 'N' */	0x871A,0x6996,0x58E1,0x0000,0x0000,
/* 'O' */	0x388A,0x0C18,0x3051,0x1C00,0x0000,
/* 'P' */	0xF463,0x1F42,0x1000,0x0000,
/* 'Q' */	0x388A,0x0C18,0x3351,0x1D00,0x0000,
/* 'R' */	0xFA18,0x7E92,0x28A1,0x0000,0x0000,
/* 'S' */	0x7A18,0x1818,0x185E,0x0000,0x0000,
/* 'T' */	0xF908,0x4210,0x8400,0x0000,
/* 'U' */	0x8618,0x6186,0x185E,0x0000,0x0000,
/* 'V' */	0x8305,0x1222,0x8504,0x0800,0x0000,
/* 'W' */	0x8865,0x194A,0x52A2,0xA8A4,0x1104,0x0000,0x0000,
/* 'X' */	0x8524,0x8C31,0x24A1,0x0000,0x0000,
/* 'Y' */	0x8289,0x1141,0x0204,0x0800,0x0000,
/* 'Z' */	0x7C21,0x0420,0x843F,0x0000,0x0000,
/* '[' */	0xEAAA,0xB000,
/* '\' */	0x9124,0x8900,0x0000,
/* ']' */	0xD555,0x7000,
/* '^' */	0x2295,0x1000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0x03F0,0x0000,
/* '`' */	0x9000,0x0000,
/* 'a' */	0x001D,0x17C6,0x6D00,0x0000,
/* 'b' */	0x842D,0x98C7,0x3600,0x0000,
/* 'c' */	0x001D,0x1842,0x2E00,0x0000,
/* 'd' */	0x085B,0x38C6,0x6D00,0x0000,
/* 'e' */	0x001D,0x1FC2,0x2E00,0x0000,
/* 'f' */	0x2BA4,0x9200,0x0000,
/* 'g' */	0x001B,0x38C6,0x6D0F,0x8000,
/* 'h' */	0x842D,0x98C6,0x3100,0x0000,
/* 'i' */	0xBF00,
/* 'j' */	0xBF80,
/* 'k' */	0x889A,0xCAA9,0x0000,
/* 'l' */	0xFF00,
/* 'm' */	0x0002,0xF699,0x3264,0xC900,0x0000,
/* 'n' */	0x003D,0x18C6,0x3100,0x0000,
/* 'o' */	0x001D,0x18C6,0x2E00,0x0000,
/* 'p' */	0x002D,0x98C7,0x3684,0x0000,
/* 'q' */	0x001B,0x38C6,0x6D08,0x4000,
/* 'r' */	0x02E9,0x2400,0x0000,
/* 's' */	0x001D,0x160A,0x2E00,0x0000,
/* 't' */	0xAEAB,0x0000,
/* 'u' */	0x0023,0x18C6,0x6D00,0x0000,
/* 'v' */	0x0023,0x1528,0x8400,0x0000,
/* 'w' */	0x0000,0x2232,0x9552,0xA888,0x4400,0x0000,0x0000,
/* 'x' */	0x0022,0xA211,0x5100,0x0000,
/* 'y' */	0x0023,0x1528,0x8422,0x0000,
/* 'z' */	0x003E,0x2211,0x1F00,0x0000,
/* '{' */	0x2928,0x9244,0x0000,
/* '|' */	0xFFC0,
/* '}' */	0x8922,0x9250,0x0000,
/* '~' */	0x0000,0xDB00,0x0000,0x0000,
/* 0x7F */	0xF999,0x9999,0xF000,
};

static const u16 Latha10_offset[95] = {
    0,    1,    4,    9,   13,   20,   25,   26,   29,   32,   35,   39,   40,   43,   44,   47,
   51,   54,   58,   62,   66,   70,   74,   78,   82,   86,   87,   88,   92,   96,  100,  104,
  111,  116,  121,  126,  131,  135,  139,  144,  149,  150,  153,  158,  162,  167,  172,  177,
  181,  186,  191,  196,  200,  205,  210,  217,  222,  227,  232,  234,  237,  239,  243,  248,
  250,  254,  258,  262,  266,  270,  273,  277,  281,  282,  283,  286,  287,  292,  296,  300,
  304,  308,  311,  315,  317,  321,  325,  332,  336,  340,  344,  347,  348,  351,  355
};

static const u8 Latha10_width[95] = {
 1, 3, 6, 5, 9, 6, 1, 3, 3, 3, 5, 1, 3, 1, 3, 5,
 3, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 5, 5, 5, 5,10,
 7, 6, 6, 6, 5, 5, 7, 6, 1, 4, 6, 5, 7, 6, 7, 5,
 7, 6, 6, 5, 6, 7,10, 6, 7, 6, 2, 3, 2, 5, 6, 2,
 5, 5, 5, 5, 5, 3, 5, 5, 1, 1, 4, 1, 7, 5, 5, 5,
 5, 3, 5, 2, 5, 5, 9, 5, 5, 5, 3, 1, 3, 5, 4
};

Latha10::Latha10(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Latha10_glyphdata,
	Latha10_offset,
	Latha10_width,
	11,
	3,
	10) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
