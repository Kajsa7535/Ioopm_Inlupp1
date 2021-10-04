#pragma once

/**
 * @file hash_table.h
 * @ Kajsa Odén Uhrenius & William Brunnsberg
 * @date Sep 28 2021
 * @brief Defines the union elem, that can contain arbitrary data types.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

#define int_elem(x) (elem_t) { .int_value=(x) }
#define void_elem(x) (elem_t) { .void_value=(x) }
#define bool_elem(x) (elem_t) { .bool_value=(x) }
#define float_elem(x) (elem_t) { .float_value=(x) }
#define string_elem(x) (elem_t) { .string_value=(x) }
#define char_elem(x) (elem_t) { .char_value=(x) }

typedef union elem elem_t;

typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

union elem
{
  int int_value;
  unsigned int unsigned_int_value;
  bool bool_value;
  float float_value;
  void *void_value;
  char *string_value;
  char char_value;
};
