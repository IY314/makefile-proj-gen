CC := clang
WFLAGS := -Wall -Werror -Wextra
OFLAGS := -c ${WFLAGS}
CFLAGS := -o bin/mpg ${WFLAGS}

.PHONY: all link clean clean-objs

all: link

link: bin/main.o
	${CC} $^ ${CFLAGS}

bin/%.o: src/%.c bin
	${CC} $< -o $@ ${OFLAGS}

bin:
	mkdir -p bin

clean: clean-objs
	rm bin/mpg

clean-objs:
	rm bin/*.o
