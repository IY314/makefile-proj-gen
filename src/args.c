#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpg.h"

#define flag(s, var) \
    case s:          \
        var = 1;     \
        break

#define str_flag(s, var)                  \
    case s:                               \
        var = malloc(strlen(optarg) + 1); \
        strcpy(var, optarg);              \
        break

#define func_null_flag(s, func) \
    case s:                     \
        func;                   \
        return NULL

void version(const char *const prog) {
    mpg_msg = malloc(sizeof(char) * 32);
    sprintf(mpg_msg, "%s version %d.%d\n", prog, VERSION_MAJ, VERSION_MIN);
}

void help(const char *const prog) {
    mpg_msg = malloc(sizeof(char) * 319);
    sprintf(mpg_msg, HELP, prog);
}

struct Project *get_proj(const char *const prog, const int argc,
                         char *const *argv) {
    int opt, optidx;
    int cxx = 0;
    int git = 0;
    int makefile = 0;
    char *name = NULL;
    char *compiler = NULL;
    char *std = NULL;
    struct option long_options[] = {{"version", no_argument, 0, 'v'},
                                    {"help", no_argument, 0, 'h'},
                                    {"c++", no_argument, &cxx, 1},
                                    {"git", no_argument, &git, 1},
                                    {"makefile", no_argument, &makefile, 1},
                                    {"compiler", required_argument, 0, 'c'},
                                    {"std", required_argument, 0, 's'},
                                    {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "vh+gmc:s:", long_options,
                              &optidx)) != -1) {
        switch (opt) {
            func_null_flag('v', version(prog));
            func_null_flag('h', help(prog));
            flag('+', cxx);
            flag('g', git);
            flag('m', makefile);
            str_flag('c', compiler);
            str_flag('s', std);
            case '?':
                return NULL;
                break;
            default:
                if (opt != 0) abort();
                break;
        }
    }
    if (optind < argc) {
        name = malloc(strlen(argv[optind]) + 1);
        strcpy(name, argv[optind]);
    } else {
        if (compiler != NULL) free(compiler);
        if (std != NULL) free(std);
        mpg_msg = malloc(29);
        mpg_status = 1;
        strcpy(mpg_msg, "no name provided for project");
        return NULL;
    }

    if (compiler == NULL) {
        compiler = malloc(sizeof(char) * 4);
        strcpy(compiler, cxx ? "c++" : "cc");
    }

    if (std == NULL) {
        std = malloc(sizeof(char) * 6);
        strcpy(std, cxx ? "c++11" : "c99");
    }

    struct Project *project =
        init_proj(name, compiler, std, cxx, git, makefile);
    free(name);
    free(compiler);
    free(std);

    return project;
}
