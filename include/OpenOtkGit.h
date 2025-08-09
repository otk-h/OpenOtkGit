#ifndef __OPEN_OTK_GIT__
#define __OPEN_OTK_GIT__

#include "blob.h"
#include "tree.h"
#include "index.h"

void Git_Version();
void Git_Help();
void Git_Init();
void Git_Add(const char* path);
void Git_Commit(const char* message);
void Git_Branch(const char* branch_name);
void Git_Checkout(const char* branch_name);
void Git_Merge();

#endif
