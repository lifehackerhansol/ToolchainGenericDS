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
#include "typedefsTGDS.h"
#include "dsregs.h"
#include "dsregs_asm.h"
#include "soundTGDS.h"
#include "ipcfifoTGDS.h"
#include "powerTGDS.h"
#include "timerTGDS.h"
#include "posixHandleTGDS.h"

#ifdef ARM9
#include "utilsTGDS.h"
#endif

void initSound(){
	#ifdef ARM7
	SoundPowerON(127);		//volume
	#endif
	
	#ifdef ARM9
	SendFIFOWords(FIFO_INITSOUND, 0);
	#endif
}

//Sound Sample Context: Plays raw sound samples at VBLANK intervals
void startSound(int sampleRate, const void* data, u32 bytes, u8 channel, u8 vol,  u8 pan, u8 format)
{
	#ifdef ARM9
	uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
	coherent_user_range_by_size((uint32)fifomsg, sizeof(TGDSIPC->fifoMesaggingQueue));
	coherent_user_range_by_size((uint32)data, bytes);	//coherent sound buffer if within cached EWRAM
	
	fifomsg[0] = (uint32)sampleRate;
	fifomsg[1] = (uint32)data;
	fifomsg[2] = (uint32)bytes;
	
	u32 packedSnd = (u32)( ((channel&0xff) << 24) | ((vol&0xff) << 16) | ((pan&0xff) << 8) | (format&0xff) );
	fifomsg[3] = (uint32)packedSnd;
	SendFIFOWords(FIFO_PLAYSOUND, (uint32)fifomsg);
	#endif
	
	#ifdef ARM7
	//If channel busy, allocate it and play later.
	if(isFreeSoundChannel(channel) == -1){
		channel = (u8)getFreeSoundChannel();
		struct soundSampleContext * curSoundSampleContext = getsoundSampleContextByIndex(channel);
		curSoundSampleContext->sampleRate = (int)sampleRate;
		curSoundSampleContext->arm9data = (s16 *)data;//
		curSoundSampleContext->arm9LInterpolated = NULL;
		curSoundSampleContext->bytes = bytes;
		curSoundSampleContext->channel = channel;
		curSoundSampleContext->vol = vol;
		curSoundSampleContext->pan = pan;
		curSoundSampleContext->format = format;
		curSoundSampleContext->status = SOUNDSAMPLECONTEXT_PENDING;
		return;
	}
	
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(sampleRate);
	SCHANNEL_SOURCE(channel) = (u32)data;
	SCHANNEL_LENGTH(channel) = bytes >> 2;
	SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | (format << 29);
	#endif
}


#ifdef ARM7
int SoundTGDSCurChannel = 0;
int soundSampleContextCurrentMode = SOUNDSAMPLECONTEXT_SOUND_IDLE;

void initSoundSampleContext(){
	int i = 0;
	for(i = 0; i < SoundSampleContextChannels; i++){
		struct soundSampleContext * soundSampleCtx = getsoundSampleContextByIndex(i);
		freesoundSampleContext(soundSampleCtx);
	}
	EnableSoundSampleContext(SOUNDSAMPLECONTEXT_SOUND_SAMPLEPLAYBACK);
}
#endif

struct soundSampleContext * getsoundSampleContextByIndex(int index){
	if((index < 0) || (index > SoundSampleContextChannels)){
		return NULL;
	}
	
	return(struct soundSampleContext *)&TGDSIPC->soundContextShared.soundSampleCxt[index];
}

bool freesoundSampleContext(struct soundSampleContext * sampleInst){
	if(sampleInst != NULL){
		memset(sampleInst, 0, sizeof(struct soundSampleContext));		
		sampleInst->sampleRate = -1;
		sampleInst->arm9data = NULL;
		sampleInst->bytes = -1;
		sampleInst->channel = -1;
		sampleInst->vol = -1;
		sampleInst->pan = -1;
		sampleInst->status = -1;
		sampleInst->format = -1;
		sampleInst->status = SOUNDSAMPLECONTEXT_IDLE;
		return true;
	}
	return false;
}

