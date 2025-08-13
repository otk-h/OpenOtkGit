#include "ref.h"

int get_cur_branch(char* branch_name) {
    if (branch_name == NULL) { return 0; }

    int fd = -1;
    fd = open(GIT_HEAD_PATH, O_RDONLY);

    char buffer[64];
    size_t bytes_read = read(fd, buffer, sizeof(buffer));
    close(fd);

    memcpy(branch_name, buffer + strlen(DEFAULT_HEAD_STR), bytes_read - strlen(DEFAULT_HEAD_STR));

    return 1;
}

int update_refs(char* hash) {
    if (hash == NULL) { return 0; }

    char branch_name[64];
    char branch_path[128];
    
    if (get_cur_branch(branch_name) == 0) { return 0; }
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);

    int fd = -1;
    fd = open(branch_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, hash, strlen(hash));
    close(fd);

    return 1;

}

int is_detached() {
    int fd = -1;
    fd = open(GIT_HEAD_PATH, O_RDONLY);

    char buffer[64];
    read(fd, buffer, sizeof(buffer));
    close(fd);

    return (strncmp(buffer, DEFAULT_HEAD_STR, strlen(DEFAULT_HEAD_STR)) != 0);
}

int get_commit_hash(char* hash) {
    // if (hash == NULL) { return 0; }

    // char branch_name[64];
    // get_cur_branch(branch_name);

    // if (is_detached()) {

    // } else {
    //     get_cur_branch(cur_branch);

    // }

    // // TODO

    return 1;

}
