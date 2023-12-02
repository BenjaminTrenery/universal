#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define NUM_FRAMES 256

//global variables and arrays
int pageTable[FRAME_SIZE];
int TLB[TLB_SIZE][2];
int currentTLBSize = 0;
int pageFaultCount = 0;
int TLBHitCount = 0;
int totalAccesses = 0;
char physicalMemory[NUM_FRAMES][FRAME_SIZE];
int currentFrameNum = 0;

//functions that checks if inputted page number is in the TLB table
//the most recent page number and frame number are put at the bottom of the array and all other values are shifted up
//returns the frame num (if page number not found the returned value is -1)
int checkTLB(int pageNumber){

    int frameNum = -1;
    int pageFound = 0;
    int i;

    for(i = 0; i < TLB_SIZE; i++){
        if(TLB[i][0] == pageNumber){

            TLBHitCount++;
            frameNum = TLB[i][1];
            pageFound = 1;
            // printf("Found TLB \n");
            break;
        }
    }

    if(pageFound == 0){

        for(int x = 0; x < (TLB_SIZE - 1); x++){

            TLB[x][0] = TLB[x + 1][0];
            TLB[x][1] = TLB[x + 1][1];
        
        }

    }
    else {
        for(int y = i; y < (TLB_SIZE - 1); y++){
            
            TLB[y][0] = TLB[y + 1][0];
            TLB[y][1] = TLB[y + 1][1];

        }
    }

    TLB[15][0] = pageNumber;
    TLB[15][1] = pageTable[pageNumber];

    return frameNum;

}

//this function uses the BACKED_STORAGE.bin file and gets the char values based on the position
//looked to see if the page exists then reads the values into the buffer for a length of FRAME_SIZE
//then each individual char is put into the physical memory array
//finally, the pageTable is updated with the currentFrameNum value 
void pageFault(int pageNumber, FILE *b_Store){


    signed char buffer[FRAME_SIZE];

    if(fseek(b_Store, pageNumber * PAGE_SIZE, SEEK_SET) != 0){
        printf("Page not found! \n");
        exit(0);
    }

    fread(buffer, FRAME_SIZE, 1, b_Store);

    for(int i = 0; i < FRAME_SIZE; i++){
        physicalMemory[currentFrameNum][i] = buffer[i];
    }

    pageTable[pageNumber] = currentFrameNum;
    currentFrameNum++;

}

//uses the pageTable to find the frame number
//examines if the page is in bounds
//if the frame doesn't exist (-1), then it calls the pageFault function
//returns the frameNum at the end
int getLoadFrame(int logicalAddress, FILE *b_Store, int pageNumber){

    if(pageNumber < 0 || pageNumber >= PAGE_SIZE){
        printf("ERROR: Page number out of bounds \n");
        exit(0);
    }

    if(pageTable[pageNumber] == -1){

        pageFaultCount++;
        pageFault(pageNumber, b_Store);
    }

    return pageTable[pageNumber];
}

int main(int argc, char* argv[]){
    
    //checks to see if there are the right amount of arguments
    if(argc != 2){
        printf("ERROR: incorrect number of arguments \n");
        exit(0);
    }

    //opens the address file and checks if it exists
    FILE *addressFile = fopen(argv[1], "r");

    if(addressFile == NULL){
        printf("ERROR: couldn't open address file, try again! \n");
        fclose(addressFile);
        exit(0);
    }
    
    //opens the BACKING_STORE.bin file and checks if it exists
    FILE *b_Store = fopen("BACKING_STORE.bin", "rb");

    if(b_Store == NULL){
        printf("Couldn't open file! \n");
        fclose(b_Store);
        exit(0);
    }

    //initializes the pageTable values -1
    for(int i = 0; i < NUM_FRAMES; i++){
        pageTable[i] = -1;
    }

    //initializes the TLB table with values of -1
    for(int x = 0; x < TLB_SIZE; x++){

        for(int y = 0; y < 2; y++){

            TLB[x][y] = -1;
        }
    }


    char lineOfData[FRAME_SIZE];
    int logicalAddress;
    
    //parsing the addressFile, while the current line is not null, it will get the logical address
    //it will then find the page_offset and pageNumber from the logical address
    //finally it will check the TLB if the pageNum is in it
    //if it isn't it looks in the pageTable
    //finally the signedByte and physical address are calcualted using the information and the results are printed
    while(fgets(lineOfData, sizeof(lineOfData), addressFile) != NULL){
        
        if (sscanf(lineOfData, "%d", &logicalAddress) == 1) {

            totalAccesses++;
            int page_offset = logicalAddress & 0xFF;
            int pageNumber = (logicalAddress >> 8) & 0xFF;
            int frameNum;
            frameNum = checkTLB(pageNumber);
            
            if(frameNum == -1){

                frameNum = getLoadFrame(logicalAddress, b_Store, pageNumber);
            }

            int physicalAddress = (frameNum * FRAME_SIZE) + page_offset;

            char signedByte = physicalMemory[frameNum][page_offset];

            printf("Logical Address: %d, Physical Address: %d, Signed Byte: %d \n", logicalAddress, physicalAddress, signedByte);
        }
        else {
            // Handle parsing error
            fprintf(stderr, "Error parsing integer from file \n");
        }
    }

    //used for calculating the rate of page faults and TLB hits over all logical address accesses and prints it
    float pageFaultRate = (float)pageFaultCount / totalAccesses;
    float TLBHitRate = (float)TLBHitCount / totalAccesses;

    printf("Page-fault rate: %.2f%% \n", pageFaultRate * 100);
    printf("TLB hit rate: %.2f%% \n", TLBHitRate * 100);

    //closes the two open files
    fclose(addressFile);
    fclose(b_Store);

    return 0;
}