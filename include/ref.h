#ifndef __REF_H__
#define __REF_H__

#include "utils.h"

typedef struct head {
    union {
        char branch_ref[PATH_LENGTH];
        // e.g. ref: refs/heads/master
        char commit_hash[HASH_LENGTH + 1];
        // Detached HEAD only
    } target;
} head_t;

typedef struct ref_head {
    char hash[HASH_LENGTH + 1];
} ref_head_t;

void get_cur_branch(char* branch_name);
void update_refs(char* hash);
int get_commit_hash(char* hash);
int is_detached();
int checkout_safe_check(const char* branch_name);

#endif
