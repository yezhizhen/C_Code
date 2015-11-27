#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FAT_ENTRY 4
int devicefd = -1;
unsigned int fat_start_address;
unsigned int start_data_address;
unsigned int BytesPerClus;
char* tokens[1025];
char* rtokens[1025];
int tokencount = 1;

#pragma pack(push,1)
struct DirEntry
{
	unsigned char DIR_Name[11]; /* File name */
	unsigned char DIR_Attr; /* File attributes */
	unsigned char DIR_NTRes; /* Reserved */
	unsigned char DIR_CrtTimeTenth; /* Created time (tenths of second) */
	unsigned short DIR_CrtTime; /* Created time (hours, minutes, seconds) */
	unsigned short DIR_CrtDate; /* Created day */
	unsigned short DIR_LstAccDate; /* Accessed day */
	unsigned short DIR_FstClusHI; /* High 2 bytes of the first cluster address */
	unsigned short DIR_WrtTime; /* Written time (hours, minutes, seconds */
	unsigned short DIR_WrtDate; /* Written day */
	unsigned short DIR_FstClusLO; /* Low 2 bytes of the first cluster address */
	unsigned long DIR_FileSize; /* File size in bytes. (0 for directories) */
};
#pragma pack(pop)

void inittokens(char** tokens)
{
	int i = 0;
	for(i=0;i<1024;i++)
	{
		//since should all in 8.3
		tokens[i] = malloc(14);
		memset(tokens[i], '\0',14);
	}
}

long unsigned int get_cluster_from_directory_entry(struct DirEntry *entry)
{
		unsigned short a[2];
		//unsigned char *gen = (unsigned char *)(entry->DIR_FstClusHI);
		unsigned short *gen = &(entry->DIR_FstClusHI);
		a[0] = *gen;
		gen = &(entry->DIR_FstClusLO);
		a[1] = *gen;
		long unsigned int starting_cluster = a[0]*16*16 + a[1];
		//starting cluster num
		return starting_cluster;
}

void tokenize(char path[],char **tokens)
{
	int i=0;
	char* temp[1024];
	temp[i++]=strtok(path,"/");
	while(temp[i-1])
	{
		temp[i++] = strtok(NULL,"/");
	}
	int k=0;
	while(temp[k++])
	{
		strcpy(tokens[k-1],temp[k-1]);
	}
	//i-1 is first empty
		tokencount = i;
}

#pragma pack(push,1)
	struct BootEntry {
		unsigned char BS_jmpBoot[3]; /* Assembly instruction to jump to boot code */
		unsigned char BS_OEMName[8]; /* OEM Name in ASCII */
		unsigned short BPB_BytsPerSec; /* Bytes per sector. Allowed values include 512,
										  1024, 2048, and 4096 */
		unsigned char BPB_SecPerClus; /* Sectors per cluster (data unit). Allowed values
										 are powers of 2, but the cluster size must be 32KB
										 or smaller */
		unsigned short BPB_RsvdSecCnt; /* Size in sectors of the reserved area */
		unsigned char BPB_NumFATs; /* Number of FATs */
		unsigned short BPB_RootEntCnt; /* Maximum number of files in the root directory for
										  FAT12 and FAT16. This is 0 for FAT32 */
		unsigned short BPB_TotSec16; /* 16-bit value of number of sectors in file system */
		unsigned char BPB_Media; /* Media type */
		unsigned short BPB_FATSz16; /* 16-bit size in sectors of each FAT for FAT12 and
									   FAT16. For FAT32, this field is 0 */
		unsigned short BPB_SecPerTrk; /* Sectors per track of storage device */
		unsigned short BPB_NumHeads; /* Number of heads in storage device */
		unsigned long BPB_HiddSec; /* Number of sectors before the start of partition */
		unsigned long BPB_TotSec32; /* 32-bit value of number of sectors in file system.
									   Either this value or the 16-bit value above must be
									   0 */

		unsigned long BPB_FATSz32; /* 32-bit size in sectors of one FAT */

		unsigned short BPB_ExtFlags; /* A flag for FAT */

		unsigned short BPB_FSVer; /* The major and minor version number */
	
		unsigned long BPB_RootClus; /* Cluster where the root directory can be
								   found */
	
		unsigned short BPB_FSInfo; /* Sector where FSINFO structure can be
								  found */
	
		unsigned short BPB_BkBootSec; /* Sector where backup copy of boot sector is
									 located */

		unsigned char BPB_Reserved[12]; /* Reserved */

		unsigned char BS_DrvNum; /* BIOS INT13h drive number */
	
		unsigned char BS_Reserved1; /* Not used */
	
		unsigned char BS_BootSig; /* Extended boot signature to identify if the
								 next three values are valid */

		unsigned long BS_VolID; /* Volume serial number */

		unsigned char BS_VolLab[11]; /* Volume label in ASCII. User defines when
									creating the file system */

		unsigned char BS_FilSysType[8]; /* File system type label in ASCII */
	};
