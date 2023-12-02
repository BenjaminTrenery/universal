#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define NUM_FRAMES 256

int pageTable[PAGE_SIZE];
int TLB[TLB_SIZE][2];
int currentTLBSize = 0;
int pageFaultCount = 0;
int TLBHitCount = 0;
int totalAccesses = 0;
int physicalMemory[NUM_FRAMES][FRAME_SIZE];
int currentPhysicalMemoryPos = 0;

void checkTLB(){



}

void pageFault(int pageNumber, int page_offset){

    FILE *b_Store = fopen("BACKING_STORE.bin", "rb");

    if(b_Store == NULL){
        printf("Couldn't open file! \n");
        exit(0);
    }

    signed char buffer[256];

    if(fseek(b_Store, pageNumber * PAGE_SIZE, SEEK_SET) != 0){
        printf("Page not found! \n");
        exit(0);
    }

    fread(buffer, sizeof(buffer), 1, b_Store);

    for(int i = 0; i < 256; i++){
        physicalMemory[currentPhysicalMemoryPos][i] = buffer[i];
    }

    pageTable[pageNumber] = pageNumber;
    currentPhysicalMemoryPos++;

    fclose(b_Store);

}

int translateAddress(int logicalAddress){

    int pageNumber = (logicalAddress >> 8) & 0xFF;
    int page_offset = logicalAddress & 0xFF;

    if(pageNumber < 0 || pageNumber >= PAGE_SIZE){
        printf("ERROR: Page number out of bounds \n");
        exit(0);
    }

    if(pageTable[pageNumber] == -1){

        pageFaultCount++;
        pageFault(pageNumber, page_offset);
    }

    int physicalAddress = (pageTable[pageNumber] * FRAME_SIZE) + page_offset;

    return physicalAddress;
}

int main(int argc, char* argv[]){
    
    if(argc != 2){
        printf("ERROR: incorrect number of arguments \n");
        exit(0);
    }

    FILE *addressFile = fopen(argv[1], "r");

    if(addressFile == NULL){
        printf("ERROR: couldn't open address file, try again! \n");
        exit(0);
    }

    memset(pageTable, -1, sizeof(pageTable));
    memset(TLB, -1, TLB_SIZE * 2 * sizeof(TLB[0][0]));

    char lineOfData[256];
    int logicalAddress;
    
    while(fgets(lineOfData, sizeof(lineOfData), addressFile) != NULL){
        
        if (sscanf(lineOfData, "%d", &logicalAddress) == 1) {

            totalAccesses++;
            int physicalAddress = translateAddress(logicalAddress);

            printf("%d\n", logicalAddress);
        }
        else {
            // Handle parsing error
            fprintf(stderr, "Error parsing integer from file \n");
        }
    }

    fclose(addressFile);

    return 0;
}