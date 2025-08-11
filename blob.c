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

    // create object
    if (create_object((void*)blob, blob_sz, hash) == 0) {
        free(blob);
        return 0;
    }
    free(blob);
    return 1;

error:
    perror(NULL);
    return 0;
}
