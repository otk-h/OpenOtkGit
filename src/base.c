#include "base.h"

void calculate_sha1(const char* path, char* hash) {
    char sha1_cmd[1024];
    sprintf(sha1_cmd, "sha1sum %s", path);

    FILE* fd = popen(sha1_cmd, "r");
    if (fd == NULL) {
        perror("calculate_sha1: ");
        exit(1);
    }
    fscanf(fd, "%40s", hash);

    pclose(fd);
}

int is_Initialized() {
    struct stat st;
    if (stat(GIT_DIR, &st) == -1
        || stat(GIT_OBJECTS_DIR, &st) == -1
        || stat(GIT_REFS_DIR, &st) == -1
        || stat(GIT_REFS_HEADS_DIR, &st) == -1
        || stat(GIT_INDEX_PATH, &st) == -1
        || stat(GIT_HEAD_PATH, &st) == -1
    ) {
        return 0;
    }
    return 1;
}

int get_entry_name(const char* path, const char* dir_path, char* name) {
    if (path == NULL || dir_path == NULL || name == NULL) { return 0; }
    size_t dir_len = strlen(dir_path);

    const char* remaining = path + dir_len;
    if (*remaining != '/') { return 0; }

    remaining++;
    const char* end = strchr(remaining, '/');
    size_t name_len = 0;
    if (end == NULL) {
        name_len = strlen(remaining);
    } else {
        name_len = end - remaining;
    }

    strncpy(name, remaining, name_len);
    name[name_len] = '\0';
    return 1;

}

int create_object(void* ptr, size_t size, char* hash) {
    if (ptr == NULL || hash == NULL || size == 0) { return 0; }

    // calculate SHA-1
    char tmp_path[] = "/tmp/git_tmp_XXXXXX";
    int tmp_fd = -1;
    if ((tmp_fd = mkstemp(tmp_path)) == -1) { return 0; }

    write(tmp_fd, ptr, size);
    close(tmp_fd);
    calculate_sha1(tmp_path, hash);
    hash[HASH_LENGTH] = '\0';
    unlink(tmp_path);

    // create object
    int fd = -1;
    char obj_path[64];
    snprintf(obj_path, sizeof(obj_path), "%s/%s", GIT_OBJECTS_DIR, hash);
    if ((fd = open(obj_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
        || write(fd, ptr, size) != size
    ) {
        close(fd);
        return 0;
    }
    close(fd);
    return 1;
}

int clean_working_dir() {
    // TODO
    
    return 0;
}
