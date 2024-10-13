#include <stdio.h>

char stack[50];
int top=-1;
int is_empty()
{
	if(top==-1)
		return 1;
	return 0;
}
void push(char x)
{
	top++;
	stack[top] = x;
}
char pop()
{
	if(is_empty())
	{
		printf("Stack is empty!\n");
		return '\0';
	}
	char x = stack[top];
	top--;
	return x;
}
char peek()
{
	if(is_empty())
	{
		printf("Stack is empty!\n");
		return '\0';
	}
	return stack[top];
}


int main()
{
	push('a');
	push('b');
	printf("peek1=%c\n",peek());
	pop();
	printf("peek2=%c\n",peek());
	pop();
	printf("peek3=%c\n",peek());
	pop();
}