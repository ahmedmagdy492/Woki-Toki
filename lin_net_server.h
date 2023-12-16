#pragma once
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 35780
#define REC_BUFF_LEN 4069

static int server_sock = -1;

int setup_sock_server() {
  int result;

  server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_sock == -1) {
    return -1;
  }

  struct sockaddr_in bind_addr;
  memset(&bind_addr, 0, sizeof(struct sockaddr_in));
  bind_addr.sin_family = AF_INET;
  bind_addr.sin_port = htons(PORT);
  inet_aton("0.0.0.0", &bind_addr.sin_addr);
  result = bind(server_sock, (struct sockaddr*)&bind_addr, sizeof(struct sockaddr_in));
  if(result == -1) {
    return -2;
  }

  result = listen(server_sock, 10);
  if(result == -1) {
    return -3;
  }

  return 0;
}

int accept_client() {
  if(server_sock == -1) {
    return -1;
  }

  int result = accept(server_sock, NULL, NULL);
  if(result == -1) {
    return -2;
  }

  return 0;
}

int recv_data(char* buffer, int* len) {
  if(server_sock == -1) {
    return -1;
  }

  int result = recv(server_sock, buffer, REC_BUFF_LEN, MSG_WAITALL);
  if(result == -1) {
    return -2;
  }

  *len = result;
  return 0;
}

void cleanup() {
  if(server_sock != -1)
    close(server_sock);
}
