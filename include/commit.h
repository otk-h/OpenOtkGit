#ifndef __COMMIT_H__
#define __COMMIT_H__

#include "utils.h"
#include "ref.h"

typedef struct commit {
    char tree_hash[HASH_LENGTH + 1];     // point to top tree obj
    char parent_hash[4][HASH_LENGTH + 1];// point to parent commit obj, TODO: limited parent
    char parent_cnt;                     // cnt of parent

    // char author[64];                  // TODO: fixed len
    char message[MESSAGE_LENGTH];        // TODO: fixed len

    // char* author_name;
    // char* author_email;
    // time_t author_time;
    // int author_timezone;

    // char* committer_name;
    // char* committer_email;
    // time_t committer_time;
    // int committer_timezone;

} commit_t;

void set_parent_commit(commit_t* commit);
void create_commit(const char* message, const char* tree_hash, char* commit_hash);

#endif
