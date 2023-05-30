#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#define MAX_LENGTH 100
#define NUM_OF_NONTERMINAL 9
char *representation[] = {
    "V=E",   // S
    "i",     // V
    "B",     // E
    "ATB|#", // B
    "FC",    // T
    "MFC|#", // C
    "(E)|i", // F
    "+|-",   // A
    "*|/"    // M
};
char *representation2[] = {
    "V=E",      // S
    "i",        // V
    "B",        // E
    "ATB", "#", // B
    "FC",       // T
    "MFC", "#", // C
    "(E)", "i", // F
    "+", "-",   // A
    "*", "/"    // M
};
char note[] = {'S', 'V', 'E', 'B', 'B', 'T', 'C', 'C', 'F', 'A', 'A', 'M', 'M'};
int first[NUM_OF_NONTERMINAL][NUM_OF_NONTERMINAL] = {0};
int follow[NUM_OF_NONTERMINAL][NUM_OF_NONTERMINAL] = {0};
int select_table[14][NUM_OF_NONTERMINAL] = {0};
int temp[NUM_OF_NONTERMINAL][NUM_OF_NONTERMINAL] = {0};

bool isNonterminal(char symbol) // 非终结符则返回1
{
  return symbol == 'S' || symbol == 'V' || symbol == 'E' || symbol == 'B' || symbol == 'T' || symbol == 'C' || symbol == 'F' || symbol == 'A' || symbol == 'M';
}

int getcol(char symbol)
{
  switch (symbol)
  {
  case 'i':
    return 0;
  case '(':
    return 1;
  case ')':
    return 2;
  case '+':
    return 3;
  case '-':
    return 4;
  case '*':
    return 5;
  case '/':
    return 6;
  case '=':
    return 7;
  case '$':
    return 8;
  case '#':
    return 8;
  default:
    return -1;
  }
}
int getrow(char symbol)
{
  switch (symbol)
  {
  case 'S':
    return 0;
  case 'V':
    return 1;
  case 'E':
    return 2;
  case 'B':
    return 3;
  case 'T':
    return 4;
  case 'C':
    return 5;
  case 'F':
    return 6;
  case 'A':
    return 7;
  case 'M':
    return 8;
  default:
    return -1;
  }
}

bool compare_or_save(int New[][NUM_OF_NONTERMINAL], int old[][NUM_OF_NONTERMINAL])
{
  for (int i = 0; i < NUM_OF_NONTERMINAL; i++)
  {
    for (int j = 0; j < NUM_OF_NONTERMINAL; j++)
    {
      if (New[i][j] != old[i][j])
      {
        temp[i][j] = New[i][j];
        return false;
      }
    }
  }
  return true;
}
char GetFirst(int nonterminal) // 求first集
{
  for (int i = 0; i < NUM_OF_NONTERMINAL; i++)
  {
    if (representation[nonterminal][i] == '|' || i == 0)
    {
      int temp;
      if (i == 0)
      {
        temp = i;
      }
      else
      {
        temp = i + 1;
      }
      if (!isNonterminal(representation[nonterminal][temp]))
      {
        // S=a...
        return representation[nonterminal][temp];
      }
      else
      {
        while (GetFirst(getrow(representation[nonterminal][temp])) == '#')
        {
          temp++;
        }
        return GetFirst(getrow(representation[nonterminal][temp]));
      }
    }
  }
}

void InsertFirst() // 置入FIRST集
{
  for (int nonterminal = 0; nonterminal < NUM_OF_NONTERMINAL; nonterminal++)
  {
    char input = GetFirst(nonterminal);
    first[nonterminal][getcol(input)] = 1;
  }
}

