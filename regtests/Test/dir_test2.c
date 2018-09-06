/*
 * dir_test.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include"adflib.h"


void MyVer(char *msg)
{
    fprintf(stderr,"Verbose [%s]\n",msg);
}

void myPrintEntry(struct bEntryBlock* entry) {
  printf("--- Entry for sector [%d] ---\n", entry->headerKey);
  printf("type             [%d]\n", entry->type);
  printf("\n");
  printf("headerKey        [%d]\n", entry->headerKey);
  printf("\n");
  printf("r1[0]            [%d]\n", entry->r1[0]);
  printf("r1[1]            [%d]\n", entry->r1[1]);
  printf("r1[2]            [%d]\n", entry->r1[2]);
  printf("\n");
  printf("checkSum         [%u]\n", (unsigned int)entry->checkSum);
  printf("\n");
  for(int i = 0; i < HT_SIZE; i++) {
    printf("hashTable[%2d]    [%d]\n", i, entry->hashTable[i]);
  }
  printf("\n");
  printf("r2[0]            [%d]\n", entry->r2[0]);
  printf("r2[1]            [%d]\n", entry->r2[1]);
  printf("\n");
  printf("access           [%d]\n", entry->access);
  printf("\n");
  printf("byteSize         [%d]\n", entry->byteSize);
  printf("\n");
  printf("commLen          [%d]\n", entry->commLen);
  printf("\n");
  for(char i = 0; i < MAXCMMTLEN+1; i++) {
     printf("comment[%2d]      [%c]\n", i, entry->comment[i]);
  }
  printf("\n");
  for(int i = 0; i < 91-(MAXCMMTLEN+1); i++) {
	 printf("r3[%2d]           [%d]\n", i, entry->r3[i]);
  }
  printf("\n");
  printf("days             [%d]\n", entry->days);
  printf("\n");
  printf("mins             [%d]\n", entry->mins);
  printf("\n");
  printf("ticks            [%d]\n", entry->ticks);
  printf("\n");
  printf("nameLen          [%d]\n", entry->nameLen);
  printf("\n");
  for(char i = 0; i < MAXNAMELEN+1; i++) {
	printf("name[%2d]         [%c]\n", i, entry->name[i]);
  }
  printf("\n");
  printf("r4               [%d]\n", entry->r4);
  printf("\n");
  printf("realEntry        [%d]\n", entry->realEntry);  
  printf("\n");
  printf("nextLink         [%d]\n", entry->nextLink);
  printf("\n");
  printf("r5[0]            [%d]\n", entry->r5[0]); 
  printf("r5[1]            [%d]\n", entry->r5[1]);
  printf("r5[2]            [%d]\n", entry->r5[2]);
  printf("r5[3]            [%d]\n", entry->r5[3]);
  printf("r5[4]            [%d]\n", entry->r5[4]);
  printf("\n");
  printf("nextSameHash     [%d]\n", entry->nextSameHash);
  printf("\n");
  printf("parent           [%d]\n", entry->parent);
  printf("\n");
  printf("extension        [%d]\n", entry->extension);
  printf("\n");
  printf("secType          [%d]\n", entry->secType);
  printf("-----------------------------\n"); 
}


SECTNUM 
myGetSectNum(struct Entry* entry)
{
    return entry->sector;
}

struct bEntryBlock* 
myGetDirEntry(struct Volume* vol, SECTNUM sector)
{
	size_t entry_size = sizeof(struct bEntryBlock);
    struct bEntryBlock* dir = malloc(entry_size);
    memset(dir, 0, sizeof(entry_size));
    RETCODE on_read_dir = adfReadEntryBlock(vol, 
                                            sector, 
                                            dir);
    if (on_read_dir != RC_OK) {
        printf(
            "Failed to read entry block for sector [%d].\n", 
            sector);
        return NULL;
    }
    return dir;
}

void myWriteDirEntry(struct Volume* vol, 
                     SECTNUM sector,
                     struct bEntryBlock* dir)  
{
	RETCODE on_write_dir = adfWriteEntryBlock(
	    vol, 
	    sector, 
	    dir);
 	if (on_write_dir != RC_OK) {
 	    printf(
 	       "Failed to write entry block for sector [%d].\n", 
 	       sector);
 	}
}
 	




/*
 *
 *
 */
int main(int argc, char *argv[])
{
    struct Device *hd;
    struct Volume *vol;
    struct List *list, *cell;
 
    adfEnvInitDefault();

//	adfSetEnvFct(0,0,MyVer,0);

    /* mount existing device */
    hd = adfMountDev( argv[1],FALSE );
    if (!hd) {
        fprintf(stderr, "can't mount device\n");
        adfEnvCleanUp(); exit(1);
    }

    vol = adfMount(hd, 0, FALSE);
    if (!vol) {
        adfUnMountDev(hd);
        fprintf(stderr, "can't mount volume\n");
        adfEnvCleanUp(); exit(1);
    }

    adfVolumeInfo(vol);
  
  
    //////////////////////////////////////////////
    printf("\n> Start hacking!\n");   
    char* dir_name = "mydir";
    int dir_hash = adfGetHashValue(dir_name, 0);
    printf("Hash for name [%s] is [%d].\n", 
           dir_name, 
           dir_hash);    
    // Create a directory
    adfCreateDir(vol, vol->curDirPtr, dir_name);
    SECTNUM mydir_sector = myGetSectNum(
        adfGetDirEnt(vol, vol->curDirPtr) 
        -> next 
        -> content);   
	// Create nested directory
    adfCreateDir(vol, mydir_sector, dir_name);
    struct bEntryBlock* mydir_entry = myGetDirEntry(
        vol, 
        mydir_sector);
    myPrintEntry(mydir_entry);
    SECTNUM mydir_nested_sector = myGetSectNum(
        adfGetDirEnt(vol, mydir_sector)
        -> content);
    
    
    // Make two directories with the same name possible
    /*
    if (dir_hash < HT_SIZE - 1) {
        mydir_entry -> 
        hashTable[dir_hash + 1] = 
        mydir_nested_sector;
    } else {
        mydir_entry -> 
        hashTable[dir_hash - 1] = 
        mydir_nested_sector;
    }
    */ 
  
    
    // Make "infinite" listing of directory
    /*
    mydir_entry -> 
    nextSameHash =
    mydir_sector;
  
    mydir_entry -> 
    hashTable[dir_hash] = 
    mydir_sector;
    */
  
    
    // Make circular directory (to itself)
    ///*
    mydir_entry -> 
    hashTable[dir_hash - 1] = 
    mydir_nested_sector;
  
    mydir_entry -> 
    hashTable[dir_hash] = 
    mydir_sector;
    //*/
  
    
    myWriteDirEntry(vol, mydir_sector, mydir_entry);
	printf("\n> Finish hacking!\n");
    //////////////////////////////////////////////


    adfUnMount(vol);
    adfUnMountDev(hd);
    adfEnvCleanUp();
    return 0;
}
