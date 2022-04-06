#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define C_SOURCE \
"#include <stdio.h>\n" \
"\n"\
"int main() {\n" \
"    printf(\"Hello, World!\\n\");\n" \
"    return 0;\n" \
"}\n"

#define CXX_SOURCE \
"#include <iostream>\n" \
"\n" \
"int main(void) {\n" \
"    std::cout << \"Hello, World!\" << std::endl;\n" \
"    return 0;\n" \
"}\n"

#define MAKEFILE_SOURCE \
"%s := %s\n" \
"WARN := -Wall -Wextra -Werror\n" \
"STD := -std=%s\n" \
"INCLUDE := -Iinclude\n" \
"\n" \
"TARGET := bin/%s\n" \
"OBJECTS := obj/main.o\n" \
"\n" \
".PHONY: all clean\n" \
"\n" \
"all: $(TARGET)\n" \
"\n" \
"$(TARGET): $(OBJECTS) bin\n" \
"\t%s $(WARN) $(STD) $(OBJECTS) -o $(TARGET)\n" \
"\n" \
"obj/%%.o: src/%%.%s obj\n" \
"\t%s $(WARN) $(STD) $(INCLUDE) -c $< -o $@\n" \
"\n" \
"obj:\n" \
"\tmkdir -p obj\n" \
"\n" \
"bin:\n" \
"\t@mkdir -p bin\n" \
"\n" \
"clean:\n" \
"\trm -rf $(TARGET) obj\n"

// A struct Project to hold a char *name and an int cxx
struct Project {
    char *name, *compiler, *std;
    int cxx;
};

// A function makeProject to make a Project
struct Project *makeProject(char *name, char *compiler, char *std, int cxx) {
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

// A function freeProject to free a Project
void freeProject(struct Project *project) {
    free(project->name);
    free(project);
}

// A function buildDirectory to build a directory
int buildDirectory(struct Project *project) {
    int status;
    
    status = mkdir(project->name, S_IRWXU);
    if (status == -1) {
        perror("mpg: mkdir");
        return 1;
    }

    status = chdir(project->name);
    if (status == -1) {
        perror("mpg: chdir");
        return 1;
    }

    status = mkdir("src", S_IRWXU);
    if (status == -1) {
        perror("mpg: mkdir");
        return 1;
    }

    status = mkdir("include", S_IRWXU);
    if (status == -1) {
        perror("mpg: mkdir");
        return 1;
    }

    status = chdir("src");
    if (status == -1) {
        perror("mpg: chdir");
        return 1;
    }

    FILE *file = fopen(project->cxx ? "main.cc" : "main.c", "w");
    if (file == NULL) {
        perror("mpg: fopen");
        return 1;
    }

    fprintf(file, project->cxx ? CXX_SOURCE : C_SOURCE);
    fclose(file);

    status = chdir("..");
    if (status == -1) {
        perror("mpg: chdir");
        return 1;
    }

    file = fopen("Makefile", "w");
    if (file == NULL) {
        perror("mpg: fopen");
        return 1;
    }

    char *compiler_var = malloc(sizeof(char) * 7);
    char *ext_var = malloc(sizeof(char) * 3);
    strcpy(compiler_var, project->cxx ? "$(CXX)" : "$(CC)");
    strcpy(ext_var, project->cxx ? "cc" : "c");

    fprintf(file, MAKEFILE_SOURCE, compiler_var, project->compiler, project->std, project->name, compiler_var, ext_var, compiler_var);

    free(compiler_var);
    free(ext_var);

    fclose(file);
    return 0;
}

int main() {
    struct Project *project = makeProject("mpg", "clang", "c99", 0);
    int status = buildDirectory(project);
    freeProject(project);
    if (status == 0) {
        status = system("make");
        if (status == -1) {
            perror("mpg: system");
            return 1;
        }
    }
    return 0;
}
