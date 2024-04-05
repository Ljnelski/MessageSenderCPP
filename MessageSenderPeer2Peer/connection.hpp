#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>


class Connection
{
private:
	WSADATA _wsaData;
	SOCKET _connection;
	SOCKET _listen;

	addrinfo* _address;

public:
	int StartUpWSA();
	void CreateAddressInfo(std::string host, std::string port);
	int Connect();
	int Listen();
	int Accept();

	int SendMsg(int length);
	int RecvMsg(int length);

	void printAddress();
};

