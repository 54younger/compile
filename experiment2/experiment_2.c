#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 100

const char *TokenTypeString[] =
    {
        // �ؼ���0-11
        "void", "int", "float", "double", "char", "if", "else", "for", "do", "while", "return", "ID",
        // ���ַ��ֽ��12-27
        "+", "-", "*", "/", "=", "<", ">", "!", "(", ")", "{", "}", "[", "]", ";", "\n",
        // ˫�ַ��ֽ��28-44
        "==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "()", "[]", "{}", "//", "/*"
        // ����45
        ,"num"
};

// ��ǰ��ȡ�Ķ�Ԫʽ
int current_token_type;
char ch;
// ��ȡ��һ����Ԫʽ
void read_token(FILE *fp)
{
  int num = 0;
  while ((ch = fgetc(fp)) != EOF)
  {
    if (isspace(ch))
    {
      continue; // ���Կո��Ʊ�������з���
    }
    if (ch == '(')
    {
      ch = fgetc(fp);
      num = ch - '0';
      if ((ch = fgetc(fp)) != ',')
      {
        num *= 10;
        num += ch - '0';
        ch = fgetc(fp);
      }
      ch = fgetc(fp);
      while ((ch = fgetc(fp)) != ')')
      {
      }
      current_token_type = num;
      num = 0;
      break;
    }
  }
}
// ƥ���ս��
void match(int type, FILE *fp)
{

    if (current_token_type == type || (type == 0 && (current_token_type < 12)||current_token_type==45))
    {
      printf("matched %s\n", TokenTypeString[current_token_type]);
      read_token(fp);
    }
    else
    {
      printf("error: unexpected token %s\n", TokenTypeString[current_token_type]);
      system("pause");
      exit(1);
    }
}
// ��ǰ����
void S(FILE *fp);
void E(FILE *fp);
void Eprime(FILE *fp);
void T(FILE *fp);
void Tprime(FILE *fp);
void F(FILE *fp);
void A(FILE *fp);
void M(FILE *fp);
void V(FILE *fp);

// s ->V=E
void S(FILE *fp)
{
  V(fp);
  match(16, fp); // =
  E(fp);
}
// E �� TE'
void E(FILE *fp)
{
  T(fp);
  Eprime(fp);
}
// E' �� ATE' | ��
void Eprime(FILE *fp)
{
  if (current_token_type == 12 || current_token_type == 13)
  {
    A(fp);
    T(fp);
    Eprime(fp);
  }
}
// T �� FT'
void T(FILE *fp)
{
  F(fp);
  Tprime(fp);
}
// T' �� MFT' | ��
void Tprime(FILE *fp)
{
  if (current_token_type == 14 || current_token_type == 15)
  {
    M(fp);
    F(fp);
    Tprime(fp);
  }
}
// F �� (E) | i
void F(FILE *fp)
{
  if (current_token_type == 20)
  { // (
    match(20, fp);
    E(fp);
    match(21, fp); // )
  }
  else
  {
    V(fp);
  }
}
// A �� + | -
void A(FILE *fp)
{
  if (current_token_type == 12)
  {
    match(12, fp); // +
    //printf("matched +\n");
  }
  else if (current_token_type == 13)
  {
    match(13, fp); // -
    //printf("matched -\n");
  }
  else
  {
    printf("error: unexpected token %s\n", TokenTypeString[current_token_type]);
    system("pause");
    exit(1);
  }
}
// M �� * | /
void M(FILE *fp)
{
  if (current_token_type == 14)
  {
    match(14, fp); // *
    //printf("matched *\n");
  }
  else if (current_token_type == 15)
  {
    match(15, fp); // /
    //printf("matched /\n");
  }
  else
  {
    printf("error: unexpected token %s\n", TokenTypeString[current_token_type]);
    system("pause");
    exit(1);
  }
}
// V �� i
void V(FILE *fp)
{
  match(0, fp); // i
  //printf("matched %s\n", TokenTypeString[current_token_type]);
}

void test1()
{
  FILE *fp;
  fp = fopen("input1.txt", "r");
  if (fp == NULL)
  {
    printf("error: cannot open file\n");
    system("pause");
    exit(1);
  }
  read_token(fp);
  S(fp);
  if (ch != -1)
  {
    printf("error: unexpected token %s\n", TokenTypeString[current_token_type]);
  }
  else
  {
    printf("success: expression is valid\n");
  }
  fclose(fp);
  system("pause");
}
void test2()
{
  FILE *fp;
  fp = fopen("input2.txt", "r");
  if (fp == NULL)
  {
    printf("error: cannot open file\n");
    system("pause");
    exit(1);
  }
  read_token(fp);
  S(fp);
  if (ch != -1)
  {
    printf("error: unexpected token %s\n", TokenTypeString[current_token_type]);
  }
  else
  {
    printf("success: expression is valid\n");
  }
  fclose(fp);
  system("pause");
}

int main()
{
  test1();
  test2();
  return 0;
}
