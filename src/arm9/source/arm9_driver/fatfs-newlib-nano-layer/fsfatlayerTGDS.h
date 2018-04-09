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

//Coto: this was rewritten by me so it could fit the following setup:
//newlib libc nano ARM Toolchain. dirent.h is not supported in this newlib version so we restore it

#ifndef __fsfat_layer_h__
#define __fsfat_layer_h__

#ifdef __cplusplus
using namespace std;
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <string>

#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "limitsTGDS.h"
#include "typedefsTGDS.h"
#include "ff.h"	//DIR struct definition. DIR dirent.h is deprecated. (implementation defined)
#include "dirent.h"

#include "devoptab_devices.h"
#include "posixHandleTGDS.h"

//POSIX default init values
#define dirent_default_d_ino	(sint32)(-1)
#define dirent_default_d_name	(sint8 *)("")

//for use with fsfat_layer
//must be typecasted otherwise strb happens (we don't want that)
#define structfd_isfile	(sint32)0
#define structfd_isdir	(sint32)1

#define structfd_isused 	(sint32)1
#define structfd_isunused	(sint32)0

#define structfd_isattydefault	(sint32)0

#define structfd_descriptorflagsdefault	(sint32)0
#define structfd_status_flagsdefault	(sint32)0
#define structfd_fildir_offsetdefault	(sint32)0

//these must be invalid values so false positives do not arise in lookup functions
#define structfd_posixFileDescrdefault	(sint32)-1
#define structfd_internalFileDescrdefault	(sint32)-1

//FT_NONE = 0
//FT_FILE = 1
//FT_DIR = 2
#define FT_NONE (int)(0)
#define FT_FILE (int)(1)
#define FT_DIR (int)(2)

//libfat attributes so gccnewlibnano_to_fsfat is compatible with libfat homebrew
#ifndef ATTRIB_ARCH
#define ATTRIB_ARCH	(int)(0x20)			// Archive
#define ATTRIB_DIR	(int)(0x10)			// Directory
#define ATTRIB_LFN	(int)(0x0F)			// Long file name
#define ATTRIB_VOL	(int)(0x08)			// Volume
#define ATTRIB_SYS	(int)(0x04)			// System
#define ATTRIB_HID	(int)(0x02)			// Hidden
#define ATTRIB_RO	(int)(0x01)			// Read only
#endif


#ifdef __cplusplus

//C++: Shared File Handling for misc directory functions
class FileClass
{
  public:
    int Index;
	std::string filename;
    std::string path;
    int type = 0;	//FT_DIR / FT_FILE / FT_NONE	//  setup on Constructor / updated by getFileFILINFOfromPath(); / must be init from the outside 
    // Constructor
    FileClass(int indexInst, std::string filenameInst, std::string pathInst, int typeInst)
	{
		Index = indexInst;
		filename = filenameInst;
		path = pathInst;
		type = typeInst;
	}
	
	//helpers if/when Constructor is not available
	int getindex()
    {
		return Index;
    }
    std::string getfilename()
    {
		return filename;
    }
	std::string getpath()
    {
		return path;
    }
	int gettype()
    {
		return type;
    }
	
	void setindex(int IndexInst){
		Index = IndexInst;
	}
	void setfilename(std::string filenameInst){
		filename = filenameInst;
	}
	void setpath(std::string pathInst){
		path = pathInst;
	}
	void settype(int typeInst){
		type = typeInst;
	}
};

#endif

struct fd {
	int fd_posix;				//POSIX file descriptor index (so stdin,stdout,stderr are registered here)
	struct stat stat;			//File attributes : POSIX Compliant
	int isatty;
	int isused;					//1 means in use (structfd_isused) / 0 free (structfd_isunused)
	int descriptor_flags;
	int status_flags;
	long loc;		//current file pointer if this struct fd is DIR * (generated by opendir)
    struct dirent cur_entry;	//dirent POSIX extension used in fopen / fatfs_readdir_r
	
	sint8 fd_name[MAX_TGDSFILENAME_LENGTH+1];	/* d_name dirent but holds full file path. Only when this is a file */
	sint8 fullPathFName[MAX_TGDSFILENAME_LENGTH+1];	//filled by updateDir, requires previously to opendir / fopen
	
	//FILE/DIR Context in here
	FIL  * filPtr;	//reuse C ability to parse NULL structs later
	DIR  * dirPtr;
	FIL  fil;	//if aboveptr is NULL then it is not FIL
	DIR  dir;	//if aboveptr is NULL then it is not DIR
};

#endif



