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

#ifdef ARM7
#include "wifi_arm7.h"
#include "soundTGDS.h"
#include "ARM7FS.h"
#endif

#ifdef ARM9
#include "wifi_arm9.h"
#include "dswnifi_lib.h"
#endif

#include "InterruptsARMCores_h.h"
#include "ipcfifoTGDS.h"
#include "keypadTGDS.h"
#include "eventsTGDS.h"
#include "dldi.h"

void IRQInit(){
	//fifo setups
	REG_IME = 0;
	REG_IPC_SYNC = (1 << 14);	//14    R/W  Enable IRQ from remote CPU  (0=Disable, 1=Enable)
    REG_IPC_FIFO_CR = IPC_FIFO_SEND_CLEAR | RECV_FIFO_IPC_IRQ  | FIFO_IPC_ENABLE;
	
	//Set up PPU IRQ: HBLANK/VBLANK/VCOUNT
    REG_DISPSTAT = (DISP_HBLANK_IRQ | DISP_VBLANK_IRQ | DISP_YTRIGGER_IRQ);
	
	//Set up PPU IRQ Vertical Line
	setVCountIRQLine(TGDS_VCOUNT_LINE_INTERRUPT);
	
	#ifdef ARM7
	REG_IE = IRQ_TIMER1 | IRQ_HBLANK | IRQ_VBLANK | IRQ_VCOUNT | IRQ_IPCSYNC | IRQ_RECVFIFO_NOT_EMPTY | IRQ_SCREENLID;
	#endif
	
	#ifdef ARM9
	REG_IE = IRQ_HBLANK| IRQ_VBLANK | IRQ_VCOUNT | IRQ_IPCSYNC | IRQ_RECVFIFO_NOT_EMPTY ;
	#endif
	
	INTERRUPT_VECTOR = (uint32)&NDS_IRQHandler;
	REG_IF = REG_IF;
	REG_IME = 1;
}