//Returns any available SoundSampleContext, or NULL
struct soundSampleContext * getFreeSoundSampleContext(){
	int i = 0;
	
	for(i = 0; i < SoundSampleContextChannels; i++){
		struct soundSampleContext * thisSoundSampleCtx = getsoundSampleContextByIndex(i);
		if(thisSoundSampleCtx->status == SOUNDSAMPLECONTEXT_IDLE){
			return thisSoundSampleCtx;
		}
	}
	return NULL;
}

//Sound stream code taken from DSOrganize because it works 100% through interrupts and that's wonderful


/***************************************************************************
 *                                                                         *
 *  This file is part of DSOrganize.                                       *
 *                                                                         *
 *  DSOrganize is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  DSOrganize is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with DSOrganize.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                         *
 ***************************************************************************/
 
 
#ifdef ARM9

// sound out
void (*wavDecode)() = NULL;
static bool sndPaused = false;
FILE * GlobalSoundStreamFile = NULL;
static bool playing = false;
s16 * SharedEWRAM0 = NULL;	//ptr start = 0
s16 * SharedEWRAM1 = NULL;	//ptr start = 0 + 0x4000
static int SwapSoundStreamBuffers = 0;
static int SwapSoundStreamBufferSize = 0;
s16 *lBufferSwapped = NULL;
s16 *rBufferSwapped = NULL;
static bool cutOff = false;

void mallocData9TGDS(int size)
{
	struct soundPlayerContext * soundPlayerCtx = (struct soundPlayerContext *)&TGDSIPC->sndPlayerCtx;
	SwapSoundStreamBuffers = 0;
	
	u32 *siTemp = (u32 *)SharedEWRAM0;
	int i=0;
	for(i=0;i<0x2000;++i)
		siTemp[i] = 0;
	
	SwapSoundStreamBufferSize = size;
	
	lBufferSwapped = SharedEWRAM0;
	rBufferSwapped = SharedEWRAM0 + SwapSoundStreamBufferSize;
	
	soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_L_BUFFER].arm9data = NULL; // temporary
	soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_R_BUFFER].arm9data = NULL; // temporary
}

void freeData9TGDS()
{
	lBufferSwapped = NULL;
	rBufferSwapped = NULL;
}

void swapDataTGDS()
{
	struct soundPlayerContext * soundPlayerCtx = (struct soundPlayerContext *)&TGDSIPC->sndPlayerCtx;
	
	SwapSoundStreamBuffers = 1 - SwapSoundStreamBuffers;
	
	switch(SwapSoundStreamBuffers)
	{
		case 0:
			lBufferSwapped = SharedEWRAM0;
			rBufferSwapped = SharedEWRAM0 + SwapSoundStreamBufferSize;
			
			soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_L_BUFFER].arm9data = SharedEWRAM1; 
			soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_R_BUFFER].arm9data = (SharedEWRAM1 + SwapSoundStreamBufferSize);
			break;
		case 1:
			lBufferSwapped = SharedEWRAM1;
			rBufferSwapped = SharedEWRAM1 + SwapSoundStreamBufferSize;
			
			soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_L_BUFFER].arm9data = SharedEWRAM0; 
			soundPlayerCtx->soundSampleCxt[SOUNDSTREAM_R_BUFFER].arm9data = (SharedEWRAM0 + SwapSoundStreamBufferSize);
			
			break;
	}
}

void swapAndSendTGDS(u32 type)
{
	swapDataTGDS();
	SendFIFOWords(type,0);
}

void startSound9(u32 srcFrmt)
{	
	if(!playing){
		SendFIFOWords(ARM7COMMAND_START_SOUND, (u32)srcFrmt);
	}
	playing = true;
}

void stopSound(u32 srcFrmt)	//ARM9 impl.
{
	if(playing){
		SendFIFOWords(ARM7COMMAND_STOP_SOUND, srcFrmt);
	}
	playing = false;
}

void setSoundLength(u32 len)
{
	SendFIFOWords(ARM7COMMAND_SOUND_SETLEN, len);
}

void setSoundFrequency(u32 freq)
{
	SendFIFOWords(ARM7COMMAND_SOUND_SETRATE, freq);
}

