#ifndef APRIORI_FILE_H_INCLUDED
#define APRIORI_FILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_SUPPORT 3
#define TR_SIZE 100

typedef struct item{
    char label[10];
    int support;
};

int itemcount;
struct item* itemlist;

void apriori_gen(int k);
void init_candidateItem_file();
void init_frequentItem_file();
void apply_hash(int k);
int isnull();

#endif // APRIORI_FILE_H_INCLUDED
