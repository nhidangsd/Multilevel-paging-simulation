#include <stdlib.h>
#include <stdio.h>
#include "pageFunctions.h"

/*  LogicalToPage()
*   -   Given a page table and a logical address, 
*   return the appropriate entry of the page table.
*/  
unsigned int LogicalToPage( unsigned int logicalAddr, unsigned int mask, unsigned int shift ){

    return (logicalAddr & mask) >> shift;
}


/*  PageLookup()
*   -   Given a page table and a logical address
*   -   returns the appropriate entry of the page table.
*/
MAP* PageLookup(PAGETABLE* pageTab, unsigned int logicalAddr){

    return pageLookupRecursive(pageTab->root, logicalAddr);
}


/*  pageLookupRecursive()
*   -   Recursive call to next level until the reaching the leaf node
*/
MAP* pageLookupRecursive(LEVEL* level, unsigned int logicalAddr){

    // Get Page index to look up
    unsigned int shift = level->pageTab->shiftAry[level->depth];
    unsigned int bitMask = level->pageTab->bitmaskAry[level->depth];
    unsigned int pageIndex = LogicalToPage(logicalAddr, bitMask, shift);
    // printf("------LOOKUP-----\n");
    // printf("shift = %d\n", shift);
    // printf("bitMask = %08X\n", bitMask);
    // printf("pageIndex = %08X\n", pageIndex);
    // printf("level info:\n-  hasNextLevel = %d\n-    depth = %d\n", 
    // level->hasNextLevel, level->depth);

    if (level->hasNextLevel){
        // printf("------LOOKUP NEXT LEVEL-----\n");
        // return NULL if Page is not found else go to next level
        return (level->nextLevel[pageIndex] != NULL) 
                ?   pageLookupRecursive(level->nextLevel[pageIndex], logicalAddr)
                :   NULL ;
    }

    // Return NULL if map is valid else return map
    else return (level->map[pageIndex].isValid) ? &level->map[pageIndex] : NULL;
    
}


/*  PageInsert()
*   -   adds new entries to the page table when we have discovered that a page has not yet been allocated 
*/
unsigned int PageInsert(PAGETABLE* pageTab, unsigned int logicalAddr, unsigned int frame){

    if (PageLookup(pageTab, logicalAddr) == NULL){
        return pageInsertRecursive(pageTab->root, logicalAddr, frame);
    }
    else return 0;
}


/*  pageInsertRecursive()
*   -   adds new entries to the page table when we have discovered that a page has not yet been allocated 
*/
unsigned int pageInsertRecursive(LEVEL* level, unsigned int logicalAddr, unsigned int frame){
    
    // Get Page index to look up
    unsigned int shift = level->pageTab->shiftAry[level->depth];
    unsigned int bitMask = level->pageTab->bitmaskAry[level->depth];
    unsigned int pageIndex = LogicalToPage(logicalAddr, bitMask, shift);
    // printf("\n------INSERT LEVEL %d-----\n", level->depth);
    // printf("shift = %d\n", shift);
    // printf("bitMask = %08X\n", bitMask);
    // printf("pageIndex = %d\n", pageIndex);
    // printf("level info:\n-  hasNextLevel = %d\n-    depth = %d\n", 
    // level->hasNextLevel, level->depth);

    if (level->hasNextLevel){

        if(level->nextLevel[pageIndex] == NULL){
            level->nextLevel[pageIndex] = newLevel(level->pageTab, level->depth +1);
        }
        // level->map[pageIndex].isValid = 0;
        return pageInsertRecursive(level->nextLevel[pageIndex], logicalAddr, frame);
    }
    else{
        level->map[pageIndex].isValid = 1;
        level->map[pageIndex].frame = frame;
        return 1;
    }
}


/*  newPageTab() 
*   -   creates a Page Table from given levelCount and num of bits in each level, 
*/
PAGETABLE* newPageTab(int levelCount, unsigned int* bitAry){

    // Allocate space for Page Table
    PAGETABLE* pageTab = (PAGETABLE* ) calloc(1, sizeof(PAGETABLE));

    if (pageTab == NULL) {
        printf("allocating memory didn't work!\n");
        exit(1);
    }

    pageTab->levelCount = levelCount;   // Init levelCount

    // Allocate space for bitmaskAry, shiftAry, enryCountAry
    pageTab->bitmaskAry = (unsigned int *) calloc(pageTab->levelCount, sizeof(unsigned int));
    pageTab->shiftAry = (unsigned int*) calloc(pageTab->levelCount, sizeof(unsigned int));
    pageTab->entryCountAry = (unsigned int *) calloc(pageTab->levelCount, sizeof(unsigned int));

    // Init bitmaskAry, shiftAry, enryCountAry
    int i;
    int bitsUsed = 0;
    for (i = 0; i < pageTab->levelCount; i++){

        pageTab->bitmaskAry[i] = generateBitmask(ADDRESS_LENGTH - bitsUsed, bitAry[i]);
        pageTab->shiftAry[i] = ADDRESS_LENGTH - bitsUsed - bitAry[i];
        pageTab->entryCountAry[i] = 1 << bitAry[i]; // 2 ^ (bitAry[i])
        
        bitsUsed += bitAry[i];  // increment bitsUsed by bitAry[i]
    }
    
    pageTab->root = newLevel(pageTab, 0);   // Init root

    return pageTab;
}


/*  newLevel()
*   - creates a new Level node & set its depth from the given arg
*/
LEVEL* newLevel(PAGETABLE* pageTab, int depth){

    LEVEL* level = (LEVEL*) calloc(1, sizeof(LEVEL));   // Declare a new level

    if (level == NULL) {
        printf("allocating memory didn't work!\n");
        exit(1);
    }

    // Init all attributes of the new level
    level->depth = depth;
    level->hasNextLevel = (depth < pageTab->levelCount - 1)? 1 : 0;
    level->pageTab = pageTab;

    if (level->hasNextLevel){
        level->nextLevel = (LEVEL**) calloc(pageTab->entryCountAry[depth], sizeof(LEVEL**));
        level->map = NULL;
    }
    else{
        level->map = (MAP*) calloc(pageTab->entryCountAry[depth], sizeof(MAP));
        level->nextLevel = NULL;
    }
    return level;
}



