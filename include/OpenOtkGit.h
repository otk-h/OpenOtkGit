#ifndef __OPEN_OTK_GIT__
#define __OPEN_OTK_GIT__

#include "index.h"
#include "blob.h"
#include "tree.h"
#include "commit.h"

static struct option global_options[] = {
    {"version", no_argument,        0, 'V'},
    {"help",    no_argument,        0, 'h'},
    {"init",    no_argument,        0, 'i'},
    {"add",     required_argument,  0, 'a'},
    {"commit",  required_argument,  0, 'c'},
    {"branch",  required_argument,  0, 'b'},
    {"checkout",required_argument,  0, 'o'},
    {"merge",   no_argument,        0, 'm'},
    {"rebase",  required_argument,  0, 'r'},
    {0, 0, 0, 0}
};

void Git_Version();
void Git_Help();
void Git_Init();
void Git_Add(const char* path);
void Git_Commit(const char* message);
void Git_Branch(const char* branch_name);
void Git_Checkout(const char* branch_name);
void Git_Merge();
void Git_Rebase();

#endif
