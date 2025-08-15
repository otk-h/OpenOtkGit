#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

// --- macro defination ---
// size
#define HASH_LENGTH 40
#define PATH_LENGTH 64

// branch
#define DEFAULT_BRANCH "master"
#define DEFAULT_HEAD_STR "ref: refs/heads/"

// path
#define GIT_DIR ".OpenOtkGit"
#define GIT_OBJECTS_DIR ".OpenOtkGit/objects"
#define GIT_REFS_DIR ".OpenOtkGit/refs"
#define GIT_REFS_HEADS_DIR GIT_REFS_DIR"/heads"
#define GIT_HEAD_PATH ".OpenOtkGit/HEAD"
#define GIT_INDEX_PATH ".OpenOtkGit/index"

// --- assist func ---
// encapsulate
void write_func(const char* path, void* buf, size_t size);
void read_func(const char* path, void* buf, size_t size);

// dir
void clean_working_dir(const char* path);
int get_entry_name(const char* path, const char* dir_path, char* name);

// necessary
int is_Initialized();
void create_object(void* ptr, size_t size, char* hash);
void calculate_sha1(const char* path, char* hash);

#endif
