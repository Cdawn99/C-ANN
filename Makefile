CFLAGS = -Wall -Werror -Wextra
CFLAGS += -pedantic -pedantic-errors
CFLAGS += -std=c11 -ggdb

IDIRS = -I./raylib-5.0_linux_amd64/include
LDFLAGS = -L./raylib-5.0_linux_amd64/lib
LDLIBS = -lraylib -lm

all: ./build/visualize_data

./build/visualize_data: visualize_data.c | build
	$(CC) -o $@ $^ $(CFLAGS) $(IDIRS) $(LDFLAGS) $(LDLIBS)

build:
	mkdir -p build

.PHONY: clean
clean:
	rm -r build
