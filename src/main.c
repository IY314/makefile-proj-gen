#include <stdio.h>

#include "mpg.h"

int main(int argc, char **argv) {
    struct Project *proj = getOptions(argc, argv);

    int status = buildDirectory(proj);
    if (status == 0) {
        printf("mpg: created project '%s'\n", proj->name);
    } else {
        printf("mpg: failed to create project '%s'\n", proj->name);
    }

    freeProject(proj);

    return 0;
}
