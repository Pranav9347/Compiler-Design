#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define max_production_size 10

int n, nt=0, t=0;
char** productions;
char* NT;
char* T;
void get_NT();
void get_T();
int get_pos(int sym, char s); // sym=0 for T, sym=1 for NT

char FIRST[2][10][10]; // FIRST[0] for T, FIRST[1] for NT

int rem_epsilon(char* str, char s, int k) // k is the position of the next terminal to be added in the first of s
{
    int flag = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '#')
        {
            flag++;
            continue;
        }
        else
        {
            FIRST[1][get_pos(1, s)][k++] = str[i];
        }
    }
    FIRST[1][get_pos(1, s)][k++] = '\0';
    return flag;
}

void init_first(void)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            FIRST[i][j][0] = '\0'; // Initialize all first sets to empty strings
        }
    }
}

void first(char s)
{
    // Don't find again if already found:
    int sym = isupper(s); // Determine whether s is a non-terminal (1) or terminal (0)
    int pos = get_pos(sym, s); // Get position of s in NT or T array
    if (pos < 0) return; // Invalid position, handle gracefully

    if (FIRST[sym][pos][0] != '\0') // If the first set is already computed, return
        return;

    int k = 0;
    if (islower(s))  // If s is a terminal
    {
        FIRST[0][get_pos(0, s)][0] = s;
        FIRST[0][get_pos(0, s)][1] = '\0'; // Null-terminate to denote the end of string
    }
    else    // If s is a non-terminal
    {
        for (int i = 0; i < n; i++)
        {
            if (productions[i][0] == s)
            {
                if (islower(productions[i][1]) || productions[i][1] == '#')  // Add to first if it's a terminal
                {
                    FIRST[1][pos][k++] = productions[i][1];
                }
                else    // If the production derives a non-terminal first, find first of that NT
                {
                    if (productions[i][1] == s)  // Left recursion case, ignore
                        continue;

                    int j = 1; // Start after the non-terminal (lhs)
                    while (productions[i][j] != '\0')
                    {
                        char c = productions[i][j];
                        if (islower(c))  // Add terminal to first
                        {
                            FIRST[1][pos][k++] = c;
                            break;
                        }
                        else
                        {
                            // If the first of NT is not found yet, find it first
                            first(c);
                            int found_epsilon = rem_epsilon(FIRST[1][get_pos(1, c)], s, k);
                            if (!found_epsilon) // Stop if no epsilon in this NT's first set
                                break;
                        }
                        j++;
                    }
                }
            }
        }
    }
    FIRST[1][pos][k] = '\0'; // Null-terminate the first set
}

int main()
{
    FILE* fptr = fopen("grammar.txt", "r");
    fscanf(fptr, "%d\n", &n);

    productions = (char**)malloc(n * sizeof(char*)); // Allocate space for n strings
    for (int i = 0; i < n; i++)
    {
        productions[i] = (char*)malloc(sizeof(char) * max_production_size); // Allocate space for each string
    }

    for (int i = 0; i < n; i++)
    {
        fscanf(fptr, "%c=%s\n", &productions[i][0], productions[i] + 1);
    }

    get_NT();
    get_T();
    printf("no of NT: %d\n", nt);
    printf("no of T: %d\n", t);
    init_first();
    for (int i = 0; i < nt; i++)
    {
        first(NT[i]);
        printf("First(%c)=%s\n", NT[i], FIRST[1][i]);
    }

    fclose(fptr);
    return 0;
}

void get_NT()
{
    NT = (char*)malloc(n * sizeof(char));
    for (int i = 0; i < n; i++)
    {
        int found = 0;
        for (int j = 0; j < nt; j++)
        {
            if (NT[j] == productions[i][0])
            {
                found = 1;
                break;
            }
        }
        if (!found)
            NT[nt++] = productions[i][0];
    }
    printf("NT: ");
    for (int i = 0; i < nt; i++)
    {
        printf("%c ", NT[i]);
    }
    printf("\n");
}

void get_T()
{
    T = (char*)malloc(n * 5 * sizeof(char));
    for (int i = 0; i < n; i++)
    {
        for (int j = 1; productions[i][j] != '\0'; j++)
        {
            if (islower(productions[i][j]))
            {
                int found = 0;
                for (int k = 0; k < t; k++)
                {
                    if (T[k] == productions[i][j])
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    T[t++] = productions[i][j];
            }
        }
    }
    printf("T: ");
    for (int i = 0; i < t; i++)
    {
        printf("%c ", T[i]);
    }
    printf("\n");
}

int get_pos(int sym, char s)
{
    if (sym == 0) // For terminals
    {
        for (int i = 0; i < t; i++)
            if (T[i] == s)
                return i;
    }
    else // For non-terminals
    {
        for (int i = 0; i < nt; i++)
            if (NT[i] == s)
                return i;
    }
    return -1; // Return -1 if not found, handle in calling functions
}

