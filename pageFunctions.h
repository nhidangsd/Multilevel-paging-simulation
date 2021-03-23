#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "helpers.h"
#include "structures.h"

PAGETABLE* newPageTab(int, unsigned int*);
LEVEL* newLevel(PAGETABLE*, int);
unsigned int LogicalToPage(unsigned int, unsigned int, unsigned int);
MAP* PageLookup(PAGETABLE*, unsigned int);
MAP* pageLookupRecursive(LEVEL* level, unsigned int);
void PageInsert(PAGETABLE*, unsigned int, unsigned int);
void pageInsertRecursive(LEVEL*, unsigned int, unsigned int);


#endif // PAGE_FUNCTIONS_H