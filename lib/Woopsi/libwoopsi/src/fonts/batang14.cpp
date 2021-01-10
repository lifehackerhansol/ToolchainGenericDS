#include "batang14.h"
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"

using namespace WoopsiUI;

static const u16 Batang14_glyphdata[576] = {
/* '!' */	0xFE60,
/* '"' */	0xB6D0,0x0000,0x0000,
/* '#' */	0x0009,0x1212,0xFF24,0x24FF,0x4848,0x9000,0x0000,
/* '$' */	0x1021,0xF4A9,0x321C,0x0E13,0x254B,0xE102,0x0000,
/* '%' */	0x0006,0x113C,0x4489,0x20C8,0x0130,0x4912,0x2248,0x8600,0x0000,0x0000,
/* '&' */	0x1C08,0x8220,0x9018,0x1C78,0x8A22,0x8510,0x93D8,0x0000,0x0000,
/* ''' */	0xE000,
/* '(' */	0x1244,0x8888,0x8442,0x1000,
/* ')' */	0x8422,0x1111,0x1224,0x8000,
/* '*' */	0x0000,0x4ABA,0xA400,0x0000,0x0000,
/* '+' */	0x0000,0x4081,0x1FC4,0x0810,0x0000,0x0000,0x0000,
/* ',' */	0x0000,0x1580,
/* '-' */	0x0000,0x0000,0x1FC0,0x0000,0x0000,0x0000,0x0000,
/* '.' */	0x0060,
/* '/' */	0x0410,0x8210,0x4108,0x2104,0x1082,0x0000,
/* '0' */	0x3128,0x6186,0x1861,0x8523,0x0000,0x0000,
/* '1' */	0x2708,0x4210,0x8421,0x3E00,0x0000,
/* '2' */	0x3128,0x6108,0x4210,0x862F,0x8000,0x0000,
/* '3' */	0x3128,0x6108,0xC081,0x8523,0x0000,0x0000,
/* '4' */	0x1830,0xA144,0x8922,0x7F08,0x1070,0x0000,0x0000,
/* '5' */	0xFE08,0x20F2,0x2041,0x8523,0x0000,0x0000,
/* '6' */	0x3128,0x61B3,0x2861,0x8523,0x0000,0x0000,
/* '7' */	0x7E86,0x0820,0x4102,0x0810,0x2040,0x0000,0x0000,
/* '8' */	0x3128,0x6148,0xC4A1,0x8523,0x0000,0x0000,
/* '9' */	0x3128,0x6185,0x3341,0x8523,0x0000,0x0000,
/* ':' */	0x3180,
/* ';' */	0x0501,0x6000,
/* '<' */	0x0421,0x0842,0x0408,0x1020,0x4000,0x0000,
/* '=' */	0x0000,0x000F,0xE03F,0x8000,0x0000,0x0000,0x0000,
/* '>' */	0x8102,0x0408,0x1084,0x2108,0x0000,0x0000,
/* '?' */	0x7462,0x1110,0x8401,0x0800,0x0000,
/* '@' */	0x0F82,0x0880,0xA354,0x9322,0x648C,0x928D,0x9001,0x031F,0x8000,0x0000,
/* 'A' */	0x0804,0x0502,0x8141,0x10F8,0x4441,0x20B8,0xE000,0x0000,
/* 'B' */	0xFC42,0x4242,0x427E,0x4141,0x4141,0xFE00,0x0000,
/* 'C' */	0x3E42,0x8180,0x8080,0x8080,0x8142,0x3C00,0x0000,
/* 'D' */	0xFC42,0x4141,0x4141,0x4141,0x4142,0xFC00,0x0000,
/* 'E' */	0xFE42,0x4140,0x447C,0x4440,0x4142,0xFE00,0x0000,
/* 'F' */	0xFE42,0x4140,0x447C,0x4440,0x4040,0xE000,0x0000,
/* 'G' */	0x3F20,0xA030,0x0804,0x3E05,0x0281,0x218F,0x4000,0x0000,
/* 'H' */	0xE3A0,0x9048,0x2413,0xF904,0x8241,0x20B8,0xE000,0x0000,
/* 'I' */	0xE924,0x924B,0x8000,
/* 'J' */	0x3884,0x2108,0x4214,0x9800,0x0000,
/* 'K' */	0xE721,0x1109,0x0503,0x8120,0x8842,0x20B8,0xE000,0x0000,
/* 'L' */	0xE040,0x4040,0x4040,0x4040,0x4142,0xFE00,0x0000,
/* 'M' */	0xC068,0x0983,0x3066,0x0CA2,0x9452,0x524A,0x488B,0x9380,0x0000,0x0000,
/* 'N' */	0xC3A0,0x984A,0x2512,0x4914,0x8A43,0x20B8,0x4000,0x0000,
/* 'O' */	0x3C42,0x8181,0x8181,0x8181,0x8142,0x3C00,0x0000,
/* 'P' */	0xFC85,0x0A14,0x2F90,0x2040,0x8380,0x0000,0x0000,
/* 'Q' */	0x3C21,0x2050,0x2814,0x0A05,0x02B9,0x230F,0x2060,0x0000,
/* 'R' */	0xFC21,0x1048,0x2423,0xE110,0x8442,0x2138,0xE000,0x0000,
/* 'S' */	0x7D0A,0x0C06,0x0301,0x0182,0x85F0,0x0000,0x0000,
/* 'T' */	0x7F24,0xA221,0x0080,0x4020,0x1008,0x0407,0x0000,0x0000,
/* 'U' */	0xE3A0,0x9048,0x2412,0x0904,0x8241,0x1107,0x0000,0x0000,
/* 'V' */	0xE3A0,0x9044,0x4221,0x1050,0x2814,0x0402,0x0000,0x0000,
/* 'W' */	0xEEE8,0x8911,0x2222,0xA855,0x0AA0,0xDC11,0x0220,0x4400,0x0000,0x0000,
/* 'X' */	0xE742,0x2424,0x1818,0x1824,0x2442,0xE700,0x0000,
/* 'Y' */	0xE3A0,0x8884,0x4140,0xA020,0x1008,0x0407,0x0000,0x0000,
/* 'Z' */	0x7F42,0x8404,0x0810,0x2020,0x4182,0xFE00,0x0000,
/* '[' */	0xEAAA,0xAAC0,
/* '\' */	0xEEE8,0x8911,0x222F,0xFE55,0x0AA0,0xD811,0x0220,0x4400,0x0000,0x0000,
/* ']' */	0xD555,0x55C0,
/* '^' */	0x1051,0x1410,0x0000,0x0000,0x0000,0x0000,0x0000,
/* '_' */	0x0000,0x0000,0x0000,0x000F,0xC000,0x0000,
/* '`' */	0x9400,0x0000,
/* 'a' */	0x0000,0x0072,0x2392,0x8A66,0xC000,0x0000,
/* 'b' */	0xC081,0x0207,0x8890,0xA142,0x89E0,0x0000,0x0000,
/* 'c' */	0x0000,0x007A,0x1820,0x8217,0x8000,0x0000,
/* 'd' */	0x0C08,0x1023,0xC8A1,0x4284,0x98D8,0x0000,0x0000,
/* 'e' */	0x0000,0x007A,0x187F,0x8217,0x8000,0x0000,
/* 'f' */	0x2544,0xF444,0x44E0,0x0000,
/* 'g' */	0x0000,0x0076,0x289C,0x81E8,0x6178,0x0000,
/* 'h' */	0xC081,0x0205,0x8C91,0x2244,0x8BB8,0x0000,0x0000,
/* 'i' */	0x480C,0x924B,0x8000,
/* 'j' */	0x2406,0x4924,0xD400,
/* 'k' */	0xC081,0x0205,0xC914,0x3448,0x8BB8,0x0000,0x0000,
/* 'l' */	0xC924,0x924B,0x8000,
/* 'm' */	0x0000,0x0000,0x000D,0xB8C8,0x9112,0x2244,0x488B,0xBB80,0x0000,0x0000,
/* 'n' */	0x0000,0x000D,0x8C91,0x2244,0x8BB8,0x0000,0x0000,
/* 'o' */	0x0000,0x007A,0x1861,0x8617,0x8000,0x0000,
/* 'p' */	0x0000,0x000D,0x8C90,0xA142,0xC962,0x0E00,0x0000,
/* 'q' */	0x0000,0x0003,0x69A1,0x4284,0x98D0,0x21E0,0x0000,
/* 'r' */	0x0000,0x0D35,0x0842,0x3800,0x0000,
/* 's' */	0x0000,0x07C6,0x0E0C,0x7C00,0x0000,
/* 't' */	0x0044,0xF444,0x4430,0x0000,
/* 'u' */	0x0000,0x000C,0xC891,0x2244,0x98D8,0x0000,0x0000,
/* 'v' */	0x0000,0x000E,0xE891,0x1428,0x2040,0x0000,0x0000,
/* 'w' */	0x0000,0x0000,0x000E,0xEE88,0x9111,0x542A,0x8220,0x4400,0x0000,0x0000,
/* 'x' */	0x0000,0x000E,0xE88A,0x0828,0x8BB8,0x0000,0x0000,
/* 'y' */	0x0000,0x000E,0xE891,0x1428,0x2045,0x0C00,0x0000,
/* 'z' */	0x0000,0x007E,0x2108,0x421F,0x8000,0x0000,
/* '{' */	0x1222,0x22C2,0x2222,0x1000,
/* '|' */	0xFFF8,
/* '}' */	0x8444,0x4434,0x4444,0x8000,
/* '~' */	0x0000,0x0000,0x0003,0xC61C,0x0000,0x0000,0x0000,0x0000,
/* 0x7F */	0x07E3,0x18C6,0x318C,0x7E00,0x0000,
};

