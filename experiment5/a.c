#include <stdio.h>
#include <stdlib.h>

#define INT int
#define ___L int
#define ___LL long long

int main()
{
  ___L _ = 1;
  ___L __ = 2;
  ___LL ___ = _ + __;
  printf("%lld\n",___);
  _++;
  ___ = _ + __;
  printf("%lld\n", ___);
  system("pause");
  return 0;
}
