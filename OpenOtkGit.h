#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
// #include <fcntl.h>

#define GIT_DIR ".OpenOtkGit"
#define GIT_OBJECTS_DIR ".OpenOtkGit/objects"
#define GIT_REFS_DIR ".OpenOtkGit/refs"

#define HASH_LENGTH 40

static struct option global_options[] = {
    {"version", no_argument,        0, 'V'},
    {"help",    no_argument,        0, 'h'},
    {"init",    no_argument,        0, 'i'},
    {"add",     required_argument,  0, 'a'},
    {"commit",  required_argument,  0, 'c'},
    {0, 0, 0, 0}
};

void Git_Version();
void Git_Help();
void Git_Init();
void Git_Add(const char* path);
void Git_Commit(const char* message);
