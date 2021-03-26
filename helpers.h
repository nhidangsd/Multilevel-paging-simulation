#ifndef HELPERS_H
#define HELPERS_H

#include "structures.h"

/*  runMultiPageTabSimulator()
*   - performs the MultiPaging simulaion
*/
void runMultiPageTabSimulator(int, char**, OPTION_ARGS , int ,char* , int );

/*  parseArgs()
*   - scans the argc and argv to get num of levels
*   - set the Option Arguments on if there is any
*/
void parseArgs(int argc, char** argv, OPTION_ARGS* flags, unsigned int* addrLimit, char** outputMode, int* levelCount);

/*  getBitAry()
*   -   extracts num of bits for each level from argv and converts to int
*   -   returns num of bits used for Virtual Page Number
*/
unsigned int getBitAry(unsigned int*, int, int, char**);

/*  generateBitmask()
*   -   creates a bit mask from a starting bit with specified length
*/
unsigned int generateBitmask(unsigned int, unsigned int);

/*  getPhysicalAddr()
*   -   converts Logical Addr to Physical Addr
*/
unsigned int getPhysicalAddr(unsigned int frame, unsigned offset, unsigned int vpnBitLen);

/*  getPages()
*   -   creates an array containing the page index for each level
*/
unsigned int* getPages(unsigned int logicalAdrr, PAGETABLE* pageTab);

/*  getPageSize()
*   -   calculate the page size in bytes
*/
unsigned int getPageSize(unsigned int vpnBitLen);

#endif // HELPERS_H