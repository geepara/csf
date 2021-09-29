#include <stdio.h>
#include "textsearch_fns.h"

int read_line(FILE *in, char *buf)
{
  int i = 0;
  int c = 0;
  while (i < MAXLINE && c != EOF)
  {
    c = fgetc(in);
    if (c == (int)'\n')
    {
      break;
    }
    buf[i] = (char)c;
    i++;
  }
  if (c == EOF)
  {
    return 0;
  }
  buf[i] = '\0';
  return 1;
}

unsigned count_occurrences(const char *line, const char *str)
{
  if (line == NULL || str == NULL)
  {
    return 0;
  }
  int lineSize = find_string_length(line);
  int strSize = find_string_length(str);
  unsigned occurences = 0;

  if (strSize == 0)
  {
    return 0;
  }

  for (int i = 0; i < lineSize - strSize + 1; i++)
  {
    int match = 0;
    for (int j = 0, k = i; j < strSize; j++, k++)
    {
      if (str[j] != line[k])
      {
        match = 1;
      }
    }
    if (match == 0)
    {
      occurences++;
    }
  }
  return occurences;
}

unsigned find_string_length(const char *s)
{
  if (s == NULL)
  {
    return 0;
  }
  int i = 0;
  while (s[i] != (int)'\0')
  {
    i++;
  }
  return i;
}

int starts_with(const char *s, const char *pfx)
{
  if (s == NULL || pfx == NULL)
  {
    return 0;
  }
  int size = find_string_length(pfx);

  if (size == 0)
  {
    return 0;
  }

  for (int i = 0; i < size; i++)
  {
    if (pfx[i] != s[i])
    {
      return 0;
    }
    i++;
  }

  return 1;
}

int strings_equal(const char *s1, const char *s2)
{
  if (s1 == NULL || s2 == NULL)
  {
    return 0;
  }
  int size = find_string_length(s1);
  if (size != (int)find_string_length(s2))
  {
    return 0;
  }

  int match = 1;
  for (int i = 0; i < size; i++)
  {
    if (s1[i] != s2[i])
    {
      match = 0;
    }
  }
  return match;
}

FILE *get_file(char **argv, int cArg)
{
  FILE *in;
  if (!cArg)
  {
    in = fopen(argv[2], "r");
  }
  else
  {
    in = fopen(argv[3], "r");
  }
  return in;
}

char *get_target_str(char **argv, int cArg)
{
  if (!cArg)
  {
    return argv[1];
  }
  return argv[2];
}

void c_textsearch(FILE *in, char *targetStr, int cArg)
{
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
}

// TODO: implement functions declared in textsearch_fns.h