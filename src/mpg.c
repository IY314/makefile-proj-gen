#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpg.h"
#include "util.h"

char *mpg_msg = NULL;
int mpg_status = 0;

void set_err(const char *const err) {
    if (str_empty(err)) return;
    malloc_check(mpg_msg, MAX_ERR_LEN, return );
    sprintf(mpg_msg, "%s%s", err, strerror(errno));
    realloc_str(mpg_msg);
}

void mpg_quit() {
    // Deallocate message
    free_if_notnull(mpg_msg);
}
