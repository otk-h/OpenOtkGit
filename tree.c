#include "tree.h"

int create_tree(index_t* index, const char* dir_path, char* hash) {
    tree_t* tree = malloc(sizeof(tree_hdr_t) + sizeof(tree_entry_t));

    if (hash == NULL || dir_path == NULL || index == NULL || tree == NULL) { return 0; }

    tree->thdr.magic = TREE_MAGIC;
    tree->thdr.entry_cnt = 0;

    // TODO: slow
    for (int i = 0; i < index->ihdr.entry_cnt; i++) {
        index_entry_t* entry = &index->entry[i];
        struct stat st;
        
        char name[64];
        char path[128];

        if (is_file_in_dir(entry->path, dir_path) == 0
            || stat(entry->path, &st) == -1
            || get_entry_name(entry->path, dir_path, name) == 0
        ) { continue; }

        snprintf(path, sizeof(path), "%s/%s", dir_path, name);
        if (stat(path, &st) == -1) { continue; }
        // TODO: sub_dir的文件会被多次处理，因为多次判断均为is_file_in_dir
        if (S_ISDIR(st.st_mode)) {
            char sub_hash[41] = {0};
            create_tree(index, path, sub_hash);
            add_entry_to_tree(name, sub_hash, st, tree);
        } else if (S_ISREG(st.st_mode)) {
            add_entry_to_tree(name, entry->hash, st, tree);
        }
    }
    
    // // calculate SHA-1
    // char tmp_path[] = "/tmp/git_tree_XXXXXX";
    // int tmp_fd = mkstemp(tmp_path);
    // write(tmp_fd, tree, sizeof(tree));// TODO
    // close(tmp_fd);
    // calculate_sha1(tmp_path, hash);
    // hash[HASH_LENGTH] = '\0';
    // unlink(tmp_path);
    
    // // create tree
    // int fd = -1;
    // char obj_path[64];
    // snprintf(obj_path, sizeof(obj_path), "%s/%s", GIT_OBJECTS_DIR, hash);
    // if ((fd = open(obj_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
    //     || write(fd, tree, sizeof(tree)) != sizeof(tree)// TODO
    // ) {
    //     close(fd);
    //     free(tree);
    //     return 0;
    // }
    // close(fd);
    // free(tree);
    // return 1;
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

int add_entry_to_tree(const char* name, const char* hash, struct stat st, tree_t* tree) {
    if (name == NULL || hash == NULL || tree == NULL) { return 0; }
    
    tree_t* new_tree = realloc(tree, sizeof(tree) + sizeof(tree_entry_t));
    if (new_tree == NULL) { return 0; }

    tree = new_tree;
    int idx = tree->thdr.entry_cnt;
    tree->entry[idx].mode = st.st_mode;
    memcpy(tree->entry[idx].name, name, strlen(name) + 1);
    memcpy(tree->entry[idx].hash, hash, strlen(hash) + 1);
    // printf("%s: %s\n", tree->entry[idx].name, tree->entry[idx].hash);

    tree->thdr.entry_cnt += 1;

    // TODO

    return 0;
}
