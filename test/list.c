/**
 * test-list.c
 *
 * Test list functionality
 *
 *  Created on: Jun 1, 2011
 *      Author: wozniak
 */

#include <stdio.h>
#include <string.h>

#include "src/util/list.h"

int
cmp(void* s1, void* ignored)
{
  if (strlen(s1) == 4)
    return 0;
  return 1;
}

int
main(int argc, char* argv[])
{
  struct list* L;

  puts("PARSE");

  L = list_parse("jkl iop l");
  list_printf("%s", L);
  fflush(stdout);
  list_destroy(L);

  L = list_parse(" jkl iop  l  ");
  list_printf("%s", L);
  list_destroy(L);

  puts("SINGLE INSERT");

  L = list_create();
  char* s1 = "hi";
  list_add(L, s1);
  printf("size: %i\n", L->size);
  list_printf("%s", L);
  list_remove(L, s1);
  printf("size: %i\n", L->size);
  list_printf("%s", L);
  list_destroy(L);

  puts("MULTIPLE INSERT");

  L = list_create();
  char* s2 = "howdy";
  char* s3 = "bye";
  list_add(L, s1);
  list_add(L, s2);
  list_add(L, s3);
  list_printf("%s", L);
  list_remove(L, s2);
  list_printf("%s", L);
  list_remove(L, s1);
  list_printf("%s", L);
  list_remove(L, s3);
  list_printf("%s", L);
  list_free(L);

  puts("POP_WHERE");
  char* s4 = "okay";
  char* s5 = "ok";
  L = list_create();
  list_add(L, s1);
  list_add(L, s2);
  list_add(L, s3);
  list_add(L, s4);
  list_add(L, s5);
  list_printf("%s", L);
  struct list* result = list_pop_where(L, cmp, NULL);
  puts("popped:");
  list_printf("%s", result);
  puts("left:");
  list_printf("%s", L);
  list_free(L);
  list_free(result);

  puts("DONE");

  return 0;
}
