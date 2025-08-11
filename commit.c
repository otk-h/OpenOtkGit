#include "commit.h"

int create_commit(const char* message, const char* tree_hash, char* commit_hash) {
    if (message == NULL || tree_hash == NULL) { return 0; }

    commit_t* commit = malloc(sizeof(commit_t));
    memset(commit, 0, sizeof(commit_t));
    memcpy(commit->tree_hash, tree_hash, strlen(tree_hash));
    memcpy(commit->message, message, strlen(message));
    
    // TODO
    // get parent commit
    // set parent
    // set parent cnt

    if (create_object((void*)commit, sizeof(commit_t), commit_hash) == 0) {
        free(commit);
        return 0;
    }
    free(commit);

    // update HEAD

    return 1;

}
