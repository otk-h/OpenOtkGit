#ifndef __TREE_H__
#define __TREE_H__

#include "base.h"
// tree = tree_hdr + n * tree_entry

#define TREE_MAGIC 0x54524545 // "TREE"

typedef struct tree_hdr {
    uint32_t magic;     // "TREE"
    uint32_t tree_sz;
} tree_hdr_t;

typedef struct tree_entry {
    // file or sub_dir
    char mode[6];       // reg 100644
                        // exe 100755
                        // dir 040000
    char name[64];      // TODO: fixed len
    char hash[41];      // SHA-1 str + '\0'
} tree_entry_t;

#endif
