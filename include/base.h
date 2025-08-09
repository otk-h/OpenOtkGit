#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define GIT_DIR ".OpenOtkGit"
#define GIT_OBJECTS_DIR ".OpenOtkGit/objects"
// #define GIT_REFS_DIR ".OpenOtkGit/refs"
#define GIT_HEAD_PATH ".OpenOtkGit/HEAD"
#define GIT_INDEX_PATH ".OpenOtkGit/index"
// #define GIT_CONFIG_PATH ".OpenOtkGit/config"

#define HASH_LENGTH 40

void calculate_sha1(const char* path, char* hash);
int is_Initialized();

#endif