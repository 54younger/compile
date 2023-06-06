#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_STATES 1000

char grammar_ext[MAX_SYMBOLS][MAX_SYMBOLS] = {'\0'};
char grammar[MAX_SYMBOLS][MAX_SYMBOLS] = {'\0'};
int num_product_ext;
int num_product;
char terminals[MAX_SYMBOLS];
int num_terminals;
char non_terminals[MAX_SYMBOLS];
int num_non_terminals;
char non_terminals_ori[MAX_SYMBOLS];
int num_non_terminals_ori;
int start_symbol;
int num_states;
char action[MAX_STATES][MAX_SYMBOLS];
int goto_table[MAX_STATES][MAX_SYMBOLS];
int follow[MAX_SYMBOLS][MAX_SYMBOLS] = {0};
int follow_ori[MAX_SYMBOLS][MAX_SYMBOLS] = {0};
int first[MAX_SYMBOLS][MAX_SYMBOLS] = {0};
int temp[MAX_SYMBOLS][MAX_SYMBOLS] = {0};
typedef struct
{
  int position[MAX_SYMBOLS];
  int num_state_product;
  char state_product[MAX_SYMBOLS][MAX_SYMBOLS];
  // int state;
  // state_stack *next;
} state_stack;

state_stack States_I[MAX_STATES] = {-1};

int if_state_exit(state_stack *state, int num_states)
{
  state_stack *temp = States_I;
  for (int i = 0; i < num_states; i++)
  {
    if ((temp + i)->num_state_product == state->num_state_product)
    {
      for (int i = 0; i < temp->num_state_product; i++)
      {
        if (strcmp((temp + i)->state_product[i], state->state_product[i]) != 0)
        {
          break;
        }
        if (i == (temp + i)->num_state_product - 1)
        {
          return i;
        }
      }
    }
  }
  return -1;
}

int is_terminal(char symbol)
{
  int i;
  for (i = 0; i < num_terminals; i++)
  {
    if (terminals[i] == symbol)
    {
      return i;
    }
  }
  return -1;
}

int is_non_terminal(char symbol)
{
  int i;
  for (i = 0; i < num_non_terminals; i++)
  {
    if (non_terminals[i] == symbol)
    {
      return i;
    }
  }
  return -1;
}

