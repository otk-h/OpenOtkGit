#include "blob.h"

int create_blob(const char* path, char* hash) {
    if (hash == NULL) { return 0; }
    
    struct stat st;
    int fd = -1;
    if (stat(path, &st) == -1) { goto error; }

    // generate blob
    size_t blob_sz = sizeof(blob_hdr_t) + st.st_size;
    blob_t* blob = (blob_t*)malloc(blob_sz);
    blob->bhdr.magic = BLOB_MAGIC;
    blob->bhdr.file_sz = st.st_size;
    fd = open(path, O_RDONLY);
    if (read(fd, blob->content, st.st_size) != st.st_size) { goto error; }
    close(fd);

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
