#ifndef HELPERS_H
#define HELPERS_H

#include "structures.h"

// void parseArgs(int, char**, OPTION_ARGS*, int*, char*, int*);
unsigned int getBitAry(unsigned int*, int, int, char**);
unsigned int generateBitmask(unsigned int, unsigned int);
unsigned int getPhysicalAddr(unsigned int frame, unsigned offset, unsigned int vpnBitLen);
unsigned int* getPages(unsigned int logicalAdrr, PAGETABLE* pageTab);
unsigned int getPageSize(unsigned int vpnBitLen);

#endif // HELPERS_H