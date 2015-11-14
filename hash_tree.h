#ifndef HASH_TREE_H_INCLUDED
#define HASH_TREE_H_INCLUDED

#include "apriori_file.h"
#include <math.h>

int gmk;
struct hashitem
{
    char label[10];
    int value;
};

struct hashitem* hashlist;

struct hashcandidate
{
 char candidate[100][100];
 int tid;
 int hashval;
 int support;
};

struct hashcandidate* candlist;

void assign_hash_values(int k);
void hash_candidates(int k);
int hash_value(char arr[]);
int find_leaf(char** arr,int k);
void processSubsets(char** tran,int k);
void processLargerSubsets(char** tran,char** subset,int subsetSize, int nextIndex, int k);
void hashSubset(char** subset, int k);
int matches(char** arr1, char** arr2, int k);
void updateFrequentItemFile(int k);
void put_in_file(FILE* ffp, char** arr,int k);

#endif // HASH_TREE_H_INCLUDED
