#include <stdio.h>
#include "compiler.h"

struct compile_process *compile_process_creat(const char *filename, const char *filename_out, int flag)
{
  FILE *file = fopen(filename, "r"); // ֻ����file,ֻд��outfile(��������ļ���)
  if (!file)
  {
    return NULL;
  }
  FILE *out_file = NULL;
  if (filename_out)
  {
    out_file = fopen(filename_out, "w");
    if (!out_file)
    {
      return NULL;
    }
  }
  struct compile_process* process = calloc(1,sizeof(struct compile_process));
}
