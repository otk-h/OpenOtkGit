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
    //     || stat(GIT_REFS_DIR, &st) == -1
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
