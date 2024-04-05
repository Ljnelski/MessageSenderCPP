#include "client.hpp"
#include <string>

void Client::start()
{
	printf("This is the Client\n");

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		//return 1;
	}
	else
	{
		printf("WSAStartup Successful");
	}
}

void Client::connectToServer()
{
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	printf("Attempting to resolve server address and port\n");
	// Resolve the server address and port
	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		//return 1;
	}
	else
	{
		printf("Resolved Server Address\n");
	}

	printf("Attempting to connect to server socket\n");
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		_connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_connectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			//return 1;
		}

		// Connect to server.
		iResult = connect(_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(_connectSocket);
			_connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (_connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		//return 1;
	}
	else
	{
		printf("Successfuly connected to server\n");
	}

	_userState = WRITING;
}

void Client::run()
{
	//int int_data = getInput_int();=

	char buffer[200];
	std::cin.getline(buffer, 200);

	unsigned __int64 iResult = send(_connectSocket, buffer, 200, 0);
	
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "\nsend failed with error: %d\n" + WSAGetLastError();
		closesocket(_connectSocket);
		WSACleanup();
		//return 1;
	}


	iResult = recv(_connectSocket, buffer, 200, 0);

	if (iResult > 0)
		printf("recived message: %s from server\n", buffer);
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());
}

void Client::stop()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(_connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(_connectSocket);
		WSACleanup();
		//return 1;
	}
	else
	{
		printf("Shuting down connection");
	}

	// cleanup
	closesocket(_connectSocket);
	WSACleanup();
}

int Client::getInput_int()
{
	bool invalid = true;
	std::string input;
	int input_int;

	do
	{
		printf("\nEnter your number => ");
		std::cin >> input;

		try
		{
			input_int = std::stoi(input);
			invalid = false;
		}
		catch (const std::invalid_argument&)
		{
			printf("input could not be converted to integer, try again\n");
		}

	} while (invalid);

	return input_int;
}

char* Client::getInput_string()
{
	bool invalid = true;
	char input[200];

	printf("\nEnter your String => ");
	std::cin.getline(input, 200);
	std::cout.write(input, 200);

	return input;
}