#pragma pack(pop)

void readBoot(struct BootEntry *b, char* diskpath){
	//Open in READ-ONLY way
	if((devicefd = open(diskpath, O_RDONLY))==-1){	
		perror(diskpath);
		exit(-1);
	}
	read(devicefd,b,90);
	//close(devicefd);
	
	//Test whether it can work
//	FILE* fp = fopen("OUTPUT.TXT","w+");
//	fwrite(b,1,90,fp);
// 	printf("BytesPerSec:%u\n",b->BPB_BytsPerSec);
// 	printf("SecPerCluster:%u\n",b->BPB_SecPerClus);
// 	printf("Size of reserved area:%u (sectors)\n",b->BPB_RsvdSecCnt);
// 	printf("Number of FAT:%u\n",b->BPB_NumFATs);
// //	printf("16-bit value of number of sectors in file system: %u\n",b->BPB_TotSec16);
// 	printf("32-bit value of number of sectors in file system: %lu\n",b->BPB_TotSec32);
// 	printf("32-bit size in sectors of one FAT:%lu\n",b->BPB_FATSz32);
// 	printf("Cluster where the root directory can be found:%lu\n",b->BPB_RootClus);

}

char* getName(unsigned char* bname , unsigned char attr)
{
	//printf("attr:%u\n",attr);	
	//bname is entry->name. 0-7 8-10
	char *result=malloc(14);
	memset(result,0,14);	
	int count = 0;
	//This is a deleted one
	if(bname[0]==0xe5)
	{
		result[0] = '?';
		count++;
	}
	while(count<8 && bname[count]!=0x20)
	{
		result[count] = bname[count];
		count++;
	}
	//printf("count:%d\n",count);
	int isdir = 0;
	//if it is directory
	if(attr==0x10) isdir = 1;	
	//add extension
	if(bname[8]!=0x20) result[count] = '.';
	else 
	{
		if(isdir)	result[count] = '/';
		return result;
	}
	count++;
	int count2 = 8;
	//add following
	while(count2<11&&bname[count2])
	{
		result[count++] = bname[count2++];
	}
	if(isdir)	result[count] = '/';
	return result;

}

unsigned int cluster_to_address(unsigned int clusnum)
{
	return 	(clusnum-2)*BytesPerClus + start_data_address;

}

unsigned int checkCurrentCluster(unsigned int c_cluster)
{
	unsigned int fat_c_address = fat_start_address + c_cluster*FAT_ENTRY;	
	//read fat entry of current
	lseek(devicefd,fat_c_address,SEEK_SET);
	unsigned int data;
	read(devicefd,&data,FAT_ENTRY);
	return data;
}

