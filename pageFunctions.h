#ifndef PAGE_FUNCTIONS_H
#define PAGE_FUNCTIONS_H

#include "helpers.h"
#include "structures.h"


/*  newPageTab() 
*   -   creates a Page Table from given levelCount and num of bits in each level, 
*/
PAGETABLE* newPageTab(int, unsigned int*);

/*  newLevel()
*   - creates a new Level node & set its depth from the given arg
*/
LEVEL* newLevel(PAGETABLE*, int);

/*  LogicalToPage()
*   -   Given a page table and a logical address, 
*   return the appropriate entry of the page table.
*/  
unsigned int LogicalToPage(unsigned int, unsigned int, unsigned int);

/*  PageLookup()
*   -   Given a page table and a logical address
*   -   returns the appropriate entry of the page table.
*/
MAP* PageLookup(PAGETABLE*, unsigned int);

/*  pageLookupRecursive()
*   -   Recursive call to next level until the reaching the leaf node
*/
MAP* pageLookupRecursive(LEVEL* level, unsigned int);

/*  PageInsert()
*   -   adds new entries to the page table when we have discovered that a page has not yet been allocated 
*/
unsigned int PageInsert(PAGETABLE*, unsigned int, unsigned int);

/*  pageInsertRecursive()
*   -   adds new entries to the page table when we have discovered that a page has not yet been allocated 
*/
unsigned int pageInsertRecursive(LEVEL*, unsigned int, unsigned int);


#endif // PAGE_FUNCTIONS_H