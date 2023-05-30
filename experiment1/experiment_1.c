#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define Word 1
#define Num 2
#define Char 0
#define MAXLENGTH 32
const char *TokenTypeString[] =
    {
        // 关键字0-11
        "void", "int", "float", "double", "char", "if", "else", "for", "do", "while", "return", "ID",
        // 单字符分界符12-27
        "+", "-", "*", "/", "=", "<", ">", "!", "(", ")", "{", "}", "[", "]", ";", "\n",
        // 双字符分界符28-44
        "==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "()", "[]", "{}", "//", "/*"
        //数字45
        };
_Bool reco_word(char token)
{
  if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
    return true;
  return false;
}

_Bool reco_num(char token)
{
  if (token >= '0' && token <= '9')
    return true;
  else
    return false;
}

_Bool if_neither(char token)
{
  if (!(reco_num(token) || reco_word(token)))
    return true;
  else
    return false;
}

void WRONG()
{
  printf("WRONG!\n词法错误\n");
  system("pause");
  exit(0);
}

int WhatIsIt(char token)
{
  if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
    return Word;
  else if (token >= '0' && token <= '9')
    return Num;
  else
    return Char;
}

int charge_ID(char token[]) // 判断是否为关键字和保留字
{
  int i = 0;
  for (; strcmp("ID", TokenTypeString[i]) != 0; i++)
  {
    if (strcmp(token, TokenTypeString[i]) == 0)
      return i;
  }
  return i;
}
int charge_deli(char token[]) // 判断为 什么分隔符(不识别空格)
{
  int i;
  if (strlen(token) == 1) // 单字符
  {
    i = 12;
    for (; strcmp("\n", TokenTypeString[i]) != 0; i++)
    {
      if (strcmp(token, TokenTypeString[i]) == 0)
        return i;
    }
  }
  else if (strlen(token) == 2) // 双字符
  {
    i = 28;
    for (; strcmp("/*", TokenTypeString[i - 1]) != 0; i++)
    {
      if (strcmp(token, TokenTypeString[i]) == 0)
        return i;
    }
  }
  return -1;
}

void lexer(FILE *fp, FILE *fw)
{
  int position = 0, temp = 0, times = 0;
  char ch_next, ch_now;
  char token_temp[2];
  char token[MAXLENGTH]; // 标识符
  memset(token, '\0', sizeof(token));
  do // 去掉开头的空格
  {
    ch_now = fgetc(fp);
  } while (ch_now == ' ' || ch_now == '\t' || ch_now == '\n');
  ch_next = fgetc(fp);
  while (!feof(fp))
  {
    position = 0, times = 0;
    if (ch_now == ' ' || ch_now == '\t' || ch_now == '\n') // 为空格或制表符或换行符则不处理，直接输出，获取下一个字符
    {
      fprintf(fw, "%c", ch_now);
    }
    else
    {
      switch (WhatIsIt(ch_now)) // 判断每个字是什么类型
      {
      case Word:
        while (!feof(fp) && (if_neither(ch_next) == if_neither(ch_now))) // 判断到啥时候是一个词
        {
          if (times > MAXLENGTH)
            WRONG();
          else
            times++;
          token[position++] = ch_now;
          ch_now = ch_next;
          if (!feof(fp))
            ch_next = fgetc(fp);
          continue;
        }
        token[position++] = ch_now;
        temp = charge_ID(token);
        //fprintf(fw, "(%d,%s,%s)", temp, TokenTypeString[temp], token);
        fprintf(fw, "(%d,%s)", temp, token);
        break;
      case Num:
        while (!feof(fp) && (reco_num(ch_next) == reco_num(ch_now))) // 判断到啥时候是一个数
        {
          token[position++] = ch_now;
          ch_now = ch_next;
          if (!feof(fp))
            ch_next = fgetc(fp);
          continue;
        }
        token[position++] = ch_now;
        if (WhatIsIt(ch_next) == Word) // 数字不能出现字母，只能跟分隔符
        {
          WRONG();
        }
        else
        {
          //fprintf(fw, "(%s,NUM,%s)", token, token);
          fprintf(fw, "(45,%s)", token);
        }
        break;
      default: // ch_next为分隔符
        memset(token_temp, '\0', sizeof(token_temp));
        token[position++] = ch_now;
        token_temp[0] = ch_now;
        if (if_neither(ch_next) && ch_next != ' ' && ch_next != '\n') // 判断是单字符+双字符还是双字符
        {
          token[position++] = ch_next;
          if (charge_deli(token_temp) == -1) // 啥都不是
          {
            WRONG();
          }
          else if (charge_deli(token) != -1) // 说明是双字符
          {
            ch_now = ch_next;
            if (!feof(fp))
              ch_next = fgetc(fp);
            //fprintf(fw, "(%d,Double_DELI,%s)", charge_deli(token), token);
            fprintf(fw, "(%d,%s)", temp, token);
            if (!strcmp(TokenTypeString[charge_deli(token)], "//")) // 判断是否为//型注释
            {
              fprintf(fw, "\"ExplanatoryNote:");
              while (ch_next != '\n')
              {
                ch_now = ch_next;
                if (!feof(fp))
                  ch_next = fgetc(fp);
                fprintf(fw, "%c", ch_now);
              }
              fprintf(fw, "\"");
            }
            else if (!strcmp(TokenTypeString[charge_deli(token)], "/*")) // 判断是否为/*型注释
            {
              fprintf(fw, "\"ExplanatoryNote:");
              do
              {
                ch_now = ch_next;
                if (!feof(fp))
                  ch_next = fgetc(fp);
                fprintf(fw, "%c", ch_now);
              } while (!(ch_next == '/' && ch_now == '*'));
              ch_now = ch_next;
              if (!feof(fp))
                ch_next = fgetc(fp);
              fprintf(fw, "%c\"", ch_now);
            }
          }
          else if ((charge_deli(token_temp) != -1) && (charge_deli(token) == -1)) // 是单+双
          {
            //fprintf(fw, "(%d,DELI,%s)", charge_deli(token_temp), token_temp);
            fprintf(fw, "(%d,%s)", charge_deli(token_temp), token_temp);
          }
          else
            WRONG();
        }
        else
          //fprintf(fw, "(%d,DELI,%s)", charge_deli(token), token);
          fprintf(fw, "(%d,%s)", charge_deli(token_temp), token_temp);
        break;
      }
    }
    fflush(fw);
    memset(token, '\0', sizeof(token));
    position = 0;
    ch_now = ch_next;
    if (!feof(fp))
      ch_next = fgetc(fp);
  }
}
int main()
{
  FILE *fp = NULL; // 输入文件
  fp = fopen("TestInput.txt", "r");
  FILE *fw = NULL; // 输出文件
  fw = fopen("TestOutput.txt", "wb");
  if (fp == NULL || fw == NULL)
  {
    if (fp == NULL)
    {
      printf("WRONG\n文件读失败\n");
      system("pause");
      return 0;
    }

    printf("WRONG\n文件写失败\n");
    system("pause");
    return 0;
  }
  lexer(fp, fw);
File_end:
  fclose(fp);
  fclose(fw);
  printf("FINISH\n");
  system("pause");
  return 0;
}
