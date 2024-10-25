#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define max_production_size 10

int n,nt=0,t=0;
char** productions;
char* NT;
char* T;
void get_NT();
void get_T();

typedef struct item
{
    int item_no;
    int no_of_productions;
    char prod[10][10];//set of productions with dot
}item;
char* insert_dot(char* s)
{
    char* insert_prod = (char*)malloc(10*sizeof(char));
    insert_prod[0] = s[0];
    insert_prod[1] = '.';
    int o=1;
    while(s[o]!='\0'){
        insert_prod[o+1] = s[o];
        o++;
}
    insert_prod[o+1]='\0';
    return insert_prod;
}
void closure(item* i)//for every production in the item, if NT follows a dot expand and include it
{
    char* append;
    int k,m;
    for(int j=0; j<i->no_of_productions; j++)
    {
        k=0;
        while(i->prod[j][k] != '.')
            k++;
        if(isupper(i->prod[j][k+1]))//if NT follows a dot
            for(int l=0; l<n; l++)
                if(productions[l][0]==i->prod[j][k+1])
                {
                    append = insert_dot(productions[l]);
                    //add that production if not added already with dot
                    for(m=0; m<i->no_of_productions; m++)
                        if(strcmp(i->prod[m],append) == 0)
                            break;
                    if(m == i->no_of_productions)//if the production is not added already
                    {
                        strcpy(i->prod[m],append);
                        i->no_of_productions++; 
                        free(append);
                    }
                }
    }
    for(int j=0; j<i->no_of_productions; j++)
        printf("%c->%s\n",i->prod[j][0],(i->prod[j])+1);
    printf("\n");
}
// void lr0_items()
// {
//     //initialize 1st item:
//     char p[10][10];
//     char go[10]="\0";
//     for(int i=0;i<10;i++)
//     {
//         go[i]=0;
//         for(int j=0;j<10;j++)
//         {
//             p[i][j]='\0';
//         }
//     }
//     strcpy(p[0],"X.");
//     p[0][2] = productions[0][0];
//     item i0 = {0,p,'\0'};
//     printf("item id:%d,productions=%s,goto=%s",i0.item_no,i0.prod[0],i0.GOTO);
// }
int main()
{
    // printf("Enter the number of productions: ");
    // scanf("%d",&n);
    FILE* fptr = fopen("grammar1.txt","r");
    fscanf(fptr,"%d\n",&n);

    productions = (char**)malloc(n*sizeof(char*));//allocating space for n strings
    for(int i=0;i<n;i++)
    {
        productions[i] = (char*)malloc(sizeof(char)*max_production_size);//allocating space for each string
    }

    // printf("Enter the productions: ");
    // for(int i=0;i<n;i++)
    // {
    //     scanf(" %c=%s",&productions[i][0],productions[i]+1);
    // }
    for(int i=0;i<n;i++)
    {
        fscanf(fptr,"%c=%s\n",&productions[i][0],productions[i]+1);
    }
    
    get_NT();
    get_T();
    printf("no of NT: %d\n",nt);
    printf("no of T: %d\n",t);
    item i = {0,1,"Z.S"};//creating a new item with augmented production
    closure(&i);
    fclose(fptr);
    return 0;
}
void get_NT()
{
    int w=0;
    NT = (char*)malloc(n*sizeof(char));
    for(int i=0;i<n;i++)
    {
        w=0;
        for(int j=0;j<nt;j++)
            if(NT[j]==productions[i][0])
            {
                w++;
                break;
            }
        if(w==0)
            NT[nt++] = productions[i][0];
    }
    printf("NT: ");
    for(int i=0;i<nt;i++)
    {
        printf("%c ",NT[i]);
    }
    printf("\n");
}

void get_T()
{
    int w=0;
    T = (char*)malloc(n*5*sizeof(char));
    for(int i=0;i<n;i++)
    {
        w=0;
        for(int j=1;productions[i][j]!='\0';j++)
        {
            if(islower(productions[i][j]))
            {    
                for(int k=0;k<t;k++)
                    if(T[k]==productions[i][j])
                    {
                        w++;
                        break;
                    }
                if(w==0)
                    T[t++] = productions[i][j];
                
            }
        }
    }
    printf("T: ");
    for(int i=0;i<t;i++)
    {
        printf("%c ",T[i]);
    }
    printf("\n");
}
