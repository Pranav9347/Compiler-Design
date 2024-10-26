/*Name: Pranav Vijay Nadgir
Roll no: CS22B1034
Calculate First and Follow sets for the following grammar

S -> A B | b  | ε
A -> a A | ε
B -> b B | ε
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int n=0,nt=0,t=0;
int k=0;//to insert into first
char productions[7][4];
char NT[3],T[2];
int get_pos(int sym, char s);
int rem_epsilon(char* append, char s, int n);
void initialize_grammar();

void initialize_first_follow_sets();
char FIRST[2][3][4];
char FOLLOW[3][4];
void first(char s)
{
    k=0;
    if(islower(s))//if s is a terminal:
    {
        FIRST[0][get_pos(0,s)][0] = s;
        return;
    }
    //if s is a NT
    char y;
    int j=1;
    for(int i=0; i<n; i++)//iterate through each production
    {
        if(productions[i][0] == s)//if lhs matches
        {
            do
            {
                y=productions[i][j++];//the 1st symbol it derives
                if(y=='\0')//if at the end add epsilon to the first of s
                    FIRST[1][get_pos(1,s)][k++] = '#';
                if(islower(y) || y=='#')//if y is a terminal or epsilon then add it to the first of s
                    FIRST[1][get_pos(1,s)][k++] = y;
                else//if y is a NT recursively call first for the follwwing NTs
                {
                    //calculate it's first if not calculated
                    if(FIRST[1][get_pos(1,y)][0]=='\0')
                        first(y);
                    
                }
            }while(rem_epsilon(FIRST[1][get_pos(1,y)],s,strlen(FIRST[1][get_pos(1,y)])));
        }
    }
    // for(int i=0;i<k;i++)
    // { remove duplicates is left
    //     for(int j=1;j<k;j++)
    //     {
    //         if(FIRST[1][get_pos(1,y)][i]==FIRST[1][get_pos(1,y)][j])
                
    //     }
    // }
    if(isupper(s))
        printf("FIRST(%c)={%s}\n",NT[get_pos(1,s)],FIRST[1][get_pos(1,s)]);
}
void follow(char s)
{
    if(s=='S')//start symbol
        FOLLOW[0][0]='$';
    
    printf("FOLLOW(%c)={%s}\n",NT[get_pos(1,s)],FOLLOW[get_pos(1,s)]);
}
int main()
{
    
    initialize_grammar();
    initialize_first_follow_sets();
    for(int i=0; i<nt; i++)
    {
        first(NT[i]);
    }
    for(int i=0; i<nt; i++)
    {
        follow(NT[i]);
    }
    
    return 0;
}


void initialize_grammar()
{
    // printf("Enter the number of productions: ");
    // scanf(" %d",&n);
    // printf("Enter the production rules with '='(# for epsilon):\n");
    // for(int i=0; i<n; i++)
    // {
    //     scanf(" %c=%s",&productions[i][0],productions[i]+1);
    // }
    
    //input hardcoded as grammar is fixed:
    n=7;
    nt=3;
    t=2;
    strcpy(productions[0],"SAB"); 
    strcpy(productions[1],"Sb"); 
    strcpy(productions[2],"S#"); 
    strcpy(productions[3],"AaA"); 
    strcpy(productions[4],"A#");
    strcpy(productions[5],"BbB"); 
    strcpy(productions[6],"B#");
    NT[0] = 'S';
    NT[1] = 'A';
    NT[2] = 'B';
    T[0] = 'a';
    T[1] = 'b';
    for(int i=0; i<n; i++)
    {
        printf("%c=%s\n",productions[i][0],(productions[i]+1));
    }
    printf("NT: ");
    for(int i=0;i<nt;i++)
        printf("%c ",NT[i]);
    printf("\nT: ");
    for(int i=0;i<t;i++)
        printf("%c ",T[i]);
    printf("\n");
}

int get_pos(int sym, char s)
{
    if(sym == 0)//terminal
    {
        for(int i=0; i<t; i++)
        {
            if(T[i] == s)
                return i;
        }
    }
    else
    {
        for(int i=0; i<nt; i++)
        {
            if(NT[i] == s)
                return i;
        }
    }
    return -1;
}

void initialize_first_follow_sets()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0;j<3;j++)
        {
            for(int k=0;k<4;k++)
            {
                FIRST[i][j][k]='\0';
                FOLLOW[j][k]='\0';
            }
        }
    }
}
int rem_epsilon(char* append, char s, int n)//adds all terminals except epsilon to the first of S and returns 1 if epsilon was present, else 0
{
    int flag = 0;
    for(int i=0;i<n;i++)
    {
        if(append[i] == '#')
        {
            flag=1;
            continue;
        }
        else
        {
            FIRST[1][get_pos(1,s)][k++] = append[i];
        }
    }
    return flag;
}