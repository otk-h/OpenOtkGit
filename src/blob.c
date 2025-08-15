#include "blob.h"

int create_blob(const char* path, char* hash) {
    if (path == NULL || hash == NULL) { return 0; }
    
    struct stat st;
    if (stat(path, &st) == -1) { goto error; }

    // generate blob
    size_t blob_sz = sizeof(blob_hdr_t) + st.st_size;
    blob_t* blob = (blob_t*)malloc(blob_sz);
    blob->bhdr.magic = BLOB_MAGIC;
    blob->bhdr.file_sz = st.st_size;
    read_func(path, blob->content, st.st_size);

    // create object
    create_object((void*)blob, blob_sz, hash);
    free(blob);
    return 1;

error:
    perror(NULL);
    return 0;
}
