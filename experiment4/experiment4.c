#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LENGTH 100
#define NUM_OF_NONTERMINA_EXT 8
#define NUM_OF_NONTERMINA 3
#define NUM_OF_TERMINA 8
char *representation[] = {
    "E+T", // E
    "E-T", // E
    "T",   // E
    "T*F", // T
    "T/F", // T
    "F",   // T
    "(E)", // F
    "i"    // F
};
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
char NONTERMINA_NOTE_EXT[] = {'E', 'E', 'E', 'T', 'T', 'T', 'F', 'F'};
char NONTERMINA_NOTE[] = {'E', 'T', 'F'};
char TERMINA_NOTE[] = {'+', '-', '*', '/', '(', ')', 'i', '#'};
int firstvt[NUM_OF_NONTERMINA][NUM_OF_TERMINA] = {0};
int lastvt[NUM_OF_NONTERMINA][NUM_OF_TERMINA] = {0};
int table[NUM_OF_TERMINA][NUM_OF_TERMINA] = {0};
int top;                 // 栈顶指针
char stack[30] = {'\0'}; // 分析栈
char *stack_bottom;      // 指向栈底
char *now;               // 指向当前输入符号

int sum(int *arr, int n)
{
  int sum = 0;
  for (int i = 0; i < n; i++)
  {
    sum += arr[i];
  }
  return sum;
}

int GetNontermina(char c) // 获取非终结符的下标
{
  if (c == 'E')
    return 0;
  else if (c == 'T')
    return 1;
  else if (c == 'F')
    return 2;
  else
    return -1;
}
int GetTermina(char c) // 获取终结符的下标
{
  if (c == '+')
    return 0;
  else if (c == '-')
    return 1;
  else if (c == '*')
    return 2;
  else if (c == '/')
    return 3;
  else if (c == '(')
    return 4;
  else if (c == ')')
    return 5;
  else if (c == 'i')
    return 6;
  else if (c == '#')
    return 7;
  else
    return -1;
}

void GetFirstvt(char nonterminal)
{
  for (int i = 0; i < NUM_OF_NONTERMINA_EXT; i++)
  {
    if (NONTERMINA_NOTE_EXT[i] == nonterminal)
    {
      if (GetTermina(representation[i][0]) > -1) // 第一个是终结符
      {
        firstvt[GetNontermina(nonterminal)][GetTermina(representation[i][0])] = 1;
      }
      else if (GetTermina(representation[i][1]) > -1) // 第二个是终结符
      {
        firstvt[GetNontermina(nonterminal)][GetTermina(representation[i][1])] = 1;
      }
      else
      {
        GetFirstvt(representation[i][0]);
        for (int j = 0; j < NUM_OF_TERMINA; j++)
        {
          if (firstvt[GetNontermina(representation[i][0])][j] == 1)
          {
            firstvt[GetNontermina(nonterminal)][j] = 1;
          }
        }
      }
    }
  }
}
void GetLastvt(char nontermina)
{
  for (int i = 0; i < NUM_OF_NONTERMINA_EXT; i++)
  {
    if (NONTERMINA_NOTE_EXT[i] == nontermina)
    {
      if (GetTermina(representation[i][strlen(representation[i]) - 1]) > -1) // 最后一个是终结符
      {
        lastvt[GetNontermina(nontermina)][GetTermina(representation[i][strlen(representation[i]) - 1])] = 1;
      }
      else if (GetTermina(representation[i][strlen(representation[i]) - 2]) > -1) // 倒数第二个是终结符
      {
        lastvt[GetNontermina(nontermina)][GetTermina(representation[i][strlen(representation[i]) - 2])] = 1;
      }
      else
      {
        GetLastvt(representation[i][strlen(representation[i]) - 1]);
        for (int j = 0; j < NUM_OF_TERMINA; j++)
        {
          if (lastvt[GetNontermina(representation[i][strlen(representation[i]) - 1])][j] == 1)
          {
            lastvt[GetNontermina(nontermina)][j] = 1;
          }
        }
      }
    }
  }
}

