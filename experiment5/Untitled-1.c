#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_STATES 100
#define MAX_PRODUCTS 100

char grammar[MAX_PRODUCTS][MAX_SYMBOLS];
char non_terminals[MAX_SYMBOLS];
char terminals[MAX_SYMBOLS];
int num_product = 0;
int num_non_terminal = 0;
int num_terminal = 0;
int num_state = 0;

int is_non_terminal(char symbol)
{
    for (int i = 0; i < num_non_terminal; i++)
    {
        if (non_terminals[i] == symbol)
        {
            return i;
        }
    }
    return -1;
}

int is_terminal(char symbol)
{
    for (int i = 0; i < num_terminal; i++)
    {
        if (terminals[i] == symbol)
        {
            return i;
        }
    }
    return -1;
}

void calculate_non_terminals()
{
    for (int i = 0; i < num_product; i++)
    {
        char non_terminal = grammar[i][0];
        if (is_non_terminal(non_terminal) == -1)
        {
            non_terminals[num_non_terminal++] = non_terminal;
        }
    }
}

void calculate_terminals()
{
    for (int i = 0; i < num_product; i++)
    {
        for (int j = 0; j < strlen(grammar[i]); j++)
        {
            char symbol = grammar[i][j];
            if (is_non_terminal(symbol) == -1 && is_terminal(symbol) == -1 && symbol != '|')
            {
                terminals[num_terminal++] = symbol;
            }
        }
    }
}

void calculate_first(char symbol, int *first_set)
{
    if (is_terminal(symbol) != -1)
    {
        first_set[is_terminal(symbol)] = 1;
    }
    else
    {
        for (int i = 0; i < num_product; i++)
        {
            if (grammar[i][0] == symbol)
            {
                int j = 3;
                while (j < strlen(grammar[i]) && grammar[i][j] != '|')
                {
                    calculate_first(grammar[i][j], first_set);
                    j++;
                }
            }
        }
    }
}

void calculate_follow(char symbol, int *follow_set)
{
    if (symbol == grammar[0][0])
    {
        follow_set[num_terminal - 1] = 1;
    }
    for (int i = 0; i < num_product; i++)
    {
        for (int j = 3; j < strlen(grammar[i]); j++)
        {
            if (grammar[i][j] == symbol)
            {
                if (j == strlen(grammar[i]) - 1 || grammar[i][j + 1] == '|')
                {
                    if (grammar[i][0] != symbol)
                    {
                        calculate_follow(grammar[i][0], follow_set);
                    }
                }
                else
                {
                    int *first_set = (int *)calloc(num_terminal, sizeof(int));
                    calculate_first(grammar[i][j + 1], first_set);
                    for (int k = 0; k < num_terminal; k++)
                    {
                        if (first_set[k] == 1)
                        {
                            follow_set[k] = 1;
                        }
                    }
                    if (first_set[is_terminal('e')] == 1)
                    {
                        if (grammar[i][0] != symbol)
                        {
                            calculate_follow(grammar[i][0], follow_set);
                        }
                    }
                }
            }
        }
    }
}

