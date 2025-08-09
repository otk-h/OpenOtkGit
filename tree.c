#include "tree.h"

int create_tree(index_t* index, const char* dir_path, char* hash) {
    tree_t* tree = malloc(sizeof(tree_hdr_t) + sizeof(tree_entry_t));

    if (hash == NULL || dir_path == NULL || index == NULL || tree == NULL) { return 0; }

    tree->thdr.magic = TREE_MAGIC;
    tree->thdr.entry_cnt = 0;
    
    // TODO: slow
    for (int i = 0; i < index->ihdr.entry_cnt; i++) {
        index_entry_t* entry = &index->entry[i];

        if (is_tree_in_dir(entry->path, dir_path)) {
            struct stat st;
            if (stat(entry->path, &st) == -1) { continue; }
            
            char name[64];
            // TODO: get name
            
            if (0) {// 文件
                add_entry_to_tree(name, entry->hash, st, tree);
            } else if (0) {// 文件夹
                char sub_hash[41] = {0};
                create_tree(index, "", sub_hash);
                add_entry_to_tree(name, sub_hash, st, tree);
            }
        }
    }
    
    // calculate SHA-1
    char tmp_path[] = "/tmp/git_tree_XXXXXX";
    int tmp_fd = mkstemp(tmp_path);
    write(tmp_fd, tree, sizeof(tree));// TODO
    close(tmp_fd);
    calculate_sha1(tmp_path, hash);
    hash[HASH_LENGTH] = '\0';
    unlink(tmp_path);
    
    // create tree
    int fd = -1;
    char obj_path[64];
    snprintf(obj_path, sizeof(obj_path), "%s/%s", GIT_OBJECTS_DIR, hash);
    if ((fd = open(obj_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
        || write(fd, tree, sizeof(tree)) != sizeof(tree)// TODO
    ) {
        close(fd);
        free(tree);
        return 0;
    }
    close(fd);
    free(tree);
    return 1;
}

int is_tree_in_dir(const char* path, const char* dir_path) {
    // TODO

    return 0;
}

int add_entry_to_tree(const char* name, const char* hash, struct stat st, tree_t* tree) {
    // TODO

    return 0;
}
