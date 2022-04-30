#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpg.h"

struct Project *init_proj(const char *name, const char *compiler,
                          const char *std, const int cxx, const int git,
                          const int makefile) {
    struct Project *project = malloc(sizeof(struct Project));
    project->name = malloc(strlen(name) + 1);
    project->compiler = malloc(strlen(compiler) + 1);
    project->std = malloc(strlen(std) + 1);
    strcpy(project->name, name);
    strcpy(project->compiler, compiler);
    strcpy(project->std, std);
    project->cxx = cxx;
    project->git = git;
    project->makefile = makefile;
    return project;
}

int build_proj_dir(struct Project *const project) {
    if (project == NULL) return 1;

    static FILE *file;
    static char *compiler_var, *ext_var;

    if (project->makefile) {
        // Open Makefile in root of project
        file = fopen("Makefile", "w");
        if (file == NULL) {
            set_err("fopen: ");
            return 1;
        }

        // Populate Makefile with content
        compiler_var = malloc(sizeof(char) * 7);
        ext_var = malloc(sizeof(char) * 3);
        strcpy(compiler_var, project->cxx ? "$(CXX)" : "$(CC)");
        strcpy(ext_var, project->cxx ? "cc" : "c");

        fprintf(file, MAKEFILE_SOURCE, compiler_var, project->compiler,
                project->std, project->name, compiler_var, ext_var,
                compiler_var);

        free(compiler_var);
        free(ext_var);

        fclose(file);
    } else {
        // Create project directory and change into it
        if (make_dir(project->name)) return 1;
        if (change_dir(project->name)) return 1;

        if (project->git) git_init();

        // Create directories inside project
        if (make_dir("src")) return 1;
        if (make_dir("include")) return 1;
        if (make_dir("lib")) return 1;

        // Open Makefile in root of project
        file = fopen("Makefile", "w");
        if (file == NULL) {
            set_err("fopen: ");
            return 1;
        }

        // Populate Makefile with content
        compiler_var = malloc(sizeof(char) * 7);
        ext_var = malloc(sizeof(char) * 3);
        strcpy(compiler_var, project->cxx ? "$(CXX)" : "$(CC)");
        strcpy(ext_var, project->cxx ? "cc" : "c");

        fprintf(file, MAKEFILE_SOURCE, compiler_var, project->compiler,
                project->std, project->name, compiler_var, ext_var,
                compiler_var);

        free(compiler_var);
        free(ext_var);

        fclose(file);

        if (project->git) {  // Open .gitignore in root of project
            file = fopen(".gitignore", "w");
            if (file == NULL) {
                set_err("fopen: ");
                return 1;
            }

            // Populate .gitignore with content
            fputs(GITIGNORE, file);
            fclose(file);
        }

        // Change into source directory
        if (change_dir("src")) return 1;

        // Open source file main.c or main.cc
        file = fopen(project->cxx ? "main.cc" : "main.c", "w");
        if (file == NULL) {
            set_err("fopen: ");
            return 1;
        }

        // Populate source file with content
        fprintf(file, project->cxx ? CXX_SOURCE : C_SOURCE);
        fclose(file);

        if (change_dir("..")) return 1;

        if (project->git) git_commit("Init");
    }

    return 0;
}

void destroy_proj(struct Project *const project) {
    free(project->name);
    free(project);
}
