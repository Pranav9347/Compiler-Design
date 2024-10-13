#include <stdio.h>
#include <ctype.h>
#include <string.h>

int lookahead = 0;
char str[20];
char string[20];

int S();
int E();
int Edash();
int T();
int Tdash();
int F();
int V();
int N();
int Ddash();
int D();

int main()
{
	printf("Enter the input string: ");
	scanf(" %[^\n]",str);
	int j=0;
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]==' ')
			continue;
		else string[j++] = str[i];
	}
	string[j]='\0';
	strcpy(str,string);
	if(S())
	{
		printf("Accepted\n");
	}
	else
		printf("Rejected\n");
	//printf("pos=%d\nlookahead=%c",lookahead,str[lookahead]);
	return 0;
}

int S()
{
	if(E())
		return 1;
	return 0;
}

int E()
{
	if(T())
	{
		if(Edash())
			return 1;
		return 0;
	}
	return 0;
}

int Edash()
{
	if(str[lookahead]=='+' || str[lookahead]=='-')
	{
		lookahead++;
		if(T())
		{
			if(Edash())
				return 1;
			return 0;
		}
		return 0;
	}
	else return 1;
}

int T()
{
	if(F())
	{
		if(Tdash())
			return 1;
		return 0;
	}
	else return 0;
}

int Tdash()
{
	if(str[lookahead]=='*'||str[lookahead]=='/')
	{
		lookahead++;
		if(F())
		{
			if(Tdash())
				return 1;
			return 0;
		}
		return 0;
	}
	else return 1;
}

int F()
{
	if(str[lookahead]=='-'||str[lookahead]=='+')
	{
		lookahead++;
		if(F())
			return 1;
		return 0;
	}
	else if(str[lookahead]=='(')
	{
		lookahead++;
		if(E())
		{
			if(str[lookahead]==')')
			{
				lookahead++;
				return 1;
			}
			return 0;
		}
		return 0;
	}
	else if(N())
		return 1;
	else if(V())
		return 1;
	else return 0;

}

int V()
{
	if(str[lookahead]=='x'||str[lookahead]=='y'||str[lookahead]=='z')
	{
		lookahead++;
		return 1;
	}
	else return 0;
}

int N()
{
	if(D())
	{
		if(Ddash())
			return 1;
		return 0;
	}
	else return 0;
}

int Ddash()
{
	if(N())
		return 1;
	else return 1;
}

int D()
{
	if(isdigit(str[lookahead]))
	{
		lookahead++;
		return 1;
	}
	else return 0;
}