#include "message.hpp"

char* Message::GetInput(int length)
{
	char* input = new char[length];

	printf("\nEnter your String => ");
	std::cin.getline(input, length);
	std::cout.write(input, length);

	return input;
}
