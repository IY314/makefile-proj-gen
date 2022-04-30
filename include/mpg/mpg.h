#pragma once

#define VERSION_MAJ 1
#define VERSION_MIN 0

extern char *mpg_msg;
extern int mpg_status;

struct Project {
    char *name, *compiler, *std;
    int cxx, git;
};

struct Project *init_proj(const char *const name, const char *const compiler,
                          const char *const std, const int cxx, const int git);

int build_proj_dir(struct Project *const project);

struct Project *get_proj(const char *const prog, const int argc,
                         char *const *argv);

void destroy_proj(struct Project *const project);

void mpg_quit();
