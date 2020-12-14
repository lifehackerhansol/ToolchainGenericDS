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

#ifndef __soundTGDS_h__
#define __soundTGDS_h__

#include "typedefsTGDS.h"
#include "dsregs.h"

//IPC bits
#define REG_IPC_FIFO_TX		(*(vuint32*)0x4000188)
#define REG_IPC_FIFO_RX		(*(vuint32*)0x4100000)
#define REG_IPC_FIFO_CR		(*(vuint16*)0x4000184)

#define REG_IPC_SYNC	(*(vuint16*)0x04000180)
#define IPC_SYNC_IRQ_ENABLE		(uint16)(1<<14)
#define IPC_SYNC_IRQ_REQUEST	(uint16)(1<<13)
#define IPC_FIFO_SEND_EMPTY		(uint16)(1<<0)
#define IPC_FIFO_SEND_FULL		(uint16)(1<<1)
#define IPC_FIFO_SEND_IRQ		(uint16)(1<<2)
#define IPC_FIFO_SEND_CLEAR		(uint16)(1<<3)
#define IPC_FIFO_RECV_EMPTY		(uint16)(1<<8)
#define IPC_FIFO_RECV_FULL		(uint16)(1<<9)
#define IPC_FIFO_RECV_IRQ		(uint16)(1<<10)
#define IPC_FIFO_ERROR			(uint16)(1<<14)
#define IPC_FIFO_ENABLE			(uint16)(1<<15)

//Linear sound sample playback: Sound Sample Context cmds (ARM9 -> ARM7)
#define FIFO_PLAYSOUND	(uint32)(0xffff0203)
#define FIFO_INITSOUND	(uint32)(0xffff0204)
#define FIFO_FLUSHSOUNDCONTEXT	(uint32)(0xffff0210)


#ifdef ARM9
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

#define SoundSampleContextChannels (int)(16)
#define SOUNDSAMPLECONTEXT_IDLE (u32)(0xffff1110)
#define SOUNDSAMPLECONTEXT_PENDING (u32)(0xffff1111)	//play now!
#define SOUNDSAMPLECONTEXT_PLAYING (u32)(0xffff1112)	//if SOUNDSAMPLECONTEXT_PLAYING && hw channel == disabled, -> SOUNDSAMPLECONTEXT_IDLE

//soundTGDS playback modes
#define SOUNDSAMPLECONTEXT_SOUND_IDLE (int)(0)
#define SOUNDSAMPLECONTEXT_SOUND_SAMPLEPLAYBACK (int)(1)
#define SOUNDSAMPLECONTEXT_SOUND_STREAMPLAYBACK (int)(2)

//Hardware Channels used by soundStream playback
#define SOUNDSTREAM_L_BUFFER (int)(0)
#define SOUNDSTREAM_R_BUFFER (int)(1)
//Auxiliary Hardware Channels used by soundStream playback
#define SOUNDSTREAM_L_BUFFER_AUX (int)(0+2)
#define SOUNDSTREAM_R_BUFFER_AUX (int)(1+2)

//Free Hardware channels for either sampleStream or soundStream
#define SOUNDSTREAM_FREE_CHANNEL (int)(SOUNDSTREAM_R_BUFFER_AUX + 1)

/*
#define SRC_NONE	(int)(0)
#define SRC_WAV		(int)(1)
#define SRC_MIKMOD	(int)(2)
#define SRC_MP3		(int)(3)
#define SRC_OGG		(int)(4)
#define SRC_AAC		(int)(5)
#define SRC_FLAC	(int)(6)
#define SRC_SID		(int)(7)
#define SRC_NSF		(int)(8)
#define SRC_SPC		(int)(9)
#define SRC_SNDH	(int)(10)
#define SRC_GBS		(int)(11)
#define SRC_STREAM_MP3		(int)(12)
#define SRC_STREAM_OGG		(int)(13)
#define SRC_STREAM_AAC		(int)(14)
#define SRC_WAVADPCM		(int)(15)

#define WAV_READ_SIZE 4096
*/

//IMA-ADPCM
struct dvi_adpcmblockheader_tag {
	int iSamp0;
	char bStepTableIndex;
	char bReserved;
};


//Extended Header WAV 

#define WAVE_FORMAT_DVI_ADPCM	(short)(0x0011)					/* Intel Corporation */
#define WAVE_FORMAT_IMA_ADPCM	(short)(WAVE_FORMAT_DVI_ADPCM)	/* Intel Corporation */


//Sound Context
struct soundSampleContext{
	int sampleRate;
	s16 *arm9data;
	s16 *arm9LInterpolated; //auxiliary buffer if interpolation is desired. Needs to be same size as s16 * arm9data
	u32 bytes;
	u8 channel;
	u8 vol;
	u8 pan;
	u32 status;
	u8 format;
}__attribute__((aligned (4)));

//SoundSample;TGDS format: Runs through irqs
struct soundSampleContextList{
	struct soundSampleContext soundSampleCxt[SoundSampleContextChannels];	//Each channel used as a hardware channel
}__attribute__((aligned (4)));