void calculate_closure(int *state, int *num_state_product)
{
    int i = 0;
    while (i < *num_state_product)
    {
        int dot_position = -1;
        for (int j = 3; j < strlen(grammar[state[i]]); j++)
        {
            if (grammar[state[i]][j] == '.' && (j == 3 || grammar[state[i]][j - 1] == '|'))
            {
                dot_position = j;
                break;
            }
        }
        if (dot_position != -1 && dot_position < strlen(grammar[state[i]]) - 1)
        {
            char symbol = grammar[state[i]][dot_position + 1];
            int *first_set = (int *)calloc(num_terminal, sizeof(int));
            calculate_first(symbol, first_set);
            for (int j = 0; j < num_product; j++)
            {
                if (grammar[j][0] == symbol)
                {
                    int k = 3;
                    while (k < strlen(grammar[j]) && grammar[j][k] != '|')
                    {
                        int l = 0;
                        while (l < *num_state_product && state[l] != j)
                        {
                            l++;
                        }
                        if (l == *num_state_product)
                        {
                            state[*num_state_product] = j;
                            (*num_state_product)++;
                        }
                        for (int m = 0; m < num_terminal; m++)
                        {
                            if (first_set[m] == 1)
                            {
                                calculate_first(grammar[j][k + 1], first_set);
                                if (first_set[is_terminal('e')] == 1)
                                {
                                    calculate_follow(symbol, first_set);
                                    for (int n = 0; n < num_terminal; n++)
                                    {
                                        if (first_set[n] == 1)
                                        {
                                            calculate_follow(grammar[j][0], first_set);
                                            if (first_set[num_terminal - 1] == 1)
                                            {
                                                calculate_follow(grammar[j][0], follow_set);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        k++;
                    }
                }
            }
        }
        i++;
    }
}

void calculate_goto(int *state, int num_state_product, int symbol, int *next_state)
{
    int *temp_state = (int *)calloc(num_state_product, sizeof(int));
    int num_temp_state_product = 0;
    for (int i = 0; i < num_state_product; i++)
    {
        int dot_position = -1;
        for (int j = 3; j < strlen(grammar[state[i]]); j++)
        {
            if (grammar[state[i]][j] == '.' && (j == 3 || grammar[state[i]][j - 1] == '|'))
            {
                dot_position = j;
                break;
            }
        }
        if (dot_position != -1 && dot_position < strlen(grammar[state[i]]) - 1 && grammar[state[i]][dot_position + 1] == symbol)
        {
            temp_state[num_temp_state_product] = state[i];
            num_temp_state_product++;
        }
    }
    for (int i = 0; i < num_temp_state_product; i++)
    {
        int dot_position = -1;
        for (int j = 3; j < strlen(grammar[temp_state[i]]); j++)
        {
            if (grammar[temp_state[i]][j] == '.' && (j == 3 || grammar[temp_state[i]][j - 1] == '|'))
            {
                dot_position = j;
                break;
            }
        }
        if (dot_position != -1 && dot_position < strlen(grammar[temp_state[i]]) - 1)
        {
            char next_symbol = grammar[temp_state[i]][dot_position + 1];
            int j = 0;
            while (j < num_temp_state_product && temp_state[j] != temp_state[i])
            {
                j++;
            }
            if (j == num_temp_state_product)
            {
                temp_state[num_temp_state_product] = temp_state[i];
                num_temp_state_product++;
            }
        }
    }
    memcpy(next_state, temp_state, num_temp_state_product * sizeof(int));
}

void calculate_states(state_stack *I0)
{
    state_stack *current_state = I0;
    while (current_state != NULL)
    {
        int *state = (int *)calloc(num_product, sizeof(int));
        int num_state_product = 0;
        for (int i = 0; i < current_state->num_state_product; i++)
        {
            state[num_state_product] = current_state->state_product[i];
            num_state_product++;
        }
        calculate_closure(state, &num_state_product);
        for (int i = 0; i < num_terminal; i++)
        {
            int *next_state = (int *)calloc(num_product, sizeof(int));
            calculate_goto(state, num_state_product, terminals[i], next_state);
            if (next_state[0] != -1)
            {
                int j = 0;
                while (j < current_state->num_next && memcmp(current_state->next[j]->state_product, next_state, num_state_product * sizeof(int)) != 0)
                {
                    j++;
                }
                if (j == current_state->num_next)
                {
                    state_stack *new_state = (state_stack *)malloc(sizeof(state_stack));
                    memcpy(new_state->state_product, next_state, num_state_product * sizeof(int));
                    new_state->num_state_product = num_state_product;
                    new_state->position = 0;
                    new_state->state = num_state;
                    num_state++;
                    new_state->next = NULL;
                    current_state->next[current_state->num_next] = new_state;
                    current_state->num_next++;
                }
                else
                {
                    free(next_state);
                }
            }
            else
            {
                free(next_state);
            }
        }
        current_state = current_state->next_state;
    }
}

void print_first()
{
    printf("First sets:\n");
    for (int i = 0; i < num_non_terminal; i++)
    {
        printf("  %c: { ", non_terminals[i]);
        int *first_set = (int *)calloc(num_terminal, sizeof(int));
        calculate_first(non_terminals[i], first_set);
        for (int j = 0; j < num_terminal; j++)
        {
            if (first_set[j] == 1)
            {
                printf("%c ", terminals[j]);
            }
        }
        printf("}\n");
    }
}

void print_follow()
{
    printf("Follow sets:\n");
    for (int i = 0; i < num_non_terminal; i++)
    {
        printf("  %c: { ", non_terminals[i]);
        int *follow_set = (int *)calloc(num_terminal, sizeof(int));
        calculate_follow(non_terminals[i], follow_set);
        for (int j = 0; j < num_terminal; j++)
        {
            if (follow_set[j] == 1)
            {
                printf("%c ", terminals[j]);
            }
        }
        printf("}\n");
    }
}

void print_states(state_stack *I0)
{
    printf("States:\n");
    state_stack *current_state = I0;
    while (current_state != NULL)
    {
        printf("  State %d:\n", current_state->state);
        for (int i = 0; i < current_state->num_state_product; i++)
        {
            printf("    %s\n", grammar[current_state->state_product[i]]);
        }
        printf("    Goto:\n");
        for (int i = 0; i < current_state->num_next; i++)
        {
            printf("      %c -> State %d\n", terminals[current_state->next[i]->position], current_state->next[i]->state);
        }
        current_state = current_state->next_state;
    }
}

int main()
{
    strcpy(grammar[num_product++], "S->E");
    strcpy(grammar[num_product++], "E->E+T");
    strcpy(grammar[num_product++], "E->T");
    strcpy(grammar[num_product++], "T->T*F");
    strcpy(grammar[num_product++], "T->F");
    strcpy(grammar[num_product++], "F->(E)");
    strcpy(grammar[num_product++], "F->i");

    calculate_non_terminals();
    calculate_terminals();

    print_first();
    print_follow();

    state_stack *I0 = inilaize_state_stack();
    calculate_states(I0);
    print_states(I0);

    return 0;
}
