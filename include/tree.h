#ifndef __TREE_H__
#define __TREE_H__

#include "utils.h"
#include "index.h"

#define TREE_MAGIC 0x54524545 // "TREE"

typedef struct tree_hdr {
    uint32_t magic;     // "TREE"
    uint32_t entry_cnt; // cnt of entry
} tree_hdr_t;

typedef struct tree_entry {
    // file or sub_dir
    mode_t mode;
    char name[PATH_LENGTH];     // TODO: fixed len
    char hash[HASH_LENGTH + 1]; // SHA-1 str + '\0'
    
    // mode: oct #include <sys/stat.h>
    //      reg 0100644
    //      exe 0100755
    //      dir 040000
} tree_entry_t;

typedef struct tree {
    tree_hdr_t    thdr;
    tree_entry_t  entry[];
} tree_t;

void create_tree(index_t* index, const char* dir_path, char* hash);
void create_tree_func(index_t* index, const char* dir_path, char* hash, int* is_processed);
void add_entry_to_tree(const char* name, const char* hash, struct stat st, tree_t** tree);
int is_file_in_dir(const char* path, const char* dir_path);
void rebuild_index_from_tree(const char* tree_hash);
void rebuild_index_from_tree_func(index_t** index, const char* tree_hash, const char* dir_path);
void rebuild_working_dir_from_tree(const char* base_path, tree_t* tree);

#endif
