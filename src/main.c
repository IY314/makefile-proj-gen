#include <stdio.h>

#include "mpg.h"

int main(int argc, char **argv) {
    struct Project *proj = get_proj("mpg", argc, argv);
    if (proj == NULL) {
        printf("mpg: %s\n", mpg_msg);
        mpg_quit();
        return mpg_status;
    }

    if (build_proj_dir(proj) == 0) {
        printf("mpg: created project '%s'\n", proj->name);
    } else {
        printf("mpg: failed to create project '%s'\n", proj->name);
    }

    destroy_proj(proj);
    mpg_quit();

    return 0;
}
