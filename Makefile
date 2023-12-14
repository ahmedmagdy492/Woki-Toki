CC=gcc

all:
	gcc -o main main.c -lws2_32 -mwindows %GTK_PKG_CONFIG_PATH%
