#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mpg.h"

char *mpg_msg = NULL;
int mpg_status = 0;

struct Project *init_proj(char *name, char *compiler, char *std, int cxx) {
    struct Project *project = malloc(sizeof(struct Project));
    project->name = malloc(strlen(name) + 1);
    project->compiler = malloc(strlen(compiler) + 1);
    project->std = malloc(strlen(std) + 1);
    strcpy(project->name, name);
    strcpy(project->compiler, compiler);
    strcpy(project->std, std);
    project->cxx = cxx;
    return project;
}

void destroy_proj(struct Project *project) {
    free(project->name);
    free(project);
}

int change_dir(const char *dir) {
    if (chdir(dir) == -1) {
        perror("mpg: chdir");
        return 1;
    }
    return 0;
}

int make_dir(const char *dir) {
    if (mkdir(dir, S_IRWXU) == -1) {
        perror("mpg: mkdir");
        return 1;
    }
    return 0;
}

int build_proj_dir(struct Project *project) {
    if (make_dir(project->name)) return 1;
    if (change_dir(project->name)) return 1;
    if (make_dir("src")) return 1;
    if (make_dir("include")) return 1;
    if (make_dir("lib")) return 1;

    FILE *file = fopen("Makefile", "w");
    if (file == NULL) {
        perror("mpg: fopen");
        return 1;
    }

    char *compiler_var = malloc(sizeof(char) * 7);
    char *ext_var = malloc(sizeof(char) * 3);
    strcpy(compiler_var, project->cxx ? "$(CXX)" : "$(CC)");
    strcpy(ext_var, project->cxx ? "cc" : "c");

    fprintf(file, MAKEFILE_SOURCE,
            compiler_var, project->compiler,
            project->std, project->name,
            compiler_var, ext_var, compiler_var
            );

    free(compiler_var);
    free(ext_var);

    fclose(file);

    if (change_dir("src")) return 1;

    file = fopen(project->cxx ? "main.cc" : "main.c", "w");
    if (file == NULL) {
        perror("mpg: fopen");
        return 1;
    }

    fprintf(file, project->cxx ? CXX_SOURCE : C_SOURCE);
    fclose(file);

    return 0;
}

void version(const char *prog) {
    mpg_msg = malloc(sizeof(char) * 32);
    sprintf(mpg_msg, "%s version %d.%d\n", prog, VERSION_MAJ, VERSION_MIN);
}

void help(const char *prog) {
    mpg_msg = malloc(sizeof(char) * 319);
    sprintf(mpg_msg, HELP, prog);
}

struct Project *get_proj(const char *prog, int argc, char **argv) {
    int opt, optidx;
    int cxx = 0;
    char *name, *compiler, *std;
    struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"c++", no_argument, &cxx, 1},
        {"compiler", required_argument, 0, 'c'},
        {"std", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "vh+c:s:", long_options, &optidx)) != -1) {
        switch (opt) {
            case 0:
                break;
            case 'v':
                version(prog);
                return NULL;
                break;
            case 'h':
                help(prog);
                return NULL;
                break;
            case '+':
                cxx = 1;
                break;
            case 'c':
                compiler = malloc(strlen(optarg) + 1);
                strcpy(compiler, optarg);
                break;
            case 's':
                std = malloc(strlen(optarg) + 1);
                strcpy(std, optarg);
                break;
            case '?':
                return NULL;
                break;
            default:
                abort();
                break;
        }
    }
    if (optind < argc) {
        name = malloc(strlen(argv[optind]) + 1);
        strcpy(name, argv[optind]);
    } else {
        mpg_msg = malloc(34);
        mpg_status = 1;
        strcpy(mpg_msg, "mpg: no name provided for project");
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

    struct Project *project = init_proj(name, compiler, std, cxx);
    free(name);
    free(compiler);
    free(std);

    return project;
}

void mpg_quit() {
    free(mpg_msg);
}
