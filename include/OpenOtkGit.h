#ifndef __OPEN_OTK_GIT__
#define __OPEN_OTK_GIT__

#include <getopt.h>

#include "index.h"
#include "blob.h"
#include "tree.h"
#include "commit.h"

void Git_Version();
void Git_Help(int argc, char* argv[]);
void Git_Init(int argc, char* argv[]);
void Git_Add(int argc, char* argv[]);
void Git_Commit(int argc, char* argv[]);
void Git_Branch(int argc, char* argv[]);
void Git_Checkout(int argc, char* argv[]);
void Git_Status(int argc, char* argv[]);
void Git_Log(int argc, char* argv[]);
void Git_Merge(int argc, char* argv[]);
void Git_Rebase(int argc, char* argv[]);

static struct option help_options[] = {
    {0, 0, 0, 0}
};

static struct option init_options[] = {
//     {"bare",    no_argument,        0, 'b'},
//     {"template",required_argument,  0, 't'},
//     {"quiet",   no_argument,        0, 'q'},
    {0, 0, 0, 0}
};

static struct option add_options[] = {
//     {"all",         no_argument, 0, 'A'},   // add all modified file
//     {"update",      no_argument, 0, 'u'},   // add all tracked file
//     {"patch",       no_argument, 0, 'p'},
//     {"interactive", no_argument, 0, 'i'},
//     {"force",       no_argument, 0, 'f'},   // allow ignored file
//     {"dry-run",     no_argumentm 0, 'n'},   // display but not add
    {0, 0, 0, 0}
};

static struct option commit_options[] = {
    {"message", required_argument,  0, 'm'},
    // {"author",  required_argument,  0, 'a'},
    // {"date",    required_argument,  0, 'd'},
    // {"verbose", no_argument,        0, 'v'},    // display diff
    // {"signoff", no_argument,        0, 's'},    // add Signed-off-by line
    // amend, no-edit, ...
    {0, 0, 0, 0}
};

static struct option branch_options[] = {
//     {"all",         no_argument,        0, 'a'},
//     {"remote",      no_argument,        0, 'r'},
    {"delte",       required_argument,  0, 'd'},
    // -D
//     {"move",        required_argument,  0, 'm'},
//     {"copy",        required_argument,  0, 'c'},
//     {"list",        no_argument,        0, 'l'},
//     {"show-current",no_argument,        0, 's'},
//     // contains, no-contains, ...
    {0, 0, 0, 0}
};

static struct option status_options[] = {
//     {"short",   no_argument, 0, 's'},
//     {"branch",  no_argument, 0, 'b'},
//     // porcelain, ignored, untracked-files, ...
    {0, 0, 0, 0}
};

static struct option log_options[] = {
//     {"max-count",   required_argument,  0, 'n'},
//     {"patch",       no_argument,        0, 'p'},
//     // oneline, graph, decorate, author, grep, stat, since, after, until, before, ...
    {0, 0, 0, 0}
};

static struct option checkout_options[] = {
    {"branch",  required_argument, 0, 'b'}, // create and switch
    // {"Branch",  required_argument, 0, 'B'}, // create, reset and switch
    // {"orphan",  required_argument, 0, 'o'}, // create isolated branch
    // {"force",   no_argument,       0, 'f'},
    // // -- <file>, track, ...
    {0, 0, 0, 0}
};

static struct option merge_options[] = {
    {"ff-only",   required_argument, 0, 'o'},
    {"no-commit", required_argument, 0, 'c'},
    {"no-ff",     required_argument, 0, 'f'},
    {0, 0, 0, 0}
};

#endif
