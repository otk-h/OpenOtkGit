#include "ref.h"

void get_cur_branch(char* branch_name) {
    if (branch_name == NULL) { exit(1); }

    int fd = -1;
    fd = open(GIT_HEAD_PATH, O_RDONLY);

    char buffer[64];
    size_t bytes_read = read(fd, buffer, sizeof(buffer));
    close(fd);

    memcpy(branch_name, buffer + strlen(DEFAULT_HEAD_STR), bytes_read - strlen(DEFAULT_HEAD_STR));

}

void update_refs(char* hash) {
    if (hash == NULL) { exit(1); }

    char branch_name[64];
    char branch_path[128];
    
    get_cur_branch(branch_name);
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);

    write_func(branch_path, hash, strlen(hash));

}

int is_detached() {
    char buffer[64];
    read_func(GIT_HEAD_PATH, buffer, sizeof(buffer));

    return (strncmp(buffer, DEFAULT_HEAD_STR, strlen(DEFAULT_HEAD_STR)) != 0);
}

int get_commit_hash(char* hash) {
    if (hash == NULL) { return 0; }

    int fd = -1;

    if (is_detached()) {
        read_func(GIT_HEAD_PATH, hash, HASH_LENGTH);
        return 1;
    }

    char branch_name[64];
    get_cur_branch(branch_name);
    char branch_path[128];
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
    read_func(branch_path, hash, HASH_LENGTH);
    hash[HASH_LENGTH] = '\0';

    return 1;

}

int checkout_safe_check(const char* branch_name) {
    // TODO: check index commit
    printf("uncommitted file '%s' in index.\n", GIT_DIR);
    return 0;

    // TODO: check working dir untracked
    printf("untracked file '%s' in working dir", GIT_DIR);
    return 0;
    
    // // TODO: check overwritten of untracked file
    // printf("untracked file overwritten.\n");
    // return 0;
    
    return 1;
}
