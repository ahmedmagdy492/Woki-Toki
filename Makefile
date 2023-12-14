CC=gcc

all:
	gcc -o main main.c -lws_32 -mwindows %GTK_PKG_CONFIG_PATH%
