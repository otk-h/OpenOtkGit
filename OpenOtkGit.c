#include "OpenOtkGit.h"

static void copy_file(const char* src, const char* dst);
static int Git_Add_create_blob(const char* file_path, const char* blob_path);
static int Git_Add_update_index(const char* file_path, const char* blob_path);

// --- main func ---

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

    FILE* fd = NULL;
    index_hdr_t ihdr;
    ihdr.magic = INDEX_MAGIC;
    ihdr.entry_cnt = 0;

    if (mkdir(GIT_DIR, 0755) == -1
        || mkdir(GIT_OBJECTS_DIR, 0755) == -1
    //     || mkdir(GIT_REFS_DIR, 0755) == -1
        || (fd = fopen(GIT_INDEX_PATH, "w")) == NULL
        || fwrite(&ihdr, 1, sizeof(ihdr), fd) != sizeof(fd)
        || fclose(fd) != 0
        || (fd = fopen(GIT_HEAD_PATH, "w")) == NULL
        || fclose(fd) != 0
    ) {
        perror(NULL);
        exit(1);
    }

    printf("Git_Init: finish\n\n");
}

void Git_Add(const char* path) {
    struct stat st;
    // TODO: "./"
    // TODO: if dir, do nothing
    // TODO: add a tracked deleted file, delete from index
    //       or rm <filename>
    if (is_Initialized() == 0
        || stat(path, &st) == -1
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

    char hash[41];
    create_tree(index, ".", hash);
    // TODO

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

// --- assist func ---



static void copy_file(const char* src, const char* dst) {
//     FILE* src_fd = NULL;
//     FILE* dst_fd = NULL;
//     char buf[4096];
//     size_t bytes_read;

//     if ((src_fd = fopen(src, "rb")) == NULL) { goto error; }
//     if ((dst_fd = fopen(dst, "wb")) == NULL) { goto error; }

//     while ((bytes_read = fread(buf, 1, sizeof(buf), src_fd)) > 0) {
//         if (fwrite(buf, 1, bytes_read, dst_fd) != bytes_read) {
//             goto error;
//         }
//     }

//     fclose(src_fd);
//     fclose(dst_fd);
//     return;
// error:
    // perror(NULL);
    // if (src_fd != NULL) { fclose(src_fd); }
    // if (dst_fd != NULL) { fclose(dst_fd); }
    // exit(1);
}

static int Git_Add_create_blob(const char* file_path, const char* blob_path) {
    // struct stat st;
    // if (stat(blob_path, &st) != -1) {
    //     printf("Git_Add: file exist\n\n");
    //     return 0;
    // }

    // copy_file(file_path, blob_path);
    // return 1;
}

static int Git_Add_update_index(const char* file_path, const char* blob_path) {
    
}