void CreateLastvt() // 创建Lastvt集
{
  for (int i = 0; i < NUM_OF_NONTERMINA; i++)
  {
    if (sum(lastvt[i], NUM_OF_TERMINA) == 0)
    {
      GetLastvt(NONTERMINA_NOTE_EXT[i]);
    }
  }
  // print Lastvt
  printf("Lastvt:\n");
  for (int i = 0; i < NUM_OF_NONTERMINA; i++)
  {
    printf("%c| ", NONTERMINA_NOTE[i]);
    for (int j = 0; j < NUM_OF_TERMINA; j++)
    {
      if (lastvt[i][j] == 1)
      {
        printf(" %c |", TERMINA_NOTE[j]);
      }
    }
    printf("\n");
  }
}
void CreateFirstvt() // 创建Firstvt集
{
  for (int i = 0; i < NUM_OF_NONTERMINA; i++)
  {
    if (sum(firstvt[i], NUM_OF_TERMINA) == 0)
    {
      GetFirstvt(NONTERMINA_NOTE_EXT[i]);
    }
  }
  // print Firstvt
  printf("Firstvt:\n");
  for (int i = 0; i < NUM_OF_NONTERMINA; i++)
  {
    printf("%c| ", NONTERMINA_NOTE[i]);
    for (int j = 0; j < NUM_OF_TERMINA; j++)
    {
      if (firstvt[i][j] == 1)
      {
        printf(" %c |", TERMINA_NOTE[j]);
      }
    }
    printf("\n");
  }
}

/*FOR 每个产生式 P->X1 X2 ……Xn
DO  FOR  i:=1  TO   n-1    DO
      IF  X[i]和X[i+1]均为终结符
        THEN   置 X[i]=X[i+1]
      IF  X[i]和X[i+2]均为终结符,X[i+1]为非终结符,i≤n-2,
        THEN   置 X[i]=X[i+2]
      IF  X[i]为终结符, 但X[i+1]为非终结符
      THEN  FOR  FIRSTVT(X[i+1])中的每个a
          DO  置 X[i]<a
      IF  Xi为非终结符, 但X i+1 为终结符
          THEN  FOR  LASTVT(X i )中的每个a
              DO   置 a>X[i+1]
*/
void GetTable()
{
  for (int i = 0; i < NUM_OF_NONTERMINA_EXT; i++)
  {
    for (int j = 0; j < strlen(representation[i]) - 1; j++)
    {
      if (GetTermina(representation[i][j]) > -1 && GetTermina(representation[i][j + 1]) > -1)
      {
        table[GetTermina(representation[i][j])][GetTermina(representation[i][j + 1])] = '=';
      }
      if (GetTermina(representation[i][j]) > -1 && GetTermina(representation[i][j + 2]) > -1 && GetNontermina(representation[i][j + 1]) > -1)
      {
        table[GetTermina(representation[i][j])][GetTermina(representation[i][j + 2])] = '=';
      }
      if (GetTermina(representation[i][j]) > -1 && GetNontermina(representation[i][j + 1]) > -1)
      {
        for (int k = 0; k < NUM_OF_TERMINA - 1; k++)
        {
          if (firstvt[GetNontermina(representation[i][j + 1])][k] == 1)
          {
            table[GetTermina(representation[i][j])][k] = '<';
          }
        }
      }
      if (GetNontermina(representation[i][j]) > -1 && GetTermina(representation[i][j + 1]) > -1)
      {
        for (int k = 0; k < NUM_OF_TERMINA - 1; k++)
        {
          if (lastvt[GetNontermina(representation[i][j])][k] == 1)
          {
            table[k][GetTermina(representation[i][j + 1])] = '>';
          }
        }
      }
    }
  }
  // 对于终结符#，#=#,#<firstvt(E),lastvt(E)>#
  table[GetTermina('#')][GetTermina('#')] = '=';
  for (int i = 0; i < NUM_OF_TERMINA; i++)
  {
    if (firstvt[GetNontermina('E')][i] == 1)
    {
      table[GetTermina('#')][i] = '<';
    }
    if (lastvt[GetNontermina('E')][i] == 1)
    {
      table[i][GetTermina('#')] = '>';
    }
  }
  // print table
  printf("table:\n");
  printf("  |");
  for (int i = 0; i < NUM_OF_TERMINA; i++)
  {
    printf("%c |", TERMINA_NOTE[i]);
  }
  printf("\n");
  for (int i = 0; i < NUM_OF_TERMINA; i++)
  {
    printf("%c |", TERMINA_NOTE[i]);
    for (int j = 0; j < NUM_OF_TERMINA; j++)
    {
      printf("%c |", table[i][j]);
    }
    printf("\n");
  }
}
// 出错判断
bool isOperator(char c) // 判断是否为运算符
{
  if (c == '+' || c == '-' || c == '*' || c == '/')
    return true;
  else
    return false;
}
int whyWrong(char *p, int k, char *psc) // 判断表达式是否合法，并给出非法理由
{
  if (k == 1 && p[k] == '#' && isOperator(*psc))
  {
    printf("\n表达式开头不能为运算符!\n");
    return 0;
  }

  if (isOperator(*psc) && isOperator(*(psc + 1)))
  {
    printf("\n运算符号不能相邻!\n");
    return 0;
  }
  if (*psc == '#' && isOperator(*(psc - 1)))
  {
    printf("\n运算符后面没有操作数!\n");
    return 0;
  }
  return 1;
}

