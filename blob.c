#include "blob.h"

int create_blob(const char* path, char* hash) {
    if (hash == NULL) { return 0; }
    
    struct stat st;
    int fd = -1;
    if (stat(path, &st) == -1) { goto error; }

    // generate blob hdr
    blob_hdr_t bhdr;
    bhdr.magic = BLOB_MAGIC;
    bhdr.file_sz = st.st_size;

    // read content
    fd = open(path, O_RDONLY);
    char* content = malloc(st.st_size);
    if (read(fd, content, st.st_size) != st.st_size) {
        free(content);
        goto error;
    }
    close(fd);

    // generate blob
    size_t blob_sz = sizeof(blob_hdr_t) + st.st_size;
    char* blob = malloc(blob_sz);
    memcpy(blob, &bhdr, sizeof(blob_hdr_t));
    memcpy(blob + sizeof(blob_hdr_t), content, st.st_size);
    free(content);

    // calculate SHA-1
    char tmp_path[] = "/tmp/git_blob_XXXXXX";
    int tmp_fd = mkstemp(tmp_path);
    write(tmp_fd, blob, blob_sz);
    close(tmp_fd);
    calculate_sha1(tmp_path, hash);
    hash[HASH_LENGTH] = '\0';
    unlink(tmp_path);

    // create blob
    char obj_path[64];
    snprintf(obj_path, sizeof(obj_path), "%s/%s", GIT_OBJECTS_DIR, hash);
    if ((fd = open(obj_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1
        || write(fd, blob, blob_sz) != blob_sz
    ) {
        close(fd);
        free(blob);
        goto error;
    }
    close(fd);
    free(blob);
    return 1;
    
error:
    perror(NULL);
    return 0;
}

int update_index(const char* path, char* hash) {
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
    memcpy(entry.path, path, sizeof(path));

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
