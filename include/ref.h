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

typedef struct merge_config {
    uint8_t ff_only;   // fast-forward only, do not create a merge commit
    uint8_t no_commit; // do not create commit after merge
    uint8_t no_ff;     // do not fast-forward, always create a merge commit
    char*   target_branch;
} merge_config_t;

void get_cur_branch(char* branch_name);
void update_refs(char* hash);
void get_commit_hash(const char* branch, char* hash);
// int is_detached();
int checkout_safe_check(const char* branch_name);
void find_common_base(const char* branch1, const char* branch2, char* base);
void find_all_base(const char* commit, char base[][41], int* cnt);
void three_way_merge(const char* base_tree_hash, const char* branch1_tree_hash, const char* branch2_tree_hash);

#endif
