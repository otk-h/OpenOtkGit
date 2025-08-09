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

void calculate_sha1(const char* path, char* hash);
int is_Initialized();

#endif