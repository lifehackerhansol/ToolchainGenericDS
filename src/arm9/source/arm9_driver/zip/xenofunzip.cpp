/*
 * xenofunzip - gzip/zip decompression interface for zlib, based on info-zip's funzip
 * note: xenogzip uses gzio but xenofunzip uses normal inflateInit2()/inflate().
 */

//Coto: rewritten compressor code so it's ZLIB standard through malloc/free default memory allocator.
//Also use EWRAM as stack for giant compressed .zip / .gz files, see zipSwapStack.S for specifics.

#include "xenofunzip.h"

#include "typedefs.h"
#include "dsregs.h"
#include "console.h"
#include <stdlib.h>
#include "zlib.h"
#include "posix_hook_shared.h"	//add Toolchain Generic DS Filesystem Support
#include "mem_handler_shared.h"	//malloc support

//zalloc / zfree (zlib) default function prototypes look like:
//void * 	zalloc (void *opaque, unsigned items, unsigned size)
//void 		zfree (void *opaque, void *ptr)

int funzipstdio(FILE *in, FILE *out){
	int encrypted;
	ush n;
	uch h[LOCHDR];                // first local header (GZPHDR < LOCHDR)
	int g = 0;                    // true if gzip format
	unsigned int method = 0;      // initialized here to shut up gcc warning
	int size = -1;

	//info-zip's funzip stuff
	n = fgetc_fs(in);  n |= fgetc_fs(in) << 8;
	if (n == ZIPMAG){
		if (fread_fs((char *)h, 1, LOCHDR, in) != LOCHDR || SH(h) != LOCREM)
			err("invalid zipfile");
		switch (method = SH(h + LOCHOW)) {
			case STORED:
			case DEFLATED:
				break;
			default:
				err("first entry not deflated or stored");
				break;
		}
		for (n = SH(h + LOCFIL); n--; ) g = fgetc_fs(in);
		for (n = SH(h + LOCEXT); n--; ) g = fgetc_fs(in);
		g = 0;
		size = LG(h+LOCSIZ);
		encrypted = h[LOCFLG] & CRPFLG;
	}
	else if (n == GZPMAG){
		if (fread_fs((char *)h, 1, GZPHDR, in) != GZPHDR){
			err("invalid gzip file");
		}
		if ((method = h[GZPHOW]) != DEFLATED && method != ENHDEFLATED){
			err("gzip file not deflated");
		}
		if (h[GZPFLG] & GZPMUL){
			err("cannot handle multi-part gzip files");
		}
		if (h[GZPFLG] & GZPISX){
			n = fgetc_fs(in);  n |= fgetc_fs(in) << 8;
			while (n--) g = fgetc_fs(in);
		}
		if (h[GZPFLG] & GZPISF){
			while ((g = fgetc_fs(in)) != 0 && g != EOF) ;
		}
		if (h[GZPFLG] & GZPISC){
			while ((g = fgetc_fs(in)) != 0 && g != EOF) ;
		}
		g = 1;
		encrypted = h[GZPFLG] & GZPISE;
	}
	else{
		err("input not a zip or gzip file");
	}
	//now in points to deflated entry. let's just inflate it using zlib.

	//if entry encrypted, decrypt and validate encryption header
	if (encrypted){
		err("encrypted zip unsupported");
	}
	//decompress
	if (g || h[LOCHOW]){ //deflate
		Bytef *ibuffer, *obuffer;
		z_stream z;
		int result;
		
		//use the default malloc/free		
		z.zalloc = Z_NULL;
		z.zfree = Z_NULL;
		z.opaque = Z_NULL;
		
		result = inflateInit2( &z,-MAX_WBITS );
		if( result != Z_OK ) {
			char buf[200];
			sprintf(buf,"inflateInit2 failed:%d",result);
			err( buf );
		}
		
		ibuffer = (Bytef *)malloc(BUFFER_SIZE);
		obuffer = (Bytef *)malloc(BUFFER_SIZE);
		
		memset ( ibuffer, 0, BUFFER_SIZE);
		memset ( obuffer, 0, BUFFER_SIZE);
		
		z.next_in = (Bytef *)ibuffer;
		z.avail_in = 0;
		
		for(;;){
			
			z.next_out = (Bytef *)obuffer;
			z.avail_out = BUFFER_SIZE;
		
			if( z.avail_in == 0 ){
				z.next_in = ibuffer;
				if(size>=0){
					if(size>0){
						z.avail_in = fread_fs( ibuffer, 1, min(size,BUFFER_SIZE), in );
						size-=min(size,BUFFER_SIZE);
					}
				}else{
					z.avail_in = fread_fs( ibuffer, 1, BUFFER_SIZE, in );
				}
			}
			result = inflate( &z, Z_SYNC_FLUSH );
			if( result != Z_OK && result != Z_STREAM_END ) {
				inflateEnd( &z );
				char buf[200];
				sprintf(buf,"inflate error:%d",result);
				err(buf);
			}
			
			int wrote = fwrite_fs( obuffer, 1, BUFFER_SIZE - z.avail_out, out );
			if(wrote >= 0){
				sint32 FDToSync = fileno(out);
				fsync(FDToSync);
			}
			if(result==Z_STREAM_END){
				break;
			}
			
		}
		
		inflateEnd( &z );
		free(ibuffer);
		free(obuffer);
		
	}else{ //stored
		while (size--) {
			int c = fgetc_fs(in);fputc_fs(c,out);
		}
	}

	//should check CRC32 but...
	return 0;
}


char * tmpFile = "TempFile.smc";

//UserCode: (char*)"path1ToFileZipped","path2ToFileToCreateUnzipped"
int load_gz(char *fname, char *newtempfname)	//fname,newtempfname must use getfatfsPath() outside!
{
	int ret = do_decompression_ewramstack(fname, newtempfname);	//use EWRAM as STACK
	return ret;
}

int do_decompression(char *inname, char *outname){ //dszip frontend
	FILE *in=fopen_fs(inname,"r");
	if(!in)return -1;
	FILE *out=fopen_fs(outname,"w+");
	if(!out){fclose_fs(in);return -1;}
	int ret = funzipstdio(in,out);
	
	fclose_fs(in);
	fclose_fs(out);
	
	return ret;
}