// 求每个非终结符的follow集
void GetFollow()
{
  // 如果是开始符号，follow集加入$
  follow[0][getcol('$')] = 1;
  do
  {
    for (int i = 0; i < NUM_OF_NONTERMINAL; i++)
    {
      for (int j = 0; j < strlen(representation[i]); j++)
      {
        if (isNonterminal(representation[i][j])) // 如果是非终结符
        {
          // 如果下一个是非终结，则first集加入follow集
          if (isNonterminal(representation[i][j + 1]))
          {
            for (int k = 0; k < NUM_OF_NONTERMINAL - 1; k++)
            {
              follow[getrow(representation[i][j])][k] = first[getrow(representation[i][j + 1])][k] || follow[getrow(representation[i][j])][k];
            }
          }
          else if (representation[i][j + 1] != '|' || representation[i][j + 1] != '\0')
          { // 如果是最后一个，则follow集加入follow集
            for (int k = 0; k < NUM_OF_NONTERMINAL; k++)
            {
              follow[getrow(representation[i][j])][NUM_OF_NONTERMINAL - 1] = follow[i][k] || follow[getrow(representation[i][j])][NUM_OF_NONTERMINAL - 1];
            }
          }
          else
          {
            // 如果下一个是终结符，直接加入follow集
            follow[getrow(representation[i][j])][getcol(representation[i][j + 1])] = 1;
          }
        }
      }
    }
  } while (!compare_or_save(follow, temp));
}
// 判断是否是空串
bool ifEmpty(int i)
{
  if (strlen(representation2[i]) == 1 && isNonterminal(representation2[i][0]))
  {
    for (int j = 0; i < 14; j++)
    {
      if (note[j] == representation2[i][0])
      {
        if (ifEmpty(j))
          return true;
      }
    }
    return false;
  }
  else if (representation2[i][0] == '#')
  {
    return true;
  }
  else if (strlen(representation2[i]) > 1)
  {
    return false;
  }
}
// 得到select_table集
void Getselect_table()
{
  for (int i = 0; i < 14; i++)
  {
    if (ifEmpty(i))
    {
      for (int j = 0; j < NUM_OF_NONTERMINAL; j++)
      {
        select_table[i][j] = first[getrow(note[i])][j] || follow[getrow(note[i])][j];
      }
    }
    else
    {
      for (int j = 0; j < NUM_OF_NONTERMINAL; j++)
      {
        select_table[i][j] = first[getrow(note[i])][j];
      }
    }
  }
}
char Auto_ll1[][9][9] = {'\0'};
// 生成ll1表
void get_ll1()
{
  for (int i = 0; i < 14; i++)
  {
    for (int j = 0; j < NUM_OF_NONTERMINAL; j++)
    {
      if (select_table[i][j])
      {
        strcpy(Auto_ll1[getrow(note[i])][j], representation2[i]);
      }
    }
  }
}
void Print_ll1()
{
  printf("LL1表如下:\n");
  printf("    ");
  for (int i = 0; i < NUM_OF_NONTERMINAL; i++)
  {
   // printf("%-10s", nonterminal_names[i]);
  }
  printf("\n");
  for (int i = 0; i < NUM_OF_NONTERMINAL; i++)
  {
   // printf("%-4s", nonterminal_names[i]);
    for (int j = 0; j < NUM_OF_NONTERMINAL; j++)
    {
      if (Auto_ll1[i][j][0] != '\0')
      {
        printf("%-10s", Auto_ll1[i][j]);
      }
      else
      {
        printf("%-10s", "\\0");
      }
    }
    printf("\n");
  }
}
char *TokenTypeString[] = {
    // 关键字0-11
    "void", "int", "float", "double", "char", "if", "else", "for", "do", "while", "return", "ID",
    // 单字符分界符12-27
    "+", "-", "*", "/", "=", "<", ">", "!", "(", ")", "{", "}", "[", "]", ";", "\n",
    // 双字符分界符28-44
    "==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "()", "[]", "{}", "//", "/*"
    // 数字45
    ,
    "num"};
char *LL1_TABLE[][9] = {
    {"V=E", "", "", "", "", "", "", "", ""},        // S
    {"i", "", "", "", "", "", "", "", ""},          // V
    {"TB", "TB", "", "", "", "", "", "", ""},       // E
    {"", "", "e", "ATB", "ATB", "", "", "", "e"},   // B
    {"FC", "FC", "", "", "", "", "", "", ""},       // T
    {"", "", "e", "e", "e", "MFC", "MFC", "", "e"}, // C
    {"i", "(E)", "", "", "", "", "", "", ""},       // F
    {"", "", "", "+", "-", "", "", "", ""},         // A
    {"", "", "", "", "", "*", "/", "", ""}};        // M
