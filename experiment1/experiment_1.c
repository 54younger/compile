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
        // �ؼ���0-11
        "void", "int", "float", "double", "char", "if", "else", "for", "do", "while", "return", "ID",
        // ���ַ��ֽ��12-27
        "+", "-", "*", "/", "=", "<", ">", "!", "(", ")", "{", "}", "[", "]", ";", "\n",
        // ˫�ַ��ֽ��28-44
        "==", "!=", "<=", ">=", "&&", "||", "++", "--", "+=", "-=", "*=", "/=", "()", "[]", "{}", "//", "/*"
        //����45
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
  printf("WRONG!\n�ʷ�����\n");
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

int charge_ID(char token[]) // �ж��Ƿ�Ϊ�ؼ��ֺͱ�����
{
  int i = 0;
  for (; strcmp("ID", TokenTypeString[i]) != 0; i++)
  {
    if (strcmp(token, TokenTypeString[i]) == 0)
      return i;
  }
  return i;
}
int charge_deli(char token[]) // �ж�Ϊ ʲô�ָ���(��ʶ��ո�)
{
  int i;
  if (strlen(token) == 1) // ���ַ�
  {
    i = 12;
    for (; strcmp("\n", TokenTypeString[i]) != 0; i++)
    {
      if (strcmp(token, TokenTypeString[i]) == 0)
        return i;
    }
  }
  else if (strlen(token) == 2) // ˫�ַ�
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
  char token[MAXLENGTH]; // ��ʶ��
  memset(token, '\0', sizeof(token));
  do // ȥ����ͷ�Ŀո�
  {
    ch_now = fgetc(fp);
  } while (ch_now == ' ' || ch_now == '\t' || ch_now == '\n');
  ch_next = fgetc(fp);
  while (!feof(fp))
  {
    position = 0, times = 0;
    if (ch_now == ' ' || ch_now == '\t' || ch_now == '\n') // Ϊ�ո���Ʊ�����з��򲻴���ֱ���������ȡ��һ���ַ�
    {
      fprintf(fw, "%c", ch_now);
    }
    else
    {
      switch (WhatIsIt(ch_now)) // �ж�ÿ������ʲô����
      {
      case Word:
        while (!feof(fp) && (if_neither(ch_next) == if_neither(ch_now))) // �жϵ�ɶʱ����һ����
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
        while (!feof(fp) && (reco_num(ch_next) == reco_num(ch_now))) // �жϵ�ɶʱ����һ����
        {
          token[position++] = ch_now;
          ch_now = ch_next;
          if (!feof(fp))
            ch_next = fgetc(fp);
          continue;
        }
        token[position++] = ch_now;
        if (WhatIsIt(ch_next) == Word) // ���ֲ��ܳ�����ĸ��ֻ�ܸ��ָ���
        {
          WRONG();
        }
        else
        {
          //fprintf(fw, "(%s,NUM,%s)", token, token);
          fprintf(fw, "(45,%s)", token);
        }
        break;
      default: // ch_nextΪ�ָ���
        memset(token_temp, '\0', sizeof(token_temp));
        token[position++] = ch_now;
        token_temp[0] = ch_now;
        if (if_neither(ch_next) && ch_next != ' ' && ch_next != '\n') // �ж��ǵ��ַ�+˫�ַ�����˫�ַ�
        {
          token[position++] = ch_next;
          if (charge_deli(token_temp) == -1) // ɶ������
          {
            WRONG();
          }
          else if (charge_deli(token) != -1) // ˵����˫�ַ�
          {
            ch_now = ch_next;
            if (!feof(fp))
              ch_next = fgetc(fp);
            //fprintf(fw, "(%d,Double_DELI,%s)", charge_deli(token), token);
            fprintf(fw, "(%d,%s)", temp, token);
            if (!strcmp(TokenTypeString[charge_deli(token)], "//")) // �ж��Ƿ�Ϊ//��ע��
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
            else if (!strcmp(TokenTypeString[charge_deli(token)], "/*")) // �ж��Ƿ�Ϊ/*��ע��
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
          else if ((charge_deli(token_temp) != -1) && (charge_deli(token) == -1)) // �ǵ�+˫
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
  FILE *fp = NULL; // �����ļ�
  fp = fopen("TestInput.txt", "r");
  FILE *fw = NULL; // ����ļ�
  fw = fopen("TestOutput.txt", "wb");
  if (fp == NULL || fw == NULL)
  {
    if (fp == NULL)
    {
      printf("WRONG\n�ļ���ʧ��\n");
      system("pause");
      return 0;
    }

    printf("WRONG\n�ļ�дʧ��\n");
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
