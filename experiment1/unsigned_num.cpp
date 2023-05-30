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
  printf("������һ���޷�������");
  scanf("%s", input);

  if (is_unsigned_number(input))
  {
    printf("%s\n���ַ�����һ����Ч���޷�������\n", input);
  }
  else
  {
    error("���ַ�������һ����Ч���޷�������");
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
  printf("����%s\n", msg);
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

/*������һЩ����������

��������1��
���룺123
��������ַ�����һ����Ч���޷�������

��������2��
���룺0.123
��������ַ�����һ����Ч���޷�������

��������3��
���룺123.456
��������ַ�����һ����Ч���޷�������

��������4��
���룺1.2E3
��������ַ�����һ����Ч���޷�������

��������5��
���룺1E-3
��������ַ�����һ����Ч���޷�������

��������6��
���룺123ABC
��������ַ�������һ����Ч���޷�������

��������7��
���룺123.4.5
��������ַ�������һ����Ч���޷�������

��������8��
���룺1.2E3E4
��������ַ�������һ����Ч���޷�������

��������9��
���룺123.4E-5.6
��������ַ�������һ����Ч���޷�������*/