int search_table(int a, int b) // 查表，将<,>,=转换为1,-1,0
{
  if (table[a][b] == '<')
  {
    return -1;
  }
  else if (table[a][b] == '>')
  {
    return 1;
  }
  else if (table[a][b] == '=')
  {
    return 0;
  }
  else
  {
    return 2;
  }
}

char *change_format(FILE *fp) // 将二元式转换为一般形式
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
  output[strlen(output)] = '#';
  return output;
}

// 根据table判断函数输入字符串是否符合文法
int AnalyzeString(char in_c[])
{
  int j;
  char q;
  int flag;
  int n = 1;
  top = 1;
  stack[top] = '#';
  stack[top + 1] = '\0';    // 初始化
  stack_bottom = stack + 1; // 指向栈底
  now = in_c;
  printf("\n");
  while (1)
  {
    if (whyWrong(stack, top, now) == 0)
    {
      printf("\nwrong\n");
      break;
    }
    if (GetTermina(stack[top]) != -1)
      j = top;
    else
      j = top - 1;
    flag = search_table(GetTermina(stack[j]), GetTermina(*now));
    if (flag == 1)
    { // 如果stack[j] > 当前输入字符
      do
      {
        q = stack[j];
        if (GetTermina(stack[j - 1]) != -1)
          j--;
        else
          j = j - 2;
      } while (search_table(GetTermina(stack[j]), GetTermina(q)) != -1);
      printf("当前进行第%d步，当前分析符号为%c，算符优先表中关系为>，进行归约操作，剩余输入串为%s\n", n++, *now, now + 1);
      top = j + 1;
      stack[top] = 'N';
      stack[top + 1] = '\0';
      continue;
    }
    else if (flag == -1)
    {
      printf("当前进行第%d步，当前分析符号为%c，算符优先表中关系为<，进行移进操作，剩余输入串为%s\n", n++, *now, now + 1);
      top++;
      stack[top] = *now;
      stack[top + 1] = '\0';
      now++;
      continue;
    }
    else if (flag == 0)
    {
      if (stack[j] == '#')
      {
        printf("当前进行第%d步，当前分析符号为#，算符优先表中关系为=，进行接受操作\n", n);
        printf("\nsuccess!\n");
        break;
      }
      else
      {
        printf("当前进行第%d步，当前分析符号为%c，算符优先表中关系为=，进行移进操作，剩余输入串为%s\n", n++, *now, now + 1);
        top++;
        stack[top] = *now;
        stack[top + 1] = '\0';
        now++;
        continue;
      }
    }
    else
    {
      printf("当前进行第%d步，当前分析符号为%c，算符优先表中关系为空，进行错误操作，剩余输入串为%s\n", n++, *now, now + 1);
      printf("\nwrong!\n");
      break;
    }
  }
}
int main()
{
  CreateFirstvt();
  CreateLastvt();
  GetTable();
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
  AnalyzeString(test1);
  // 错误测试用例
  char *test2 = change_format(fp_wrong);
  printf("\n错误测试用例1: %s\n", test2);
  AnalyzeString(test2);
  system("pause");
  return 0;
}
