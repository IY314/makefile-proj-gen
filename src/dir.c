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