unsigned int getNextCluster(unsigned int c_cluster)
{
	//printf("c_cluster:%d\n",c_cluster);
	//goal: return the next Cluster number, and seek to that place
	//first, find fat[c_cluster}
	unsigned int fat_c_address = fat_start_address + c_cluster*FAT_ENTRY;	
	//read fat entry of current
	lseek(devicefd,fat_c_address,SEEK_SET);
	unsigned int data;
	read(devicefd,&data,FAT_ENTRY);
	data = data|0xF0000000;
	//if this is the end sector
	if((data>=0xFFFFFFF8) && (data<=0xFFFFFFFF))
	{
		printf("End of file\n");
		return -1;
	}
	unsigned int next_clu_num;
	unsigned int next_address;
	//not end. Find next sector.
	if((data>=0xF0000002)&&(data<=0xFFFFFFEF))
	{
		next_clu_num = data - 0xF0000000;
		printf("next_cluster: %u\n", next_clu_num);
		//seek to that place
		next_address = cluster_to_address(next_clu_num);
		lseek(devicefd,next_address,SEEK_SET);
		//now, return	
		return next_clu_num;
	}	
	
}

void printusage(char token1[])
{
	printf("Usage: %s -d [device filename] [other arguments]\n",token1);
	printf("-l target\tList the target directory\n");
	printf("-r target -o dest\tRecover the target pathname\n");
	exit(EXIT_SUCCESS);
}



