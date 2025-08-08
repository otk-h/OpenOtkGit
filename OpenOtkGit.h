#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
// #include <fcntl.h>

#define GIT_DIR ".OpenOtkGit"
#define GIT_OBJECTS_DIR ".OpenOtkGit/objects"
#define GIT_REFS_DIR ".OpenOtkGit/refs"
#define GIT_HEAD_PATH ".OpenOtkGit/HEAD"
#define GIT_INDEX_PATH ".OpenOtkGit/index"
#define GIT_CONFIG_PATH ".OpenOtkGit/config"

#define HASH_LENGTH 40

#define INDEX_MAGIC 0x44495243 // "DIRC"
#define BLOB_MAGIC  0x424C4F42 // "BLOB"

static struct option global_options[] = {
    {"version", no_argument,        0, 'V'},
    {"help",    no_argument,        0, 'h'},
    {"init",    no_argument,        0, 'i'},
    {"add",     required_argument,  0, 'a'},
    {"commit",  required_argument,  0, 'c'},
    {"branch",  required_argument,  0, 'b'},
    {"checkout",required_argument,  0, 'o'},
    {"merge",   no_argument,        0, 'm'},
    {0, 0, 0, 0}
};

// ./OpenOtkGit/index = index_hdr + n * blob_hdr

typedef struct INDEX {
    uint32_t magic;     // "DIRC"
    uint32_t entries;   // num of file
} index_hdr;

typedef struct BLOB {
    uint32_t magic;     // "BLOB"
    uint32_t size;      // file size
    char file_path[64];
    char blob_path[64];
} blob_hdr;

void Git_Version();
void Git_Help();
void Git_Init();
void Git_Add(const char* path);
void Git_Commit(const char* message);
void Git_Branch(const char* branch_name);
void Git_Checkout(const char* branch_name);
void Git_Merge();
