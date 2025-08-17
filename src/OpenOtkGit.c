#include "OpenOtkGit.h"

void Git_Version() {
    printf("OpenOtkGit version\n\n");
}

void Git_Help(int argc, char* argv[]) {
    printf("Usage: OpenOtkGit <command> [<args>]\n");
    printf("    --version | -v          Show Version Info\n");
    printf("    --help    | -h          Show help message\n\n");
    printf("These are common OpenOtkGit used in various situations\n\n");

    printf("start a working area\n");
    printf("    init                    Create an empty OpenOtkGit repository\n\n");
    
    printf("work on the current change\n");
    printf("    add                     Add file contents to the index\n\n");

    printf("examine the history and state\n");
    printf("    log                     Show commit logs\n");
    printf("    status                  Show the working tree status\n\n");

    printf("grow, mark and tweak your common history\n");
    printf("    branch                  List, create, or delete branches\n");
    printf("    commit                  Record changes to the repository\n");
    printf("    merge                   Join two or more development histories together\n");
    printf("    rebase                  Reapply commits on top of another base tip\n\n");
}

void Git_Init(int argc, char* argv[]) {
    // clear exist dir
    char rm_cmd[64];
    sprintf(rm_cmd, "rm -rf %s", GIT_DIR);
    system(rm_cmd);

    // create dir
    if (mkdir(GIT_DIR, 0755) == -1
        || mkdir(GIT_OBJECTS_DIR, 0755) == -1
        || mkdir(GIT_REFS_DIR, 0755) == -1
        || mkdir(GIT_REFS_HEADS_DIR, 0755) == -1
    ) {
        perror(NULL);
        return;     
    }

    // create index
    index_hdr_t ihdr;
    memset(&ihdr, 0, sizeof(ihdr));
    ihdr.magic = INDEX_MAGIC;
    write_func(GIT_INDEX_PATH, &ihdr, sizeof(ihdr));

    // create HEAD
    head_t head;
    memset(&head, 0, sizeof(head));
    snprintf((char*)&head, sizeof(head), "%s%s", DEFAULT_HEAD_STR, DEFAULT_BRANCH);
    write_func(GIT_HEAD_PATH, &head, sizeof(head));

    printf("Git_Init: finish\n\n");
}

void Git_Add(int argc, char* argv[]) {
    char* path = argv[1];
    struct stat st;

    if (argc < 2) { 
        printf("Nothing specified, nothing added.\n");
        return;
    } else if (is_Initialized() == 0) {
        printf("Not initialized.\n");
        return;
    } else if (strncmp(path, "./", strlen("./")) != 0) {
        printf("Path should begin with './'.\n");
        return;
    } else if (stat(path, &st) == -1) {
        printf("Pathspec '%s' did not match any files.\n", path);
        return;
    } else if (S_ISDIR(st.st_mode)) {
        printf("Pathspec '%s' should be a file.\n", path);
        return;
    }

    char hash[HASH_LENGTH + 1];
    create_blob(path, hash);

    index_t* index = NULL;
    get_index(&index);
    add_entry_to_index(path, hash, st.st_size, &index);
    update_index(index);
    
    printf("Git_Add: finish\n\n");

}

void Git_Commit(int argc, char* argv[]) {
    struct stat st;
    stat(GIT_INDEX_PATH, &st);

    if (argc < 2) { 
        printf("Aborting commit due to empty commit message.\n");
        return;
    } else if (is_Initialized() == 0) {
        printf("Not initialized, nothing committed.\n");
        return;
    } else if (st.st_size <= sizeof(index_hdr_t)) {
        printf("Empty index, nothing committed.\n");
        return;
    }

    // get message
    int opt = -1;
    char* message = NULL;
    if ((opt = getopt_long(argc, argv, "m:", commit_options, NULL)) != -1) {
        switch (opt) {
            case 'm':
                message = optarg;
                break;
            default: 
                return;
        }
    }

    index_t* index = NULL;
    get_index(&index);

    char tree_hash[HASH_LENGTH + 1];
    char commit_hash[HASH_LENGTH + 1];

    create_tree(index, ".", tree_hash);
    create_commit(message, tree_hash, commit_hash);
    update_refs(commit_hash);
    
    free(index);

    printf("Git_Commit: finish\n\n");

}