void setSoundInterpolation(u32 mult)
{
	SendFIFOWords(ARM7COMMAND_SOUND_SETMULT, mult);
}

//Returns the next offset right after "data". Which is the raw Waveform length (4 bytes) and then the raw Waveform
int parseWaveData(FILE * fh){
    u32 bytes[4];
	int fileOffset = 0;
	
	// Read first 4 bytes.
	// (Should be RIFF descriptor.)
	if (fread((u8*)&bytes[0], 1, 4, fh) < 0) {
		return -1;
	}
	
	fileOffset+=4;
	
	// First subchunk will always be at byte 12.
	// (There is no other dependable constant.)
	fseek(fh, 8, SEEK_CUR);
	fileOffset+=8;
	
	for (;;) {
		// Read chunk length.
		if (fread((u8*)&bytes[0], 1, sizeof(bytes), fh) < 0) {
			return -1;
		}
		
		int match = 0;
		//Aligned / not aligned headers. (why would somebody build a non aligned header!!!!)
		int j = 0;
		int k = 0;
		for(j=0; j < sizeof(bytes)/sizeof(u32); j++){
			u32 word = bytes[j];
			for(k=0; k < sizeof(u32); k++){
				u8 var = ((word >> (8*k))&0xff);
				if((u8)var == (u8)0x61){
					match++;
				}
				else if((u8)var == (u8)0x74){
					match++;
				}
				if((u8)var == (u8)0x61){
					match++;
				}
				if((u8)var == (u8)0x64){
					match++;
				}
				if(match>=4){
					return (fileOffset+(j*sizeof(u32))+k);
				}
				
			}
		}
		
		
		fileOffset+=16;
	}
	return -1;
}

void wavDecode8Bit()
{
	// 8bit wav file
	u8 *s8Data = (u8 *)TGDSARM9Malloc(WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels);
	int rSize = getWavData(s8Data, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels), GlobalSoundStreamFile);
	if(rSize < (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels))
	{
		cutOff = true;
		memset(s8Data + rSize, 0, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels) - rSize);
	}
	coherent_user_range((uint32)SharedEWRAM0, (uint32)(32*1024));
	
	u8 *lData = (u8 *)lBufferSwapped + 1;
	u8 *rData = (u8 *)rBufferSwapped + 1;
	if(TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels == 2)
	{
		uint i=0;
		for(i=0;i<(WAV_READ_SIZE << 1);i+=2)
		{
			lData[i] = (s8Data[i] - 128);
			rData[i] = (s8Data[i + 1] - 128);
		}
	}
	else
	{
		uint i=0,j=0;
		for(i=0,j=0;i<(WAV_READ_SIZE << 1);i+=2,++j)
		{
			lData[i] = (s8Data[j] - 128);
			rData[i] = lData[i];
		}
	}
	TGDSARM9Free(s8Data);
}

void wavDecode16Bit()
{
	// 16bit wav file
	s16 *tmpData = (s16 *)TGDSARM9Malloc(WAV_READ_SIZE * 2 * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels);
	int rSize = getWavData(tmpData, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 2), GlobalSoundStreamFile);
	if(rSize < (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 2))
	{
		cutOff = true;
		memset(tmpData + (rSize >> 1), 0, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 2) - rSize);
	}
	coherent_user_range((uint32)SharedEWRAM0, (uint32)(32*1024));
	
	if(TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels == 2)
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{					
			lBufferSwapped[i] = tmpData[i << 1];
			rBufferSwapped[i] = tmpData[(i << 1) | 1];
		}
	}
	else
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{
			lBufferSwapped[i] = tmpData[i];
			rBufferSwapped[i] = tmpData[i];
		}
	}
		
	TGDSARM9Free(tmpData);
}

