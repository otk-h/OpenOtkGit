#include "blob.h"

void create_blob(const char* path, char* hash) {
    if (path == NULL || hash == NULL) { 
        printf("error while creating blob: memory empty.\n");
        exit(1);
    }
    
    struct stat st;
    if (stat(path, &st) == -1) {
        printf("error while creating blob: file not exist.\n");
        exit(1);
    }

    // generate blob
    size_t blob_sz = sizeof(blob_hdr_t) + st.st_size;
    blob_t* blob = (blob_t*)malloc(blob_sz);
    blob->bhdr.magic = BLOB_MAGIC;
    blob->bhdr.file_sz = st.st_size;
    read_func(path, blob->content, st.st_size);

    // create object
    create_object((void*)blob, blob_sz, hash);
    free(blob);

}
