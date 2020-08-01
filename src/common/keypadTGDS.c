
/*

			Copyright (C) 2017  Coto
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
USA

*/

#include "keypadTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"
#include "ipcfifoTGDS.h"
#include <string.h>
#include "dmaTGDS.h"
#include "biosTGDS.h"

#ifdef ARM9
#include "consoleTGDS.h"
#endif

uint32 global_keys_arm9;
uint32 last_frame_keys_arm9;
uint32 buffered_keys_arm9;

//usage:
//struct touchScr touchScrStruct;
//touchScrRead(&touchScrStruct);
//read values..
void touchScrRead(struct touchScr * touchScrInst){
	struct sIPCSharedTGDS * sIPCSharedTGDSInst = getsIPCSharedTGDS();
	touchScrInst->buttons7	=	sIPCSharedTGDSInst->buttons7;			// X, Y, /PENIRQ buttons
	touchScrInst->touchX	=	sIPCSharedTGDSInst->touchX;
	touchScrInst->touchY	=	sIPCSharedTGDSInst->touchY;				// raw x/y TSC SPI
	touchScrInst->touchXpx	=	sIPCSharedTGDSInst->touchXpx;
	touchScrInst->touchYpx	=	sIPCSharedTGDSInst->touchYpx;			// TFT x/y pixel (converted)
	touchScrInst->KEYINPUT7	=	sIPCSharedTGDSInst->KEYINPUT7;			// TFT x/y pixel (converted)
}