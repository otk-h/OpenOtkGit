#include "index.h"

int get_index(index_t* index) {
    int index_fd = -1;
    
    // 孩子们，惊人的 if 是存在的
    if (index == NULL
        || (index_fd = open(GIT_INDEX_PATH, O_RDONLY)) == -1
        || read(index_fd, &(index->ihdr), sizeof(index_hdr_t)) != sizeof(index_hdr_t)
        || index->ihdr.magic != INDEX_MAGIC
        // || (index->entry = malloc(sizeof(index_entry_t) * index->ihdr.entry_cnt)) == NULL
        // || read(index_fd, index->entry, sizeof(index_entry_t) * index->ihdr.entry_cnt) != sizeof(index_entry_t) * index->ihdr.entry_cnt
    ) {
        goto error;
    }

    size_t entries_size = sizeof(index_entry_t) * index->ihdr.entry_cnt;
    index->entry = malloc(sizeof(index_entry_t) * index->ihdr.entry_cnt);
    if (index->entry == NULL
        || read(index_fd, index->entry, entries_size) != entries_size
    ) {
        goto error;
    }

    // // DEBUG
    // for (int i = 0; i < index->ihdr.entry_cnt; i++) {
    //     printf("%s: %s\n", index->entry[i].path, index->entry[i].hash);
    // }

    return 1;

error:
    perror(NULL);
    return 0;
}