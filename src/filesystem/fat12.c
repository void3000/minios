#include <filesystem.h>

typedef struct{	__uint8		Jump[3];
				__uint8		OEM[8];
				__uint16	BytesPersector;
				__uint8		SectorsPerCluster;
				__uint16	Reserved;
				__uint8		NmrOfFats;
				__uint16	RootEntries;
				__uint16	TotalSectors;
				__uint8		Media;
				__uint16	SectorsPerFat;
				__uint16	SectorsPerTrack;
				__uint16	HeadsPerCylinder;
				__uint32	HiddenSectors;
				__uint32	TotalBigSectors;
				__uint8		DriveNmr;
				__uint8		Unused;
				__uint8		BootSignature;
				__uint32	SerialNmr;
				__uint8		VolumeLabel[11];
				__uint8		FileSystem[8];
				__uint8		Pad[465];
				__uint16	PartitionSig;

}__attribute__((packed)) BOOTSECT, *PBOOTSECT; /* 512 bytes */

typedef struct{	__uint8 Filename[8];
				__uint8 Ext[3];
				__uint8 Attrib;
				__uint8 Reserved;
				__uint8 TimeCreatedMs;
				__uint16 TimeCreated;
				__uint16 DateCreated;
				__uint16 DateLastAccessed;
				__uint16 EAIndex;
				__uint16 LastModTime;
				__uint16 LastModDate;
				__uint16 FirstCluster;
				__uint32 FileSize;

}__attribute__((packed)) DIRECTORY, *PDIRECTORY;

enum ATTRIB{
	\
	READ_ONLY	=	0x01,
	HIDDEN_FILE	=	0x02,
	SYSTEM_FILE	=	0x04,
	VOL_LABEL	=	0x08,
	SUB_DIR		=	0x10,
	ARCHIVE		=	0x20,
	DEVICE		=	0x60
};

#define EOF			((unsigned)(0xFF8))
#define BAD_CLUSTER	((unsigned)(0xFF7))
#define UNUSED		((unsigned)(0x00))
#define CLUSTER_END ((unsigned)(0x00))

#define CLUSTER_SIZE	((unsigned)512)

#define fs_get_day(n)	((unsigned)(n & 0x1F ))
#define fs_get_month(n)	((unsigned)((n & 0x1E0) >> 5))
#define fs_get_year(n)	((unsigned)((n & 0xFE00) >> 9))

#define fs_get_sec(n)	((unsigned)(n & 0x1F))
#define fs_get_min(n)	((unsigned)((n & 0x7E0) >> 5))
#define fs_get_hour(n)	((unsigned)((n & 0xF800) >> 11 ))

static unsigned char FAT12[CLUSTER_SIZE] = {0};

unsigned int eof(PFILE file);
void readfile(PFILE file, char *buff);
void closefile(PFILE);
PFILE findfile(const char *filename);
PFILE openfile(const char *filename, char Options[2]);

PFILE findfile(const char *filename)
{
	unsigned block;
	unsigned entry;
	unsigned offset = 19;
	char* __filename = (char)filename;
	PFILE file;
	PDIRECTORY dir;

/* There are 224 root entries and each entry
   has a size of 32 bytes, thus 224 * 32 gives
   number of sectors 14. Within the sector (512/32), 
   there are 16 root entries.
 */
	for (block = 0; block < 14; block++){
		dir = (PDIRECTORY)read_block_fat12(offset + block,0x700);
		for (entry = 0; entry < 16; entry++){
			if (strcmpc(dir->Filename,__filename,sizeof(__filename)))
			{
				memset(file->filename,0,sizeof(file->filename));
				memcopy(file->filename, __filename, sizeof(__filename));
				file->filelength		= dir->FileSize;
				file->currentcluster	= dir->FirstCluster;
				file->flags				= dir->Attrib;
				file->eof				= 0x0000;
				return file;
			}else {
				dir++;
			}
		}
	}
	return 0;

}

void readfile(PFILE file, char *buff)
{
	unsigned int target_sector;
	unsigned char* sector_buff;
	unsigned int fat_selector;
	unsigned int fat_offset;
	unsigned int fat_sector;
	unsigned int entry_offset;
	unsigned short queue_cluster;
	unsigned char* sector_buff2;
	unsigned int active_cluster = (unsigned int)file->currentcluster;
	unsigned i;
	
	if (file){
		target_sector	= (32 + (active_cluster - 1));
		sector_buff		= (unsigned char*)read_block_fat12(target_sector,0x700);
		memcopy(buff,sector_buff, 512);
		fat_selector	= (1);
		fat_offset		= (active_cluster + (active_cluster / 2));
		fat_sector		= (fat_selector + (fat_offset / CLUSTER_SIZE));
		entry_offset	= (fat_offset % CLUSTER_SIZE);
		sector_buff2	= (unsigned char*)read_block_fat12(fat_sector,0x500);

		for (i = 0; i < 512; i++){
			FAT12[i] = sector_buff[i];
		}
		queue_cluster = *(unsigned short*)&FAT12[entry_offset];

		queue_cluster = ((active_cluster & 0x0001) ? (queue_cluster >> 4) : (queue_cluster & 0x0FFF));

		if ((queue_cluster >= EOF) || (queue_cluster == CLUSTER_END)){
			file->eof = 0x0001;
			return;
		}

		file->currentcluster = queue_cluster;

		return;
	}
}

unsigned int eof(PFILE file)
{
	if (!file->eof)
		return 0;
	return 1;
}

static get_dir_DateTime(DIRECTORY *r, char *day, char *month, char *year, char *sec, char *min, char *hour)
{

}

extern void readfile(PFILE file, char* buff);
extern PFILE findfile(const char filename[11]);
