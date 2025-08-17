#include "utils.h"

// --- encapsulate ---

void write_func(const char* path, void* buf, size_t size) {
    if (path == NULL || buf == NULL || size == 0) { goto error; }

    int fd = -1;
    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { goto error; }

    size_t bytes_written = 0;
    bytes_written = write(fd, buf, size);
    close(fd);

    if (bytes_written != size) { goto error; }
    return;
error:
    perror(NULL);
    exit(1);
}

void read_func(const char* path, void* buf, size_t size) {
    if (path == NULL || buf == NULL || size == 0) { goto error; }

    int fd = -1;
    fd = open(path, O_RDONLY);
    if (fd == -1) { goto error; }

    size_t bytes_read = 0;
    bytes_read = read(fd, buf, size);
    close(fd);
    
    if (bytes_read != size) { goto error; }

    return;
error:
    perror(NULL);
    exit(1);
}

// --- dir ---

void clean_working_dir(const char* path) {
    if (path == NULL) { goto error; }

    DIR* dir = opendir(path);
    if (dir == NULL) { goto error; }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0
            || strcmp(entry->d_name, "..") == 0
            || strcmp(entry->d_name, "OpenOtkGit") == 0
            || strcmp(entry->d_name, ".OpenOtkGit") == 0
        ) { continue; }
        
        char rm_cmd[300];
        snprintf(rm_cmd, sizeof(rm_cmd), "rm -rf %s/%s", path, entry->d_name);
        system(rm_cmd);

    }
    closedir(dir);
    return;
error:
    perror(NULL);
    exit(1);
}

void set_file_mode(const char* path, mode_t mode) {
    int fd = -1;
    fd = open(path, O_RDONLY);
    if (fd == -1) { goto error; }
    if (fchmod(fd, mode & 07777) == -1) { goto error; }
    close(fd);
    return;
error:
    perror(NULL);
    exit(1);
}

int get_entry_name(const char* path, const char* dir_path, char* name) {
    if (path == NULL || dir_path == NULL || name == NULL) { return 0; }
    size_t dir_len = strlen(dir_path);
    
    const char* remaining = path + dir_len;
    if (*remaining != '/') { return 0; }

    remaining++;
    const char* end = strchr(remaining, '/');
    size_t name_len = 0;
    if (end == NULL) {
        name_len = strlen(remaining);
    } else {
        name_len = end - remaining;
    }

    strncpy(name, remaining, name_len);
    name[name_len] = '\0';
    return 1;

}

// --- necessary ---

void calculate_sha1(const char* path, char* hash) {
    if (path == NULL || hash == NULL) { return; }

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

void create_object(void* ptr, size_t size, char* hash) {
    if (ptr == NULL || hash == NULL || size == 0) { goto error; }

    // calculate SHA-1
    char tmp_path[] = "/tmp/git_tmp_XXXXXX";
    int tmp_fd = -1;
    tmp_fd = mkstemp(tmp_path);
    if (tmp_fd == -1) { goto error; }
    write(tmp_fd, ptr, size);
    close(tmp_fd);
    calculate_sha1(tmp_path, hash);
    hash[HASH_LENGTH] = '\0';
    unlink(tmp_path);

    // create object
    char obj_path[64];
    snprintf(obj_path, sizeof(obj_path), "%s/%s", GIT_OBJECTS_DIR, hash);
    write_func(obj_path, ptr, size);
    return;

error:
    perror(NULL);
    exit(1);
}

int is_Initialized() {
    struct stat st;
    if (stat(GIT_DIR, &st) == -1
        || stat(GIT_OBJECTS_DIR, &st) == -1
        || stat(GIT_REFS_DIR, &st) == -1
        || stat(GIT_REFS_HEADS_DIR, &st) == -1
        || stat(GIT_INDEX_PATH, &st) == -1
        || stat(GIT_HEAD_PATH, &st) == -1
    ) {
        return 0;
    }
    return 1;
}
