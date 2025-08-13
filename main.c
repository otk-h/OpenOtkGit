#include "OpenOtkGit.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        Git_Help(argc-1, argv+1);
        return 0;
    }

    char* cmd = argv[1];

    if (strcmp(cmd, "-v") == 0 || strcmp(cmd, "--version") == 0) {
        Git_Version();
    } else if(strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0) {
        Git_Help(argc-1, argv+1);
    } else if (strcmp(cmd, "init") == 0) {
        Git_Init(argc-1, argv+1);
    } else if (strcmp(cmd, "add") == 0) {
        Git_Add(argc-1, argv+1);
    } else if (strcmp(cmd, "commit") == 0) {
        Git_Commit(argc-1, argv+1);
    } else if (strcmp(cmd, "branch") == 0) {
        // Git_Branch(argc-1, argv+1);
    } else if (strcmp(cmd, "checkout") == 0) {
        // Git_Checkout(argc-1, argv+1);
    } else if (strcmp(cmd, "merge") == 0) {

    } else if(strcmp(cmd, "rebase") == 0) {

    } else {
        Git_Help(argc-1, argv+1);
        return 1;
    }

    return 0;
}
