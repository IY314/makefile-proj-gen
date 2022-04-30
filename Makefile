$(CC) := cc
WARN := -Wall -Wextra -Werror
STD := -std=c99
INCLUDE := -Iinclude

EXEC := bin/mpg
LIB := bin/libmpg.a
OBJECTS := obj/mpg.o obj/args.o obj/filesys.o obj/proj.o

.PHONY: all clean clean-obj clean-lib clean-exec clean-tests

all: $(EXEC) $(LIB)

$(EXEC): obj/main.o $(OBJECTS) bin
	$(CC) $(WARN) $(STD) $< $(OBJECTS) -o $(EXEC)

$(LIB): $(OBJECTS) bin
	$(AR) -rcs $(LIB) $(OBJECTS)

test-%: test/%.c $(LIB)
	$(CC) $(WARN) $(INCLUDE) $(STD) -Lbin -lmpg $< -o bin/$@

obj/%.o: src/%.c obj
	$(CC) $(WARN) $(STD) $(INCLUDE) -c $< -o $@

obj:
	@mkdir -p obj

bin:
	@mkdir -p bin

clean: clean-obj clean-lib clean-exec

clean-obj:
	@rm -rf obj

clean-lib:
	@rm -rf $(LIB)

clean-exec:
	@rm -rf $(EXEC)

clean-tests:
	@rm -rf bin/test-*
