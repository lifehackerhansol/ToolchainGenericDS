#ifdef ARM9

#include <stdio.h>
#include <string.h>
#include "dsregs.h"
#include "dldi.h"
#include "dmaTGDS.h"
#include "busTGDS.h"

const uint32  DLDI_MAGIC_NUMBER = 
	0xBF8DA5ED;	
	
// Stored backwards to prevent it being picked up by DLDI patchers
const sint8 DLDI_MAGIC_STRING_BACKWARDS [DLDI_MAGIC_STRING_LEN] =
	{'\0', 'm', 'h', 's', 'i', 'h', 'C', ' '} ;



const DLDI_INTERFACE* io_dldi_data = &_io_dldi_stub;

const struct DISC_INTERFACE_STRUCT* dldiGetInternal (void) {
	if (_io_dldi_stub.ioInterface.features & FEATURE_SLOT_GBA) {
		SetBusSLOT1ARM7SLOT2ARM9();
	}
	if (_io_dldi_stub.ioInterface.features & FEATURE_SLOT_NDS) {
		SetBusSLOT1ARM9SLOT2ARM7();
	}
	
	return &dldiGet()->ioInterface;
}

struct DLDI_INTERFACE* dldiGet(void) {
	if (_io_dldi_stub.ioInterface.features & FEATURE_SLOT_GBA) {
		SetBusSLOT1ARM7SLOT2ARM9();
	}
	if (_io_dldi_stub.ioInterface.features & FEATURE_SLOT_NDS) {
		SetBusSLOT1ARM9SLOT2ARM7();
	}
	return &_io_dldi_stub;
}


//DLDI bits (must extend from ARM7DLDI later)
//ARM9DLDI:
//ARM7: NULL ptr
//ARM9: Global Physical DLDI section (rather than &_dldi_start, since it's discarded at TGDS init)
u32 * DLDIARM7Address = NULL;

void setDLDIARM7Address(u32 * address){
	DLDIARM7Address = address;
}

u32 * getDLDIARM7Address(){
	return DLDIARM7Address;
}

bool dldi_handler_init(){
	bool status = false;
	struct DLDI_INTERFACE* dldiInit = dldiGet();	//ensures SLOT-1 / SLOT-2 is mapped to ARM7/ARM9 now
	if( (!dldiInit->ioInterface.startup()) || (!dldiInit->ioInterface.isInserted()) ){
		status = false;
	}
	else{
		status = true;	//init OK!
		DLDIARM7Address = (u32*)dldiInit;
	}
	return status;
}

void dldi_handler_deinit(){
	struct DLDI_INTERFACE* dldiInit = dldiGet();	//ensures SLOT-1 / SLOT-2 is mapped to ARM7/ARM9 now
	dldiInit->ioInterface.clearStatus();
	dldiInit->ioInterface.shutdown();
}

/////////////////////////////////////////////////// RAM Disk DLDI Implementation ////////////////////////////////////////////

bool _DLDI_isInserted(void)
{
	return true;	//Always True
}

bool _DLDI_clearStatus(void)
{
    return true;	//Always True
}

bool _DLDI_shutdown(void)
{
    return true;	//Always True
}

bool _DLDI_startup(void)
{
    return true;	//Always True
} 

bool _DLDI_writeSectors(uint32 sector, uint32 sectorCount, const uint8* buffer)
{
	int sectorSize = 512;
	int curSector = 0;
	while(sectorCount > 0)
	{
        memcpy(((u8*)0x08000000 + ((sector+curSector)*sectorSize)), (buffer + (curSector*sectorSize)), sectorSize);
		curSector++;
		--sectorCount;
	}
    return true;
}

bool _DLDI_readSectors(uint32 sector, uint32 sectorCount, uint8* buffer)
{
	int sectorSize = 512;
	int curSector = 0;
	while(sectorCount > 0)
	{
        memcpy(buffer + (curSector*sectorSize), ((u8*)0x08000000 + ((sector+curSector)*sectorSize)), sectorSize);
		curSector++;
		--sectorCount;
	}
    return true;
}

//////////////////////////////////////////////// RAM Disk DLDI Implementation End ///////////////////////////////////////////