void inilaize_state_stack()
{
  States_I->num_state_product = num_product;
  for (int i = 0; i < num_product; i++)
  {
    strcpy(States_I->state_product[i], grammar[i]);
    States_I->position[i] = 3;
  }

  int move = 0;
  int new_position = 1;
  int i = 1;
  for (i = 1; (States_I + i)->num_state_product != -1; i++)
  {
    // new_position += move;
    move = 0;
    // �ֱ����ÿ���ս���ͷ��ս��

    for (int j = 0; j < num_terminals; j++) // �����ս��
    {
      int flag = 0;
      for (int k = 0; k < num_product; k++)
      {
        if ((States_I + i - 1)->state_product[k][(States_I + i - 1)->position[k]] == terminals[j]) // ����ò���ʽ�ĵ�ǰ������������ս��
        {
          flag = 1;
          // ���ò���ʽ���뵽Ii��
          strcpy((States_I + new_position)->state_product[(States_I + new_position)->num_state_product], grammar[k]);
          // ���ò���ʽ��λ������ƶ�һλ
          (States_I + new_position)->position[(States_I + new_position)->num_state_product] = (States_I + i - 1)->position[k] + 1;
          (States_I + new_position)->num_state_product++;
        }
      }
      if (flag)
      { // ѭ���鿴�����²�����״̬������ƶ���ĵ�ǰ����Ϊ���ս���������ս����Ӧ�Ĳ���ʽ���ظ��ؼ��뵽Ii�У�����λ����Ϊ0(3)
        for (int k = 0; k < (States_I + new_position)->num_state_product; k++)
        {
          if (is_non_terminal((States_I + new_position)->state_product[k][(States_I + new_position)->position[k]]))
          {
            for (int l = 0; l < num_product; l++)
            {
              if ((States_I + new_position)->state_product[k][(States_I + new_position)->position[k]] == grammar[l][0])
              {
                strcpy((States_I + new_position)->state_product[(States_I + new_position)->num_state_product], grammar[l]);
                (States_I + new_position)->position[(States_I + new_position)->num_state_product] = 3;
                (States_I + new_position)->num_state_product++;
              }
            }
          }
        }
        if (if_state_exit(States_I + new_position, new_position) == -1)
        {
          action[i - 1][j] = new_position;
          move++;
          new_position++;
        }
        else
        {
          action[i - 1][j] = if_state_exit(States_I + new_position, new_position);
        }
        (States_I + new_position)->num_state_product = 0;
        printf("Input: %c\n", terminals[j]);
        printf("States_I[%d] = \n", action[i - 1][j]);
        for (int k = 0; k < (States_I + action[i - 1][j])->num_state_product; k++)
        {
          printf("             ");
          printf("%s ", (States_I + action[i - 1][j])->state_product[k]);
          printf("position: ");
          printf("%d \n", (States_I + action[i - 1][j])->position[k]);
        }
        printf("\n\n");
      }
    }
    for (int j = 0; j < num_non_terminals; j++) // �������ս��
    {
      int flag = 0;
      for (int k = 0; k < num_product; k++)
      {
        if ((States_I + i - 1)->state_product[k][(States_I + i - 1)->position[k]] == non_terminals[j]) // ����ò���ʽ�ĵ�ǰ����������ķ��ս��
        {
          flag = 1;
          // ���ò���ʽ���뵽Ii��
          strcpy((States_I + new_position)->state_product[(States_I + new_position)->num_state_product], grammar[k]);
          // ���ò���ʽ��λ������ƶ�һλ
          (States_I + new_position)->position[(States_I + new_position)->num_state_product] = (States_I + i - 1)->position[k] + 1;
          (States_I + new_position)->num_state_product++;
        }
      }
      if (flag)
      {
        // ѭ���鿴�����²�����״̬������ƶ���ĵ�ǰ����Ϊ���ս���������ս����Ӧ�Ĳ���ʽ���ظ��ؼ��뵽Ii�У�����λ����Ϊ0(3)
        for (int k = 0; k < (States_I + new_position)->num_state_product; k++)
        {
          if (is_non_terminal((States_I + new_position)->state_product[k][(States_I + new_position)->position[k]]))
          {
            for (int l = 0; l < num_product; l++)
            {
              if ((States_I + new_position)->state_product[k][(States_I + new_position)->position[k]] == grammar[l][0])
              {
                strcpy((States_I + new_position)->state_product[(States_I + new_position)->num_state_product], grammar[l]);
                (States_I + new_position)->position[(States_I + new_position)->num_state_product] = 3;
                (States_I + new_position)->num_state_product++;
              }
            }
          }
        }
        if (if_state_exit(States_I + new_position, new_position) == -1)
        {
          goto_table[i - 1][j] = new_position;
          move++;
          new_position++;
        }
        else
        {
          goto_table[i - 1][j] = if_state_exit(States_I + new_position, new_position);
        }
        (States_I + new_position)->num_state_product = 0;
        printf("Input: %c\n", non_terminals[j]);
        printf("States_I[%d] = \n", goto_table[i - 1][j]);
        for (int k = 0; k < (States_I + goto_table[i - 1][j])->num_state_product; k++)
        {
          printf("             ");
          printf("%s ", (States_I + goto_table[i - 1][j])->state_product[k]);
          printf("position: ");
          printf("%d \n", (States_I + goto_table[i - 1][j])->position[k]);

        }
printf("\n\n");
      }

    }
    num_states = i;
  }
}
bool compare_or_save(int New[][MAX_SYMBOLS], int old[][MAX_SYMBOLS])
{
  bool flag = true;
  for (int i = 0; i < num_non_terminals + 1; i++)
  {
    for (int j = 0; j < num_non_terminals + 1; j++)
    {
      if (New[i][j] != old[i][j])
      {
        temp[i][j] = New[i][j];
        flag = false;
      }
    }
  }
  return flag;
}

