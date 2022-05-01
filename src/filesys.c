#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mpg.h"
#include "util.h"

int change_dir(const char *const dir) {
    if (str_empty(dir)) return 1;

    if (chdir(dir) == -1) {
        set_err("chdir: ");
        return 1;
    }
    return 0;
}

int make_dir(const char *const dir) {
    if (str_empty(dir)) return 1;

    if (mkdir(dir, S_IRWXU) == -1) {
        set_err("mkdir: ");
        return 1;
    }
    return 0;
}

void git_init() { system("git init"); }

void git_commit(const char *const msg) {
    if (str_empty(msg)) return;

    static char *cmd;

    system("git add .");

    malloc_struct_array(cmd, char, strlen(msg) + 15, {
        set_err("Could not allocate memory for git commit command.\n");
        return;
    });

    sprintf(cmd, "git commit -m \"%s\"", msg);

    system(cmd);

    free_if_notnull(cmd);
}