bool dldi_handler_read_sectors(sec_t sector, sec_t numSectors, void* buffer){
	//ARM7 DLDI implementation
	#ifdef ARM7_DLDI
		#ifdef ARM7
		struct  DLDI_INTERFACE* dldiInterface = (struct DLDI_INTERFACE*)DLDIARM7Address;
		return dldiInterface->ioInterface.readSectors(sector, numSectors, buffer);
		#endif
		#ifdef ARM9
		void * targetMem = (void *)((int)ARM7DLDIBuf + 0x400000);
		struct sIPCSharedTGDS * TGDSIPC = TGDSIPCStartAddress;
		uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
		setValueSafe(&fifomsg[20], (uint32)sector);
		setValueSafe(&fifomsg[21], (uint32)numSectors);
		setValueSafe(&fifomsg[22], (uint32)targetMem);
		setValueSafe(&fifomsg[23], (uint32)0xFEFEFEFE);
		sendByteIPC(IPC_READ_ARM7DLDI_REQBYIRQ);
		while(getValueSafe(&fifomsg[23]) == (uint32)0xFEFEFEFE){
			swiDelay(18);
		}
		memcpy((uint16_t*)buffer, (uint16_t*)targetMem, (numSectors * 512));
		return true;
		#endif	
	#endif	
	#ifdef ARM9_DLDI
		#ifdef ARM7
		return false;
		#endif
		#ifdef ARM9
		struct  DLDI_INTERFACE* dldiInterface = (struct  DLDI_INTERFACE*)DLDIARM7Address;
		return dldiInterface->ioInterface.readSectors(sector, numSectors, buffer);
		#endif
	#endif
}

bool dldi_handler_write_sectors(sec_t sector, sec_t numSectors, const void* buffer){
	//ARM7 DLDI implementation
	#ifdef ARM7_DLDI
		#ifdef ARM7
		struct  DLDI_INTERFACE* dldiInterface = (struct DLDI_INTERFACE*)DLDIARM7Address;
		return dldiInterface->ioInterface.writeSectors(sector, numSectors, buffer);
		#endif
		#ifdef ARM9
		void * targetMem = (void *)((int)ARM7DLDIBuf + 0x400000);
		memcpy((uint16_t*)targetMem, (uint16_t*)buffer, (numSectors * 512));
		struct sIPCSharedTGDS * TGDSIPC = TGDSIPCStartAddress;
		uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
		setValueSafe(&fifomsg[24], (uint32)sector);
		setValueSafe(&fifomsg[25], (uint32)numSectors);
		setValueSafe(&fifomsg[26], (uint32)targetMem);
		setValueSafe(&fifomsg[27], (uint32)0xFEFEFEFE);
		sendByteIPC(IPC_WRITE_ARM7DLDI_REQBYIRQ);
		while(getValueSafe(&fifomsg[27]) == (uint32)0xFEFEFEFE){
			swiDelay(18);
		}
		return true;
		#endif	
	#endif	
	#ifdef ARM9_DLDI
		#ifdef ARM7
		return false;
		#endif
		#ifdef ARM9
		struct  DLDI_INTERFACE* dldiInterface = (struct  DLDI_INTERFACE*)DLDIARM7Address;
		return dldiInterface->ioInterface.writeSectors(sector, numSectors, buffer);
		#endif
	#endif
}
#endif


static const data_t dldiMagicString[12] = "\xED\xA5\x8D\xBF Chishm";	// Normal DLDI file
static const data_t dldiMagicLoaderString[12] = "\xEE\xA5\x8D\xBF Chishm";	// Different to a normal DLDI file

enum DldiOffsets {
	DO_magicString = 0x00,			// "\xED\xA5\x8D\xBF Chishm"
	DO_magicToken = 0x00,			// 0xBF8DA5ED
	DO_magicShortString = 0x04,		// " Chishm"
	DO_version = 0x0C,
	DO_driverSize = 0x0D,
	DO_fixSections = 0x0E,
	DO_allocatedSpace = 0x0F,

	DO_friendlyName = 0x10,

	DO_text_start = 0x40,			// Data start
	DO_data_end = 0x44,				// Data end
	DO_glue_start = 0x48,			// Interworking glue start	-- Needs address fixing
	DO_glue_end = 0x4C,				// Interworking glue end
	DO_got_start = 0x50,			// GOT start					-- Needs address fixing
	DO_got_end = 0x54,				// GOT end
	DO_bss_start = 0x58,			// bss start					-- Needs setting to zero
	DO_bss_end = 0x5C,				// bss end

