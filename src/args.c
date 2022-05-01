#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpg.h"
#include "util.h"

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
    malloc_struct_array(mpg_msg, char, 32, {
        set_err("Could not allocate memory for version message.\n");
        return;
    });

    sprintf(mpg_msg, "%s version %d.%d\n", prog, VERSION_MAJ, VERSION_MIN);
}

void help(const char *const prog) {
    malloc_struct_array(mpg_msg, char, 1024, {
        set_err("Could not allocate memory for help message.\n");
        return;
    });

    sprintf(mpg_msg, HELP, prog);
}

struct Project *get_proj(const char *const prog, const int argc,
                         char *const *argv) {
    if (str_empty(prog)) {
        set_err("Program name cannot be NULL.\n");
        return NULL;
    }

    static int opt, optidx;
    static int cxx = 0;
    static int git = 0;
    static int makefile = 0;
    static char *name = NULL;
    static char *compiler = NULL;
    static char *std = NULL;
    static const struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"c++", no_argument, &cxx, 1},
        {"git", no_argument, &git, 1},
        {"makefile", no_argument, &makefile, 1},
        {"compiler", required_argument, 0, 'c'},
        {"std", required_argument, 0, 's'},
        {0, 0, 0, 0}};

    static struct Project *project;

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
        malloc_strcpy(name, argv[optind], {
            set_err("Could not allocate memory for project name.\n");
            return NULL;
        });
    } else {
        free_if_notnull(compiler);
        free_if_notnull(std);
        malloc_struct_array(mpg_msg, char, MAX_ERR_LEN, {
            set_err("Could not allocate memory for error message.\n");
        });

        mpg_status = 1;
        return NULL;
    }

    if (str_empty(compiler)) {
        malloc_strcpy(compiler, cxx ? "c++" : "cc", {
            free(name);
            return NULL;
        });
    }

    if (str_empty(std)) {
        std = malloc(sizeof(char) * 6);
        strcpy(std, cxx ? "c++11" : "c99");
    }

    project = init_proj(name, compiler, std, cxx, git, makefile);

    free_if_notnull(name);
    free_if_notnull(compiler);
    free_if_notnull(std);

    return project;
}
