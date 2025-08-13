#include "OpenOtkGit.h"

// --- assist func ---
static int checkout_safe_check();
static void clean_working_dir();
static void rebuild_working_dir();

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

    // set index hdr
    index_hdr_t ihdr;
    ihdr.magic = INDEX_MAGIC;
    ihdr.entry_cnt = 0;

    // set HEAD
    head_t head;
    memset(&head, 0, sizeof(head));
    snprintf((char*)&head, sizeof(head), "%s%s", DEFAULT_HEAD_STR, DEFAULT_BRANCH);

    FILE* fd = NULL;

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
    if ((fd = fopen(GIT_INDEX_PATH, "w")) == NULL
        || fwrite(&ihdr, 1, sizeof(ihdr), fd) != sizeof(ihdr)
        || fclose(fd) != 0
    ) {
        perror(NULL);
        return;
    }

    // create HEAD
    if ((fd = fopen(GIT_HEAD_PATH, "w")) == NULL
        || fwrite(&head, 1, sizeof(head), fd) != sizeof(head)
        || fclose(fd) != 0
    ) {
        perror(NULL);
        return;
    }

    printf("Git_Init: finish\n\n");
}

void Git_Add(int argc, char* argv[]) {
    char* path = argv[1];
    struct stat st;

    if (argc < 2) { 
        printf("Nothing specified, nothing added.\n");
        return;
    } else if (is_Initialized() == 0) {
        printf("Not initialized, nothing added.\n");
        return;
    } else if (strncmp(path, "./", strlen("./")) != 0) {
        printf("Path should begin with './', nothing added.\n");
        return;
    } else if (stat(path, &st) == -1) {
        printf("Pathspec '%s' did not match any files.", path);
        return;
    } else if (S_ISDIR(st.st_mode)) {
        printf("Pathspec should be a file.\n");
        return;
    }

    char hash[HASH_LENGTH + 1];
    if (create_blob(path, hash)
        && add_entry_to_index(path, hash)
    ) {
        printf("Git_Add: finish\n\n");
        return;
    }

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

    index_t* index = malloc(sizeof(index_hdr_t));
    if (get_index(index) == 0) { return; }

    char tree_hash[HASH_LENGTH + 1];
    char commit_hash[HASH_LENGTH + 1];

    if (create_tree(index, ".", tree_hash)
        && create_commit(message, tree_hash, commit_hash)
        && update_refs(commit_hash)
    ) {
        printf("Git_Commit: finish\n\n");
    }

    free(index);

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
    } else if (checkout_safe_check() == 0) {
        return;
    }

    int fd = -1;
    int opt = -1;
    if ((opt = getopt_long(argc, argv, "b:", checkout_options, NULL)) != -1) {
        switch (opt) {
            case 'b':
                char* branch_name = optarg;
                
                char cur_branch[64];
                get_cur_branch(cur_branch);
                if (strcmp(branch_name, cur_branch) == 0) {
                    printf("Same branch, do nothing.\n");
                    return;
                }
                // get commit hash
                char commit_hash[41];
                get_commit_hash(commit_hash);
                
                // get cur branch path
                char branch_path[128];
                snprintf(branch_path, sizeof(branch_path), "%s/%s", GIT_REFS_HEADS_DIR, branch_name);
                struct stat st;
                if (stat(branch_path, &st) == -1) {
                    // not exist, create and copy hash
                    fd = open(branch_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    write(fd, commit_hash, sizeof(commit_hash));
                    close(fd);
                } else {
                    char tree_path[128];
                    snprintf(tree_path, sizeof(tree_path), "%s/%s", GIT_OBJECTS_DIR, commit_hash);
                    stat(tree_path, &st);
                    fd = open(tree_path, O_RDONLY);
                    tree_t* tree = (tree_t*)malloc(st.st_size);
                    read(fd, tree, st.st_size);
                    close(fd);

                    // update index with tree object

                    // clean work dir
                    // rebuild work dir
                    
                }
                
                // update HEAD
                head_t head;
                memset(&head, 0, sizeof(head));
                snprintf((char*)&head, sizeof(head), "%s%s", DEFAULT_HEAD_STR, branch_name);
                fd = open(GIT_HEAD_PATH, O_WRONLY);
                write(fd, head_content, sizeof(head_content));
                close(fd);

                return;
            default: 
                return;
        }
    }


}

// void Git_Merge(int argc, char* argv[]) {
//     printf("Git_Merge: \n\n");

//     // TODO
// }

// void Git_Rebase() {
//     printf("Git_Rebase: \n\n");

//     // TODO
// }


// --- assist func ---

static int checkout_safe_check() {
    // TODO: check index commit
    printf("uncommitted file '%s' in index.\n", GIT_DIR);
    return 0;
    // TODO: check working dir untracked
    printf("untracked file '%s' in working dir", GIT_DIR);
    return 0;
    // // TODO: check overwritten of untracked file
    return 1;
}

static void clean_working_dir() {
    
}

static void rebuild_working_dir() {

}
