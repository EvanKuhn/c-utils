#include "hash.h"

//==============================================================================
// Utilities
//==============================================================================

int hash_key_free_count = 0;
int hash_val_free_count = 0;

void _hash_key_free(void* k) { ++hash_key_free_count; }
void _hash_val_free(void* k) { ++hash_val_free_count; }

hash_t* _new_test_hash() {
  hash_t* h = hash_new(NULL, NULL);
  hash_set_str(h, "foo", "FOO");
  hash_set_str(h, "bar", "BAR");
  hash_set_str(h, "xyz", "XYZ");
  return h;
}

//==============================================================================
// hash_t
//==============================================================================

void test__hash_new() {
  hash_t* h = hash_new(NULL, NULL);
  nu_check_not_null(h);
  hash_free(h);
}

void test__hash_free() {
  hash_t* h = hash_new(_hash_key_free, _hash_val_free);
  hash_set_str(h, "foo", "FOO");
  hash_set_str(h, "bar", "BAR");
  hash_free(h);
  nu_check_int_eq(hash_key_free_count, 2);
  nu_check_int_eq(hash_val_free_count, 2);
}

void test__hash_set_get_del() {
  hash_t* h = hash_new(NULL, NULL);
  hash_set(h, "foo", sizeof("foo"), "FOO");
  hash_set(h, "bar", sizeof("bar"), "BAR");
  nu_check_int_eq(hash_size(h), 2);
  nu_check_str_eq(hash_get(h, "foo", sizeof("foo")), "FOO");
  nu_check_str_eq(hash_get(h, "bar", sizeof("bar")), "BAR");
  nu_check_true(hash_del(h, "foo", sizeof("foo")));
  nu_check_int_eq(hash_size(h), 1);
  nu_check_false(hash_del(h, "foo", sizeof("foo")));
  hash_free(h);
}

void test__hash_del() {
  hash_t* h = hash_new(NULL, NULL);
  nu_not_implemented();
  hash_free(h);
}

void test__hash_clear() {
  hash_t* h = hash_new(NULL, NULL);
  hash_set_str(h, "foo", "FOO");
  nu_check_int_eq(hash_size(h), 1);
  nu_check_not_null(hash_get_str(h, "foo"));
  hash_clear(h);
  nu_check_int_eq(hash_size(h), 0);
  nu_check_null(hash_get_str(h, "foo"));
  hash_free(h);
}

void test__hash_size() {
  hash_t* h = hash_new(NULL, NULL);
  nu_check_int_eq(hash_size(h), 0);
  hash_set_str(h, "foo", "FOO");
  nu_check_int_eq(hash_size(h), 1);
  hash_set_str(h, "bar", "BAR");
  nu_check_int_eq(hash_size(h), 2);
  hash_free(h);
}

void test__str_hash_set_get_del() {
  hash_t* h = hash_new(NULL, NULL);
  hash_set_str(h, "foo", "FOO");
  hash_set_str(h, "bar", "BAR");
  nu_check_str_eq(hash_get_str(h, "foo"), "FOO");
  nu_check_str_eq(hash_get_str(h, "bar"), "BAR");
  nu_check_null(hash_get_str(h, "xyz"));
  nu_check_int_eq(hash_size(h), 2);
  nu_check_true(hash_del_str(h, "foo"));
  nu_check_null(hash_get_str(h, "foo"));
  nu_check_int_eq(hash_size(h), 1);
  nu_check_false(hash_del_str(h, "foo"));
  hash_free(h);
}

void test__int_hash_set_get_del() {
  hash_t* h = hash_new(NULL, NULL);
  hash_set_int(h, 1, "one");
  hash_set_int(h, 2, "two");
  hash_set_int(h, -3, "minus three");
  nu_check_str_eq(hash_get_int(h, 1), "one");
  nu_check_str_eq(hash_get_int(h, 2), "two");
  nu_check_str_eq(hash_get_int(h, -3), "minus three");
  nu_check_int_eq(hash_size(h), 3);
  nu_check_true(hash_del_int(h, 1));
  nu_check_null(hash_get_int(h, 1));
  nu_check_int_eq(hash_size(h), 2);
  nu_check_false(hash_del_int(h, 1));
  hash_free(h);
}