static const u16 Batang14_offset[95] = {
    0,    1,    4,   11,   18,   28,   37,   38,   42,   46,   51,   58,   60,   67,   68,   74,
   80,   85,   91,   97,  104,  110,  116,  123,  129,  135,  136,  138,  144,  151,  157,  162,
  172,  180,  187,  194,  201,  208,  215,  223,  231,  234,  239,  247,  254,  264,  272,  279,
  286,  294,  302,  309,  317,  325,  333,  343,  350,  358,  365,  367,  377,  379,  386,  392,
  394,  400,  407,  413,  420,  426,  430,  436,  443,  446,  449,  456,  459,  469,  476,  482,
  489,  496,  501,  506,  510,  517,  524,  534,  541,  548,  554,  558,  559,  563,  571
};

static const u8 Batang14_width[95] = {
 1, 3, 8, 7,11,10, 1, 4, 4, 5, 7, 2, 7, 1, 6, 6,
 5, 6, 6, 7, 6, 6, 7, 6, 6, 1, 2, 6, 7, 6, 5,11,
 9, 8, 8, 8, 8, 8, 9, 9, 3, 5, 9, 8,11, 9, 8, 7,
 9, 9, 7, 9, 9, 9,11, 8, 9, 8, 2,11, 2, 7, 6, 2,
 6, 7, 6, 7, 6, 4, 6, 7, 3, 3, 7, 3,11, 7, 6, 7,
 7, 5, 5, 4, 7, 7,11, 7, 7, 6, 4, 1, 4, 9, 5
};

Batang14::Batang14(u8 fixedWidth) : PackedFont1 (
	33,
	127,
	Batang14_glyphdata,
	Batang14_offset,
	Batang14_width,
	14,
	4,
	11) {
	if (fixedWidth) setFontWidth(fixedWidth);
};
