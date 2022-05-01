#include <stdio.h>

#include "mpg.h"
#include "util.h"

int main(const int argc, char *const *argv) {
    struct Project *const proj = get_proj("mpg", argc, argv);
    if (is_null(proj)) {
        printf("mpg: %s\n", mpg_msg);
        mpg_quit();
        return mpg_status;
    }

    if (build_proj_dir(proj) == 0) {
        printf("mpg: created '%s'\n", proj->name);
    } else {
        printf("mpg: failed to create '%s'\n", proj->name);
    }

    destroy_proj(proj);
    mpg_quit();

    return 0;
}
