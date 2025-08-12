#include "commit.h"

int create_commit(const char* message, const char* tree_hash, char* commit_hash) {
    if (message == NULL || tree_hash == NULL) { return 0; }

    commit_t* commit = malloc(sizeof(commit_t));
    memset(commit, 0, sizeof(commit_t));
    memcpy(commit->tree_hash, tree_hash, strlen(tree_hash));
    memcpy(commit->message, message, strlen(message));

    set_parent_commit(commit);

    if (create_object((void*)commit, sizeof(commit_t), commit_hash) == 0) {
        free(commit);
        return 0;
    }
    free(commit);

    return 1;

}

int set_parent_commit(commit_t* commit) {
    char branch_name[64];
    char branch_path[128];
    char hash[HASH_LENGTH + 1];
    struct stat st;
    
    if (get_cur_branch(branch_name) == 0) { return 0; }
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
    if (stat(branch_path, &st) == -1) { return 1; }

    int fd = -1;
    fd = open(branch_path, O_RDONLY);
    read(fd, hash, sizeof(hash));
    hash[HASH_LENGTH] = '\0';

    memcpy(commit->parent_hash[commit->parent_cnt], hash, strlen(hash));
    commit->parent_cnt += 1;
    
    return 1;

}
