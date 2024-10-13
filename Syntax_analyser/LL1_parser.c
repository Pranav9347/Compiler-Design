#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "stack.h"
#define max_production_size 10

int n,nt=0,t=0;
char** productions;
char* NT;
char* T;
void get_NT();
void get_T();
int get_pos(int sym, char s);//sym=0 for T, sym=1 for NT

char FIRST[2][10][10];//FIRST[0]-for t, FIRST[1]- for NT
char FOLLOW[10][10];
void rem_duplicates(char*, int);

int parse_table[10][10];

int contains_epsilon(char* str,int n)
{
    for(int i=0; i<n; i++)
    {    
        if(str[i] == '#')
        {
            return 1;          
        }
    }
    return 0;
}
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
    //dont find again if already found:
    if(FIRST[isupper(s)?1:0][get_pos(isupper(s)?1:0,s)][0] != '\0')
        return;
    int k=0;
    if(!isupper(s))  //if a terminal
    {
        FIRST[0][get_pos(0,s)][0] = s;
        FIRST[0][get_pos(0,s)][1] = '\0';//null-terminate to denote end of string
        return;
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
void init_follow(void)
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            FOLLOW[i][j] = '\0';
        }
    }
}
void follow(char s)
{
    if(!isupper(s))
    {
        printf("Follow of terminal is not defined!\n");
        return;
    }
    
    else
    {
        int k=0,l=0,m=0,flag=0;
        if(s == productions[0][0])//if start symbol
        {
            //follow(S)={$}
            FOLLOW[0][k++]='$';
        }
        
        //check in all productions' RHS:
        for(int i=0; i<n; i++)
        {
            flag=0;
            for(int j=1; productions[i][j]!='\0'; j++)
            {
                if((productions[i][j]==s) || flag==1)//where that NT occurs or when flag is set to 1(its set to 1 when first of the NT contains #, so u gotta find first of the next symbol) 
                {
                    char y = productions[i][j+1];//the next following symbol of s
                    if(y == '\0')//if s occurs at the end
                    {
                        //if follow(LHS) is not found, then find it first
                        y=productions[i][0];//lhs NT
                        if(FOLLOW[get_pos(1,y)][0]=='\0')
                            follow(productions[i][0]);
                        if(y==s)
                            break;//to prevent infinite loop

                        //then add follow(lhs) to follow(s)
                        l=0;
                        char x = FOLLOW[get_pos(1,y)][l++];
                        do
                        {
                            FOLLOW[get_pos(1,s)][k++] = x;
                            x=FOLLOW[get_pos(1,y)][l++];
                        }while(x != '\0');
                        break;
                    }
                    if(FIRST[isupper(y)?1:0][get_pos(isupper(y)?1:0,y)][0]=='\0')//if first of y not found
                        first(y);//then find its first
                    if(isupper(y) && contains_epsilon(FIRST[1][get_pos(1,y)],strlen(FIRST[1][get_pos(1,y)])))//if y derives epsilon
                    {
                        //add all terminals in first(y) except epsilon:
                        l=0;
                        char c=FIRST[1][get_pos(1,y)][l++];
                        if(c == '#')
                        {
                            c=FIRST[1][get_pos(1,y)][l++];
                        }
                        do
                        {
                            if(c == '#')
                            {
                                c=FIRST[1][get_pos(1,y)][l++];
                                if(c=='\0')
                                    break;
                            }
                            FOLLOW[get_pos(1,s)][k++] = c;
                            c = FIRST[1][get_pos(1,y)][l++];
                        }while(c != '\0');
                        //now add first of the next symbol to follow(s)
                        flag=1;
                        continue;
                    }
                    else//if the next symbol is a terminal, or a NT and doesnt produce epsilon
                    {
                        m=0;
                        //then add the first of that next symbol to the follow(s)
                        char c=FIRST[isupper(y)?1:0][get_pos(isupper(y)?1:0,y)][m++];
                        do
                        {
                            FOLLOW[get_pos(1,s)][k++] = c;
                            c = FIRST[isupper(y)?1:0][get_pos(isupper(y)?1:0,y)][m++];
                        }while(c != '\0');
                    }                    
                }
            }
        }
    }
    rem_duplicates(FOLLOW[get_pos(1,s)],strlen(FOLLOW[get_pos(1,s)]));
}
char* first_str(char* alpha)
{
    char* first_of_alpha = (char*)calloc(15,sizeof(char));
    int q=0,r=0,u=0,flag=1;
    char c = alpha[r++];
    while(c!='\0' && flag)
    {
        if(c=='#')//if alpha is epsilon
        {
            first_of_alpha[q++]='#';
            flag=0;
        }
        else if (!isupper(c))//if alpha begins with a terminal
        {
            first_of_alpha[q++] = c;
            flag=0;        
        }
        else //if it's a NT
        {
            u=0;
            char d = FIRST[1][get_pos(1,c)][u++];
            while(d!='\0')
            {
                if(d=='#')//if the NT derives epsilon
                {
                    if(alpha[r]=='\0')
                        first_of_alpha[q++] = d;
                    d=FIRST[1][get_pos(1,c)][u++];
                    flag++;
                    continue;
                }
                else
                {
                    first_of_alpha[q++] = d;
                    d=FIRST[1][get_pos(1,c)][u++];
                }
            }
            if(flag==1)
                flag=0;
        }
        c=alpha[r++];
    }
    return first_of_alpha;
}
void generate_parse_table()
{
    //initializing parse table with -1:(indicates error)
    for(int i=0;i<nt;i++)
    {
        for(int j=0;j<t+1;j++)
        {
            parse_table[i][j]=-1;
        }
    }
    printf("Parse Table:\n    ");
    for(int i=0;i<t;i++)
    {
        printf("%c   ",T[i]);
    }
    printf("$\n");
    //for each production A->alpha:
    for(int i=0;i<n;i++)
    {
        int k=0;
        char A = productions[i][0];
        char* alpha = productions[i]+1;
        //find the first(alpha)
        char* first_of_alpha = first_str(alpha);

        char ter = first_of_alpha[k++];
        while(ter != '\0')
        {
            if(ter == '#')//if epsilon is in first(alpha)
            {
                int m=0;
                //add the production(production no.) for all terminals ter in follow(A) to       parse_table[A][ter]
                char te = FOLLOW[get_pos(1,A)][m++];
                while(te!='\0')
                {
                    if(te=='$')
                    {
                        if(parse_table[get_pos(1,A)][t]!=-1)
                        {
                            return;
                            printf("Not LL1-grammar!");
                        }
                        parse_table[get_pos(1,A)][t] = i;
                        break;
                    }
                    if(parse_table[get_pos(1,A)][get_pos(0,te)]!=-1)
                    {
                        return;
                        printf("Not LL1-grammar!");
                    }
                    parse_table[get_pos(1,A)][get_pos(0,te)] = i;
                    te = FOLLOW[get_pos(1,A)][m++];
                }
                ter = first_of_alpha[k++];
            }
            else
            {
                if(parse_table[get_pos(1,A)][get_pos(0,ter)]!=-1)
                {
                    return;
                    printf("Not LL1-grammar!");
                }
                parse_table[get_pos(1,A)][get_pos(0,ter)] = i;
                ter = first_of_alpha[k++];
            }
        }

    }
    for(int i=0;i<nt;i++)
    {
        printf("%c  ",NT[i]);
        for(int j=0;j<t+1;j++)
        {
            if(parse_table[i][j]>0)
                printf(" ");
            printf("%d  ",parse_table[i][j]);
        }
        printf("\n");
    }
}

