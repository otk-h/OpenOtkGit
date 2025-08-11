#include "OpenOtkGit.h"

void Git_Version() {
    printf("OpenOtkGit version\n\n");
}

void Git_Help() {
    printf("Usage: OpenOtkGit <command> [<args>]\n");
    printf("Commands:\n");
    printf("    --version | -v          Show Version Info\n");
    printf("    --help    | -h          Show help message\n");
    printf("    --init                  Initialize a new myGit repository\n");
    printf("    --add <file>            Add file to staging area\n");
    printf("    --commit [options]      Commit staged changes\n");
    printf("    --log\n\n");
}

void Git_Init() {
    // clear exist dir
    char rm_cmd[64];
    sprintf(rm_cmd, "rm -rf %s", GIT_DIR);
    system(rm_cmd);

    index_hdr_t ihdr;
    ihdr.magic = INDEX_MAGIC;
    ihdr.entry_cnt = 0;

    head_t head;
    memset(&head, 0, sizeof(head));
    snprintf((char*)&head, sizeof(head), "%s%s", DEFAULT_HEAD_STR, DEFAULT_BRANCH);

    FILE* fd = NULL;

    if (mkdir(GIT_DIR, 0755) == -1
        || mkdir(GIT_OBJECTS_DIR, 0755) == -1
        || mkdir(GIT_REFS_DIR, 0755) == -1
        || mkdir(GIT_REFS_HEADS_DIR, 0755) == -1
        // index
        || (fd = fopen(GIT_INDEX_PATH, "w")) == NULL
        || fwrite(&ihdr, 1, sizeof(ihdr), fd) != sizeof(ihdr)
        || fclose(fd) != 0
        // HEAD
        || (fd = fopen(GIT_HEAD_PATH, "w")) == NULL
        || fwrite(&head, 1, sizeof(head), fd) != sizeof(head)
        || fclose(fd) != 0
    ) {
        perror(NULL);
        exit(1);
    }

    printf("Git_Init: finish\n\n");
}

void Git_Add(const char* path) {
    struct stat st;
    // TODO: add a tracked deleted file, delete from index
    //       or rm <filename>
    if (is_Initialized() == 0
        || strncmp(path, "./", strlen("./")) != 0
        || stat(path, &st) == -1
        || S_ISDIR(st.st_mode)
    ) {
        perror(NULL);
        exit(1);
    }

    char hash[HASH_LENGTH + 1];
    if (create_blob(path, hash)
        && add_entry_to_index(path, hash)
    ) {
        printf("Git_Add: finish\n\n");
    }

}

void Git_Commit(const char* message) {
    struct stat st;
    if (is_Initialized() == 0
        || stat(GIT_INDEX_PATH, &st) == -1
        || st.st_size <= sizeof(index_hdr_t)
    ) {
        perror(NULL);
        exit(1);
    }

    index_t* index = malloc(sizeof(index_t));
    get_index(index);

    char tree_hash[HASH_LENGTH + 1];
    char commit_hash[HASH_LENGTH + 1];
    if (create_tree(index, ".", tree_hash)
        && create_commit(message, tree_hash, commit_hash)
    ) {
        printf("Git_Commit: finish\n\n");
    }

    free(index->entry);
    free(index);

}

void Git_Branch(const char* branch_name) {
    printf("Git_Branch: %s\n\n", branch_name);
    
    // TODO
}

void Git_Checkout(const char* branch_name) {
    printf("Git_Checkout: %s\n\n", branch_name);

    // TODO
}

void Git_Merge() {
    printf("Git_Merge: \n\n");

    // TODO
}

void Git_Rebase() {
    printf("Git_Rebase: \n\n");

    // TODO
}
