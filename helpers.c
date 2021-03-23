// <getopt.h> is included in <unistd.h> 
// and only the unix standard* header need be included.
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "pageFunctions.h"


/*  getBitAry()
*   -   extracts num of bits for each level from argv and converts to int
*   -   returns num of bits used for Virtual Page Number
*/
unsigned int getBitAry(unsigned int* bitAry, int levelCount, int argc, char** argv){
    int startIdx = argc - levelCount;
    unsigned int sum = 0;    // num of logical address bits
    int i;

    for(i = 0; i < levelCount; i++){
        sum += atoi(argv[startIdx]);
        bitAry[i] = atoi(argv[startIdx++]);
    }
    
    // Check if num of the total of bits used is valid
    if ( sum >= ADDRESS_LENGTH){
        fprintf(stderr,"Logical address length should be less than ADDRESS_LENGTH = %d bits\n", ADDRESS_LENGTH);
        exit(1);
    }

    return sum;
}

/*  generateBitmask()
*   -   creates a bit mask from a starting bit with specified length
*/
unsigned int generateBitmask(unsigned int start, unsigned int maskLen){

   unsigned int r = 0;
   unsigned int i;
   for (i=start; i<= maskLen; i++)
       r |= 1 << i;

   return r;
}

unsigned int getPhysicalAddr(unsigned int frame, unsigned offset, unsigned int vpnBitLen){
    // printf("frame %05X\n", frame);
    return frame << (32 - vpnBitLen) | offset;
}

unsigned int* getPages(unsigned int logicalAdrr, PAGETABLE* pageTab){
    unsigned int* pages = (unsigned int*) calloc(pageTab->levelCount, sizeof(unsigned int *));
    int i;

    for( i = 0; i < pageTab->levelCount; i++){
        pages[i] = LogicalToPage(logicalAdrr, pageTab->bitmaskAry[i], pageTab->shiftAry[i]);
    }
    return pages;
}

unsigned int getPageSize(){
    return 1;
}