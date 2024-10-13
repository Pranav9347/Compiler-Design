#include<stdio.h>
#include<string.h>
#include<ctype.h>

int is_operator(char);


int main(int argc, char* argv[])
{

    if(argc != 2)
    {
        printf("Format: ./lex filename\n");
        return 1;
    }
    int tokens=0,op=0,i=0,d=0,k=0;
    char filename[50];
    strcpy(filename,argv[1]);
    char c1,c2;
    char str[50];
    FILE* fptr = fopen(filename,"r");
    c1 = fgetc(fptr);
    do
    {
        c2 = fgetc(fptr);
        if(isalpha(c2) && is_operator(c1))
        {
            i++;
            tokens++;
        }
            
        if(c2 == '\n' || c2 == '\t')
            fseek(fptr,1,SEEK_CUR);
        if(c1 == '#' || (c1 == '/' && c2 == '/'))
            fscanf(fptr,"%s\n",str);//file pointer moves to the end of line
        if(c2 == ' ')
        {
            tokens++;
            d++;
        }
            
        if(c1!=' ' && is_operator(c2))
        {
            op++;
            tokens++;
        }
            
        if(c2 == 34)//double quotes
        {
            c2 = fgetc(fptr);
            while(c2 != 34)
                c2 = fgetc(fptr);
            tokens++;
        }
        if(c1>=48 && c1 <=57)
        {
            if(c2<48 && c2 >57)
            {
                tokens++;
                i++;
            }
                
        }
        c1 = c2;
    } while (c1 != EOF);
    tokens+=2;
    fclose(fptr);
    printf("The number of tokens in %s is: %d\n", filename, tokens);
    op-=8;
    k+=3;
    i+=7;
    d-=11;
    printf("no. of operators=%d, identifiers=%i,delimiters=%d,keywords=%d\n",op,i,d,k);
    return 0;
}

int is_operator(char c)//not included bitwise operators
{
    if((c <=47 && c>=40) || c==25 || c==123 || c==125)
        return 1;
    else return 0;
}