void GetFirst(char nonterminal) // ��first��
{

  for (int i = 0; i < num_product_ext; i++)
  {
    if (grammar_ext[i][0] != nonterminal)
    {
      continue;
    }
    else if (grammar_ext[i][3] == '#')
    {
      first[is_non_terminal(grammar_ext[i][0])][num_terminals] = 1;
    }
    else
    {
      if (is_terminal(grammar_ext[i][3]) != -1)
      {
        // S=a...
        first[is_non_terminal(grammar_ext[i][0])][is_terminal(grammar_ext[i][3])] = 1;
      }
      else
      {
        // S=B...
        GetFirst(grammar_ext[i][3]);
        for (int j = 0; j < num_terminals; j++)
        {
          first[is_non_terminal(grammar_ext[i][0])][j] = first[is_non_terminal(grammar_ext[i][0])][j] || first[is_non_terminal(grammar_ext[i][3])][j];
        }
      }
    }
  }
}

void generate_First()
{
  for (int i = 0; i < num_non_terminals; i++)
  {
    GetFirst(non_terminals[i]);
  }
}

// ��ÿ�����ս����follow��
void generate_Follow()
{
  // ����ǿ�ʼ���ţ�follow������$
  follow[is_non_terminal('S')][num_terminals] = 1;
  do
  {
    for (int i = 0; i < num_product_ext; i++) // ��ǰ����ʽ
    {
      for (int j = 3; j < strlen(grammar_ext[i]); j++) // ��ǰ����
      {
        if (is_non_terminal(grammar_ext[i][j]) != -1) // ����Ƿ��ս��
        {
          // �����һ���Ƿ��սᣬ��first������follow��
          if (is_non_terminal(grammar_ext[i][j + 1]) != -1)
          {
            if (first[is_non_terminal(grammar_ext[i][j + 1])][num_terminals] == 1)
            { // ���������Ƶ����մ�����follow������follow��
              for (int k = 0; k < num_terminals + 1; k++)
              {
                follow[is_non_terminal(grammar_ext[i][j])][k] = follow[is_non_terminal(grammar_ext[i][0])][k] || follow[is_non_terminal(grammar_ext[i][j])][k];
              }
            }
            for (int k = 0; k < num_terminals; k++)
            {
              follow[is_non_terminal(grammar_ext[i][j])][k] = first[is_non_terminal(grammar_ext[i][j + 1])][k] || follow[is_non_terminal(grammar_ext[i][j])][k];
            }
          }
          else if (is_terminal(grammar_ext[i][j + 1]) != -1)
          {
            // �����һ�����ս����ֱ�Ӽ���follow��
            follow[is_non_terminal(grammar_ext[i][j])][is_terminal(grammar_ext[i][j + 1])] = 1;
          }
          else if (grammar_ext[i][j + 1] == '\0')
          { // ��������һ������follow������follow��
            for (int k = 0; k < num_terminals + 1; k++)
            {
              follow[is_non_terminal(grammar_ext[i][j])][k] = follow[is_non_terminal(grammar_ext[i][0])][k] || follow[is_non_terminal(grammar_ext[i][j])][k];
            }
          }
        }
      }
    }
  } while (!compare_or_save(follow, temp));
}

void print_grammar_ext()
{
  int i;
  printf("grammar_ext:\n");
  for (i = 0; i < num_product_ext; i++)
  {
    printf("%s\n", grammar_ext[i]);
  }
}

void print_goto()
{
  printf("GOTO:\n");
  printf("%4s", "");
  for (int i = 0; i < num_non_terminals; i++)
  {
    printf("%4c|", non_terminals[i]);
  }
  printf("\n");
  for (int i = 0; i < num_states; i++)
  {
    printf("%4d|", i);
    for (int j = 0; j < num_non_terminals; j++)
    {
      printf("%4d|", goto_table[i][j]);
    }
    printf("\n");
  }
}

