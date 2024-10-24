//assuming the semantic analyser gave the semantic tree for the given arithmetic expression, we've to generate the three address code

//The shortcut to generating the semantic tree is the infix to postfix conversion(works for arithmetic expressions only: precedence and order can be resolved)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 100
//works for variables only
typedef struct node
{
    char operator;
    struct node* next;
}node;
char* convert_to_postfix(char*);
//global top pointer
node* top=NULL;
int Isfull(node*);
int IsEmpty();
void push(char);
char pop();
char peek();
int precedence(char);
int main (void) 
{
    printf("Enter your infix expression(length<30): ");
    //Allocating memory to store the infix expression
    char* infix=(char*)malloc(MAX_LENGTH);
    char string[MAX_LENGTH];
    //Taking the infix expression input:
    int i=0;
    scanf("%[^\n]",string);
    
    int j=0;
    for(int i=0;i<strlen(string);i++)
    {
        if(string[i]==' ')
            continue;
        else infix[j++] = string[i];
    }
    infix[j]='\0';
    char* postfix=convert_to_postfix(infix);
    printf("The corresponding postfix expression is: %s\n",postfix);

    //three-address-code generation:
    char x,y;
    top = NULL;
    int q=0,k=0;
    if(postfix[1]=='=')
        k=2;
    for(int i=k; i<strlen(postfix);i++)
    {
        if((postfix[i]>47)&&(postfix[i]!=91)&&(postfix[i]!=93)&&(postfix[i]!=94)&&(postfix[i]!=123)&&(postfix[i]!=125))//Then postfix[i] is surely an operand
        {
            push(postfix[i]);
        }
        else //an operator
        {
            //take top 2 symbols and perform the operation
            x = pop();
            y=pop();
            if(isdigit(x) && !isdigit(y))
                printf("t%d = %c %c t%c\n",q,y,postfix[i],x);
            if(isdigit(y)&& !isdigit(x))
                printf("t%d = t%c %c %c\n",q,y,postfix[i],x);
            if(isdigit(y)&& isdigit(x))
                printf("t%d = t%c %c t%c\n",q,y,postfix[i],x);
            if(!isdigit(y)&& !isdigit(x))
                printf("t%d = %c %c %c\n",q,y,postfix[i],x);
            push((char)q+48);
            q++;
        }
    }
    printf("Result = t%c\n",pop());
    return 0;
}
char* convert_to_postfix(char* infix)
{
    int i=0,k=0;
    char x='a';
    char* postfix=(char*)malloc(MAX_LENGTH);
    while(infix[i]!='\0')
    {
        if((infix[i]>47)&&(infix[i]!=91)&&(infix[i]!=93)&&(infix[i]!=94)&&(infix[i]!=123)&&(infix[i]!=125))//Then infix[i] is surely an operand
        {
            postfix[k]=infix[i];
            k++;
        }
        else //infix[i] will be an operator or bracket:
        {
            if((IsEmpty())||(infix[i]=='{')||(infix[i]=='(')||(infix[i]=='[')||(precedence(infix[i])>precedence(peek())))
            {
                push(infix[i]);
            }
            else if(infix[i]==')')
            {
                do
                {
                    x=pop();
                    postfix[k]=x;
                    k++;
                } while (x!='(');
                k--; 
            }
            else if(infix[i]=='}')
            {
                do
                {
                    x=pop();
                    postfix[k]=x;
                    k++;
                } while (x!='{');
                k--; 
            }
            else if(infix[i]==']')
            {
                do
                {
                    x=pop();
                    postfix[k]=x;
                    k++;
                } while (x!='[');
                k--; 
            }
            else if(!(precedence(infix[i])>precedence(peek())))
            {
                do
                {
                    if(precedence(peek())==0)
                    break;
                    x=pop();
                    postfix[k]=x;
                    k++;
                } while ((precedence(x)!=precedence(infix[i]))&&(!IsEmpty()));
                push(infix[i]);
            }
        }
        i++;
    }
    while(!IsEmpty())
    {
        postfix[k]=pop();
        k++;
    }
    
    postfix[k]='\0';

    return postfix;
}
int Isfull(node* temp)
{
    if(temp==NULL)
    {
        printf("Memory full!");
        return 1;
    }
    return 0;
}
int IsEmpty()
{
    if(top==NULL){
        //printf("Stack is empty!\n");
    return 1;
    }
    return 0;
}
void push(char Val)
{
    node* new_node=(node*)malloc(sizeof(node));
    if(Isfull(new_node))
    return;
    new_node->operator=Val;
    new_node->next=top;
    top=new_node;
    return;
}
char pop()
{
    if(IsEmpty())
    return 0;
    node* temp=top;
    char k=temp->operator;
    top=top->next;
    free(temp);
    return k;
}
void display()
{
    node* temp=top;
    while(temp!=NULL)
    {
        printf("%d ",temp->operator);
        temp=temp->next;
    }
    printf("\n");
}
char peek()
{
    return top->operator;
}
int precedence(char c)
{
    if(c=='^')
    return 3;
    else if((c=='*')||(c=='/'))
    return 2;
    else if((c=='+')||(c=='-'))
    return 1;
    else return 0;
}