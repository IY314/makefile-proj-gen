#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mpg.h"

int change_dir(const char *const dir) {
    if (strcmp(dir, "") == 0) return 1;
    if (chdir(dir) == -1) {
        set_err("chdir: ");
        return 1;
    }
    return 0;
}

int make_dir(const char *const dir) {
    if (strcmp(dir, "") == 0) return 1;
    if (mkdir(dir, S_IRWXU) == -1) {
        set_err("mkdir: ");
        return 1;
    }
    return 0;
}

void git_init() { system("git init"); }

void git_commit(const char *const msg) {
    system("git add .");
    char *const command = malloc(sizeof(char) * (strlen(msg) + 15));
    sprintf(command, "git commit -m \"%s\"", msg);
    system(command);
    free(command);
}
