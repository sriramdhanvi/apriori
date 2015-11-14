#include "apriori_file.h"

int main()
{
    int k = 2;
    FILE* fp = fopen("datafile.txt","r");
    init_candidateItem_file();
    init_frequentItem_file();
    do{
     k = k+1;
     apriori_gen(k);
     apply_hash(k);
    }while(!isnull());

    return 0;
}
