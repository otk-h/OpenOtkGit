#include "OpenOtkGit.h"

static void calculate_sha1(const char* path, char* hash);
static void copy_file(const char* src, const char* dst);
static int is_Initialized();
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
    index_hdr ihdr = { INDEX_MAGIC, 0 };

    if (mkdir(GIT_DIR, 0755) == -1
        || mkdir(GIT_OBJECTS_DIR, 0755) == -1
        || mkdir(GIT_REFS_DIR, 0755) == -1
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
    if (stat(path, &st) == -1
        || !is_Initialized()
    ) {
        perror(NULL);
        exit(1);
    }

    char hash[HASH_LENGTH + 1];
    calculate_sha1(path, hash);

    char blob_path[1024];
    snprintf(blob_path, sizeof(blob_path), "%s/%s", GIT_OBJECTS_DIR, hash);

    if (Git_Add_create_blob(path, blob_path)
        && Git_Add_update_index(path, blob_path)
    ) {
        printf("Git_Add: finish\n\n");
    }

}

void Git_Commit(const char* message) {
    if (!is_Initialized()) {
        perror(NULL);
        exit(1);
    }

    // TODO

    char commit_hash[HASH_LENGTH + 1];
    

}

void Git_Branch(const char* branch_name) {
    printf("Git_Branch: %s\n\n", branch_name);
}

void Git_Checkout(const char* branch_name) {
    printf("Git_Checkout: %s\n\n", branch_name);
}

void Git_Merge() {
    printf("Git_Merge: \n\n");
}

// --- assist func ---

static void calculate_sha1(const char* path, char* hash) {
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

static void copy_file(const char* src, const char* dst) {
    FILE* src_fd = NULL;
    FILE* dst_fd = NULL;
    char buf[4096];
    size_t bytes_read;

    if ((src_fd = fopen(src, "rb")) == NULL) { goto error; }
    if ((dst_fd = fopen(dst, "wb")) == NULL) { goto error; }

    while ((bytes_read = fread(buf, 1, sizeof(buf), src_fd)) > 0) {
        if (fwrite(buf, 1, bytes_read, dst_fd) != bytes_read) {
            goto error;
        }
    }

    fclose(src_fd);
    fclose(dst_fd);
    return;
error:
    perror(NULL);
    if (src_fd != NULL) { fclose(src_fd); }
    if (dst_fd != NULL) { fclose(dst_fd); }
    exit(1);
}

static int is_Initialized() {
    struct stat st;
    if (stat(GIT_DIR, &st) == -1
        || stat(GIT_OBJECTS_DIR, &st) == -1
        || stat(GIT_REFS_DIR, &st) == -1
        || stat(GIT_INDEX_PATH, &st) == -1
        || stat(GIT_HEAD_PATH, &st) == -1
    ) {
        return 0;
    }
    return 1;
}

static int Git_Add_create_blob(const char* file_path, const char* blob_path) {
    struct stat st;
    if (stat(blob_path, &st) != -1) {
        printf("Git_Add: file exist\n\n");
        return 0;
    }

    copy_file(file_path, blob_path);
    return 1;
}

static int Git_Add_update_index(const char* file_path, const char* blob_path) {
    struct stat st;
    if (stat(file_path, &st) == -1) {
        perror(NULL);
        return 0;
    }
    
    uint64_t file_sz = st.st_size;

    index_hdr ihdr = {0};
    blob_hdr bhdr = {0};

    bhdr.magic = BLOB_MAGIC;
    bhdr.size = file_sz;
    memcpy(bhdr.file_path, file_path, strlen(file_path));
    memcpy(bhdr.blob_path, blob_path, strlen(blob_path));
    bhdr.file_path[strlen(file_path)] = '\0';
    bhdr.blob_path[strlen(blob_path)] = '\0';

    FILE* fd = NULL;
    fd = fopen(GIT_INDEX_PATH, "rb+");
    fseek(fd, 0, SEEK_SET);
    fread(&ihdr, sizeof(ihdr), 1, fd);
    ihdr.entries += 1;

    fseek(fd, 0, SEEK_SET);
    fwrite(&ihdr, sizeof(ihdr), 1 , fd);
    fseek(fd, 0, SEEK_END);
    fwrite(&bhdr, sizeof(bhdr), 1, fd);

    fclose(fd);

    return 1;
}
