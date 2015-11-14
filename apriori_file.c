#include "apriori_file.h"

int comparator(const void* i1,const void* i2)
{
  struct item* l = (struct item*)i1;
  struct item* r = (struct item*)i2;
  if(l->support == r->support) return strcmp(l->label,r->label);
  return (r->support) - (l->support);
}

int count_items(FILE* fp)
{
    int res=0;
    char c;
    while(!feof(fp))
    {
     c = fgetc(fp);
     if(c=='\n') res++;
    }
    return res;
}

void apriori_gen(int k)
{
  char* line = (char*)malloc(sizeof(char)*100);
  char* line2 = (char*)malloc(sizeof(char)*100);
  memset(line,'\0',100);
  memset(line2,'\0',100);
  char tlist[100][100];
  char fitem[11];
  memset(fitem,'\0',11);
  char* p = (char*)malloc(sizeof(char)*11);
  int i=0,j=0;
  FILE* ffp = fopen("frequent_file.txt","r");
  if(ffp==NULL) {printf("cannot open file\n");exit(0);}
  FILE* ofp = fopen("frequent_one_file.txt","r");
  FILE* tfp = fopen("temp_file.txt","w");
  int flag=1;
  while(fgets(line,100,ffp) != NULL)
  {
      line[strlen(line)-1]='\0';
      if(k==2)
      {
          strcpy(tlist[0],line);
          //printf("tlist : %s\n",tlist[0]);
      }
      else{
         for(p=strtok(line,","),i=0;p!=NULL;p=strtok(NULL,","),i++)
          {
              //printf(" p : %s\n",p);
              strcpy(tlist[i],p);
          }
          //printf("\n");
      }
      while(fgets(fitem,11,ofp) != NULL)
      {
          flag=1;
          fitem[strlen(fitem)-1]='\0';
          //printf("fitem : %s tlist  : %s  \n",fitem,tlist[0]);
          for(j=0;j<k-1;j++)
          {
              //printf("%s %s\n",fitem,tlist[j]);
              if(strcmp(fitem,tlist[j])==0)
                {
                    flag=0;
                    break;
                }
          }
          //printf("%d\n",flag);
          if(flag)
          {
              for(j=0;j<k-1;j++)
              {
                  //printf("\n**hi**\n");
                  fprintf(tfp,"%s,",tlist[j]);
              }
              fprintf(tfp,"%s\n",fitem);
          }
      }
      rewind(ofp);
  }

  // PRUNING STARTS HERE...

  free(line);free(line2);free(p);
  p = (char*)malloc(sizeof(char)*11);
  char* q = (char*)malloc(sizeof(char)*11);
  line = (char*)malloc(sizeof(char)*100);
  line2 = (char*)malloc(sizeof(char)*100);
  char tran[100][100];
  fclose(tfp);i=0;j=0;
  tfp = fopen("temp_file.txt","r");
  rewind(ffp);
  FILE* cfp = fopen("candidate_file.txt","w");
  int sup=0; int cand=1;
  while(fgets(line,100,tfp) != NULL)
  {
      //printf("line : %s",line);
      cand=1;
      line[strlen(line)-1] = '\0';
      //printf("line : %s\n",line);

     // printf("p values : \n");
    for(p = strtok(line,","),i=0;p != NULL; p = strtok(NULL,","),i++)
    {
      //printf("p value : %s \n",p);
      //printf("%d ",i);
      strcpy(tran[i],p);
      //printf("tran[%d] : %s\n",i,tran[i]);
      while(fgets(line2,100,ffp) != NULL)
      {
          line2[strlen(line2)-1]='\0';
          //printf("%s \n",line2);
          if(k==2)
          {
            if(strcmp(line2,p)==0)
                sup++;
          }
          else
          {
            // printf(" p : %s --- line2 : %s\n",p,line2);
              for(q = strtok(line2,",");q != NULL; q = strtok(NULL,","))
              {
                  ///printf("\nq : %s ",q);
                  if(strcmp(p,q)==0)
                  {
                      sup++;break;
                  }
              }
          }
      }
      printf("support for %s : %d\n",p,sup);
      if(sup < k-1) {cand=0;break;}
      sup=0;
      rewind(ffp);
    }
    if(cand)
    {
        //printf("\nhi\n");
        for(j=0;j<k-1;j++)
            fprintf(cfp,"%s,",tran[j]);
        fprintf(cfp,"%s\n",tran[k-1]);
    }
    sup=0;
    rewind(ffp);
  }
  fclose(cfp);
  fclose(tfp);
  fclose(ffp);
  fclose(ofp);
}

