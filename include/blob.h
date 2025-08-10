#ifndef __BLOB_H__
#define __BLOB_H__

#include "base.h"
#include "index.h"
// blob = blob_hdr + content

#define BLOB_MAGIC 0x424C4F42 // "BLOB"

typedef struct blob_hdr {
    uint32_t magic;     // "BLOB"
    uint32_t file_sz;   // file without hdr
} blob_hdr_t;

int create_blob(const char* path, char* hash);

#endif
