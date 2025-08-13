#ifndef __TREE_H__
#define __TREE_H__

#include "base.h"
#include "index.h"
// tree = tree_hdr + n * tree_entry

#define TREE_MAGIC 0x54524545 // "TREE"

typedef struct tree_hdr {
    uint32_t magic;     // "TREE"
    uint32_t entry_cnt; // cnt of entry
} tree_hdr_t;

typedef struct tree_entry {
    // file or sub_dir
    mode_t mode;
    char name[64];      // TODO: fixed len
    char hash[41];      // SHA-1 str + '\0'
    
    // mode: oct #include <sys/stat.h>
    //      reg 0100644
    //      exe 0100755
    //      dir 040000
} tree_entry_t;

typedef struct tree {
    tree_hdr_t    thdr;
    tree_entry_t  entry[];
} tree_t;

int create_tree(index_t* index, const char* dir_path, char* hash);
int create_tree_func(index_t* index, const char* dir_path, char* hash, int* is_processed);
int is_file_in_dir(const char* path, const char* dir_path);
int add_entry_to_tree(const char* name, const char* hash, struct stat st, tree_t** tree);

#endif
