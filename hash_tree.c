#include "hash_tree.h"

void assign_hash_values(int k)
{
    int i=0;
    for(i=0;i<itemcount;i++)
    {
        strcpy(hashlist[i].label,itemlist[i].label);
        hashlist[i].value = 0;
    }
    gmk = 1;
    for(i=1;i<=itemcount;i++)
    {
        if(i%k==0)
            gmk = i;
    }
    int index=0;
    for(i=0;i<itemcount;i++)
    {
        if(i<gmk)
        {
           hashlist[i].value = index;
           if(((i+1)%(itemcount/k)) == 0)
             index++;
        }
        else
        {
            hashlist[i].value = index++;
        }
    }
}

void hash_candidates(int k)
{
    FILE* cfp = fopen("candidate_file.txt","r");
    candlist = NULL;
    int i,id=0;
    char* p = (char*)malloc(sizeof(char)*11);
    char str[100];
    while(fgets(cfp,100,str) != NULL)
    {
      str[strlen(str)-1] = '\0';
      candlist[id].tid = id;
      candlist[id].support=0;
      for(p = strtok(str,","),i=0; p != NULL; p = strtok(NULL,","),i++)
      {
          strcpy(candlist[id].candidate[i],p);
      }
      candlist[id].hashval = find_leaf(candlist[id].candidate[i],k);
      id++;
    }
    fclose(cfp);

}

int find_leaf(char** arr, int k)
{
  int l = k-1;
  int finval = 0;
  int i=0;
  for(i=0;i<k;i++)
  {
    finval = finval + hash_value(arr[i])*pow(k,l);
    l--;
  }
  return finval;
}

int hash_value(char arr[])
{
    int res=0;
    int i=0;
    for(i=0;i<itemcount;i++)
    {
        if(!(strcmp(hashlist[i].label,arr)))
        {
            return hashlist[i].value;
        }
    }
    return -1;
}

void processSubsets(char** tran, int k)
{
    char subset[k][100];
    processLargerSubsets(tran,subset,0,0,k);
}

void processLargerSubsets(char** tran,char** subset,int subsetSize, int nextIndex, int k)
{
    if(subsetSize == k)
        hashSubset(subset,k);
    else
    {
        int j;
        for(j = nextIndex;j < k; j++)
        {
            strcpy(subset[subsetSize],tran[j]);
            processLargerSubsets(tran,subset,subsetSize+1,j+1,k);
        }
    }
}
void hashSubset(char** subset, int k)
{
    int leaf = find_leaf(subset,k);
    int i=0;int flag = 0;
    int len = sizeof(candlist)/sizeof(struct hashcandidate);
    for(i=0;i<len;i++)
    {
        if(candlist[i].hashval == leaf)
        {
            if(matches(candlist[i].candidate,subset,k))
            {
                candlist[i].support++;
            }
        }
    }
}
int matches(char** arr1, char** arr2, int k)
{
    int res=1;
    int i;
    for(i=0;i<k;i++)
    {
      if(strcmp(arr1[i],arr2[i]) != 0)
      {
          res = 0;
          break;
      }
    }
    return res;
}
void updateFrequentItemFile(int k)
{
    FILE* ffp = fopen("frequent_file.txt","r");
    FILE* rfp = fopen("result_file.txt","a+");
    int i;
    int len = sizeof(candlist)/sizeof(struct hashcandidate);
    for(i=0;i<len;i++)
    {
        if(candlist[i].support >= MIN_SUPPORT)
        {
            put_in_file(ffp,rfp,candidate,len);
        }
    }
    fclose(rfp);
    fclose(ffp);
}
void put_in_file(FILE* ffp, FILE* rfp,char** arr,int k)
{
    int i;
    for(i=0;i<k;i++)
    {
        fprintf(ffp,"%s ",arr[i]);
        fprintf(rfp,"%s ",arr[i]);
    }
}
