#pragma once

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
  /// other choices certainly possible
};
