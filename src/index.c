#include "index.h"

void get_index(index_t** index) {
    if (index == NULL) { exit(1); }
    if (*index != NULL) { free(*index); }
    *index = malloc(sizeof(index_hdr_t));

    read_func(GIT_INDEX_PATH, *index, sizeof(index_hdr_t));
    if ((*index)->ihdr.magic != INDEX_MAGIC) {
        printf("inValid index magic.\n");
        exit(1);
    }

    size_t entries_size = sizeof(index_entry_t) * (*index)->ihdr.entry_cnt;
    index_t* new_index = realloc(*index, sizeof(index_hdr_t) + entries_size);
    if (new_index == NULL) {
        printf("realloc index memory fail.\n");
        exit(1);
    }

    *index = new_index;
    read_func(GIT_INDEX_PATH, *index, sizeof(index_hdr_t) + entries_size);

    // // DEBUG
    // for (int i = 0; i < index->ihdr.entry_cnt; i++) {
    //     printf("%s: %s\n", index->entry[i].path, index->entry[i].hash);
    // }
    
}

void update_index(index_t* index) {
    if (index == NULL) {
        printf("empty index.\n");
        exit(1);
    }

    size_t index_sz = sizeof(index_hdr_t) + index->ihdr.entry_cnt * sizeof(index_entry_t);
    write_func(GIT_INDEX_PATH, index, index_sz);

}

void add_entry_to_index(const char* path, const char* hash, size_t file_sz, index_t** index) {
    if (path == NULL || hash == NULL || index == NULL) { exit(1); }
    // struct stat st;
    // if (stat(path, &st) == -1) { exit(1); }
    
    size_t new_index_size = sizeof(index_hdr_t) + ((*index)->ihdr.entry_cnt + 1) * sizeof(index_entry_t);
    index_t* new_index = realloc(*index, new_index_size);
    if (new_index == NULL) { exit(1); }

    *index = new_index;
    int idx = (*index)->ihdr.entry_cnt;
    memset(&(*index)->entry[idx], 0, sizeof(index_entry_t));
    (*index)->entry[idx].file_sz = file_sz;
    memcpy((*index)->entry[idx].path, path, strlen(path));
    memcpy((*index)->entry[idx].hash, hash, strlen(hash));

    // // DEBUG
    // printf("add: %s\n", idnex->entry[idx].path);

    (*index)->ihdr.entry_cnt += 1;

}
