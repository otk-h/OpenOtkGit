#include "OpenOtkGit.h"

static void calculate_sha1(const char* path, char* hash);
static void copy_file(const char* src, const char* dst);

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
    if (mkdir(GIT_DIR, 0755) == -1
        || mkdir(GIT_OBJECTS_DIR, 0755) == -1
        || mkdir(GIT_REFS_DIR, 0755) == -1
    ) {
        perror(NULL);
        exit(1);
    }
    printf("Git_Init: finish\n\n");
}

void Git_Add(const char* path) {
    struct stat st;
    if (stat(GIT_DIR, &st) == -1
        || stat(GIT_OBJECTS_DIR, &st) == -1
        || stat(GIT_REFS_DIR, &st) == -1
    ) {
        perror("no init: ");
        exit(1);
    }
    
    if (stat(path, &st) == -1) {
        perror(NULL);
        exit(1);
    }

    char hash[HASH_LENGTH + 1];
    calculate_sha1(path, hash);

    char blob_path[1024];
    snprintf(blob_path, sizeof(blob_path), "%s/%s", GIT_OBJECTS_DIR, hash);

    copy_file(path, blob_path);

    printf("Git_Add: finish\n\n");
}

void Git_Commit(const char* message) {
    printf("%s\n", message);
    printf("Git_Commit\n");
    // TODO
}

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