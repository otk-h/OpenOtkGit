#include "commit.h"

void create_commit(const char* message, const char* tree_hash, char* commit_hash) {
    if (message == NULL || tree_hash == NULL) {
        printf("error while creating commit: memory empty.\n");
        exit(1);
    }

    commit_t* commit = malloc(sizeof(commit_t));
    memset(commit, 0, sizeof(commit_t));
    memcpy(commit->tree_hash, tree_hash, strlen(tree_hash));
    memcpy(commit->message, message, strlen(message));

    set_parent_commit(commit);

    create_object((void*)commit, sizeof(commit_t), commit_hash);
    free(commit);

}

void set_parent_commit(commit_t* commit) {
    char branch_name[64];
    char branch_path[128];
    struct stat st;
    
    get_cur_branch(branch_name);
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
    if (stat(branch_path, &st) == -1) { return; }

    char hash[HASH_LENGTH + 1];
    memset(hash, 0, sizeof(hash));
    int fd = -1;
    fd = open(branch_path, O_RDONLY);
    read(fd, hash, sizeof(hash));
    close(fd);

    memcpy(commit->parent_hash[commit->parent_cnt], hash, strlen(hash));
    commit->parent_cnt += 1;

}

void get_tree_hash(const char* commit_hash, char* tree_hash) {
    if (commit_hash == NULL || tree_hash == NULL) {
        printf("empty get tree hash.\n");
        exit(1);
    }

    char commit_path[128];
    snprintf(commit_path, sizeof(commit_path), "%s/%s", GIT_OBJECTS_DIR, commit_hash);
    commit_t commit;
    memset(&commit, 0, sizeof(commit));
    read_func(commit_path, &commit, sizeof(commit));

    memcpy(tree_hash, commit.tree_hash, strlen(commit.tree_hash));

}