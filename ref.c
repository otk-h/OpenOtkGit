#include "ref.h"

int is_hash() {
    // TODO
    int fd = -1;
    fd = open(GIT_HEAD_PATH, O_RDONLY);

    char buffer[64];
    read(fd, buffer, sizeof(buffer));

    close(fd);

    return strncmp(buffer, DEFAULT_HEAD_STR, strlen(DEFAULT_HEAD_STR));
}
