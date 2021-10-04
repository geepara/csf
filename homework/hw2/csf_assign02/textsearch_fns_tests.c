#include <stdio.h>
#include <stdlib.h>
#include <string.h> // these functions may be used in the test program
#include "textsearch_fns.h"
#include "tctest.h"

typedef struct
{
  const char *pandp;
  const char *yes3times;
  const char *a5times;
  const char *nullTerminators;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

// TODO: declare test functions

void test_read_line(TestObjs *objs);
void test_count_occurences(TestObjs *objs);
void test_find_string_length(TestObjs *objs);
// void test_starts_with(TestObjs *objs);
// void test_strings_equal(TestObjs *objs);

// Example:
// void test_read_line(TestObjs *objs);

int main(int argc, char **argv)
{
  // Allow the name of the test function to execute to be specified
  // on the command line
  if (argc > 1)
  {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_read_line);
  TEST(test_count_occurences);
  TEST(test_find_string_length);
  // TEST(test_starts_with);
  // TEST(test_strings_equal);

  // TODO: invoke test functions
  // TEST(test_read_line);

  TEST_FINI();

  return 0;
}

TestObjs *setup(void)
{
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->pandp =
      "It is a truth universally acknowledged, that a single man in\n"
      "possession of a good fortune, must be in want of a wife.\n"
      "\n"
      "However little known the feelings or views of such a man may be\n"
      "on his first entering a neighbourhood, this truth is so well\n"
      "fixed in the minds of the surrounding families, that he is\n"
      "considered as the rightful property of some one or other of their\n"
      "daughters.\n";
  objs->yes3times = "yesyesyes";
  objs->a5times = "aaaaa";
  char terms[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  objs->nullTerminators = (char *)terms;

  return objs;
}

void cleanup(TestObjs *objs)
{
  free(objs);
}

// An example test function
void test_read_line(TestObjs *objs)
{
  // the fmemopen function allows us to treat a character string
  // as an input file
  FILE *in = fmemopen((char *)objs->pandp, strlen(objs->pandp), "r");
  char buf[MAXLINE + 1];

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "It is a truth universally acknowledged, that a single man in"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "possession of a good fortune, must be in want of a wife."));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, ""));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "However little known the feelings or views of such a man may be"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "on his first entering a neighbourhood, this truth is so well"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "fixed in the minds of the surrounding families, that he is"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "considered as the rightful property of some one or other of their"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "daughters."));

  // at this point we have read the last line
  ASSERT(!read_line(in, buf));

  fclose(in);
}

// TODO: implementations of other test functions

void test_count_occurences(TestObjs *objs)
{
  ASSERT(3 == count_occurrences(objs->yes3times, "yes"));

  ASSERT(0 == count_occurrences(objs->yes3times, "no"));

  ASSERT(3 == count_occurrences(objs->a5times, "aaa"));

  ASSERT(2 == count_occurrences(objs->pandp, "truth"));

  ASSERT(8 == count_occurrences(objs->pandp, "in"));

  ASSERT(1 == count_occurrences(objs->pandp, "daughters."));

  ASSERT(1 == count_occurrences(objs->pandp, "it"));

  ASSERT(0 == count_occurrences(objs->pandp, "dsahpsfuihaepai"));

  ASSERT(0 == count_occurrences(objs->pandp, ""));

  ASSERT(0 == count_occurrences(NULL, "yes"));

  ASSERT(0 == count_occurrences(objs->yes3times, NULL));
}

void test_find_string_length(TestObjs *objs)
{
  ASSERT(9 == find_string_length(objs->yes3times));

  ASSERT(5 == find_string_length(objs->a5times));

  ASSERT(0 == find_string_length(objs->nullTerminators));

  ASSERT(0 == find_string_length(NULL));
}

// void test_starts_with(TestObjs *objs)
// {
//   ASSERT(1 == starts_with(objs->yes3times, "yes"));

//   ASSERT(1 == starts_with(objs->a5times, "aaa"));

//   ASSERT(1 == starts_with(objs->pandp, "It"));

//   ASSERT(1 == starts_with(objs->yes3times, "yesyesyes"));

//   ASSERT(0 == starts_with(objs->yes3times, "no"));

//   ASSERT(1 == starts_with(objs->pandp,
//                           "It is a truth universally acknowledged, that a single man in\n"
//                           "possession of a good fortune, must be in want of a wife.\n"
//                           "\n"
//                           "However little known the feelings or views of such a man may be\n"
//                           "on his first entering a neighbourhood, this truth is so well\n"
//                           "fixed in the minds of the surrounding families, that he is\n"
//                           "considered as the rightful property of some one or other of their\n"));

//   ASSERT(0 == starts_with(objs->nullTerminators, "\0"));

//   ASSERT(0 == starts_with(objs->a5times, "AAA"));

//   ASSERT(0 == starts_with(objs->pandp, ""));

//   ASSERT(0 == starts_with(NULL, "yes"));

//   ASSERT(0 == starts_with(objs->yes3times, NULL));
// }

// void test_strings_equal(TestObjs *objs)
// {
//   ASSERT(1 == strings_equal(objs->yes3times, "yesyesyes"));

//   ASSERT(0 == strings_equal(objs->yes3times, "nonono"));

//   ASSERT(0 == strings_equal(objs->yes3times, "nesyesyes"));

//   ASSERT(0 == strings_equal(objs->yes3times, "yesyesyen"));

//   ASSERT(1 == strings_equal(objs->a5times, "aaaaa"));

//   ASSERT(0 == strings_equal(objs->a5times, "AAAAA"));

//   ASSERT(0 == strings_equal(objs->a5times, "aaaa"));

//   ASSERT(0 == strings_equal(NULL, "yes"));

//   ASSERT(0 == strings_equal(objs->yes3times, NULL));
// }