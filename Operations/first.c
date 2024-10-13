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
int get_pos(int sym, char s);//sym=0 for T, sym=1 for NT

char FIRST[2][10][10];//FIRST[0]-for t, FIRST[1]- for NT
void rem_duplicates(char*, int);
int rem_epsilon(char* str, char s, int k)//k is position of next terminal to be added in the first of s
{
    int flag=0;
    for(int i=0; i<strlen(str); i++)
    {    
        if(str[i] == '#')
        {
            flag++;
            continue;            
        }
        else
        {
            FIRST[1][get_pos(1,s)][k++] = str[i];
        }
    }
    FIRST[1][get_pos(1,s)][k++] = '\0';
    return flag;
}


void init_first(void)
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<10; j++)
        {
            FIRST[0][j][0] = '\0';
            FIRST[0][j][1] = '\0';
            for(int k=0; k<10; k++)
            {
                if(i == 1)
                    FIRST[i][j][k] = '\0';
            }
        }
    }

}
void first(char s)
{
    if(s == '#' || s=='\0')
        return;//dont find or store for epsilon or any other char
    int t;
    //dont find again if already found:
    if(FIRST[isupper(s)?1:0][get_pos(isupper(s)?1:0,s)][0] != '\0')
        return;
    int k=0;
    if(!isupper(s))  //if a terminal
    {
        FIRST[0][get_pos(0,s)][0] = s;
        FIRST[0][get_pos(0,s)][1] = '\0';//null-terminate to denote end of string
    }
    else    //if a non-terminal
    {
        for(int i=0; i<n; i++)
        {
            if(productions[i][0]==s)
            {
                if(!isupper(productions[i][1]) || productions[i][1] == '#')  //add to first if its a t
                    FIRST[1][get_pos(1,s)][k++] = productions[i][1];
                else    //if the productions derives a NT first, find first of that NT
                {
                    if(productions[i][1] == s)  //in case of left recursion, ignore that production
                        continue;
                    else 
                    {
                        int j=0;
                        FIRST[1][get_pos(1,s)][k] = '\0';
                        do
                        {
                            //update k:
                            char c = FIRST[1][get_pos(1,s)][k];
                            while(c!='\0')
                            {
                                k++;
                                c = FIRST[1][get_pos(1,s)][k];
                            }
                            j++;
                            if(productions[i][j] == '\0')  //add epsilon if its empty(if last symbol also has epsilon)
                            {
                                FIRST[1][get_pos(1,s)][k++] = '#';
                                break;
                            }
                            if(!isupper(productions[i][j]))  //add to first if its a t
                            {
                                FIRST[1][get_pos(1,s)][k++] = productions[i][j];
                                break;
                            }
                            else if(FIRST[1][get_pos(1,productions[i][j])][0] == '\0')  //if first of that NT is not found yet, find it first
                                first(productions[i][j]);
                        } while(rem_epsilon(FIRST[1][get_pos(1,productions[i][j])],s,k));//returns 1 if epsilon present and adds all terminals, except epsilon(#)
                        //update k:
                        char c = FIRST[1][get_pos(1,s)][k];
                        while(c!='\0')
                        {
                            k++;
                            c = FIRST[1][get_pos(1,s)][k];
                        }
                    }
                }
            }
        }
    }
    FIRST[1][get_pos(1,s)][k] = '\0';
    rem_duplicates(FIRST[1][get_pos(1,s)],strlen(FIRST[1][get_pos(1,s)]));
    //printf("FIRST(%c) = %s\n",s,FIRST[1][get_pos(1,s)]);
    return;
}

int main()
{
    // printf("Enter the number of productions: ");
    // scanf("%d",&n);
    FILE* fptr = fopen("grammar.txt","r");
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
    init_first();
    for(int i=0;i<nt;i++)
    {
        first(NT[i]);
        printf("First(%c)=%s\n",NT[i],FIRST[1][i]);
    }
        
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
            if(islower(productions[i][j]) || productions[i][j]=='('||productions[i][j]==')'||productions[i][j]==','||productions[i][j]=='{'||productions[i][j]=='}')
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

int get_pos(int sym, char s)
{
    if(sym == 0)
    {
        for(int i=0; i<t; i++)
            if(T[i] == s)
                return i;
    }
    else
    {
        for(int i=0; i<nt; i++)
            if(NT[i] == s)
                return i;
    }
    if(sym == 0)
		printf("Terminal not found in get_pos! (%c)\n", s);
	else
		printf("Non-terminal not found in get_pos! (%c)\n", s);
}

void rem_duplicates(char* str, int n)
{
    int g=0,h=1;
    char c = str[g++];
    char d = str[h++];
    while(c!='\0')
    {
        while(d!='\0')
        {
            if(c==d)
            {
                h--;
                int z=h;
                while(str[z]!='\0')
                {
                    str[z]=str[z+1];
                    z++;
                }
            }
            d=str[h++];
        }
        c=str[g++];
    }

}