void print_action()
{
  printf("ACTION:\n");
  printf("%4s", "");
  for (int i = 0; i < num_terminals; i++)
  {
    printf("%4c|", terminals[i]);
  }
  printf("%4s", "$|\n");
  for (int i = 0; i < num_states; i++)
  {
    printf("%4d|", i);
    for (int j = 0; j < num_terminals + 1; j++)
    {
      printf("%4d|", action[i][j]);
    }
    printf("\n");
  }
}

void print_first_ext()
{
  printf("�ع�FIRST:\n");
  printf("%4s", "");
  for (int i = 0; i < num_terminals; i++)
  {
    printf("%4c|", terminals[i]);
  }
  printf("%4s", "�մ�|\n");
  for (int i = 0; i < num_non_terminals; i++)
  {
    printf("%4c|", non_terminals[i]);
    for (int j = 0; j < num_terminals + 1; j++)
    {
      printf("%4d|", first[i][j]);
    }
    printf("\n");
  }
}

void print_follow_ext()
{

  printf("�ع�FOLLOW:\n");
  printf("%4s", "");
  for (int i = 0; i < num_terminals; i++)
  {
    printf("%4c|", terminals[i]);
  }
  printf("%4s", "$|\n");
  for (int i = 0; i < num_non_terminals; i++)
  {
    printf("%4c|", non_terminals[i]);
    for (int j = 0; j < num_terminals + 1; j++)
    {
      printf("%4d|", follow[i][j]);
    }
    printf("\n");
  }
}

void get_and_print_ori_follow()
{
  // ���ع��ķ���FOLLOW��ֵ��ԭ�ķ���FOLLOW
  for (int i = 0; i < num_non_terminals_ori; i++)
  {
    for (int j = 0; j < num_non_terminals; j++)
    {
      if (non_terminals_ori[i] == non_terminals[j])
      {
        for (int k = 0; k < num_terminals + 1; k++)
        {
          follow_ori[i][k] = follow[j][k];
        }
      }
    }
  }
  printf("ԭ�ķ�FOLLOW:\n");
  printf("%4s", "");
  for (int i = 0; i < num_terminals; i++)
  {
    printf("%4c|", terminals[i]);
  }
  printf("%4s", "$|\n");
  for (int i = 0; i < num_non_terminals_ori; i++)
  {
    printf("%4c|", non_terminals_ori[i]);
    for (int j = 0; j < num_terminals + 1; j++)
    {
      printf("%4d|", follow_ori[i][j]);
    }
    printf("\n");
  }
}
int main()
{
  printf("���������ʽ����: ");
  scanf("%d", &num_product);
  for (int i = 0; i < num_product; i++)
  {
    printf("���������ʽ %d: ", i + 1);
    scanf("%s", grammar[i]);
  }
  printf("�������ع��ķ�����ʽ����:");
  scanf("%d", &num_product_ext);
  for (int i = 0; i < num_product_ext; i++)
  {
    printf("�������ع��ķ�����ʽ %d: ", i + 1);
    scanf("%s", grammar_ext[i]);
  }
  printf("�������ս������: ");
  scanf("%d", &num_terminals);
  for (int i = 0; i < num_terminals; i++)
  {
    printf("�ս�� %d: ", i + 1);
    scanf(" %c", terminals + i);
  }
  printf("��������ս������: ");
  scanf("%d", &num_non_terminals_ori);
  for (int i = 0; i < num_non_terminals_ori; i++)
  {
    printf("���ս�� %d: ", i + 1);
    scanf(" %c", non_terminals_ori + i);
  }
  printf("�������ع����ս������: ");
  scanf("%d", &num_non_terminals);
  for (int i = 0; i < num_non_terminals; i++)
  {
    printf("�ع����ս�� %d: ", i + 1);
    scanf(" %c", non_terminals + i);
  }
  num_states = 2;
  generate_First();
  generate_Follow();
  get_and_print_ori_follow();

  system("pause");

  inilaize_state_stack();
  print_action();
  print_goto();
  system("pause");
  return 0;
}
