#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")//easy to link to library.

#define PORT 8888

//calculate A and B
void calculateAB(const char* secret, const char* guess, int* A, int* B)
{
	*A = *B = 0;

	for (int i = 0; i < 4; i++)
	{
		if (guess[i] == secret[i])
			(*A)++;
		else
			for (int j = 0; j < 4; j++)
			{
				if (guess[j] == secret[i] && i != j)
					(*B)++;
			}
	}
}

int main()
{
	WSADATA wsa;
	SOCKET server_fd, client_fd;
	struct sockaddr_in server, client;
	int c;
	char buffer[1024];
	char secret[5];

	printf("enter a 4-digit number, the numbers should not repeat:");
	scanf("%4s", secret);

	//initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("failed, error code: %d\n", WSAGetLastError());
		return 1;
	}

	//create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("could not create socket: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//prepare sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(server_fd);
		WSACleanup();
		return 1;
	}

	listen(server_fd, 1);

	printf("awaiting client...\n");
	c = sizeof(struct sockaddr_in);
	client_fd = accept(server_fd, (struct sockaddr*)&client, &c);
	if (client_fd == INVALID_SOCKET)
	{
		printf("failed to accept: %\n", WSAGetLastError());
		closesocket(server_fd);
		WSACleanup();
		return 1;
	}

	while (1)
	{
		memset(buffer, 0, sizeof(buffer));
		int recv_size = recv(client_fd, buffer, sizeof(buffer), 0);
		if (recv_size <= 0) break;
		
		int A, B;
		calculateAB(secret, buffer, &A, &B);

		char reply[20];
		snprintf(reply, sizeof(reply), "%dA%dB", A, B);
		send(client_fd, reply, strlen(reply), 0);

		if (A == 4)
		{
			printf("client guessed correctly!");
			break;
		}
	}

	closesocket(client_fd);
	closesocket(server_fd);
	WSACleanup();
	return 0;
}