//Software bios irq more or less emulated. (replaces default NDS bios for some parts)
#ifdef ARM9
__attribute__((section(".itcm")))
#endif
void NDS_IRQHandler(){
	u32 handledIRQ = (REG_IF | SWI_CHECKBITS) & REG_IE;
	if(handledIRQ & IRQ_HBLANK){
		HblankUser();
	}
	if(handledIRQ & IRQ_VBLANK){
		#ifdef ARM7
		doSPIARM7IO();
		Wifi_Update();
		#endif
		#ifdef ARM9
		//handles DS-DS Comms
		sint32 currentDSWNIFIMode = doMULTIDaemonStage1();
		#endif
		VblankUser();
	}
	if(handledIRQ & IRQ_VCOUNT){
		#ifdef ARM7
		TGDSEventHandler();
		updateSoundContextSamplePlayback();
		#endif
		VcounterUser();
	}
	if(handledIRQ & IRQ_TIMER0){
		Timer0handlerUser();
	}
	if(handledIRQ & IRQ_TIMER1){
		Timer1handlerUser();
	}
	if(handledIRQ & IRQ_TIMER2){
		Timer2handlerUser();
	}
	if(handledIRQ & IRQ_TIMER3){
		#ifdef ARM7
		//Audio playback handler
		setSwapChannel();
		SendFIFOWordsITCM(ARM9COMMAND_UPDATE_BUFFER, srcFrmt);
		#endif
		#ifdef ARM9
		//wifi arm9 irq
		Timer_10ms();
		#endif
		Timer3handlerUser();
	}
	if(handledIRQ & IRQ_IPCSYNC){
		uint8 ipcByte = receiveByteIPC();
		switch(ipcByte){
			//External ARM Core's sendMultipleByteIPC(uint8 inByte0, uint8 inByte1, uint8 inByte2, uint8 inByte3) received bytes:
			case(IPC_SEND_MULTIPLE_CMDS):{
				
				uint8 * ipcMsg = (uint8 *)&TGDSIPC->ipcMesaggingQueue[0];
				#ifdef ARM9
				coherent_user_range_by_size((uint32)ipcMsg, sizeof(TGDSIPC->ipcMesaggingQueue));
				#endif
				uint8 inByte0 = (u8)ipcMsg[0];
				uint8 inByte1 = (u8)ipcMsg[1];
				uint8 inByte2 = (u8)ipcMsg[2];
				uint8 inByte3 = (u8)ipcMsg[3];
				
				//Do stuff.
				ipcMsg[3] = ipcMsg[2] = ipcMsg[1] = ipcMsg[0] = 0;
			}
			break;
			case(IPC_ARM7READMEMORY_REQBYIRQ):{
				uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
				uint32 srcMemory = fifomsg[28];
				uint32 targetMemory = fifomsg[29];
				int bytesToRead = (int)fifomsg[30];
				memcpy((u8*)targetMemory,(u8*)srcMemory, bytesToRead);
				fifomsg[31] = fifomsg[30] = fifomsg[29] = fifomsg[28] = (uint32)0;
			}
			break;
			case(IPC_ARM7SAVEMEMORY_REQBYIRQ):{
				uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
				uint32 srcMemory = fifomsg[32];
				uint32 targetMemory = fifomsg[33];
				int bytesToRead = (int)fifomsg[34];
				#ifdef ARM9
				dmaFillWord(0, 0, (uint32)srcMemory, (uint32)bytesToRead);
				#endif
				memcpy((u8*)srcMemory, (u8*)targetMemory, bytesToRead);
				fifomsg[35] = fifomsg[34] = fifomsg[33] = fifomsg[32] = (uint32)0;
			}
			break;
			#ifdef ARM7
			//ARM7 FS Init
			case(IPC_ARM7INIT_ARM7FS):{	//ARM7
				
				uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
				char *  ARM7FS_ARM9Filename = (char *)fifomsg[9];
				int fileHandleSize = (int)fifomsg[10];
				int splitBufferSize = (int)fifomsg[11];
				int curARM7FS_HandleMethod = (int)fifomsg[12];
				u32 * debugVar = (int)fifomsg[13];
				u32 testCase = (u32)fifomsg[14]; 
				initARM7FS((char*)ARM7FS_ARM9Filename, curARM7FS_HandleMethod);
				if(testCase == (u32)0xc070c070){
					performARM7MP2FSTestCase(ARM7FS_ARM9Filename, splitBufferSize, debugVar);	//ARM7 Setup
				}
				fifomsg[15] = fifomsg[14] = fifomsg[13] = fifomsg[12] = fifomsg[11] = fifomsg[10] = fifomsg[9] = 0;
			}
			break;
			
			case(IPC_ARM7DEINIT_ARM7FS):{	//ARM7
				uint32 * fifomsg = (uint32 *)&TGDSIPC->fifoMesaggingQueue[0];
				
				//ARM7(FS) de-init
				deinitARM7FS();
				
				fifomsg[8] = 0;
			}
			break;
			
			#ifdef ARM7_DLDI
			case(IPC_INIT_DLDI7_REQBYIRQ):{
				//Init DLDI ARM7
				ARM7DLDIInit();
			}
			break;
			#endif
			
			#endif
			
			#ifdef ARM9
			case (IPC_ARM7INIT_ARM7FS):	//ARM9 
			{
				
			}
			break;
			
			case(IPC_ARM7DEINIT_ARM7FS):{	//ARM9 
				
			}
			break;
			
			#endif
			
			default:{
				IpcSynchandlerUser(ipcByte);//ipcByte should be the byte you sent from external ARM Core through sendByteIPC(ipcByte);
			}
			break;
		}
		
	}
	if(handledIRQ & IRQ_SENDFIFO_EMPTY){
		HandleFifoEmpty();
	}
	if(handledIRQ & IRQ_RECVFIFO_NOT_EMPTY){
		HandleFifoNotEmpty();
	}
	#ifdef ARM7
	//arm7 wifi irq
	if(handledIRQ & IRQ_WIFI){
		Wifi_Interrupt();
	}
	if(handledIRQ & IRQ_RTCLOCK){
		
	}
	if(handledIRQ & IRQ_SCREENLID){
		isArm7ClosedLid = false;
		SendFIFOWordsITCM(FIFO_IRQ_LIDHASOPENED_SIGNAL, 0);
		screenLidHasOpenedhandlerUser();
	}
	#endif
	REG_IF = handledIRQ;
}

void EnableIrq(uint32 IRQ){
	REG_IE	|=	IRQ;
}

void DisableIrq(uint32 IRQ){
	REG_IE	&=	~(IRQ);
}

void RemoveQueuedIrq(uint32 IRQ){
	if(REG_IF & IRQ){
		REG_IF|=IRQ;
	}
}

void QueueIrq(uint32 IRQ){
	if(!(REG_IF & IRQ)){
		REG_IF|=IRQ;
	}
}