void wavDecode24Bit()
{
	// 24bit wav file
	u8 *tmpData = (u8 *)TGDSARM9Malloc(WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 3);
	int rSize = getWavData(tmpData, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 3), GlobalSoundStreamFile);
	if(rSize < (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 3))
	{
		cutOff = true;
		memset(tmpData + rSize, 0, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 3) - rSize);
	}
	coherent_user_range((uint32)SharedEWRAM0, (uint32)(32*1024));
	
	void *oldPointer = tmpData;
	u16 tmpVal;
	s16 *tmpSound = (s16 *)&tmpVal;
	
	if(TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels == 2)
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{					
			tmpVal = (tmpData[1]) | ((tmpData[2]) << 8);
			lBufferSwapped[i] = *tmpSound;
			tmpData+=3;
			
			tmpVal = (tmpData[1]) | ((tmpData[2]) << 8);
			rBufferSwapped[i] = *tmpSound;
			tmpData+=3;
		}
	}
	else
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{	
			tmpVal = (tmpData[1]) | ((tmpData[2]) << 8);
			
			lBufferSwapped[i] = *tmpSound;
			rBufferSwapped[i] = *tmpSound;
			
			tmpData+=3;
		}
	}
	TGDSARM9Free(oldPointer);
}

void wavDecode32Bit()
{
	// 32bit wav file
	s16 *tmpData = (s16 *)TGDSARM9Malloc(WAV_READ_SIZE * 4 * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels);	
	int rSize = getWavData(tmpData, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 4), GlobalSoundStreamFile);
	if(rSize < (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 4))
	{
		cutOff = true;
		memset(tmpData + (rSize >> 1), 0, (WAV_READ_SIZE * TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels * 4) - rSize);
	}
	coherent_user_range((uint32)SharedEWRAM0, (uint32)(32*1024));
	
	if(TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels == 2)
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{					
			lBufferSwapped[i] = tmpData[(i << 2) + 1];
			rBufferSwapped[i] = tmpData[(i << 2) + 3];
		}
	}
	else
	{
		uint i=0;
		for(i=0;i<WAV_READ_SIZE;++i)
		{
			lBufferSwapped[i] = tmpData[(i << 1) + 1];
			rBufferSwapped[i] = lBufferSwapped[i];
		}
	}
	
	TGDSARM9Free(tmpData);
}

void freeSoundTGDS()
{
	stopSound(TGDSIPC->sndPlayerCtx.sourceFmt);//ARM9
	freeData9TGDS();
	if(GlobalSoundStreamFile != NULL){
		fclose(GlobalSoundStreamFile);
		GlobalSoundStreamFile = NULL;
		TGDSIPC->sndPlayerCtx.sourceFmt = SRC_NONE;	
	}
}

void initComplexSoundTGDS(u32 srcFmt)
{	
	struct soundPlayerContext * soundPlayerCtx = (struct soundPlayerContext *)&TGDSIPC->sndPlayerCtx;
	soundPlayerCtx->volume = 4;
	soundPlayerCtx->sourceFmt = srcFmt;
	SendFIFOWords(TGDS_ARM7_INITSTREAMSOUNDCTX, srcFmt);
}

