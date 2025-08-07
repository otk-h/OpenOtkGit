#include <stdio.h>
#include "OpenOtkGit.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        Git_Help();
        return 0;
    }

    int opt;
    if ((opt = getopt_long(argc, argv, "Vhia:c:", global_options, NULL)) != -1) {
        switch(opt) {
            case 'V':
                Git_Version();
                return 0;
            case 'h':
                Git_Help();
                return 0;
            case 'i':
                Git_Init();
                return 0;
            case 'a':
                Git_Add(optarg);
                return 0;
            case 'c':
                Git_Commit(optarg);
                return 0;
            default:
                return 1;
        }
    }

    return 0;
}
