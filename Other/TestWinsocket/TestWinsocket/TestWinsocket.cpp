#include <iostream>
#include <winsock.h>

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NOERROR)
	{
		fprintf(stderr, "WSAStartup failed.\n");
		exit(1);
	}

    
	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	if (bind(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(mainSocket);
		return 1;
	}

	if (listen(mainSocket,1) == SOCKET_ERROR)
	{
		printf("Error listening on socket.\n");
	}

	SOCKET acceptSocket = SOCKET_ERROR;
	printf("Waiting for a client to connect...\n");

	while (acceptSocket == SOCKET_ERROR)
	{
		acceptSocket = accept(mainSocket, NULL, NULL);
	}

	printf("Client connected.\n");
	mainSocket = acceptSocket;

	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Server says hello!";
	char recvbuf[32] = "";

	bytesRecv = recv(mainSocket, recvbuf, 32, 0);
	printf("Bytes received: %ld\n", bytesRecv);
	printf("Received text: %s\n", recvbuf);

	bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
	printf("Bytes sent: %ld\n", bytesSent);

	system("pause");
}