//Opens a file handle
bool initSoundStream(char * WAVfilename){	//ARM9 Impl.
	char tmpName[256+1] = {0};
	char ext[256+1] = {0};
	strcpy(tmpName, WAVfilename);	
	separateExtension(tmpName, ext);
	strlwr(ext);
	
	if(SharedEWRAM0 != NULL){
		TGDSARM9Free(SharedEWRAM0);
		SharedEWRAM1 = NULL;
	}
	
	SharedEWRAM0 = (s16*)TGDSARM9Malloc(32*1024);
	SharedEWRAM1 = (s16*)((u8*)SharedEWRAM0 + 0x4000);
	
	freeSoundTGDS();
	
	if(strcmp(ext,".wav") == 0)
	{
		// wav file!
		initComplexSoundTGDS(SRC_WAV); // initialize sound variables
		TGDSIPC->sndPlayerCtx.fileOffset = 0;
		
		char header[13];
		FILE *fp = fopen(WAVfilename, "r");
		fread(header, 1, 12, fp);
		
		header[12] = 0;
		header[4] = ' ';
		header[5] = ' ';
		header[6] = ' ';
		header[7] = ' ';
		
		if(strcmp(header, "RIFF    WAVE") != 0)
		{
			// wrong header
			
			fclose(fp);
			return false;
		}		
		
		fread((char*)&TGDSIPC->sndPlayerCtx.wavDescriptor, 1, sizeof(wavFormatChunk), fp);
		
		if(strncmp((char*)&TGDSIPC->sndPlayerCtx.wavDescriptor.chunkID[0], "fmt ", 4) != 0)
		{
			// wrong chunk at beginning
			
			fclose(fp);
			return false;
		}
		
		if(TGDSIPC->sndPlayerCtx.wavDescriptor.wFormatTag != 1)
		{
			// compression used, hell no to loading this
			
			fclose(fp);
			return false;
		}
		
		if(TGDSIPC->sndPlayerCtx.wavDescriptor.wChannels > 2)
		{
			// more than 2 channels.... uh no!
			
			fclose(fp);
			return false;
		}
		
		
		if(TGDSIPC->sndPlayerCtx.wavDescriptor.wBitsPerSample <= 8)
		{
			wavDecode = wavDecode8Bit;
		}
		else if(TGDSIPC->sndPlayerCtx.wavDescriptor.wBitsPerSample <= 16)
		{
			wavDecode = wavDecode16Bit;
		}
		else if(TGDSIPC->sndPlayerCtx.wavDescriptor.wBitsPerSample <= 24)
		{
			wavDecode = wavDecode24Bit;
		}
		else if(TGDSIPC->sndPlayerCtx.wavDescriptor.wBitsPerSample <= 32)
		{
			wavDecode = wavDecode32Bit;
		}
		else
		{
			// more than 32bit sound, not supported
			
			fclose(fp);
			return false;		
		}
		
		//rewind
		fseek(fp, 0, SEEK_SET);
		int wavStartOffset = parseWaveData(fp);
		
		if(wavStartOffset < 0)
		{
			// wav block not found
			fclose(fp);
			return false;
		}
		
		u32 len = 0;
		fread(&len, 1, sizeof(len), fp);
		wavStartOffset+=4;
		
		TGDSIPC->sndPlayerCtx.fileSize = len;
		TGDSIPC->sndPlayerCtx.fileOffset = wavStartOffset;
		GlobalSoundStreamFile = fp;
		
		setSoundInterpolation(1);
		setSoundFrequency(TGDSIPC->sndPlayerCtx.wavDescriptor.dwSamplesPerSec);
		
		setSoundLength(WAV_READ_SIZE);		
		mallocData9TGDS(WAV_READ_SIZE);
		
		wavDecode();
		startSound9((u32)SRC_WAV);
		
		return true;
	}
	return false;
}

//ARM9: Stream Playback handler
void updateSoundContextStreamPlayback(u32 srcFrmt){
	switch(srcFrmt)
	{
		case SRC_WAV:
		{
			if(lBufferSwapped == NULL || rBufferSwapped == NULL)
			{
				// file is done
				stopSound(TGDSIPC->sndPlayerCtx.sourceFmt); //ARM9
				return;
			}
			
			if(sndPaused)
			{
				memset(lBufferSwapped, 0, SwapSoundStreamBufferSize * 2);
				memset(rBufferSwapped, 0, SwapSoundStreamBufferSize * 2);
				
				swapAndSendTGDS(ARM7COMMAND_SOUND_COPY);
				return;
			}	
			swapAndSendTGDS(ARM7COMMAND_SOUND_COPY);
			wavDecode();
			TGDSIPC->sndPlayerCtx.fileOffset = ftell(GlobalSoundStreamFile);
		}
		break;
		default:{
			updateSoundContextStreamPlaybackUser(srcFrmt);	//User impl.
		}
		break;
	}
}

//Allocates by free sampleContext. Sound Sample Context is queued
//Returns: true if success, false if full samples alloc'ed
bool setSoundSampleContext(int sampleRate, u32 * data, u32 bytes, u8 channel, u8 vol, u8 pan, u8 format){
	struct soundSampleContext * sampleInst = getFreeSoundSampleContext();
	if(sampleInst != NULL){
		sampleInst->sampleRate = sampleRate;
		sampleInst->arm9data = (s16*)data;
		sampleInst->bytes = bytes;
		sampleInst->channel = channel;
		sampleInst->vol = vol;
		sampleInst->pan = pan;
		sampleInst->format = format;
		sampleInst->status = SOUNDSAMPLECONTEXT_PENDING;
		return true;
	}
	return false;
}

