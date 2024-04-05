#include "connection.hpp"
#include <iostream>

int Connection::StartUpWSA()
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	printf("WSAStartup Successful");
	return 0;
}

void Connection::CreateAddressInfo(std::string host, std::string port)
{
	int iResult;

	struct addrinfo hints;

	// create the connection address and port
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &_address);


}

int Connection::Connect()
{
	_connection = INVALID_SOCKET;

	// Create the connection Socket
	_connection = socket(_address->ai_family, _address->ai_socktype, _address->ai_protocol);
	if (_connection == INVALID_SOCKET)
	{
		printf("\nsocket failed with error: %ld", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	
	// connect to server
	int iResult = connect(_connection, _address->ai_addr, (int)_address->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(_connection);
		_connection = INVALID_SOCKET;
	}

	FreeAddrInfo(_address);

	if (_connection == INVALID_SOCKET)
	{
		printf("\nUnable to connect to other person!");
		WSACleanup();
		return 1;
	}
	else
	{
		printf("\nSuccessfuly connected to other person!");
	}

	return 0;
}

int Connection::Listen()
{
	// Create connection listening socket with address info
	_listen = socket(_address->ai_family, _address->ai_socktype, _address->ai_protocol);
	if (_listen == INVALID_SOCKET)
	{
		printf("\nsocket failed with error: %ld", WSAGetLastError());
		freeaddrinfo(_address);
		return 1;
	}

	// Bind the listening socket
	int iResult = bind(_listen, _address->ai_addr, (int)_address->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("\nbind failed with error: %d", WSAGetLastError());
		freeaddrinfo(_address);
		closesocket(_listen);
		return 1;
	}

	// Listen for incoming connections
	iResult = listen(_listen, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		printf("\nlisten failed with error: %d", WSAGetLastError());
		closesocket(_listen);
		WSACleanup();
		return -1;
	}

	return 0;
}


// Accept incoming connection
int Connection::Accept()
{
	printf("\nWaiting to Accept Incoming Connection");

	_connection = accept(_listen, NULL, NULL);
	if (_connection == INVALID_SOCKET)
	{
		printf("\naccept failed with error: %d", WSAGetLastError());
		closesocket(_listen);
		WSACleanup();
		return 1;
	}

	return 0;
}

// Input and Send Message
int Connection::SendMsg(int length)
{
	char* message = new char[length];
	printf("\n=> ");
	std::cin.getline(message, length);

	int iResult = send(_connection, message, length, 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "\nsend failed with error: %d\n" + WSAGetLastError();
		closesocket(_connection);
		WSACleanup();
		return 1;
	}

	delete message;

	return 0;
}

// Recive and Write Message
int Connection::RecvMsg(int length)
{
	char* recvBuffer = new char[length];

	int iResult = recv(_connection, recvBuffer, length, 0);

	if (iResult > 0)
	{
		printf("\nmsg:%s", recvBuffer);		
	}
	else if (iResult == 0)
		printf("\nConnection closed");
	else
	{
		printf("\nrecv failed with error: %d", WSAGetLastError());
		return 1;
	}

	delete recvBuffer;

	return 0;
}

void Connection::printAddress()
{
	if (_address == NULL)
	{
		printf("addressInfo is no longer avalible");
		return;
	}

	struct sockaddr_in* sa = reinterpret_cast<sockaddr_in*>(_address->ai_addr);
	char ip4[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(sa->sin_addr), ip4, INET_ADDRSTRLEN);

	printf("Ip Address is: %s", ip4);
}


