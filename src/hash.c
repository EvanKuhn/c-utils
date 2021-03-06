#include "hash.h"

//==============================================================================
// Internal hash structures
//==============================================================================

// An entry in the hashtable
typedef struct _entry_s {
    void* key;                // Pointer to key
    void* val;                // Pointer to value
    UT_hash_handle hh;        // For uthash
} _entry_t;

// Hash struct
struct hash_s {
  _entry_t* entries;          // The uthash hashtable
  free_func_t key_free_func;  // Function to free keys
  free_func_t val_free_func;  // Function to free values
};

//==============================================================================
// hash_t
//==============================================================================

// Utility function to lookup an entry
static _entry_t* _hash_find_entry(hash_t* this, void* key, size_t keysize)
{
  _entry_t* entry = NULL;
  HASH_FIND(hh, this->entries, &key, keysize, entry);
  return entry;
}

hash_t* hash_new(free_func_t key_free, free_func_t val_free)
{
  hash_t* h = malloc(sizeof(hash_t));
  h->entries = NULL;
  h->key_free_func = key_free;
  h->val_free_func = val_free;
  return h;
}

void hash_free(hash_t* this)
{
  hash_clear(this);
  free(this);
}

void hash_set(hash_t* this, void* key, size_t keysize, void* value)
{
  // Look for an existing entry
  _entry_t* entry = _hash_find_entry(this, key, keysize);

  // If we already have an entry, free the value and add the new value
  if (entry) {
    if (this->val_free_func)
      this->val_free_func(entry->val);
    entry->val = value;
  }
  // Otherwise, add a new entry
  else {
    entry = malloc(sizeof(_entry_t));
    memset(entry, 0, sizeof(_entry_t));
    entry->key = key;
    entry->val = value;
    HASH_ADD(hh, this->entries, key, keysize, entry);
  }
}

void* hash_get(hash_t* this, void* key, size_t keysize)
{
  _entry_t* entry = _hash_find_entry(this, key, keysize);
  return entry ? entry->val : NULL;
}

bool hash_del(hash_t* this, void* key, size_t keysize)
{
  // Look for an existing entry
  _entry_t* entry = _hash_find_entry(this, key, keysize);

  // If found, remove it and free everything
  if (entry) {
    HASH_DEL(this->entries, entry);
    if (this->key_free_func) this->key_free_func(entry->key);
    if (this->val_free_func) this->val_free_func(entry->val);
    free(entry);
    return true;
  }
  return false;
}

void hash_clear(hash_t* this)
{
  _entry_t *entry, *temp;
  HASH_ITER(hh, this->entries, entry, temp) {
    HASH_DEL(this->entries, entry);
    if (this->key_free_func) this->key_free_func(entry->key);
    if (this->val_free_func) this->val_free_func(entry->val);
    free(entry);
  }
}

size_t hash_size(hash_t* this)
{
  return HASH_COUNT(this->entries);
}

void str_hash_set(hash_t* this, char* key, void* value)
{
  hash_set(this, key, strlen(key), value);
}

void* str_hash_get(hash_t* this, char* key)
{
  return hash_get(this, key, strlen(key));
}

bool str_hash_del(hash_t* this, char* key)
{
  return hash_del(this, key, strlen(key));
}

int _key_cmp_str(void* a, void* b)
{
  char* key_a = ((_entry_t*)a)->key;
  char* key_b = ((_entry_t*)b)->key;
  return strcmp(key_a, key_b);
}

void str_hash_sort(hash_t* this)
{
  HASH_SORT(this->entries, _key_cmp_str);
}

void int_hash_set(hash_t* this, int key, void* value)
{
  hash_set(this, INT_TO_VOID_PTR(key), sizeof(key), value);
}

void* int_hash_get(hash_t* this, int key)
{
  return hash_get(this, INT_TO_VOID_PTR(key), sizeof(key));
}

bool int_hash_del(hash_t* this, int key)
{
  return hash_del(this, INT_TO_VOID_PTR(key), sizeof(key));
}

int _key_cmp_int(void* a, void* b)
{
  char* key_a = ((_entry_t*)a)->key;
  char* key_b = ((_entry_t*)b)->key;
  if (key_a < key_b) return -1;
  if (key_a > key_b) return  1;
  return 0;
}

void int_hash_sort(hash_t* this)
{
  HASH_SORT(this->entries, _key_cmp_int);
}

//==============================================================================
// hash_iter_t
//==============================================================================

hash_iter_t hash_iter(hash_t* hash)
{
  hash_iter_t iter;
  hash_iter_init(&iter, hash);
  return iter;
}

void hash_iter_init(hash_iter_t* this, hash_t* hash)
{
  memset(this, 0, sizeof(hash_iter_t));
  this->hash = hash;
  hash_iter_rewind(this);
}

bool hash_iter_next(hash_iter_t* this)
{
  _entry_t* next = this->next;

  // If there's a next element, update the iterator
  if (next) {
    this->curr = next;
    this->next = next->hh.next;
    this->key  = next->key;
    this->val  = next->val;
    return true;
  }
  // No next element. Reset iterator to empty.
  else {
    this->curr = NULL;
    this->next = NULL;
    this->key  = NULL;
    this->val  = NULL;
    return false;
  }
}

bool hash_iter_has_entry(hash_iter_t* this)
{
  return (this->curr != NULL);
}

void hash_iter_rewind(hash_iter_t* this)
{
  if (this->hash->entries) {
    this->curr = this->hash->entries;
    this->next = this->hash->entries->hh.next;
    this->key  = this->hash->entries->key;
    this->val  = this->hash->entries->val;
  }
}

void hash_iter_delete(hash_iter_t* this)
{
  _entry_t* curr = this->curr;

  if (curr) {
    HASH_DEL(this->hash->entries, curr);
    if (this->hash->key_free_func) this->hash->key_free_func(curr->key);
    if (this->hash->val_free_func) this->hash->val_free_func(curr->val);
    free(curr);
    this->curr = NULL;
    this->key  = NULL;
    this->val  = NULL;
  }
}
