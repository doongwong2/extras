#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

int main()
{
	WSADATA wsa;
	SOCKET sock;
	struct sockaddr_in server;
	char buffer[1024];
	
	//init winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("failed, error code: %d\n", WSAGetLastError());
		return 1;
	}

	//create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("could not create socket: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(PORT);

	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		printf("connect failed: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	while (1)
	{
		char guess[5];
		printf("enter your guess(4 digits): ");
		scanf("%4s", guess);

		send(sock, guess, strlen(guess), 0);

		memset(buffer, 0, sizeof(buffer));
		int recv_size = recv(sock, buffer, sizeof(buffer), 0);
		if (recv_size <= 0) break;

		printf("result: %s\n", buffer);

		if (strcmp(buffer, "4A0B") == 0)
		{
			printf("You've won!\n");
			break;
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}