void parse(char* str, int n)
{
    int lookahead = 0,p=-1;
//push $ to stack
    push('$');
//append $ to string
    str[n]='$';
    str[n+1]='\0';
//push start symbol to stack
    push(productions[0][0]);
    printf("STACK      INPUT\n");
    printf("  $     %s,lookahed=%d\n",str,lookahead);
    while(peek()!='$' || str[lookahead]!='$')//accept condition
    {
        if(isupper(peek()))//if TOS is a NT
        {
            //check in T[TOS][lookahead] element:
            if((p=parse_table[get_pos(1,peek())][get_pos(0,str[lookahead])])==-1)//if empty syntax error!
            {
                printf("Syntax error!\n");
                return;
            }
            else//pop and push the productions' RHS in reverse
            {
                pop();
                int n=strlen(productions[p]+1);
                n--;
                while(n!=-1)
                {
                    push((productions[p]+1)[n--]);
                }
                int t=0;
                while(t!=top+1)
                    printf("%c",stack[t++]);
                printf("    %s,lookahead=%d\n",str,lookahead);
            }
            
        }
        else//if TOS is a T
        {
            //how to handle epsilon:
            if(peek()=='#')
            {
                pop();//pop '#'
                continue;
            }
            if(peek()==str[lookahead])
                lookahead++;
            else
            {
                printf("Syntax error!\n");
                return;
            }
            pop();
            int t=0;
                while(t!=top+1)
                    printf("%c",stack[t++]);
                printf("   %s,lookahead=%d\n",str,lookahead);
        }
    }
    int t=0;
    while(t!=top+1)
        printf("%c",stack[t++]);
    printf("  %s,lookahead=%d\n",str,lookahead);
    printf("String is accepted!\n");

}
int main()
{
    // printf("Enter the number of productions: ");
    // scanf("%d",&n);
    FILE* fptr = fopen("input1.txt","r");
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
    printf("\n");
    init_follow();
    for(int i=0;i<nt;i++)
    {
        follow(NT[i]);
        printf("Follow(%c)=%s\n",NT[i],FOLLOW[i]);
    }
    fclose(fptr);
    char str[10];
    generate_parse_table();
    printf("Enter the input string: ");
    scanf(" %s",str);
    parse(str,strlen(str));
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
    if(s=='$')
        return t;
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
    {
        printf("Terminal not found in get_pos! (%c)\n", s);
        exit(0);
    }
    else
    {
        printf("Non-terminal not found in get_pos! (%c)\n", s);
        exit(0);
    }
    
}

void rem_duplicates(char* str, int n)
{
    int g=0,h=g+1;
    char c = str[g++];
    char d = str[h++];
    while(c!='\0')
    {
        h=g;
        d=str[h++];
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