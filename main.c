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

/*
*   Driver Program
*/
int main (int argc, char** argv) {

    OPTION_ARGS flags = {0};
    char* outputMode = {0};
    int levelCount;
    unsigned int addrLimit;

    // read in all args from terminal and pass them into appropriate variable
    parseArgs(argc, argv, &flags, &addrLimit, &outputMode, &levelCount);

    // execute the Simulation
    runMultiPageTabSimulator(argc, argv, flags, addrLimit, outputMode, levelCount);

    return 0;
}


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


/*  runMultiPageTabSimulator()
*   - performs the MultiPaging simulaion
*/
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

    // Display Bitmask used if -o bitmasks is on
    if(flags.o && !strcmp(outputMode, "bitmasks")){
        report_bitmasks (pageTab->levelCount, pageTab->bitmaskAry) ;       
    }        

    unsigned int hits, misses, addrCount, logicalAddr, totalByteUsed;
    hits = misses = addrCount = 0;
    MAP* map;
    totalByteUsed = sizeof(PAGETABLE);

    // Process each logical address till EOF
    while (!feof(ifp)) {

        // only process the first (addrLimit) logical addr if flag -n is ON
        if(flags.n && addrCount >= addrLimit){
            break;
        }

        // get next address and process
        if (NextAddress(ifp, &trace)) {
            addrCount++;
            logicalAddr = (unsigned int)trace.addr;

            // Insert new frame if doesnt exist
            if( PageInsert(pageTab, logicalAddr, misses)){
                misses++;
                totalByteUsed += sizeof(LEVEL) ;
            } 
            else {
                hits++;
            }

            map = PageLookup(pageTab, logicalAddr); // get the map when it's valid

            if(flags.o){

                // extract the offset bits
                unsigned int offset = (logicalAddr << vpnBitLen) >> vpnBitLen;

                if( !strcmp(outputMode, "logical2physical") ){
                    unsigned physicalAddr = getPhysicalAddr(map->frame, offset, vpnBitLen);
                    report_logical2physical (logicalAddr, physicalAddr);
                }
                else if( !strcmp(outputMode, "page2frame") ){
                    unsigned int* pageAry = getPages(logicalAddr, pageTab);
                    report_pagemap (logicalAddr, pageTab->levelCount, pageAry, map->frame);
                }
                else if( !strcmp(outputMode, "offset") ){
                    report_logical2offset (logicalAddr, offset);
                }
            }
        }
    }

    // If -o is not specify or -o is specify with "summary" as output mode, print summary report
    if(!flags.o || (flags.o && !strcmp(outputMode, "summary")) ){
        unsigned int pageSize = getPageSize(vpnBitLen);
        report_summary (pageSize, hits, addrCount, misses, totalByteUsed);
    }

    // clean up
    fclose(ifp);
}