void recoverfiles(char* r_targetpath,char* o_outputpath,struct BootEntry* b)
{
	//find the sector num of root directory
	unsigned long filesize = -1;
	unsigned int rootcluster =  b->BPB_RootClus;	
	unsigned int rootsectornum = b->BPB_RsvdSecCnt + b->BPB_NumFATs * b->BPB_FATSz32 + rootcluster -2;
	unsigned int rootaddress = rootsectornum * b->BPB_BytsPerSec;		
	//Now you know the address, first read directory Entries
	//Here, i represents layer. 
	int i=1;
	unsigned int currentcluster =rootcluster;
	//we need address for the starting point of the folder
	unsigned int address = rootaddress;
	unsigned int current_address;
	//start with root
	current_address = lseek(devicefd, rootaddress, SEEK_SET);
	//find the directory
	//use i, but not i-1. Assures find the one before file
 	while(rtokens[i][0]!='\0')
	{
		//assume you have reached layer i-1. Start with layer 0, i.e. root
		//so, try to match tokens[i-1] in the directory tokens[i-2]. Or -1 as root.  
		strcat(rtokens[i-1],"/");
		while(1)
		{
			struct DirEntry *entry = malloc(sizeof(struct DirEntry));
			//read the device entry and advance.	
			read(devicefd, entry, sizeof(struct DirEntry));
			current_address = lseek(devicefd, 0,SEEK_CUR); 
			if(current_address>(cluster_to_address(currentcluster+1)))
			{
				//look for next cluster. go to fat table.
				currentcluster = getNextCluster(currentcluster);
				read(devicefd, entry, sizeof(struct DirEntry));	
			}
			//check if it is LNF
			while(entry->DIR_Attr == 0x0f)
			{
				read(devicefd, entry, sizeof(struct DirEntry));
				if(current_address>cluster_to_address(currentcluster+1))
				{
					currentcluster = getNextCluster(currentcluster);
					read(devicefd, entry, sizeof(struct DirEntry));
				//look for next cluster. go to fat table.
				}
			}
			//check if it is directory
			if(entry->DIR_Attr != 0x10)
			{
				free(entry);
				continue;
			}
			//now, time to test name!
			char *name = getName(entry->DIR_Name,entry->DIR_Attr);	
			//check if you got it. 
			if(strcmp(name,rtokens[i-1])==0)
			{ 
				//got it. Now, seek to that place
				currentcluster = get_cluster_from_directory_entry(entry);	
				current_address = cluster_to_address(currentcluster);
				lseek(devicefd,current_address,SEEK_SET);  
				free(entry);
				free(name);
				break;
			}
			free(entry);
			free(name);
		}
		//in root, find that one
		i++;
 	}	
	//Now, you have reached the directory set containing the file. Search in this directory to get the file.	
	char copy_of_file[1024];
	strcpy(copy_of_file, rtokens[i-1]);
	copy_of_file[0] = '?';
	int emptyflag = 0;
	//printf("%s\n",rtokens[i-1]);
	while(1)
	{
		struct DirEntry *entry = malloc(sizeof(struct DirEntry));
		//read the device entry and advance.	
		read(devicefd, entry, sizeof(struct DirEntry));
		current_address = lseek(devicefd, 0,SEEK_CUR); 
		if(current_address>(cluster_to_address(currentcluster+1)))
		{
			//look for next cluster. go to fat table.
			currentcluster = getNextCluster(currentcluster);
			
			if(currentcluster == -1)
			{
				printf("%s: error - file not found\n",rtokens[i-1]);
				exit(EXIT_FAILURE);
			}
			read(devicefd, entry, sizeof(struct DirEntry));	
		}
		//check if it is LNF
		while(entry->DIR_Attr == 0x0f)
		{
			read(devicefd, entry, sizeof(struct DirEntry));
			if(current_address>cluster_to_address(currentcluster+1))
			{
				currentcluster = getNextCluster(currentcluster);
				if(currentcluster == -1)
				{
					printf("%s: error - file not found\n",rtokens[i-1]);
					exit(EXIT_FAILURE);
				}
				read(devicefd, entry, sizeof(struct DirEntry));
			//look for next cluster. go to fat table.
			}
		}
		//check if it is directory. If yes, skip it. We want file.
		if(entry->DIR_Attr == 0x10)
		{
			free(entry);
			continue;
		}
		//now, time to test name!
		char *name = getName(entry->DIR_Name,entry->DIR_Attr);	
		//check if you got it. 
		if(strcmp(name,copy_of_file)==0)
		{ 
			//got it. Now, seek to that place
			currentcluster = get_cluster_from_directory_entry(entry);	
			current_address = cluster_to_address(currentcluster);
			//check if file is empty
			if((currentcluster!=0)&&(checkCurrentCluster(currentcluster)!=0))
			{
				printf("%s: error - fail to recover\n",rtokens[i-1]);	
				exit(EXIT_FAILURE);
			}
			if(currentcluster == 0)
			{
				emptyflag = 1;
			}
			printf("%s: recovered\n",rtokens[i-1]);
			filesize = entry->DIR_FileSize;
			printf("file size:%lu\n",filesize);
			lseek(devicefd,current_address,SEEK_SET);  
			free(entry);
			free(name);
			break;
		}
		free(entry);
		free(name);
	}
	//Now, you are at the file cluster. Do recovery!
	FILE *out =NULL;
	char buf[filesize];
	//if not able to open
	if(!(out=fopen(o_outputpath,"w+")))
	{
		printf("%s: failed to open\n",rtokens[i-1]);
		exit(EXIT_FAILURE);
	}
	//Copy the Content to the file
	//write one by one	
	read(devicefd, buf,filesize);
	fwrite(buf,1,filesize,out);
	fclose(out);
	exit(EXIT_SUCCESS);	
}	

