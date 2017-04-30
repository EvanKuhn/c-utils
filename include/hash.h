#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "uthash.h"

//==============================================================================
// A hashtable implementation based internally on uthash.
//==============================================================================

struct hash_s;
typedef struct hash_s hash_t;

// Function typedef for freeing keys or values
typedef void (*free_func_t)(void*);

// Create and return a new hashtable. Takes optional functions to free the key
// and value.
hash_t* hash_new(free_func_t key_free, free_func_t val_free);

// Free the hash and all internal key-value data
void hash_free(hash_t* this);

// Set the given key-value pair. Will overwrite an existing entry.
void hash_set(hash_t* this, void* key, size_t keysize, void* value);

// Get the value for the given key, or NULL if no such entry exists
void* hash_get(hash_t* this, void* key, size_t keysize);

// Delete the entry with the given key. Returns true if an entry was deleted,
// false if not.
bool hash_del(hash_t* this, void* key, size_t keysize);

// Delete all entries, including their keys and values
void hash_clear(hash_t* this);

// Get the number of entries in the hash table
size_t hash_size(hash_t* this);

// Utilities for string-based keys
void  str_hash_set(hash_t* this, char* key, void* value);
void* str_hash_get(hash_t* this, char* key);
bool  str_hash_del(hash_t* this, char* key);
void  str_hash_sort(hash_t* this);

// Utilities for integer-based keys
void  int_hash_set(hash_t* this, int key, void* value);
void* int_hash_get(hash_t* this, int key);
bool  int_hash_del(hash_t* this, int key);
void  int_hash_sort(hash_t* this);

// Macros for converting between int and void* without producing warnings
#define INT_TO_VOID_PTR(i) (void*)(uintptr_t)(i)
#define VOID_PTR_TO_INT(p) (int)(uintptr_t)(p)

//==============================================================================
// Hash table iterator struct. Use it to iterate over a hash_t struct. Example:
//
//    // Method 1: for-loop
//    hash_iter_t iter;
//    for (iter = hash_iter(hash); hash_iter_has_entry(&iter); hash_iter_next(&iter))
//    {
//      printf("key '%s' has value '%s'\n", (char*)iter->key, (char*)iter->val);
//    }
//
//    // Method 2: do-while loop
//    hash_iter_t iter = hash_iter(hash);
//    do
//    {
//      printf("key '%s' has value '%s'\n", (char*)iter->key, (char*)iter->val);
//    } while (hash_iter_next(&iter);
//
// To check if the iterator is done (ie. no more values are available), you can
// check if iter->entry is null.
//==============================================================================

struct hash_iter_s {
  hash_t* hash;
  void* curr;   // Opaque pointers to current hash entry
  void* next;   // Opaque pointers to next hash entry
  void* key;    // Current key
  void* val;    // Current value
};
typedef struct hash_iter_s hash_iter_t;

// Return an initialized iterator for a hash. Calls hash_iter_init internally.
hash_iter_t hash_iter(hash_t* hash);

// Initialize a hash_iter object to point to the first key-value pair
void hash_iter_init(hash_iter_t* this, hash_t* hash);

// Iterate to the next key-value pair. Returns true if such a pair exists, or
// false if no more entries exist.
bool hash_iter_next(hash_iter_t* this);

// Return true if the iterator is currently pointing to an entry
bool hash_iter_has_entry(hash_iter_t* this);

// Rewind the iterator to the first entry in the hash
void hash_iter_rewind(hash_iter_t* this);

// Delete the current key-value pair.
// - Sets the 'curr', 'key' and 'val' pointers to null.
// - Call hash_iter_next to go to the next entry, if available.
void hash_iter_delete(hash_iter_t* this);