void test__hash_sort_str() {
  hash_iter_t iter;
  hash_t* h = hash_new(NULL, NULL);
  hash_set_str(h, "ccc", "C");
  hash_set_str(h, "ddd", "D");
  hash_set_str(h, "aaa", "A");
  hash_set_str(h, "bbb", "B");

  // Check existing order
  iter = hash_iter(h);
  nu_check_str_eq((char*)iter.key, "ccc");
  nu_check_str_eq((char*)iter.val, "C");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "ddd");
  nu_check_str_eq((char*)iter.val, "D");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "aaa");
  nu_check_str_eq((char*)iter.val, "A");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "bbb");
  nu_check_str_eq((char*)iter.val, "B");
  nu_check_false(hash_iter_next(&iter));

  hash_sort_str(h);

  // Check sorted order
  iter = hash_iter(h);
  nu_check_str_eq((char*)iter.key, "aaa");
  nu_check_str_eq((char*)iter.val, "A");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "bbb");
  nu_check_str_eq((char*)iter.val, "B");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "ccc");
  nu_check_str_eq((char*)iter.val, "C");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "ddd");
  nu_check_str_eq((char*)iter.val, "D");
  nu_check_false(hash_iter_next(&iter));

  hash_free(h);
}

void test__hash_sort_int() {
  hash_iter_t iter;
  hash_t* h = hash_new(NULL, NULL);
  hash_set_int(h, 3, "C");
  hash_set_int(h, 4, "D");
  hash_set_int(h, 1, "A");
  hash_set_int(h, 2, "B");

  // Check existing order
  iter = hash_iter(h);
  nu_check_int_eq((int)iter.key, 3);
  nu_check_str_eq((char*)iter.val, "C");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 4);
  nu_check_str_eq((char*)iter.val, "D");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 1);
  nu_check_str_eq((char*)iter.val, "A");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 2);
  nu_check_str_eq((char*)iter.val, "B");
  nu_check_false(hash_iter_next(&iter));

  hash_sort_str(h);

  // Check sorted order
  iter = hash_iter(h);
  nu_check_int_eq((int)iter.key, 1);
  nu_check_str_eq((char*)iter.val, "A");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 2);
  nu_check_str_eq((char*)iter.val, "B");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 3);
  nu_check_str_eq((char*)iter.val, "C");
  nu_check_true(hash_iter_next(&iter));
  nu_check_int_eq((int)iter.key, 4);
  nu_check_str_eq((char*)iter.val, "D");
  nu_check_false(hash_iter_next(&iter));

  hash_free(h);
}

void test_suite__hash() {
  nu_run_test(test__hash_new);
  nu_run_test(test__hash_free);
  nu_run_test(test__hash_set_get_del);
  nu_run_test(test__str_hash_set_get_del);
  nu_run_test(test__int_hash_set_get_del);
  nu_run_test(test__hash_clear);
  nu_run_test(test__hash_sort_str);
  nu_run_test(test__hash_sort_int);
}

//==============================================================================
// hash_iter_t
//==============================================================================

void test__empty_hash_iter() {
  hash_t* h = hash_new(NULL, NULL);
  hash_iter_t iter = hash_iter(h);
  nu_check_null(iter.entry);
  nu_check_null(iter.key);
  nu_check_null(iter.val);
  hash_free(h);
}

void test__hash_iter() {
  hash_t* h = _new_test_hash();
  hash_iter_t iter = hash_iter(h);
  nu_check_not_null(iter.entry);
  nu_check_not_null(iter.key);
  nu_check_not_null(iter.val);
  hash_free(h);
}

void test__hash_iter_init() {
  hash_t* h = _new_test_hash();
  hash_iter_t iter;
  hash_iter_init(&iter, h);
  nu_check_not_null(iter.entry);
  nu_check_not_null(iter.key);
  nu_check_not_null(iter.val);
  hash_free(h);
}

void test__hash_iter_next() {
  hash_t* h = _new_test_hash();
  hash_iter_t iter = hash_iter(h);
  nu_check_str_eq((char*)iter.key, "foo");
  nu_check_str_eq((char*)iter.val, "FOO");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "bar");
  nu_check_str_eq((char*)iter.val, "BAR");
  nu_check_true(hash_iter_next(&iter));
  nu_check_str_eq((char*)iter.key, "xyz");
  nu_check_str_eq((char*)iter.val, "XYZ");
  nu_check_false(hash_iter_next(&iter));
  hash_free(h);
}

void test__hash_iter_delete() {
  hash_t* h = _new_test_hash();
  hash_iter_t iter = hash_iter(h);
  nu_not_implemented(); // TODO
  hash_free(h);
}

void test_suite__hash_iter() {
  nu_run_test(test__empty_hash_iter);
  nu_run_test(test__hash_iter);
  nu_run_test(test__hash_iter_init);
  nu_run_test(test__hash_iter_next);
  nu_run_test(test__hash_iter_delete);
}
