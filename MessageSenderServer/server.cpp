#include "server.hpp"

Server::Server()
{
	_listenSocket = INVALID_SOCKET;
}

void Server::start()
{
	int iResult;

	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock	
	iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}
	else
	{
		printf("WSAStartup Successful\n");
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		return;
	}

	// Create a SOCKET for the server to listen for client connections.
	_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_listenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(_listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(_listenSocket);
		return;
	}

	freeaddrinfo(result);
}

// Currently only accepts on connection
void Server::acceptConnections()
{
	int iResult;

	iResult = listen(_listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return;
	}

	_temp_client_socket = accept(_listenSocket, NULL, NULL);
	if (_temp_client_socket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return;
	}
}

void Server::run()
{
	char buffer[200];
	unsigned __int64 iResult = recvMessage(buffer, 200);

	if (iResult > 0)
	{
		printf("Recived message from client: %s\n", buffer);
		//std::cout << "Recived message" << receivedString << " from client";
		// Echo the buffer back to the sender
		sendMessage(buffer, 200);
	}
	else if (iResult == 0)
		printf("Connection closing...\n");
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(_temp_client_socket);
		WSACleanup();
		//return 1;
	}
}

void Server::addSocket()
{
	printf("calling 'addSocket' will not do anything, server does not support more than one client at the moment");
	/*SOCKET clientSocket = accept(_listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return;
	}

	_clientSockets.push_back(clientSocket);*/
}

void Server::removeSocket()
{
}

void Server::stop()
{
	// No longer need server socket
	closesocket(_listenSocket);

	// Close the client socket
	closesocket(_temp_client_socket);

	// close all client sockets
	/*for (size_t i = 0; i < _clientSockets.size(); i++)
	{
		closesocket(_clientSockets[i]);
	}*/
}

void Server::sendMessage(const char* data, int messageSize)
{
	int iSendResult = send(_temp_client_socket, data, messageSize, 0);
	if (iSendResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(_temp_client_socket);
		WSACleanup();
		//return 1;
	}
}

int Server::recvMessage(char* data, int messageSize) 
{
	return recv(_temp_client_socket, data, messageSize, 0);	
}