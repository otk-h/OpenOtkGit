#include "ref.h"
#include "commit.h"
#include "queue.h"

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

// int is_detached() {
//     char buffer[64];
//     read_func(GIT_HEAD_PATH, buffer, sizeof(buffer));

//     return (strncmp(buffer, DEFAULT_HEAD_STR, strlen(DEFAULT_HEAD_STR)) != 0);
// }

void get_commit_hash(const char* branch, char* hash) {
    if (branch == NULL || hash == NULL) { printf("empty get commit hash.\n"); exit(1); }

    // if (is_detached()) {
    //     read_func(GIT_HEAD_PATH, hash, HASH_LENGTH);
    //     return 1;
    // }

    // char branch_name[64];
    // get_cur_branch(branch_name);

    char branch_path[128];
    snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch);
    read_func(branch_path, hash, HASH_LENGTH);
    hash[HASH_LENGTH] = '\0';

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

void find_common_base(const char* branch1_commit, const char* branch2_commit, char* base) {
    if (branch1_commit == NULL || branch2_commit == NULL || base == NULL) { printf("empty find_common_base.\n"); exit(1); }

    // find all base of 2 commit
    char branch1_base[100][41];
    char branch2_base[100][41];
    int branch1_base_cnt = 0;
    int branch2_base_cnt = 0;
    find_all_base(branch1_commit, branch1_base, &branch1_base_cnt);
    find_all_base(branch2_commit, branch2_base, &branch2_base_cnt);

    // find common base
    for (int i = 0; i < branch1_base_cnt; i++) {
        for (int j = 0; j < branch2_base_cnt; j++) {
            if (strcmp(branch1_base[i], branch2_base[j]) == 0) {
                memcpy(base, branch1_base[i], 40);
                return;
            }
        }
    }
    printf("No common base found.\n");

}

void find_all_base(const char* commit, char base[][41], int* cnt) {
    queue_t queue;
    my_init(&queue);
    my_push(&queue, commit);

    while (!my_empty(&queue)) {
        char commit_hash[41];
        char commit_path[128];
        my_top(&queue, commit_hash);
        my_pop(&queue);
        snprintf(commit_path, sizeof(commit_path), "%s/%s", GIT_OBJECTS_DIR, commit_hash);

        memcpy(base[*cnt], commit_hash, 41);
        *cnt += 1;

        commit_t commit;
        memset(&commit, 0, sizeof(commit));
        read_func(commit_path, &commit, sizeof(commit));

        for (int i = 0; i < commit.parent_cnt; i++) {
            my_push(&queue, commit.parent_hash[i]);
        }

    }

}

void three_way_merge(const char* base_tree_hash, const char* branch1_tree_hash, const char* branch2_tree_hash) {

}