void Git_Branch(int argc, char* argv[]) {
    if (is_Initialized() == 0) {
        printf("Not initialized.\n");
        return;
    } else if (argc < 2) { 
        DIR* dir = opendir(GIT_REFS_HEADS_DIR);
        if (dir == NULL) { return; }
        struct dirent* entry;
        int branch_cnt = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0
            ) { continue; }
            printf("%s\n", entry->d_name);
            branch_cnt += 1;
        }
        if (branch_cnt == 0) {
            printf("No local branch found.\n");
        }
        return;
    }

    int opt = -1;
    if ((opt = getopt_long(argc, argv, "d:", commit_options, NULL)) != -1) {
        switch (opt) {
            case 'd':
                char* branch_name = optarg;
                char branch_path[128];
                snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
                struct stat st;
                if (stat(branch_path, &st) == -1) {
                    printf("branch %s not found.\n", branch_name);
                } else {
                    remove(branch_path);
                    printf("branch %s removed.\n", branch_name);
                }
                return;
            default: 
                return;
        }
    }

}

void Git_Checkout(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Too few args.\n");
        return;
    } else if (is_Initialized() == 0) {
        printf("Not initialized.\n");
        return;
    }

    int fd = -1;
    int opt = -1;
    if ((opt = getopt_long(argc, argv, "b:", checkout_options, NULL)) != -1) {
        switch (opt) {
            case 'b':
                char* branch_name = optarg;

                // same branch, do nothing
                char cur_branch[64];
                get_cur_branch(cur_branch);
                if (strcmp(branch_name, cur_branch) == 0) {
                    printf("Same branch, do nothing.\n");
                    return;
                }

                // // safe check
                // if (checkout_safe_check(branch_name) == 0) { return; }
                
                // get commit hash
                char commit_hash[41];
                get_commit_hash(commit_hash);

                // get target branch path
                char branch_path[128];
                snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
                struct stat st;
                if (stat(branch_path, &st) == -1) {
                    // not exist, create and copy hash
                    
                    printf("Branch '%s' not exist, create and copy from current branch '%s'.\n", branch_name, cur_branch);
                    write_func(branch_path, commit_hash, sizeof(commit_hash));
                    
                } else {
                    // exist, get commit obj, get tree hash, update index, clean work dir, rebuild work dir

                    // get commit obj
                    commit_t commit;
                    char commit_path[128];
                    snprintf(commit_path, sizeof(commit_path), "%s/%s", GIT_OBJECTS_DIR, commit_hash);
                    if (stat(commit_path, &st) == -1) { exit(1); }
                    read_func(commit_path, &commit, st.st_size);

                    // get tree hash
                    char tree_hash[HASH_LENGTH + 1];
                    memset(tree_hash, 0, sizeof(tree_hash));
                    memcpy(tree_hash, commit.tree_hash, strlen(commit.tree_hash));

                    // rebuild index & working dir
                    rebuild_index_from_tree(tree_hash);
                    clean_working_dir(".");
                    rebuild_working_dir_from_tree(tree_hash);
                  
                }
                
                // update HEAD
                head_t head;
                memset(&head, 0, sizeof(head));
                snprintf((char*)&head, sizeof(head), "%s%s", DEFAULT_HEAD_STR, branch_name);
                write_func(GIT_HEAD_PATH, &head, sizeof(head));

                printf("Git_Checkout: from '%s' to '%s'.\n", cur_branch, branch_name);
                return;
            default: 
                return;
        }
    }


}

void Git_Status(int argc, char* argv[]) {
    printf("Git_Status: \n\n");

    // TODO
}

void Git_Log(int argc, char* argv[]) {
    printf("Git_Log: \n\n");

    // TODO
}

void Git_Merge(int argc, char* argv[]) {
    printf("Git_Merge: \n\n");

    // TODO
}

void Git_Rebase(int argc, char* argv[]) {
    printf("Git_Rebase: \n\n");

    // TODO
}