	// IO_INTERFACE data
	DO_ioType = 0x60,
	DO_features = 0x64,
	DO_startup = 0x68,	
	DO_isInserted = 0x6C,	
	DO_readSectors = 0x70,	
	DO_writeSectors = 0x74,
	DO_clearStatus = 0x78,
	DO_shutdown = 0x7C,
	DO_code = 0x80
};

static inline addr_t readAddr (data_t *mem, addr_t offset) {
	return ((addr_t*)mem)[offset/sizeof(addr_t)];
}

static inline void writeAddr (data_t *mem, addr_t offset, addr_t value) {
	((addr_t*)mem)[offset/sizeof(addr_t)] = value;
}

static inline addr_t quickFind (const data_t* data, const data_t* search, size_t dataLen, size_t searchLen) {
	const int* dataChunk = (const int*) data;
	int searchChunk = ((const int*)search)[0];
	addr_t i;
	addr_t dataChunkEnd = (addr_t)(dataLen / sizeof(int));

	for ( i = 0; i < dataChunkEnd; i++) {
		if (dataChunk[i] == searchChunk) {
			if ((i*sizeof(int) + searchLen) > dataLen) {
				return -1;
			}
			if (memcmp (&data[i*sizeof(int)], search, searchLen) == 0) {
				return i*sizeof(int);
			}
		}
	}
	return -1;
}

