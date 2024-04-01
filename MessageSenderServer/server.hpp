#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
class Server
{
private:
	WSADATA _wsaData;
	SOCKET _listenSocket;
	SOCKET _temp_client_socket;

	// Not implemented
	std::vector<SOCKET> _clientSockets;

	void addSocket();
	void removeSocket();

public:
	Server();
	void start();
	void acceptConnections();
	void run();
	void stop();

	void sendMessage(const char* data, int messageSize);
	int recvMessage(char* data, int messageSize);
};

