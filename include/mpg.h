#pragma once

#define VERSION_MAJ 1
#define VERSION_MIN 0

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
"int main() {\n" \
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

extern char *mpg_msg;
extern int mpg_status;

struct Project {
    char *name, *compiler, *std;
    int cxx;
};

struct Project *init_proj(char *name, char *compiler, char *std, int cxx);

int build_proj_dir(struct Project *project);

struct Project *get_proj(int argc, char **argv);

void destroy_proj(struct Project *project);

void mpg_quit();