//   i   +   -    *   /   (    )   #   $
char *change_format(FILE *fp)
{
  int num_out[MAX_LENGTH];
  static char output[MAX_LENGTH];
  memset(output, -1, strlen(output));
  char ch;
  int num = 0, i = 0;
  while ((ch = fgetc(fp)) != EOF)
  {
    if (isspace(ch))
    {
      continue; // 忽略空格、制表符、换行符等
    }
    if (ch == '(')
    {
      ch = fgetc(fp);

      num = ch - '0';
      while ((ch = fgetc(fp)) != ',')
      {
        num *= 10;
        num += ch - '0';
        // ch = fgetc(fp);
      }
      while ((ch = fgetc(fp)) != ')')
      {
      }

      num_out[i++] = num;
      num_out[i] = -1; // 结尾标志
      num = 0;
    }
  }
  for (int j = 0; num_out[j - 1] != -1; j++)
  {
    if (num_out[j] == 11 || num_out[j] == 45)
    {
      output[j] = 'i';
    }
    else if (num_out[j] != -1)
    {
      char *temp = TokenTypeString[num_out[j]];
      output[j] = temp[0];
    }
    else
      output[j] = '\0';
  }
  return output;
}

bool ll1_parser(char *input_string)
{
  char stack[MAX_LENGTH] = {'$', 'S', '\0'};
  char input[MAX_LENGTH];
  memset(input, '\0', strlen(input));
  strncpy(input, input_string, MAX_LENGTH - 1);
  input[MAX_LENGTH - 1] = '\0';
  strcat(input, "$");

  int pointer = 0, topIndex = 1;

  while (stack[topIndex] != '\0')
  {
    char top = stack[topIndex];
    char current_input = input[pointer];
    int row = getrow(top);
    int col = getcol(current_input);

    if (top == current_input && top == '$')
    {
      printf("分析成功\n");
      return true;
    }

    if (isNonterminal(top) && row != -1 && col != -1 && LL1_TABLE[row][col][0] != '\0')
    {
      char *production = LL1_TABLE[row][col];
      // printf("%s\n", production);
      stack[topIndex] = '\0';
      topIndex--;

      if (production[0] != 'e')
      {
        if (isNonterminal(production[0])) // 非终止符号入栈
        {
          for (int i = (int)strlen(production) - 1; i >= 0; i--)
          {
            topIndex++;
            stack[topIndex] = production[i];
            stack[topIndex + 1] = '\0';
          }
        }
        else
        {
          printf("%c\n", input[pointer]);
          pointer++;
          for (int i = (int)strlen(production) - 1; i > 0; i--)
          {
            topIndex++;
            stack[topIndex] = production[i];
            stack[topIndex + 1] = '\0';
          }
        }
      }
    }
    else if (top == current_input)
    {
      printf("%c\n", top);
      stack[topIndex] = '\0';
      topIndex--;
      pointer++;
      // printf("%s\n", LL1_TABLE[row][col]);
    }
    else if (LL1_TABLE[row][col][0] == '\0')
    {
      printf("输入串出错\n");
      return false;
    }
  }
  printf("分析失败\n");
  return false;
}
int main()
{
  // 生成first集、follow集、select集、预测分析表
  InsertFirst();
  printf("First集\n");
  GetFollow();
  printf("Follow集\n");
  Getselect_table();
  printf("Select集\n");
  get_ll1();
  printf("预测分析表\n");
  Print_ll1();
  system("pause");
}
/*
int main()
{
  // 测试用例
  FILE *fp, *fp_wrong;
  fp = fopen("input1.txt", "r");
  fp_wrong = fopen("input2.txt", "r");
  if (fp == NULL || fp_wrong == NULL)
  {
    printf("error: cannot open file\n");
    system("pause");
    exit(1);
  }
  char *test1 = change_format(fp);
  printf("正确测试用例: %s\n", test1);
  ll1_parser(test1);

  // 错误测试用例
  char *test2 = change_format(fp_wrong);
  printf("\n错误测试用例1: %s\n", test2);
  ll1_parser(test2);
  system("pause");
  return 0;
}
*/
