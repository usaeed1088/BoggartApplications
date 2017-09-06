#include "Options.h"

#include <sstream>
#include <functional>
#include <iostream>

const std::string Options::PERSONALITY_CLIENT("client");
const std::string Options::PERSONALITY_SERVER("server");

std::string Options::s_Personality;
std::string Options::s_IP;
std::int32_t Options::s_Port(0);

Options::Options()
{

}

bool Options::Process(int argc, char* argv[])
{
	std::function<void()> PrintUsage = []()
	{
		std::cout << "Demonstration_Node " << PERSONALITY_SERVER << " <port>" << std::endl;
		std::cout << "Demonstration_Node " << PERSONALITY_CLIENT << " <server_ip> <server_port>" << std::endl;
	};

	if (argc < 3)
	{
		PrintUsage();
		return false;
	}

	s_Personality = argv[1];

	if (s_Personality == PERSONALITY_SERVER)
	{
		std::stringstream ss;
		ss << argv[2];
		ss >> std::dec >> s_Port;
	}
	else if (s_Personality == PERSONALITY_CLIENT)
	{
		if (argc < 4)
		{
			PrintUsage();
			return false;
		}

		s_IP = argv[2];

		std::stringstream ss;
		ss << argv[3];
		ss >> std::dec >> s_Port;
	}
	else
	{
		PrintUsage();
		return false;
	}

	return true;
}

std::string Options::Personality()
{
	return s_Personality;
}

std::int32_t Options::Port()
{
	return s_Port;
}

std::string Options::IP()
{
	return s_IP;
}