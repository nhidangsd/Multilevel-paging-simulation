#ifndef STRUCTURES_H
#define STRUCTURES_H

#define ADDRESS_LENGTH 32

typedef struct PAGETABLE{
    struct LEVEL* root;
    int levelCount;             // num of levels
    unsigned int* bitmaskAry;    // bitmask fo level i
    unsigned int* shiftAry;               // num of bits to shift level i page bis
    unsigned int* entryCountAry;             // num of possible pages for level i

} PAGETABLE;


typedef struct LEVEL{
    int hasNextLevel;
    int depth;
    struct PAGETABLE* pageTab;
    struct LEVEL** nextLevel;
    struct MAP* map;
} LEVEL;


typedef struct MAP{
    int isValid;
    unsigned int frame;
} MAP;

typedef struct OPTION_ARGS{
    int n;
    int o;
} OPTION_ARGS;

#endif // STRUCTURES_H