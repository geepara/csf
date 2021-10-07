#include <stdio.h>
#include <stdlib.h>
#include "textsearch_fns.h"

int main(int argc, char **argv)
{
  if (argc < 3 || argc > 4)
  {
    fprintf(stderr, "Incorrect number of arguments\n");
    return 1;
  }

  int cArg = 0;
  if (strings_equal(argv[1], "-c") == 1)
  {
    cArg = 1;
  }
  else
  {
    if (argc > 3)
    {
      fprintf(stderr, "Incorrect number of arguments\n");
      return 1;
    }
  }

  char *targetStr = get_target_str(argv, cArg);
  FILE *in = get_file(argv, cArg);
  if (in == NULL)
  {
    fprintf(stderr, "Could not open file\n");
    return 1;
  }

  c_textsearch(in, targetStr, cArg);

  return 0;
}