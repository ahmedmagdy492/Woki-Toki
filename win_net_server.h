#pragma once

#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define PORT 35780
#define REC_BUFF_LEN 4096

static int server_sock = -1;

int init_server_socket() {
  WSADATA data;
  int result;

  result = WSAStartup(MAKEWORD(2, 2), &data);
  if(result != 0) {
    return -1;
  }

  return 0;
}

int setup_sock_server() {
  int result;

  server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_sock == INVALID_SOCKET) {
    server_sock = -1;
    return -1;
  }

  struct sockaddr_in bind_addr;
  memset(&bind_addr, 0, sizeof(struct sockaddr_in));

  bind_addr.sin_family = AF_INET;
  bind_addr.sin_port = htons((unsigned short)PORT);
  bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  result = bind(server_sock, (struct sockaddr*)&bind_addr, sizeof(bind_addr));

  if(result == SOCKET_ERROR) {
    return -2;
  }

  result = listen(server_sock, 10);

  if(result == SOCKET_ERROR) {
    return -3;
  }

  return 0;
}

int accept_client() {
  if(server_sock == -1) {
    return -1;
  }

  int result = accept(server_sock, NULL, NULL);
  if(result == INVALID_SOCKET) {
    return -2;
  }

  return 0;
}

int recv_data(char* buffer, int* len) {
  if(server_sock == -1) {
    return -1;
  }

  int result = recv(server_sock, buffer, REC_BUFF_LEN, MSG_WAITALL);
  if(result == SOCKET_ERROR) {
    return -2;
  }

  *len = result;
  return 0;
}

void cleanup() {
  if(server_sock != -1) {
    closesocket(server_sock);
  }
  WSACleanup();
}
