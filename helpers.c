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

    unsigned int nMask = (1 << maskLen) - 1; // 2^maskLen - 1, example : 2^3 - 1 = 1000 - 1 = 111
    return nMask << (start - maskLen); 
}

/*  getPhysicalAddr()
*   -   converts Logical Addr to Physical Addr
*/
unsigned int getPhysicalAddr(unsigned int frame, unsigned offset, unsigned int vpnBitLen){
    return frame << (32 - vpnBitLen) | offset;
}

/*  getPages()
*   -   creates an array containing the page index for each level
*/
unsigned int* getPages(unsigned int logicalAdrr, PAGETABLE* pageTab){
    unsigned int* pages = (unsigned int*) calloc(pageTab->levelCount, sizeof(unsigned int *));
    int i;

    for( i = 0; i < pageTab->levelCount; i++){
        pages[i] = LogicalToPage(logicalAdrr, pageTab->bitmaskAry[i], pageTab->shiftAry[i]);
    }
    return pages;
}

/*  getPageSize()
*   -   calculate the page size in bytes
*/
unsigned int getPageSize(unsigned int vpnBitLen){
    unsigned int offsetLen = ADDRESS_LENGTH - vpnBitLen;
    unsigned int pageSize = ( 1 << offsetLen ) ; // 2^ offsetLen
    return pageSize ;
}