//SoundStream;TGDS format: Runs synchronously in ARM9 decoder thread
struct soundPlayerContext{
	//Decoder properties
	struct soundSampleContext soundSampleCxt[SoundSampleContextChannels];	//Each channel used as sample stream context, which means a single channel here can fill n hardware channels
	
	//FS properties
	int fileSize;	//u32 len;	u32 dataLen;
	int fileOffset;	//u32 loc;	u32 dataOffset;
	
	//Playback properties
	s16 *interlaced;
	int channels;
	int psgChannel;
	u8 volume;
	u32 cr;
	u32 timer;
	int sourceFmt;
	int structFDFH;	//TGDS File Descriptor through TGDS FS API, same as ARM9's FILE * GlobalSoundStreamFile;
	bool soundStreamPause;	//Indicates wether a WAV / IMA-ADPCM / Other stream format is playing
} __attribute__((aligned (4)));

#ifdef __cplusplus
extern "C"{
#endif

//Sound Sample Context: Plays raw sound samples at VBLANK intervals
extern void startSoundSample(int sampleRate, const void* data, u32 bytes, u8 channel, u8 vol,  u8 pan, u8 format);
extern void initSound();

#ifdef ARM7
extern int soundSampleContextCurrentMode;
extern void initSoundSampleContext();
extern void initSoundStream(u32 srcFmt);
extern int SoundTGDSCurChannel;

//weak symbols : the implementation of these is project-defined, also abstracted from the hardware IPC FIFO Implementation for easier programming.
extern __attribute__((weak))	void initSoundStreamUser(u32 srcFmt);

extern u32 srcFrmt;
extern s16 *strpcmL0;
extern s16 *strpcmL1;
extern s16 *strpcmR0;
extern s16 *strpcmR1;

extern int lastL;
extern int lastR;

//TGDS
extern void setupSound(u32 srcFrmtInst);

//weak symbols : the implementation of these is project-defined, also abstracted from the hardware IPC FIFO Implementation for easier programming.
extern __attribute__((weak))	void setupSoundUser(u32 srcFrmt);
extern __attribute__((weak))	void stopSoundUser(u32 srcFrmt);

extern u32 sampleLen;
extern int multRate;
extern int sndRate;
extern u32 sndCursor;
#endif

extern void stopSoundTGDS(u32 srcFrmt);


#ifdef ARM9
//ARM9: Stream Playback handler
extern void updateSoundContextStreamPlayback();
extern __attribute__((weak))    void updateSoundContextStreamPlaybackUser(u32 srcFrmt);
extern void flushSoundContext(int soundContextIndex);
extern bool initSoundStream(char * WAVfilename);

//Stream Sound controls
extern u8 getVolume();
extern void setVolume(u8 volume);
extern void volumeUp(int x, int y);
extern void volumeDown(int x, int y);
extern void mallocData7TGDS(int size);
extern void freeData7TGDS();
#endif

extern struct soundSampleContext * getsoundSampleContextByIndex(int index);
extern bool freesoundSampleContext(struct soundSampleContext * sampleInst);	//free up a given soundSampleContext
extern struct soundSampleContext * getFreeSoundSampleContext();				//obtains a free soundSampleContext, if any

#ifdef ARM9
extern FILE * GlobalSoundStreamFile;
extern bool setSoundSampleContext(int sampleRate, u32 * data, u32 bytes, u8 channel, u8 vol, u8 pan, u8 format);
extern s16 * SharedEWRAM0;	//ptr start = 0
extern s16 * SharedEWRAM1;	//ptr start = 0 + 0x4000
extern s16 *lBufferSwapped;
extern s16 *rBufferSwapped;
extern void setWavDecodeCallback(void (*cb)());

extern int parseWaveData(FILE * fh, u32 u32chunkToSeek);
extern void setSoundLength(u32 len);
extern void setSoundFrequency(u32 freq);
extern void setSoundInterpolation(u32 mult);
extern void setSoundFrequency(u32 freq);
extern void swapAndSendTGDS(u32 type);
extern void swapDataTGDS();
extern void mallocData9TGDS(int size);
#endif

extern void EnableSoundSampleContext(int SndSamplemode);
extern void DisableSoundSampleContext();
extern void closeSoundStream();
//extern void startSound9(u32 srcFrmt);
extern void initComplexSoundTGDS(u32 srcFmt);

#ifdef __cplusplus
}
#endif

#ifdef ARM7
static inline s32 getFreeSoundChannel(){
	int i;
	for (i=SOUNDSTREAM_FREE_CHANNEL;i<16;++i){
		if (!(SCHANNEL_CR(i) & SCHANNEL_ENABLE)) return i;
	}
	return -1;
}

//returns -1 if channel is busy, or channel if idle
static inline s32 isFreeSoundChannel(u8 chan){
	if (!(SCHANNEL_CR(chan) & SCHANNEL_ENABLE)){
		return chan;
	}
	return -1;
}

static inline s16 checkClipping(int data)
{
	if(data > 32767)
		return 32767;
	if(data < -32768)
		return -32768;
	
	return data;
}

static inline int getSoundSampleContextEnabledStatus(){	
	return soundSampleContextCurrentMode;
}

extern void updateSoundContextSamplePlayback();

#endif

#endif