void listfiles(struct BootEntry* b,char* diskpath,char* lpath)
{
	//find the sector num of root directory
	unsigned int rootcluster =  b->BPB_RootClus;	
	unsigned int rootsectornum = b->BPB_RsvdSecCnt + b->BPB_NumFATs * b->BPB_FATSz32 + rootcluster -2;
	unsigned int rootaddress = rootsectornum * b->BPB_BytsPerSec;		
	//Now you know the address, first read directory Entries
	//Here, i represents layer. 
	int i=1;
	unsigned int currentcluster =rootcluster;
		
	//we need address for the starting point of the folder
	unsigned int address = rootaddress;
	unsigned int current_address;
	//start with root
	current_address = lseek(devicefd, rootaddress, SEEK_SET);
	//find the directory
 	while(tokens[i-1][0]!='\0'){
		//assume you have reached layer i-1. Start with layer 0, i.e. root
		//so, try to match tokens[i-1] in the directory tokens[i-2]. Or -1 as root.  
		strcat(tokens[i-1],"/");
		while(1)
		{
			struct DirEntry *entry = malloc(sizeof(struct DirEntry));
			//read the device entry and advance.	
			read(devicefd, entry, sizeof(struct DirEntry));
			current_address = lseek(devicefd, 0,SEEK_CUR); 
			if(current_address>(cluster_to_address(currentcluster+1)))
			{
				//look for next cluster. go to fat table.
				currentcluster = getNextCluster(currentcluster);
				read(devicefd, entry, sizeof(struct DirEntry));	
			}
			//check if it is LNF
			while(entry->DIR_Attr == 0x0f)
			{
				read(devicefd, entry, sizeof(struct DirEntry));
				if(current_address>cluster_to_address(currentcluster+1))
				{
					currentcluster = getNextCluster(currentcluster);
					read(devicefd, entry, sizeof(struct DirEntry));
				//look for next cluster. go to fat table.
				}
			}
			//check if it is directory
			if(entry->DIR_Attr != 0x10)
			{
				free(entry);
				continue;
			}
			//now, time to test name!
			char *name = getName(entry->DIR_Name,entry->DIR_Attr);	
			//check if you got it
			if(strcmp(name,tokens[i-1])==0)
			{ 
				//got it. Now, seek to that place
				currentcluster = get_cluster_from_directory_entry(entry);	
				current_address = cluster_to_address(currentcluster);
				lseek(devicefd,current_address,SEEK_SET);  
				free(entry);
				free(name);
				break;
			}
			free(entry);
			free(name);
		}
		//in root, find that one
// 		printf("tokens[%d]: %s\n",i-1,tokens[i-1]);	

		i++;

 	}	
		//Now you have found the place to start searching
		int count = 1;
 		while(1)
 		{	
			//allocate new directory entry
			struct DirEntry *entry = malloc(sizeof(struct DirEntry));
			//read the device entry and advance.	
			read(devicefd, entry, sizeof(struct DirEntry));
			current_address = lseek(devicefd, 0,SEEK_CUR); 
			if(current_address>(cluster_to_address(currentcluster+1)))
			{
				//look for next cluster. go to fat table.
				currentcluster = getNextCluster(currentcluster);
				if(currentcluster == -1)	
					exit(EXIT_SUCCESS);
				read(devicefd, entry, sizeof(struct DirEntry));	
			}
			if(entry->DIR_Attr == 0x00)
			{
				break;
			}
			//check if it is LNF
			while(entry->DIR_Attr == 0x0f)
			{
				read(devicefd, entry, sizeof(struct DirEntry));
				if(current_address>cluster_to_address(currentcluster+1))
				{
					currentcluster = getNextCluster(currentcluster);
					if(currentcluster == -1)
						exit(EXIT_SUCCESS);
					read(devicefd, entry, sizeof(struct DirEntry));
				//look for next cluster. go to fat table.
				}
			}
			//now, we reach the normal portion
			//here, a is the address of cluster 
			//starting cluster num
			long unsigned int starting_cluster = get_cluster_from_directory_entry(entry); 		
			char *name = getName(entry->DIR_Name,entry->DIR_Attr);	
			//order, name, file size, starting cluster
			printf("%d, %s, %lu, %ld\n", count, name, entry->DIR_FileSize, starting_cluster);
			count++;
			//lseek(devicefd, 32, SEEK_CUR);
			free(name);
			free(entry);

 		}
		close(devicefd);

	

}

