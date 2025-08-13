#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_BRANCH "master"
#define DEFAULT_HEAD_STR "ref: refs/heads/"

#define GIT_DIR ".OpenOtkGit"
#define GIT_OBJECTS_DIR ".OpenOtkGit/objects"
#define GIT_REFS_DIR ".OpenOtkGit/refs"
#define GIT_REFS_HEADS_DIR GIT_REFS_DIR"/heads"
#define GIT_HEAD_PATH ".OpenOtkGit/HEAD"
#define GIT_INDEX_PATH ".OpenOtkGit/index"
// #define GIT_CONFIG_PATH ".OpenOtkGit/config"

#define HASH_LENGTH 40

void calculate_sha1(const char* path, char* hash);
int is_Initialized();
int get_entry_name(const char* path, const char* dir_path, char* name);
int create_object(void* ptr, size_t size, char* hash);

#endif
