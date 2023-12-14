#pragma once

#include <stdio.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define PORT 35780

static int result = 0;

int send_data(const char* buffer, int len, const char* ip) {
  WSADATA data;

  result = WSAStartup(MAKEWORD(2, 2), &data);
  if(result != 0) {
    printf("unable to init winsock2: %d\n", result);
    return -1;
  }

  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock == INVALID_SOCKET) {
    printf("unable to create socket: %d\n", WSAGetLastError());
    WSACleanup();
    return -2;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons((unsigned short)PORT);
  if(inet_pton(AF_INET, ip, &(server_addr.sin_addr)) != 1) {
    printf("unable to parse given ip\n");
    closesocket(sock);
    WSACleanup();
    return -3;
  }

  result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

  if(result != 0) {
    printf("unable to connect to remote server: %d\n", WSAGetLastError());
    closesocket(sock);
    WSACleanup();
    return -4;
  }

  result = send(sock, buffer, len, 0);

  if(result == 0) {
    printf("cannot send data: %d\n", WSAGetLastError());
    closesocket(sock);
    WSACleanup();
    return -4;
  }

  closesocket(sock);
  WSACleanup();

  return 0;
}
