#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define max_production_size 10

int n,nt=0,t=0,item_no=0;
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

item LR0ITEMS[15];//contains (item_no-1) items
//constructing LR(0) parse table
char LR0dfa[15][10]; //transition function mapping 

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
        if(i->prod[j][k+1] == '\0')//if dot is at the end
            break;
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
    // printf("item_no: %d:\n",i->item_no);
    // for(int j=0; j<i->no_of_productions; j++)
    //     printf("%c->%s\n",i->prod[j][0],(i->prod[j])+1);
    // printf("\n");
}

char* move_dot(char* str,int v)//v is the dot position: just swap the dot and the following symbol
{
    char* new_str = (char*)malloc(10*sizeof(char));
    strcpy(new_str,str);
    int temp = new_str[v];
    new_str[v] = new_str[v+1];
    new_str[v+1] = temp;
    return new_str;
}

item GOTO(item *i, char s)//returns the goto item of an item on a NT
{
    item new = {item_no++, 0, "\0"};
    char c, *gOtO;
    int v=1;
    for(int j=0; j<i->no_of_productions; j++)
    {
        v=1;
        //move till dot and check the following symbol
        c = i->prod[j][v];
        while(c!='.')
        {
            v++;
            c = i->prod[j][v];
        }
        if(i->prod[j][v+1]==s)//if the next symbol to dot is the NT
        {
            gOtO = move_dot(i->prod[j],v);
            strcpy(new.prod[new.no_of_productions++],gOtO);
            free(gOtO);
            closure(&new);
        }
    }
    return new;
}

char closure_exists(char* pdn)
{
    int b=0;
    char c;
    do
    {
        c = pdn[b];
        b++;
    }while(c!='.');
    if(pdn[b]=='\0')
        return '\0';
    else return pdn[b];
}
int already_in_lr0items(item temp)
{
    int count;
    for(int i=0; i<item_no; i++)
    {
        count = 0;
        if(LR0ITEMS[i].no_of_productions!=temp.no_of_productions)
            continue;
        else
        {
            for(int j=0; j<temp.no_of_productions; j++)
            {
                if(strcmp(temp.prod[j],LR0ITEMS[i].prod[j])!=0)
                    break;
                else count++;
            }
            if(count == temp.no_of_productions)
                return 1;
        }
    }
    return 0;
}
void lr0_items()
{
    //initialize 1st item:
    item i0 = {item_no,1,"Z.S"};//creating a new item with augmented production
    closure(&i0);
    char goto_sym;
    LR0ITEMS[0] = i0;
    int h=++item_no;
    item temp;
    // for(int j=0; j<i0.no_of_productions;j++)
    // {
    //     LR0ITEMS[j+1] = GOTO(&i0,i0.prod[j][2]);
    // }
    for(int i=0; i<item_no; i++)
    {
        for(int j=0; j<LR0ITEMS[i].no_of_productions; j++)
        {
            if((goto_sym=closure_exists(LR0ITEMS[i].prod[j]))!='\0')//then goto exists on goto_sym
                temp = GOTO(&LR0ITEMS[i],goto_sym);
            else
                continue;
            if(!already_in_lr0items(temp))
                LR0ITEMS[h++] = temp;
            else item_no--;
        }
    }
    for(int r=0; r<h; r++)
    {
        printf("item_no: %d:\n",LR0ITEMS[r].item_no);
        for(int j=0; j<LR0ITEMS[r].no_of_productions; j++)
            printf("%c->%s\n",LR0ITEMS[r].prod[j][0],(LR0ITEMS[r].prod[j])+1);
        printf("\n");
    }
}
int main()
{
    // printf("Enter the number of productions: ");
    // scanf("%d",&n);
    FILE* fptr = fopen("g1.txt","r");
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
    
    lr0_items();
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