#ifdef __cplusplus
extern "C" {
#endif

extern FATFS dldiFs;

/* Function prototypes */

extern int		FS_init();
extern int		FS_deinit();
extern int fatfs_init();
extern int fatfs_deinit();
extern int fatfs_write (int fd, sint8 *ptr, int len);
extern int fatfs_read (int fd, sint8 *ptr, int len);
extern int fatfs_close (int fd);
extern void fill_stat(const FILINFO *fno, struct stat *out);

extern BYTE flags2mode(int flags);
extern int fresult2errno(FRESULT result);

extern int fatfs_fildir_alloc(int isfilordir);
extern void fill_fd_fil(int fildes, FIL *fp, int flags, const FILINFO *fno, char * fullFilePath);
extern void fill_fd_dir(int fildes, DIR *fp, int flags, const FILINFO *fno, char * fullFilePath);
extern void fill_fd(struct fd *pfd, int flags, const FILINFO *fno);

extern int fatfs_open_file(const sint8 *pathname, int flags, const FILINFO *fno);	//returns an internal index struct fd allocated
extern int fatfs_open_dir(const sint8 *pathname, int flags, const FILINFO *fno);	//returns an internal index struct fd allocated
extern int fatfs_open_file_or_dir(const sint8 *pathname, int flags);	//returns an internal index struct fd allocated

extern DWORD get_fattime (void);
extern int fatfs_open(const sint8 *pathname, int flags);
extern off_t fatfs_lseek(int fd, off_t offset, int whence );
extern int fatfs_unlink(const sint8 *path);
extern int fatfs_link(const sint8 *path1, const sint8 *path2);
extern int fatfs_rename(const sint8 *oldpathfile, const sint8 *newpathfile);
extern int fatfs_fsync(int fd);
extern int fatfs_stat(const sint8 *path, struct stat *buf);
extern int fatfs_mkdir(const sint8 *path, mode_t mode);
extern int fatfs_rmdir(const sint8 *path);
extern int fatfs_chdir(const sint8 *path);

extern sint8 *fatfs_getcwd(sint8 *buf, size_t size);
extern DIR *fatfs_opendir(const sint8 *path);
extern int fatfs_closedir(DIR *dirp);
extern int fatfs_dirfd(DIR *dirp);
extern DIR *fatfs_fdopendir(int fd);
extern struct dirent *fatfs_readdir(DIR *dirp);
extern int fatfs_readdir_r(DIR *dirp,struct dirent *entry,struct dirent **result);
extern void fatfs_rewinddir(DIR *dirp);
extern long fatfs_tell(struct fd *f);
extern void fatfs_seekdir(DIR *dirp, long loc);


extern sint8 * getfatfsPath(sint8 * filename);
extern volatile sint8 charbuf[MAX_TGDSFILENAME_LENGTH+1];

extern DWORD clust2sect (  /* !=0:Sector number, 0:Failed (invalid cluster#) */
    FATFS* fs,      /* File system object */
    DWORD clst      /* Cluster# to be converted */
);

extern sint32	getStructFDFirstCluster(struct fd *fdinst);
extern sint32 getStructFDSectorOffset(struct fd *fdinst,int ClusterOffset,int SectorOffset);
extern sint32 getDiskClusterSize();			/* Cluster size [sectors] */
extern sint32 getDiskClusterSizeBytes();	/* Cluster size [sectors] in bytes */
extern sint32 getDiskSectorSize();

extern int _fstat_r ( struct _reent *_r, int fd, struct stat *buf );
extern int rename(const sint8 *oldpathfile, const sint8 *newpathfile);
extern int fsync(int fd);
extern int mkdir(const sint8 *path, mode_t mode);
extern int rmdir(const sint8 *path);
extern int chdir(const sint8 *path);
extern sint8 *getcwd(sint8 *buf, size_t size);
extern DIR *opendir(const sint8 *path);
extern int closedir(DIR *dirp);
extern struct dirent *readdir(DIR *dirp);
extern int  readdir_r(DIR * dirp,struct dirent * entry,struct dirent ** result);
extern void rewinddir(DIR *dirp);
extern int dirfd(DIR *dirp);
extern int remove(const char *filename);
extern DIR *fdopendir(int fd);
extern void seekdir(DIR *dirp, long loc);
extern char * dldi_tryingInterface();

extern int gccnewlibnano_to_fsfat2libfatAttrib(int fsfatFlags);


//misc directory functions

//User
extern int 	FAT_FindFirstFile(char* filename);
extern int 	FAT_FindNextFile(char* filename);
extern u8 	FAT_GetFileAttributes(void);

//Internal
extern char lfnName[MAX_TGDSFILENAME_LENGTH+1];
extern char curDirListed[MAX_TGDSFILENAME_LENGTH+1];
extern struct fd fdCur;
extern bool FAT_GetLongFilename(char* filename);
extern int getFirstFile(char * path);
extern void updateGlobalListFromPath(char * path);
extern int getNextFile(char * path);
extern bool getLFN(char* filename);
extern bool setLFN(char* filename);
extern char lastCurrentPath[MAX_TGDSFILENAME_LENGTH];
extern int CurrentFileDirEntry;
#ifdef __cplusplus
extern FILINFO getFileFILINFOfromFileClass(FileClass * FileClassInst);
extern std::list<FileClass> * GlobalFileList;
extern std::list<FileClass> * InitGlobalFileList();
extern void DeInitGlobalFileList(std::list<FileClass> * List);
extern std::list<FileClass> buildListFromPath(char * path);
extern FileClass getEntryFromGlobalListByIndex(int EntryIndex);
extern FileClass getFirstDirEntryFromGlobalList();
extern FileClass getFirstFileEntryFromGlobalList();
extern std::string buildFullPathFromFileClass(FileClass * FileClassInst);
#endif


#ifdef __cplusplus
}
#endif