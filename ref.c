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

int is_hash() {
    // TODO
    int fd = -1;
    fd = open(GIT_HEAD_PATH, O_RDONLY);

    char buffer[64];
    read(fd, buffer, sizeof(buffer));

    close(fd);

    return strncmp(buffer, DEFAULT_HEAD_STR, strlen(DEFAULT_HEAD_STR));
}

// // 更新 HEAD 指向新的 Commit SHA-1
// void update_head_ref(unsigned char *sha1) {
//     int fd = open(".git/HEAD", O_WRONLY | O_TRUNC);
//     if (fd == -1) return;

//     char ref[64];
//     snprintf(ref, sizeof(ref), "ref: refs/heads/main\n"); // 简化：假设分支是 main
//     write(fd, ref, strlen(ref));
//     close(fd);

//     // 更新分支引用文件（如 .git/refs/heads/main）
//     char branch_path[PATH_MAX];
//     snprintf(branch_path, sizeof(branch_path), ".git/refs/heads/main");
//     fd = open(branch_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1) return;

//     char sha1_hex[41];
//     for (int i = 0; i < 20; i++) sprintf(sha1_hex + 2 * i, "%02x", sha1[i]);
//     write(fd, sha1_hex, 40);
//     write(fd, "\n", 1);
//     close(fd);
// }