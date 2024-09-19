/* Add any includes here */

#include "hw1.h"
#include <stdio.h>

/* Define any global variables or arrays here */

int g_padovan_array[ARRAY_SIZE] = {};

/*
 * The function compute_padovan_sequence takes the arguments
 * of 3 ints which are defined as the start of the padovan sequence.
 * It also takes the input of a limit, which is described as how many
 * terms the Padovan sequence will calculate. The inputted integers
 * each must be greater than 0, and the limit must be greater than 3.
 */

int compute_padovan_sequence(int init_1, int init_2, int init_3, int limit) {
  if ( (limit < 3) || (limit > ARRAY_SIZE) ) {
    return PADOVAN_ERROR;
  }

  if ( (init_1 <= 0) || (init_2 <= 0) || (init_3 <= 0) ) {
    return PADOVAN_ERROR;
  }
  int temp = 0;
  int a = init_1;
  int b = init_2;
  int c = init_3;

  if (a > b) {
    temp = a;
    a = b;
    b = temp;
  }

  if (a > c) {
    temp = a;
    a = c;
    c = temp;
  }

  if (b > c) {
    temp = b;
    b = c;
    c = temp;
  }

  g_padovan_array[0] = a;
  g_padovan_array[1] = b;
  g_padovan_array[2] = c;

  int i = 0;

  for (i = 3; i < limit; i++) {
    g_padovan_array[i] = g_padovan_array[i - 3] + g_padovan_array[i - 2];
  }

  return PADOVAN_CORRECT;

} /* compute_padovan_sequence() */



/*
 * The function check_padovan_sequence takes the argument of an integer
 * limit and performs checks on the main padovan array. It ensures that
 * the first three integers are not in non-decreasing order,
 * or in other words, it makes sure that the first three ints
 * are increasing. It checks to ensure that the padovan sequence
 * held in g_padovan_array is correct.
 */

int check_padovan_sequence(int limit) {
  if ( (limit < 3) || (limit > ARRAY_SIZE) ) {
    return PADOVAN_ERROR;
  }

  if ( (g_padovan_array[0] <= 0) ||
    (g_padovan_array[1] <= 0) ||
    (g_padovan_array[2] <= 0) ) {
    return PADOVAN_ERROR;
  }

  if ( (g_padovan_array[0] != g_padovan_array[1]) ||
    (g_padovan_array[1] != g_padovan_array[2]) ) {
    if ( (g_padovan_array[0] >= g_padovan_array[1]) &&
      (g_padovan_array[1] >= g_padovan_array[2]) ) {
      return PADOVAN_ERROR;
    }
  }

  int i = 0;
  int check_padovan_array[ARRAY_SIZE] = {};

  check_padovan_array[0] = g_padovan_array[0];
  check_padovan_array[1] = g_padovan_array[1];
  check_padovan_array[2] = g_padovan_array[2];

  for (i = 3; i < limit; i++) {
    check_padovan_array[i] =
      check_padovan_array[i - 3] + check_padovan_array[i - 2];
    if (check_padovan_array[i] == g_padovan_array[i]) {
      continue;
    }
    else {
      return i;
    }
  }

  return PADOVAN_CORRECT;

} /* check_padovan_sequence() */


