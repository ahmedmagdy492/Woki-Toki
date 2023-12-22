CC=gcc

linux:
	$(CC) `pkg-config --cflags gtk4` main.c -o ./build/woki-toki `pkg-config --libs gtk4` -lpthread -lSDL2 -lsndfile

clean:
	rm -rf ./build/woki-toki core*
