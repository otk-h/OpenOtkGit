#ifndef __REF_H__
#define __REF_H__

#include "base.h"

typedef struct head {
    union {
        char branch_ref[64];
        // e.g. ref: refs/heads/master
        char commit_hash[41];
        // Detached HEAD only
    } target;
} head_t;

typedef struct ref_head {
    char hash[41];
} ref_head_t;

int get_cur_branch(char* branch_name);
int update_refs(char* hash);
int is_detached();

#endif
