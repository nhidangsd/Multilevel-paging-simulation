#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// <getopt.h> is included in <unistd.h> 
// and only the unix standard* header need be included.
#include <unistd.h> 
#include "structures.h"
#include "pageFunctions.h"
#include "byutr.h"
#include "output_mode_helpers.h"

/*  parseArgs()
*   - scans the argc and argv to get num of levels
*   - set the Option Arguments on if there is any
*/
void parseArgs(int argc, char** argv, OPTION_ARGS* flags, unsigned int* addrLimit, char** outputMode, int* levelCount){
    
    /* check usage */
    if(argc < 2) {
        fprintf(stderr,"usage: %s input_byutr_file\n", argv[0]);
        exit(1);
    }

    int opt;
    const char *optString="n:o:"; // only take '-h' as option arg
    *levelCount = argc - 2;

    while( (opt = getopt(argc, argv, optString)) != -1){

        switch (opt){
            
            case 'n':
                flags->n = 1;
                *levelCount -= 2;
                *addrLimit = atoi(optarg);
                break;

            case 'o':
                flags->o = 1;
                *levelCount -= 2;
                *outputMode = optarg;
                break;

            // Exit if option arg is illegal
            case '?':
                return;

            default:
                break;

        }
    }
}

void runMultiPageTabSimulator(int argc, char** argv, OPTION_ARGS flags, int addrLimit,char* outputMode, int levelCount){

    FILE *ifp;	            // trace file
    p2AddrTr trace;	        // traced address
  
    // attempt to open trace file 
    int traceFileIndex = argc - levelCount - 1;
    if ((ifp = fopen(argv[traceFileIndex], "rb")) == NULL) {
        fprintf(stderr,"cannot open %s for reading\n", argv[traceFileIndex]);
        exit(1);
    }
	
     // Scan num of bits for each level from argv
    unsigned int bitAry[levelCount];
    unsigned int vpnBitLen = getBitAry(bitAry, levelCount, argc, argv);

    // Create PageTable to use
    PAGETABLE* pageTab = newPageTab(levelCount, bitAry);

    // int i;
    // for (i = 0; i < pageTab->levelCount; i++) {
    //     printf("LEVEL %i INFO: ", i);
    //     printf("levelCount %d: \n", pageTab->levelCount);
    //     printf("BitAry: %d\tMask: %08X\tShift: %i\tEntry Count: %i\n", bitAry[i], pageTab->bitmaskAry[i], pageTab->shiftAry[i], pageTab->entryCountAry[i]);
    // }

    unsigned int hits, misses, addrCount, frame, offset, logicalAddr;
    hits = misses = addrCount = frame = 0;
    MAP* map;

    while (!feof(ifp)) {

        // only the first N memory references if flag -n is ON
        if(flags.n && addrCount >= addrLimit){
            break;
        }

        // get next address and process
        if (NextAddress(ifp, &trace)) {
            addrCount++;
            logicalAddr = (unsigned int)trace.addr;
            // printf("\n===========Logical Addr: %08X\n", logicalAddr);

            // Insert frame if doesnt exist
            // if( PageInsert(pageTab, logicalAddr, frame)){
            //     misses++;
            //     frame++;
            // } 
            // else {
            //     hits++;
            // }

            PageInsert(pageTab, logicalAddr, misses) ? misses++  : hits++;

            map = PageLookup(pageTab, logicalAddr);
            offset = (logicalAddr << vpnBitLen) >> vpnBitLen;

            if(flags.o){
                if( !strcmp(outputMode, "bitmasks") ){
                    report_bitmasks (pageTab->levelCount, pageTab->bitmaskAry) ;
                }
                else if( !strcmp(outputMode, "logical2physical") ){
                    unsigned physicalAddr = getPhysicalAddr(map->frame, offset, vpnBitLen);
                    report_logical2physical (logicalAddr, physicalAddr);
                }
                else if( !strcmp(outputMode, "page2frame") ){
                    unsigned int* pages = getPages(logicalAddr, pageTab);
                    report_pagemap (logicalAddr, pageTab->levelCount, pages, map->frame);
                }
                else if( !strcmp(outputMode, "offset") ){
                    report_logical2offset (logicalAddr, offset);
                }
                else if( !strcmp(outputMode, "summary") ){
                    report_summary (256, hits, addrCount, map->frame, 866180);
                }
                else{
                    fprintf(stderr, "Output mode %s is not supported", outputMode);
                    exit(1);
                }
            }
        }
    }


    /* clean up and return success */
    fclose(ifp);
}

int main (int argc, char** argv) {

    OPTION_ARGS flags = {0};
    char* outputMode = 0;
    int levelCount;
    unsigned int addrLimit;

    parseArgs(argc, argv, &flags, &addrLimit, &outputMode, &levelCount);

    runMultiPageTabSimulator(argc, argv, flags, addrLimit, outputMode, levelCount);


    // printf("levelCount = %d\n", levelCount);
    // printf("addrLimit = %d\n", addrLimit);
    // printf("outputMode = %s\n", outputMode);
    // printf("n = %d\n", flags.n);
    // printf("o = %d\n", flags.o);

    return 0;
}
