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
    if (argc > 4)
    {
      fprintf(stderr, "Invalid arguments\n");
      return 1;
    }
  }

  FILE *in;
  if (!cArg)
  {
    in = fopen(argv[2], "r");
  }
  else
  {
    in = fopen(argv[3], "r");
  }
  if (in == NULL)
  {
    fprintf(stderr, "Could not open file\n");
    return 1;
  }

  char *targetStr;
  if (!cArg)
  {
    targetStr = argv[1];
  }
  else
  {
    targetStr = argv[2];
  }

  char line[MAXLINE];
  unsigned occurrences = 0;
  while (read_line(in, line))
  {
    unsigned lineOccurrences = count_occurrences((char *)line, targetStr);
    occurrences += lineOccurrences;
    if (lineOccurrences > 0)
    {
      if (!cArg)
      {
        printf("%s\n", (char *)line);
      }
    }
  }
  if (cArg)
  {
    printf("%d occurrence(s)\n", occurrences);
  }

  fclose(in);

  return 0;
}