#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

void write_inst_string(FILE* output, const char* name, char** args, int num_args) {
  int i;

  fprintf(output, "%s", name);
  for (i = 0; i < num_args; i++) {
    fprintf(output, " %s", args[i]);
  }
  fprintf(output, "\n");
}

void write_inst_hex(FILE *output, uint32_t instruction) {
    fprintf(output, "%08x\n", instruction);
}

int is_valid_label(const char* str) {
    int first;

    if (!str) {
        return 0;
    }

    first = 1;
    while (*str) {
        if (first) {
            if (!isalpha((int) *str) && *str != '_') {
	      return 0;   /* does not start with letter or underscore */
            } else {
                first = 0;
            }
        } else if (!isalnum((int) *str) && *str != '_') {
	         return 0;       /* subsequent characters not alphanumeric */
        }
        str++;
    }
    return first ? 0 : 1;   /* empty string is invalid  */
}

/* Translate the input string into a signed number. The number is then 
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND. 

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take 
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants. 

   You should store the result into the location that OUTPUT points to. The 
   function returns 0 if the conversion proceeded without errors, or -1 if an 
   error occurred.
 */
int translate_num(long int* output, const char* str, long int upper_bound, 
    long int lower_bound) {
    char *stop = NULL;
    long int result;

    if (!str || !output) {
        return -1;
    }

    result = strtol(str, &stop, 0);
    /* strtol() is a function that turns str into a long int. */
    /* the third parameter could be simply seen as number system the str number is in. */
    /* when the third parameter is set to 0, the function automatically seen str as decimal number unless hexidecimal numbers starting with 0x.*/
    /* stop store where the first illegal char appears. */
    /* result = 0 if stop is not a valid number at all. */

    if (result == 0 && strcmp(str, "0") != 0 && strcmp(str, "0x00") != 0 && strcmp(str, "0x0") != 0) return -1;
    /* if the number is invalid: */
    /* 1. result = 0 (if the target number itself is 0) */
    /* 2. within the lower_bound and upper_bound we set */
    /* 3. with no redundancy str (stop == "", *stop = 0)*/
    /* Any condition is unsatisfied means that this is not a valid num. */
    /* Then return -1 */
    /* If valid, return 0 */
    else if (result < lower_bound || result > upper_bound) return -1;
    else if (*stop == 0) {
        *output = result;
        return 0;
    }else return -1;
}

/* Translates the register name to the corresponding register number. Please
   see the MIPS Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char* str) {
    /*support the following registers: $zero, $at, $v0, $a0 - $a3, $t0 - $t3, $s0 - $s3, $sp, and $ra*/
    /* Look up those regs in the Green sheet */
    if (strcmp(str, "$zero") == 0)      return 0;
    /* $zero is the $0 */
    else if (strcmp(str, "$0") == 0)    return 0;
    /* $0 represents itself. */
    else if (strcmp(str, "$at") == 0)   return 1;
    /* $at locates at 1. */
    else if (strcmp(str, "$v0") == 0)   return 2;
    /* $v0 locates at 2. */
    else if (strcmp(str, "$a0") == 0)   return 4;
    /* $a0 locates at 4. */
    else if (strcmp(str, "$a1") == 0)   return 5;
    /* $a1 locates at 5. */
    else if (strcmp(str, "$a2") == 0)   return 6;
    /* $a2 locates at 6. */
    else if (strcmp(str, "$a3") == 0)   return 7;
    /* $a3 locates at 7. */
    else if (strcmp(str, "$t0") == 0)   return 8;
    /* $t0 locates at 8. */
    else if (strcmp(str, "$t1") == 0)   return 9;
    /* $t1 locates at 9. */
    else if (strcmp(str, "$t2") == 0)   return 10;
    /* $t2 locates at 10. */
    else if (strcmp(str, "$t3") == 0)   return 11;
    /* $t3 locates at 11. */
    else if (strcmp(str, "$s0") == 0)   return 16;
    /* $s0 locates at 16. */
    else if (strcmp(str, "$s1") == 0)   return 17;
    /* $s1 locates at 17. */
    else if (strcmp(str, "$s2") == 0)   return 18;
    /* $s2 locates at 18. */
    else if (strcmp(str, "$s3") == 0)   return 19;
    /* $s3 locates at 19. */
    else if (strcmp(str, "$sp") == 0)   return 29;
    /* $sp locates at 29. */
    else if (strcmp(str, "$ra") == 0)   return 31;
    /* $ra locates at 31. */

    else                                return -1;
}