void init_candidateItem_file()
{
    FILE* cfp = fopen("candidate_file.txt","w");
    if(cfp==NULL)printf("cannot open candidate_file\n"),exit(0);
    FILE* fp = fopen("datafile.txt","r");
    if(fp==NULL)printf("cannot open datafile\n"),exit(0);
    char c;
    do{
       c = fgetc(fp);
       fputc(c,cfp);
    }while(c != EOF);

    fclose(cfp);
    fclose(fp);
}

void init_frequentItem_file()
{
    FILE* dfp = fopen("distinct_item_file.txt","r");
    if(dfp==NULL)printf("cannot open distinct_item_file\n"),exit(0);
    itemcount = count_items(dfp);
    itemlist = (struct item*)malloc(sizeof(struct item)*itemcount);
    rewind(dfp);
    int i;char itemname[10];
    for(i=0;i<itemcount;i++)
    {
      fscanf(dfp,"%s",itemname);
      strcpy(itemlist[i].label,itemname);
      //printf("%s  ",itemlist[i].label);
      itemlist[i].support = 0;
    }
    pre_process();
    fclose(dfp);
}

void pre_process() // updates the "frequent_one_file";
{
    FILE* fp = fopen("datafile.txt","r");
    FILE* ffp = fopen("frequent_file.txt","w");
    FILE* ofp = fopen("frequent_one_file.txt","w");
    int i=0,c=0;
   char* q = (char*)malloc(sizeof(char)*10);
    // code snippet to increase the support count of each item.
    char* st = (char*)malloc(sizeof(char)*100);
        while(fgets(st,100,fp) != NULL)
        {
            st[strlen(st)-1]='\0';
        for(q=strtok(st,",");q != NULL;q=strtok(NULL,","))
        {
            //printf("%s ",q);
            for(i=0;i<itemcount;i++)
            {
                if(!strcmp(itemlist[i].label,q)){
                        itemlist[i].support++;
                        break;
                }
            }
        }
        //printf("\n");
         free(st);
         char* st = (char*)malloc(sizeof(char)*100);
        }
    // support incrementing code ends here...
  for(i=0;i<itemcount;i++)
    {
        //printf("%s %d\n",itemlist[i].label,itemlist[i].support);
        if(itemlist[i].support >= MIN_SUPPORT)
        {
            fprintf(ffp,"%s\n",itemlist[i].label);
            fprintf(ofp,"%s\n",itemlist[i].label);
        }
    }
    fclose(ofp);
    fclose(ffp);
    fclose(fp);
    return;
}

void apply_hash(int k)
{
    assign_hash_values(k);
    hash_candidates(k);

    FILE* dfp = fopen("datafile.txt","r");
    char line[100];
    char tran[100][100];
    char* p = (char*)malloc(sizeof(char)*11);
    int i;
    while(fgets(line,sizeof(line),dfp) != NULL)
    {
        line[strlen(line)-1] = '\0';
      for(p = strtok(line,","),i=0; p != NULL; p = strtok(NULL,","),i++)
      {
          strcpy(tran[i],p);
      }
      processSubsets(tran,k);
    }
    updateFrequentItemFile();
    fclose(dfp);
}
int isnull()
{
    FILE* tfp = fopen("temp_file.txt","r");
    int flag = 1;
    char str;
    if(fgets(str,1,tfp) != NULL)
    {
        flag = 0;
    }
    fclose(tfp);
    return flag;
}
