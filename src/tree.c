#include "tree.h"

void create_tree(index_t* index, const char* dir_path, char* hash) {
    if (index == NULL || dir_path == NULL || hash == NULL) { 
        printf("error while creating tree: memory empty.\n");
        exit(1);
    }

    int is_processed[index->ihdr.entry_cnt];
    memset(is_processed, 0, sizeof(is_processed));
    create_tree_func(index, dir_path, hash, is_processed);

}

void create_tree_func(index_t* index, const char* dir_path, char* hash, int* is_processed) {
    tree_t* tree = malloc(sizeof(tree_hdr_t));

    if (hash == NULL || dir_path == NULL || tree == NULL) { 
        printf("error while creating tree: memory empty.\n");
        exit(1);
    }

    tree->thdr.magic = TREE_MAGIC;
    tree->thdr.entry_cnt = 0;

    for (int i = 0; i < index->ihdr.entry_cnt; i++) {
        index_entry_t* entry = &index->entry[i];
        struct stat st;
        
        char name[64];
        char path[128];

        if (is_processed[i] == 1
            || is_file_in_dir(entry->path, dir_path) == 0
            || stat(entry->path, &st) == -1
            || get_entry_name(entry->path, dir_path, name) == 0
        ) { continue; }

        snprintf(path, sizeof(path), "%s/%s", dir_path, name);
        if (stat(path, &st) == -1) { continue; }

        if (S_ISDIR(st.st_mode)) {
            char sub_hash[41] = {0};
            create_tree_func(index, path, sub_hash, is_processed);
            add_entry_to_tree(name, sub_hash, st, &tree);
        } else if (S_ISREG(st.st_mode)) {
            add_entry_to_tree(name, entry->hash, st, &tree);
        }

        is_processed[i] = 1;
    }

    // create object
    size_t tree_size = sizeof(tree_hdr_t) + sizeof(tree_entry_t) * tree->thdr.entry_cnt;
    create_object((void*)tree, tree_size, hash);
    free(tree);

}

int is_file_in_dir(const char* path, const char* dir_path) {
    if (path == NULL || dir_path == NULL) { return 0; }

    size_t dir_len = strlen(dir_path);
    if (dir_len == 0) { return 0; }

    if ((strncmp(path, dir_path, dir_len) == 0)
        && (path[dir_len] == '/')
    ) {
        return 1;
    }

    return 0;
}

int add_entry_to_tree(const char* name, const char* hash, struct stat st, tree_t** tree) {
    if (name == NULL || hash == NULL || tree == NULL) { return 0; }
    
    size_t new_tree_size = sizeof(tree_hdr_t) + ((*tree)->thdr.entry_cnt + 1) * sizeof(tree_entry_t);
    tree_t* new_tree = realloc(*tree, new_tree_size);
    if (new_tree == NULL) { return 0; }

    *tree = new_tree;
    int idx = (*tree)->thdr.entry_cnt;
    memset(&(*tree)->entry[idx], 0, sizeof(tree_entry_t));
    (*tree)->entry[idx].mode = st.st_mode;
    memcpy((*tree)->entry[idx].name, name, strlen(name));
    memcpy((*tree)->entry[idx].hash, hash, strlen(hash));

    // // DEBUG
    // printf("add: %s\n", tree->entry[idx].name);

    (*tree)->thdr.entry_cnt += 1;

    return 0;
}

int reset_index_from_tree(tree_t* tree) {
    if (tree == NULL) { return 0; }
    remove(GIT_INDEX_PATH);

    int fd = -1;

    index_hdr_t ihdr;
    memset(&ihdr, 0, sizeof(ihdr));
    ihdr.magic = INDEX_MAGIC;
    ihdr.entry_cnt = 0;
    write_func(GIT_INDEX_PATH, &ihdr, sizeof(ihdr));
    
    // TODO
    for (int i = 0; i < tree->thdr.entry_cnt; i++) {

    }
    
    close(fd);
    
    return 0;
}

int rebuild_working_dir_from_tree(const char* base_path, tree_t* tree) {
    mkdir(base_path);

    for (int i = 0; i < tree->thdr.entry_cnt; i++) {

    }

}
