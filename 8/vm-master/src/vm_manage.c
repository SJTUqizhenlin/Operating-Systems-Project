/**
 * Morgan McCoy
 * CMPT 351
 * Homework #7
 * 
 * Built upon the provided code in vm.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#define FRAME_SIZE 256        // size of the frame
#define TOTAL_NUMBER_OF_FRAMES 256  // total number of frames in physical memory
#define ADDRESS_MASK  0xFFFF  //mask all but the address
#define OFFSET_MASK  0xFF //mask all but the offset
#define TLB_SIZE 16       // size of the TLB
#define PAGE_TABLE_SIZE 256  // size of the page table

// number of characters to read for each line from input file
#define BUFFER_SIZE         10

int pageTableNumbers[PAGE_TABLE_SIZE];
int pageTableFrames[PAGE_TABLE_SIZE];

int TLBPage[TLB_SIZE];
int TLBFrame[TLB_SIZE];

int physicalMemory[TOTAL_NUMBER_OF_FRAMES][FRAME_SIZE];

int pageFaults = 0;
int TLBHits = 0;
int first_Frame = 0;
int first_Pagetable = 0;
int numberOfTLBEntries = 0;



// number of bytes to read
#define CHUNK               256

// input file and backing store
FILE    *address_file;
FILE    *backing_store;



// the buffer containing reads from backing store
signed char     buffer[CHUNK];

// the value of the byte (signed char) in memory
signed char     value;

// headers for the functions used below
void getPage(int address);
void readFromMemory(int pageNumber);
void insertIntoTLB(int pageNumber, int frameNumber);

// function to take the logical address and obtain the physical address and value stored at that address
void getPage(int logical_address){
    
    int pageNumber = ((logical_address & ADDRESS_MASK)>>8);
    int offset = (logical_address & OFFSET_MASK);
    // search in the TLB
    int frameNumber = -1;
    int i;
    for(i = 0; i < TLB_SIZE; i++){
        if(TLBPage[i] == pageNumber){
            frameNumber = TLBFrame[i];
                TLBHits++;
        }
    }
    if(frameNumber == -1){
        // Not found in the TLB, search in the page Table
        for(i = 0; i < first_Pagetable; i++){
            if(pageTableNumbers[i] == pageNumber){
                frameNumber = pageTableFrames[i];
            }
        }
        // get frame number from physical memory
        if(frameNumber == -1){
            readFromMemory(pageNumber);
            pageFaults++;
            frameNumber = first_Frame - 1;
        }
    }
    // Insert the data into TLB
    insertIntoTLB(pageNumber, frameNumber);
    value = physicalMemory[frameNumber][offset];
    //printf("frame number: %d\n", frameNumber);
    //printf("offset: %d\n", offset); 

    // output the virtual address, physical address and value of the signed char to the console
    printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, (frameNumber << 8) | offset, value);
}

// function to insert a page number and frame number into the TLB with a FIFO replacement
void insertIntoTLB(int pageNumber, int frameNumber){
    
    int i;  // if it's already in the TLB, break
    for(i = 0; i < numberOfTLBEntries; i++){
        if(TLBPage[i] == pageNumber){
            break;
        }
    }
    
    // if the number of entries is equal to the index
    if(i == numberOfTLBEntries){
        if(numberOfTLBEntries < TLB_SIZE){
            TLBPage[numberOfTLBEntries] = pageNumber;
            TLBFrame[numberOfTLBEntries] = frameNumber;
        }
        else{
        // otherwise move everything over
            for(i = 0; i < TLB_SIZE - 1; i++){
                TLBPage[i] = TLBPage[i + 1];
                TLBFrame[i] = TLBFrame[i + 1];
            }
            // and insert the page and frame on the end
            TLBPage[numberOfTLBEntries-1] = pageNumber;
            TLBFrame[numberOfTLBEntries-1] = frameNumber;
        }        
    }
    // if the index is not equal to the number of entries
    else{
        for(i = i; i < numberOfTLBEntries - 1; i++){
            TLBPage[i] = TLBPage[i + 1];
            TLBFrame[i] = TLBFrame[i + 1];
        }
        if(numberOfTLBEntries < TLB_SIZE){
            TLBPage[numberOfTLBEntries] = pageNumber;
            TLBFrame[numberOfTLBEntries] = frameNumber;
        }
        else{
            TLBPage[numberOfTLBEntries-1] = pageNumber;
            TLBFrame[numberOfTLBEntries-1] = frameNumber;
        }
    }
    if(numberOfTLBEntries < TLB_SIZE){
        numberOfTLBEntries++;
    }
}


void readFromMemory(int pageNumber){
    // read the value
    fseek(backing_store, pageNumber * CHUNK, SEEK_SET);
    fread(buffer, sizeof(signed char), CHUNK, backing_store);

    int i;
    for(i = 0; i < CHUNK; i++){
        physicalMemory[first_Frame][i] = buffer[i];
    }
    
    pageTableNumbers[first_Pagetable] = pageNumber;
    pageTableFrames[first_Pagetable] = first_Frame;
    first_Frame++;
    first_Pagetable++;
}


int main(int argc, char *argv[])
{
    // how we store reads from input file
    char    address[BUFFER_SIZE];
    int     logical_address;

    // open the file containing the backing store
    backing_store = fopen("BACKING_STORE.bin", "rb");
    if (backing_store == NULL) {
        fprintf(stderr, "Error opening BACKING_STORE.bin %s\n","BACKING_STORE.bin");
        return -1;
    }
    
    // open the file containing the logical addresses
    address_file = fopen(argv[1], "r");
    if (address_file == NULL) {
        fprintf(stderr, "Error opening addresses.txt %s\n",argv[1]);
        return -1;
    }
    
    int n = 0;
    // read through the input file and output each logical address
    while (fgets(address, BUFFER_SIZE, address_file) != NULL) {
        logical_address = atoi(address);
        
        // get the physical address and value stored at that address
        getPage(logical_address);
        n++;
    }
    

    printf("Number of translated addresses = %d\n", n);
    
    printf("Page Faults = %d\n", pageFaults);
    printf("Page Fault Rate = %.3f\n",(pageFaults / (double)n));
    printf("TLB Hits = %d\n", TLBHits);
    printf("TLB Hit Rate = %.3f\n", (TLBHits / (double)n));
    
    // close the input file and backing store
    fclose(address_file);
    fclose(backing_store);
    
    return 0;
}

