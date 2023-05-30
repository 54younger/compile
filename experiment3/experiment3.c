#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#define MAX_LENGTH 100

char *TokenTypeString[] =
    {
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

bool isNonterminal(char symbol)
{
  return symbol == 'S' || symbol == 'V' || symbol == 'E' || symbol == 'B' || symbol == 'T' || symbol == 'C' || symbol == 'F' || symbol == 'A' || symbol == 'M';
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
