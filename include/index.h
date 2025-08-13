#ifndef __INDEX_H__
#define __INDEX_H__

#include "base.h"
// index = index_hdr + n * entry

#define INDEX_MAGIC 0x44495243 // "DIRC"

typedef struct index_hdr {
    uint32_t magic;     // "DIRC"
    uint32_t entry_cnt; // cnt of entry
} index_hdr_t;

// unsorted
typedef struct index_entry {
    // // time info
    // uint32_t ctime_sec;   // create time
    // uint32_t ctime_nsec;
    // uint32_t mtime_sec;   // modify time
    // uint32_t mtime_nsec;

    // // metadata
    // uint32_t dev;
    // uint32_t ino;
    // uint32_t mode;
    // uint32_t uid;
    // uint32_t gid;

    uint32_t file_sz;
    char hash[41];      // SHA-1 + '\0'
    char path[64];      // TODO: fixed len
    
} index_entry_t;

typedef struct index {
    index_hdr_t     ihdr;
    index_entry_t   entry[];
} index_t;

int get_index(index_t* index);
int add_entry_to_index(const char* path, char* hash);

#endif
