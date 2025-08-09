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