//ARM7->ARM9: Just discarded the curChannelFreed's SoundSampleContext
void flushSoundContext(int soundContextIndex){
	
}
#endif

void EnableSoundSampleContext(int SndSamplemode){
	#ifdef ARM7
	soundSampleContextCurrentMode = SndSamplemode;
	#endif
	#ifdef ARM9
	SendFIFOWords(TGDS_ARM7_ENABLESOUNDSAMPLECTX, (u32)SndSamplemode);
	#endif
}

void DisableSoundSampleContext(){
	#ifdef ARM7
	soundSampleContextCurrentMode = SOUNDSAMPLECONTEXT_SOUND_IDLE;
	#endif
	#ifdef ARM9
	SendFIFOWords(TGDS_ARM7_DISABLESOUNDSAMPLECTX, 0);
	#endif
}

#ifdef ARM7
u32 srcFrmt = 0;
u32 sampleLen = 0;
int multRate = 1;
int sndRate = 0;
u32 sndCursor = 0;

s16 *strpcmL0 = NULL;
s16 *strpcmL1 = NULL;
s16 *strpcmR0 = NULL;
s16 *strpcmR1 = NULL;

int lastL = 0;
int lastR = 0;

/////////////////////////////////////////////////////////Interrupt code //////////////////////////////////////////////////////
void mallocData7TGDS(int size)
{
	// clear vram d bank to not have sound leftover
	int i = 0;
	
	for(i=0;i<(size);++i)
	{
		strpcmL0[i] = 0;
	}
	
	for(i=0;i<(size);++i)
	{
		strpcmR0[i] = 0;
	}
}

void freeData7TGDS()
{	
	
}

void SetupSound(u32 srcFrmtInst)
{
	srcFrmt = srcFrmtInst;
    sndCursor = 0;
	if(multRate != 1 && multRate != 2 && multRate != 4){
		multRate = 1;
	}
	
	mallocData7TGDS(sampleLen * multRate);
    
	//Auxiliary + Hardware Channels used by soundStream playback
	int ch=0;
	for(ch=0;ch<4;++ch)
	{
		SCHANNEL_CR(ch) = 0;
		SCHANNEL_TIMER(ch) = SOUND_FREQ((sndRate * multRate));
		SCHANNEL_LENGTH(ch) = (sampleLen * multRate) >> 1;
		SCHANNEL_REPEAT_POINT(ch) = 0;
	}
	
	lastL = 0;
	lastR = 0;
	
	TIMERXDATA(2) = SOUND_FREQ((sndRate * multRate));
	TIMERXCNT(2) = TIMER_DIV_1 | TIMER_ENABLE;
  
	TIMERXDATA(3) = 0x10000 - (sampleLen * 2 * multRate);
	TIMERXCNT(3) = TIMER_CASCADE | TIMER_IRQ_REQ | TIMER_ENABLE;
	
	REG_IE|=(IRQ_TIMER3);
}

void StopSound(u32 srcFrmtInst) //ARM7 impl.
{
	TIMERXCNT(2) = 0;
	TIMERXCNT(3) = 0;
	
	int ch=0;
	for(ch=0;ch<4;++ch)
	{
		SCHANNEL_CR(ch) = 0;
	}
	
	REG_IE&=~(IRQ_TIMER3);
	
	freeData7TGDS();
}
/////////////////////////////////////////////////////////Interrupt code end //////////////////////////////////////////////////////


void closeSoundStream(){
	
}

void initSoundStream(u32 srcFmt){		//ARM7 Impl.
	SoundPowerON(127);		//volume
	initSoundStreamUser(srcFmt);
	EnableSoundSampleContext(SOUNDSAMPLECONTEXT_SOUND_STREAMPLAYBACK);
}

#endif