int main(int argc, char** argv)
{
	int c;
	opterr = 0;

	int dflag=0;
	int lflag=0;
	int rflag=0;
	int oflag=0;
	char* diskpath = NULL;
	char* lpath = NULL;
	char* r_targetpath = NULL;
	char* o_outputpath = NULL;
	while((c=getopt(argc,argv,"l:r:d:o:")) !=-1)
	{
		switch(c)
		{
			case 'd':
				dflag = 1;
				diskpath = optarg;					
				break;	
			case 'l':
				lflag = 1;
				lpath = optarg;
				if(dflag==0)
					printusage(argv[0]);
				break;
			case 'r':
				rflag=1;
				r_targetpath = optarg;
				if(dflag==0)
					printusage(argv[0]);
				break;
			case 'o':
				oflag=1;
				o_outputpath = optarg;
				if(dflag==0)
					printusage(argv[0]);	
				break;
			case '?':
				printusage(argv[0]);					
		
		}
	}
	//printf("d: %s\nl: %s\nr: %s\no: %s\n",diskpath, lpath, r_targetpath, o_outputpath);
	
	//now, we know d must exist. Hence consider l^(r&&o) == 1. 	
	if((lflag^(rflag&&oflag))!=1)
		printusage(argv[0]);
	//now, we know only one side is 1
	if(lflag==1)
	{
		if((rflag||oflag)==1)
			printusage(argv[0]);
	}
	int i= 0;
	struct BootEntry boot;
	readBoot(&boot,diskpath);
	BytesPerClus = boot.BPB_BytsPerSec * boot.BPB_SecPerClus;
	fat_start_address = boot.BPB_RsvdSecCnt * boot.BPB_BytsPerSec;
	start_data_address = (boot.BPB_RsvdSecCnt + boot.BPB_NumFATs * boot.BPB_FATSz32)*boot.BPB_BytsPerSec;
	//handle -l cases
	if(lflag==1)
	{
		inittokens(tokens);
		tokenize(lpath,tokens);
		char *teml;
		// has been tokenize by /
 		while(strcmp(teml=tokens[i++],"")!=0)
 		{
			int pre_count_l =0;
			int suf_count_l =0;
			int dotflag_l = 0;	
			while(*teml!='\0')
			{
				if(*teml == '.')
				{
					dotflag_l = 1;
				}
				else if(dotflag_l == 0)
				{
					if(*teml!='/')
						pre_count_l++;
				}
				else if(dotflag_l == 1)
				{
					suf_count_l++;
				}
				teml++;
			}
			if(pre_count_l >8 || suf_count_l >3 || (suf_count_l == 0 && dotflag_l == 1))
			{
				printf("Not in 8.3 format!\n");
				printusage(argv[0]);
			}
 		}
  	//Now, finished reading input
		//if not in 8.3, print
		listfiles(&boot,diskpath,lpath);	
	}	
	//handle -r -o cases.
	//input won't be empty, because of getopt
	else
	{
		inittokens(rtokens);
		tokenize(r_targetpath,rtokens);
		char *temr = r_targetpath;
 		while(strcmp(temr=rtokens[i++],"")!=0)
 		{
			int pre_count_r =0;
			int suf_count_r =0;
			int dotflag_r = 0;	
			while(*temr!='\0')
			{
				if(*temr == '.')
				{
					dotflag_r = 1;
				}
				else if(dotflag_r == 0)
				{
					if(*temr!='/')
						pre_count_r++;
				}
				else if(dotflag_r == 1)
				{
					suf_count_r++;
				}
				temr++;
			}
			if(pre_count_r >8 || suf_count_r >3 || (suf_count_r == 0 && dotflag_r == 1))
			{
				printf("Not in 8.3 format!\n");
				printusage(argv[0]);
			}
 		}
		recoverfiles(r_targetpath,o_outputpath,&boot);	
	}

	//printf("pre_count:%d\n suf_count:%d\n dotflag:%d\n",pre_count_l,suf_count_l,dotflag_l);
}
