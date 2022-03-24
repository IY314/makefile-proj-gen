#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VERSION_MAJ 1
#define VERSION_MIN 0

struct Options {
    char *name;
    bool cxx;
};

struct Options *getOptions(int argc, char **argv) {
    int opt;
    struct Options *opts = malloc(sizeof(struct Options));
    while ((opt = getopt(argc, argv, "vh+")) != -1) {
        switch (opt) {
            case 'v':
                printf("mpg v%u.%u\n", VERSION_MAJ, VERSION_MIN);
                free(opts);
                exit(0);
                break;
            case 'h':
                puts("mpg: Makefile Project Generator");
                puts("Flags:");
                puts("v -- display version and exit");
                puts("h -- display this message and exit");
                puts("+ -- use C++ instead of C");
                free(opts);
                exit(0);
                break;
            case '+':
                opts->cxx = true;
                break;
            case '?':
                free(opts);
                exit(1);
                break;
            case ':':
                printf("mpg: option '%c' needs a value", optopt);
                free(opts);
                exit(1);
        }
    }
    if (optind < argc) {
        opts->name = malloc(strlen(argv[optind]) + 1);
        strcpy(opts->name, argv[optind]);
    } else {
        puts("mpg: no name provided for project");
        free(opts);
        exit(1);
    }

    return opts;
}

int main(int argc, char **argv) {
    struct Options *opts = getOptions(argc, argv);

    free(opts);

    return 0;
}
