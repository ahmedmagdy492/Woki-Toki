#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#define PORT 35780

static int result = 0;

int send_data(const char* buffer, int len, const char* ip) {
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == -1) {
    return -1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_aton(ip, &server_addr.sin_addr);
  result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

  if(result == -1) {
    close(sock);
    return -2;
  }

  result = send(sock, buffer, len, 0);
  if(result == -1) {
    return -3;
  }

  close(sock);
  return 0;
}
