#include "nu_unit.h"

//==============================================================================
// hash_t
//==============================================================================

void test__hash_new() { nu_not_implemented(); }
void test__hash_free() { nu_not_implemented(); }
void test__hash_set() { nu_not_implemented(); }
void test__hash_get() { nu_not_implemented(); }
void test__hash_del() { nu_not_implemented(); }
void test__hash_clear() { nu_not_implemented(); }
void test__hash_size() { nu_not_implemented(); }
void test__hash_set_str() { nu_not_implemented(); }
void test__hash_get_str() { nu_not_implemented(); }
void test__hash_del_str() { nu_not_implemented(); }

void test_suite__hash() {
  nu_run_test(test__hash_new,     "hash_new");
  nu_run_test(test__hash_free,    "hash_free");
  nu_run_test(test__hash_set,     "hash_set");
  nu_run_test(test__hash_get,     "hash_get");
  nu_run_test(test__hash_del,     "hash_del");
  nu_run_test(test__hash_clear,   "hash_clear");
  nu_run_test(test__hash_size,    "hash_size");
  nu_run_test(test__hash_set_str, "hash_set_str");
  nu_run_test(test__hash_get_str, "hash_get_str");
  nu_run_test(test__hash_del_str, "hash_del_str");
}

//==============================================================================
// hash_iter_t
//==============================================================================

void test__hash_iter() { nu_not_implemented(); }
void test__hash_iter_init() { nu_not_implemented(); }
void test__hash_iter_next() { nu_not_implemented(); }
void test__hash_iter_delete() { nu_not_implemented(); }
void test__hash_iter_sort() { nu_not_implemented(); }

void test_suite__hash_iter() {
  nu_run_test(test__hash_iter,        "hash_iter");
  nu_run_test(test__hash_iter_init,   "hash_iter_init");
  nu_run_test(test__hash_iter_next,   "hash_iter_next");
  nu_run_test(test__hash_iter_delete, "hash_iter_delete");
  nu_run_test(test__hash_iter_sort,   "hash_iter_sort");
}