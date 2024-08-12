#ifndef __filesystem__
#define __filesystem__

#include <stdint.h>

typedef struct{ char			filename[32];
				__uint32		flags;
				__uint32		filelength;
				__uint32		id;
				__uint32		eof;
				__uint32		position;
				__uint32		currentcluster;
				__uint32		deviceID;

} FILE, *PFILE;

typedef struct{	__uint32		totalSectors;
				__uint32		fatOffset;
				__uint32		fatSize;
				__uint32		fatEntries;
				__uint32		nmrRootEntries;
				__uint32		rootOffset;
				__uint32		rootSize;
				__uint32		dataRegionOffset;

}MOUNT_INFO, *PMOUNT_INFO;

#endif
