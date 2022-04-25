#pragma once

#include "mpg/mpg.h"

#define C_SOURCE                          \
    "#include <stdio.h>\n"                \
    "\n"                                  \
    "int main() {\n"                      \
    "    printf(\"Hello, World!\\n\");\n" \
    "    return 0;\n"                     \
    "}\n"

#define CXX_SOURCE                                       \
    "#include <iostream>\n"                              \
    "\n"                                                 \
    "int main() {\n"                                     \
    "    std::cout << \"Hello, World!\" << std::endl;\n" \
    "    return 0;\n"                                    \
    "}\n"

#define MAKEFILE_SOURCE                             \
    "%s := %s\n"                                    \
    "WARN := -Wall -Wextra -Werror\n"               \
    "STD := -std=%s\n"                              \
    "INCLUDE := -Iinclude\n"                        \
    "\n"                                            \
    "TARGET := bin/%s\n"                            \
    "OBJECTS := obj/main.o\n"                       \
    "\n"                                            \
    ".PHONY: all clean\n"                           \
    "\n"                                            \
    "all: $(TARGET)\n"                              \
    "\n"                                            \
    "$(TARGET): $(OBJECTS) bin\n"                   \
    "\t%s $(WARN) $(STD) $(OBJECTS) -o $(TARGET)\n" \
    "\n"                                            \
    "obj/%%.o: src/%%.%s obj\n"                     \
    "\t%s $(WARN) $(STD) $(INCLUDE) -c $< -o $@\n"  \
    "\n"                                            \
    "obj:\n"                                        \
    "\t@mkdir -p obj\n"                             \
    "\n"                                            \
    "bin:\n"                                        \
    "\t@mkdir -p bin\n"                             \
    "\n"                                            \
    "clean:\n"                                      \
    "\t@rm -rf $(TARGET) obj\n"

#define HELP                                 \
    "Usage: %s [OPTION]... [PROJECT]\n"      \
    "\n"                                     \
    "Create a new C/C++ Makefile project.\n" \
    "\n"                                     \
    "Options:\n"                             \
    "  -v, --version\n"                      \
    "      Print version and exit.\n"        \
    "  -h, --help\n"                         \
    "      Print this message and exit.\n"   \
    "  -+, --cxx\n"                          \
    "      Create a C++ project.\n"          \
    "  -c, --compiler\n"                     \
    "      Specify the compiler to use.\n"   \
    "  -s, --std\n"                          \
    "      Specify the C/C++ standard to use.\n"

void set_err(const char *const err);

int change_dir(const char *const dir);

int make_dir(const char *const dir);

void version(const char *const prog);

void help(const char *const prog);
