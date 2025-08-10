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

int add_entry_to_index(const char* path, char* hash) {
    struct stat st;
    int index_fd = -1;
    index_hdr_t ihdr;
    index_entry_t entry;

    // get file_sz, index_hdr
    if (stat(path, &st) == -1
        || (index_fd = open(GIT_INDEX_PATH, O_RDWR)) == -1
        || lseek(index_fd, 0, SEEK_SET) == -1
        || read(index_fd, &ihdr, sizeof(ihdr)) != sizeof(ihdr)
    ) { 
        goto error;
    }

    // set index_hdr, index_entry
    ihdr.entry_cnt += 1;
    memset(&entry, 0, sizeof(entry));
    entry.file_sz = st.st_size;
    memcpy(entry.hash, hash, sizeof(entry.hash));
    memcpy(entry.path, path, strlen(path));

    // write index
    if (lseek(index_fd, 0, SEEK_SET) == -1
        || write(index_fd, &ihdr, sizeof(ihdr)) != sizeof(ihdr)
        || lseek(index_fd, 0, SEEK_END) == -1
        || write(index_fd, &entry, sizeof(entry)) != sizeof(entry)
    ) {
        goto error;
    }
    close(index_fd);
    return 1;
error:
    perror(NULL);
    return 0;
}
