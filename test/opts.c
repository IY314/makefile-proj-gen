#include <stdio.h>

#include "mpg.h"

int main() {
    char *opts[6] = {"mpg", "-c", "cc", "-s", "c99", "proj"};
    struct Project *proj = get_proj("mpg", 6, opts);
    printf("Name: %s\nCompiler: %s\nStandard: %s\nIs C++: %d\n", proj->name,
           proj->compiler, proj->std, proj->cxx);
    destroy_proj(proj);
    return 0;
}
