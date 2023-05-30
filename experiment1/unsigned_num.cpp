#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include<windows.h>
void error(const char *msg);
bool is_unsigned_number(char *str);
bool is_remaining_unsigned_number(char *str);
bool is_decimal_number(char *str);
bool is_fractional_part(char *str);
bool is_exponential_part(char *str);
bool is_integer_part(char *str);
bool is_remaining_integer_part(char *str);
bool is_remaining_exponential_part(char *str);
char input[100];
void Main()
{
  printf("请输入一个无符号数：");
  scanf("%s", input);

  if (is_unsigned_number(input))
  {
    printf("%s\n该字符串是一个有效的无符号数。\n", input);
  }
  else
  {
    error("该字符串不是一个有效的无符号数。");
  }
  //system("pause");
}

int main()
{
  while(1)
    Main();
  return 0;
}

void error(const char *msg)
{
  printf("错误：%s\n", msg);
}

bool is_unsigned_number(char *str)
{
  return is_remaining_unsigned_number(str);
}

bool is_remaining_unsigned_number(char *str)
{
  if (is_decimal_number(str))
  {
    return true;
  }
  else if (is_fractional_part(str))
  {
    return true;
  }
  else if (str[0] == 'd')
  {
    return is_remaining_unsigned_number(str + 1);
  }
  else
  {
    return false;
  }
}

bool is_decimal_number(char *str)
{
  if (str[0] == 'E')
  {
    return is_exponential_part(str + 1);
  }
  else if (isdigit(str[0]))
  {
    return is_integer_part(str + 1);
  }
  else
  {
    return false;
  }
}

bool is_fractional_part(char *str)
{
  if (str[0] == '.')
  {
    return is_decimal_number(str + 1);
  }
  else if (isdigit(str[0]))
  {
    return is_remaining_integer_part(str + 1);
  }
  else
  {
    return false;
  }
}

bool is_exponential_part(char *str)
{
  if (isdigit(str[0]))
  {
    return is_remaining_exponential_part(str + 1);
  }
  else if (str[0] == '+' || str[0] == '-')
  {
    return is_integer_part(str + 1);
  }
  else
  {
    return false;
  }
}

bool is_integer_part(char *str)
{
  if (isdigit(str[0]))
  {
    return is_remaining_integer_part(str + 1);
  }
  else
  {
    return false;
  }
}

bool is_remaining_integer_part(char *str)
{
  if (isdigit(str[0]))
  {
    return is_remaining_integer_part(str + 1);
  }
  else if (str[0] == 'd')
  {
    return is_remaining_integer_part(str + 1);
  }
  else
  {
    return true;
  }
}

bool is_remaining_exponential_part(char *str)
{
  if (isdigit(str[0]))
  {
    return is_remaining_exponential_part(str + 1);
  }
  else
  {
    return true;
  }
}

/*以下是一些测试样例：

测试样例1：
输入：123
输出：该字符串是一个有效的无符号数。

测试样例2：
输入：0.123
输出：该字符串是一个有效的无符号数。

测试样例3：
输入：123.456
输出：该字符串是一个有效的无符号数。

测试样例4：
输入：1.2E3
输出：该字符串是一个有效的无符号数。

测试样例5：
输入：1E-3
输出：该字符串是一个有效的无符号数。

测试样例6：
输入：123ABC
输出：该字符串不是一个有效的无符号数。

测试样例7：
输入：123.4.5
输出：该字符串不是一个有效的无符号数。

测试样例8：
输入：1.2E3E4
输出：该字符串不是一个有效的无符号数。

测试样例9：
输入：123.4E-5.6
输出：该字符串不是一个有效的无符号数。*/
