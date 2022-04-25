#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "mpg.h"

char *mpg_msg = NULL;
int mpg_status = 0;

void set_err(const char *const err) {
    if (strcmp(err, "") == 0) return;
    mpg_msg = malloc(256);
    strcat(mpg_msg, err);
    strcat(mpg_msg, strerror(errno));
    mpg_msg = realloc(mpg_msg, strlen(mpg_msg) + 1);
}

void mpg_quit() {
    // Deallocate message
    if (mpg_msg != NULL) free(mpg_msg);
}
