#include <iostream>
#include "connection.hpp"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_IP_ADDRESS "PUT_YOUR_HOST_ADDRESS_HERE"
#define DEFAULT_PORT "8645"
#define MESSAGE_SIZE 200

int main()
{

	Connection connection = Connection();
	if (connection.StartUpWSA() != 0)
	{
		return 1;
	}

	std::string input;	
	

	std::cout << "\n\n\nWelcome To Message Sender!!!!\n";

	do
	{
		std::cout << "\nc to connect to other user, l to listen for other user: ";
		std::cin >> input;
		std::cin.ignore();

		if (input.size() > 1)
		{
			std::cout << "please enter one character\n";
			continue;
		}
		// Try to Connect
		if (input[0] == 'c')
		{
			printf("Enter the ip address you want to connect to: ");
			std::cin >> input;
			std::cin.ignore();

			connection.CreateAddressInfo(input, DEFAULT_PORT);
			connection.printAddress();
			connection.Connect();
			connection.SendMsg(MESSAGE_SIZE);
			connection.RecvMsg(MESSAGE_SIZE);
			// Send message if Successful
			break;
		}
		// Listen for Connections
		else if (input[0] == 'l')
		{
			connection.CreateAddressInfo(DEFAULT_IP_ADDRESS, DEFAULT_PORT);
			connection.printAddress();
			connection.Listen();
			connection.Accept();
			connection.RecvMsg(MESSAGE_SIZE);
			break;
		}

		std::cout << "\ninvalid entry try again";

	} while (true);


	do
	{
		connection.SendMsg(MESSAGE_SIZE);
		connection.RecvMsg(MESSAGE_SIZE);

	} while (true);

	return 0;
}