//DldiRelocatedAddress == target DLDI relocated address
//dldiSourceInRam == physical DLDI section having a proper DLDI driver used as donor 
//dldiOutWriteAddress == new physical DLDI out buffer, except, relocated to a new DldiRelocatedAddress!
bool dldiRelocateLoader(bool clearBSS, u32 DldiRelocatedAddress, u32 dldiSourceInRam, u32 dldiOutWriteAddress){
	addr_t memOffset;			// Offset of DLDI after the file is loaded into memory
	addr_t patchOffset;			// Position of patch destination in the file
	addr_t relocationOffset;	// Value added to all offsets within the patch to fix it properly
	addr_t ddmemOffset;			// Original offset used in the DLDI file
	addr_t ddmemStart;			// Start of range that offsets can be in the DLDI file
	addr_t ddmemEnd;			// End of range that offsets can be in the DLDI file
	addr_t ddmemSize;			// Size of range that offsets can be in the DLDI file

	addr_t addrIter;

	data_t *pDH;
	data_t *pAH;

	size_t dldiFileSize = 0;
	
	// Target the DLDI we want to use as stub copy and then relocate it to a DldiRelocatedAddress address
	pDH = (data_t*)dldiOutWriteAddress;
	pAH = (data_t *)dldiSourceInRam;
	dldiFileSize = 1 << pAH[DO_driverSize];
	// Copy the DLDI patch into the application
	//dmaCopyWords(0, (void*)pAH, (void*)pDH, dldiFileSize);			//dmaCopyWords (uint8 channel, const void *src, void *dest, uint32 size)
	dmaTransferWord(3, (uint32)pAH, (uint32)pDH, (uint32)dldiFileSize);	//void dmaTransferWord(sint32 dmachannel, uint32 source, uint32 dest, uint32 word_count)

	if (*((u32*)(pDH + DO_ioType)) == DEVICE_TYPE_DLDI) {
		// No DLDI patch
		return false;
	}
	
	if (pDH[DO_driverSize] > pAH[DO_allocatedSpace]) {
		// Not enough space for patch
		return false;
	}
	
	memOffset = DldiRelocatedAddress;	//readAddr (pAH, DO_text_start);
	if (memOffset == 0) {
			memOffset = readAddr (pAH, DO_startup) - DO_code;
	}
	ddmemOffset = readAddr (pDH, DO_text_start);
	relocationOffset = memOffset - ddmemOffset;

	ddmemStart = readAddr (pDH, DO_text_start);
	ddmemSize = (1 << pDH[DO_driverSize]);
	ddmemEnd = ddmemStart + ddmemSize;

	// Remember how much space is actually reserved
	pDH[DO_allocatedSpace] = pAH[DO_allocatedSpace];
	
	// Fix the section pointers in the DLDI @ VRAM header
	writeAddr (pDH, DO_text_start, readAddr (pAH, DO_text_start) + relocationOffset);
	writeAddr (pDH, DO_data_end, readAddr (pAH, DO_data_end) + relocationOffset);
	writeAddr (pDH, DO_glue_start, readAddr (pAH, DO_glue_start) + relocationOffset);
	writeAddr (pDH, DO_glue_end, readAddr (pAH, DO_glue_end) + relocationOffset);
	writeAddr (pDH, DO_got_start, readAddr (pAH, DO_got_start) + relocationOffset);
	writeAddr (pDH, DO_got_end, readAddr (pAH, DO_got_end) + relocationOffset);
	writeAddr (pDH, DO_bss_start, readAddr (pAH, DO_bss_start) + relocationOffset);
	writeAddr (pDH, DO_bss_end, readAddr (pAH, DO_bss_end) + relocationOffset);
	
	// Fix the function pointers in the header
	writeAddr (pDH, DO_startup, readAddr (pAH, DO_startup) + relocationOffset);
	writeAddr (pDH, DO_isInserted, readAddr (pAH, DO_isInserted) + relocationOffset);
	writeAddr (pDH, DO_readSectors, readAddr (pAH, DO_readSectors) + relocationOffset);
	writeAddr (pDH, DO_writeSectors, readAddr (pAH, DO_writeSectors) + relocationOffset);
	writeAddr (pDH, DO_clearStatus, readAddr (pAH, DO_clearStatus) + relocationOffset);
	writeAddr (pDH, DO_shutdown, readAddr (pAH, DO_shutdown) + relocationOffset);
	
	if (pDH[DO_fixSections] & FIX_ALL) { 
		// Search through and fix pointers within the data section of the file
		for (addrIter = (readAddr(pDH, DO_text_start) - ddmemStart); addrIter < (readAddr(pDH, DO_data_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}
	
	
	if (pDH[DO_fixSections] & FIX_GLUE) { 
		// Search through and fix pointers within the glue section of the file
		for (addrIter = (readAddr(pDH, DO_glue_start) - ddmemStart); addrIter < (readAddr(pDH, DO_glue_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}
	
	if (pDH[DO_fixSections] & FIX_GOT) { 
		// Search through and fix pointers within the Global Offset Table section of the file
		for (addrIter = (readAddr(pDH, DO_got_start) - ddmemStart); addrIter < (readAddr(pDH, DO_got_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}
	
	/*
	if (clearBSS && (pDH[DO_fixSections] & FIX_BSS)) { 
		// Initialise the BSS to 0, only if the disc is being re-inited
		for(int i = 0; i < (readAddr(pDH, DO_bss_end) - readAddr(pDH, DO_bss_start)) / 4; i++)
		{
			((uint32_t*)&pAH[readAddr(pDH, DO_bss_start) - ddmemStart])[i] = 0;
		}
		
	}
	*/
	return true;
}

//arg0: binary data address to patch
//arg1: binary data size to patch
//arg2: Physical valid DLDI address (containing the DLDI binary to-be used as donor)

bool dldiPatchLoader(data_t *binData, u32 binSize, u32 physDLDIAddress)
{
	addr_t memOffset;			// Offset of DLDI after the file is loaded into memory
	addr_t patchOffset;			// Position of patch destination in the file
	addr_t relocationOffset;	// Value added to all offsets within the patch to fix it properly
	addr_t ddmemOffset;			// Original offset used in the DLDI file
	addr_t ddmemStart;			// Start of range that offsets can be in the DLDI file
	addr_t ddmemEnd;			// End of range that offsets can be in the DLDI file
	addr_t ddmemSize;			// Size of range that offsets can be in the DLDI file

	addr_t addrIter;

	data_t *pDH;
	data_t *pAH;
	size_t dldiFileSize = 0;
	pDH = (data_t*)physDLDIAddress;
	//DS DLDI
	struct  DLDI_INTERFACE* dldiInterface = (struct  DLDI_INTERFACE*)pDH;
	
	if (*((u32*)(pDH + DO_ioType)) == DEVICE_TYPE_DLDI) 
	{
		#ifdef ARM9
		printf("DLDI section not found in NTR binary. ");
		#endif
	}
	//TGDS DLDI RAMDISK? No need to patch because the offsets are outside the DLDI stub and kept in TGDS NDS Binary anyway to be reused later
	if(strncmp((char *)&dldiInterface->friendlyName[0], "TGDS RAMDISK", 12) == 0){
		return false;
	}
	else{
		#ifdef ARM9
		printf("DLDI section found in NTR binary. Patching... ");
		#endif
	}
	
	// Find the DLDI reserved space in the file
	patchOffset = quickFind(binData, dldiMagicString, binSize, sizeof(dldiMagicString));

	if (patchOffset < 0) {
		// does not have a DLDI section
		//printf("ERR: NO DLDI SECTION");
		return false;
	}
	
	pAH = &(binData[patchOffset]);

	if (pDH[DO_driverSize] > pAH[DO_allocatedSpace]) 
	{
		// Not enough space for patch
		//printf("ERR: NOT ENOUGH SPACE");
		return false;
	}
	
	dldiFileSize = 1 << pDH[DO_driverSize];

	memOffset = readAddr (pAH, DO_text_start);
	if (memOffset == 0) {
			memOffset = readAddr (pAH, DO_startup) - DO_code;
	}
	ddmemOffset = readAddr (pDH, DO_text_start);
	relocationOffset = memOffset - ddmemOffset;

	ddmemStart = readAddr (pDH, DO_text_start);
	ddmemSize = (1 << pDH[DO_driverSize]);
	ddmemEnd = ddmemStart + ddmemSize;

	// Remember how much space is actually reserved
	pDH[DO_allocatedSpace] = pAH[DO_allocatedSpace];
	// Copy the DLDI patch into the application
	memcpy (pAH, pDH, dldiFileSize);

	// Fix the section pointers in the header
	writeAddr (pAH, DO_text_start, readAddr (pAH, DO_text_start) + relocationOffset);
	writeAddr (pAH, DO_data_end, readAddr (pAH, DO_data_end) + relocationOffset);
	writeAddr (pAH, DO_glue_start, readAddr (pAH, DO_glue_start) + relocationOffset);
	writeAddr (pAH, DO_glue_end, readAddr (pAH, DO_glue_end) + relocationOffset);
	writeAddr (pAH, DO_got_start, readAddr (pAH, DO_got_start) + relocationOffset);
	writeAddr (pAH, DO_got_end, readAddr (pAH, DO_got_end) + relocationOffset);
	writeAddr (pAH, DO_bss_start, readAddr (pAH, DO_bss_start) + relocationOffset);
	writeAddr (pAH, DO_bss_end, readAddr (pAH, DO_bss_end) + relocationOffset);
	// Fix the function pointers in the header
	writeAddr (pAH, DO_startup, readAddr (pAH, DO_startup) + relocationOffset);
	writeAddr (pAH, DO_isInserted, readAddr (pAH, DO_isInserted) + relocationOffset);
	writeAddr (pAH, DO_readSectors, readAddr (pAH, DO_readSectors) + relocationOffset);
	writeAddr (pAH, DO_writeSectors, readAddr (pAH, DO_writeSectors) + relocationOffset);
	writeAddr (pAH, DO_clearStatus, readAddr (pAH, DO_clearStatus) + relocationOffset);
	writeAddr (pAH, DO_shutdown, readAddr (pAH, DO_shutdown) + relocationOffset);

	if (pDH[DO_fixSections] & FIX_ALL) { 
		// Search through and fix pointers within the data section of the file
		for (addrIter = (readAddr(pDH, DO_text_start) - ddmemStart); addrIter < (readAddr(pDH, DO_data_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	if (pDH[DO_fixSections] & FIX_GLUE) { 
		// Search through and fix pointers within the glue section of the file
		for (addrIter = (readAddr(pDH, DO_glue_start) - ddmemStart); addrIter < (readAddr(pDH, DO_glue_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	if (pDH[DO_fixSections] & FIX_GOT) { 
		// Search through and fix pointers within the Global Offset Table section of the file
		for (addrIter = (readAddr(pDH, DO_got_start) - ddmemStart); addrIter < (readAddr(pDH, DO_got_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	/*
	if (pDH[DO_fixSections] & FIX_BSS) { 
		// Initialise the BSS to 0
		memset (&pAH[readAddr(pDH, DO_bss_start) - ddmemStart] , 0, readAddr(pDH, DO_bss_end) - readAddr(pDH, DO_bss_start));
	}
	*/

	return true;
}
