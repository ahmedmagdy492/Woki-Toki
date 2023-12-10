CC=cc
build_dir=./build/
src_dir=./src/

all:
	$(CC) $(src_dir)*.c -o $(build_dir)woki-toki -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lSDL2 -ggdb

clear:
	rm -rf $(build_dir)*
