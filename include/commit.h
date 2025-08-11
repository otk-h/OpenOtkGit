#ifndef __COMMIT_H__
#define __COMMIT_H__

#include "base.h"
#include "ref.h"

typedef struct commit {
    char tree_hash[41];     // point to top tree obj
    char parent_hash[4][41];// point to parent commit obj, TODO: limited parent
    char parent_cnt;        // cnt of parent

    // char author[64];     // TODO
    char message[64];       // TODO: fixed len

    // char* author_name;
    // char* author_email;
    // time_t author_time;
    // int author_timezone;

    // char* committer_name;
    // char* committer_email;
    // time_t committer_time;
    // int committer_timezone;

} commit_t;

int create_commit(const char* message, const char* tree_hash, char* commit_hash);

#endif
