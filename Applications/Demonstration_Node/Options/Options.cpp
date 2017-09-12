#include "Options.h"

#include <sstream>
#include <functional>
#include <iostream>

const std::string Options::PERSONALITY_CLIENT("client");
const std::string Options::PERSONALITY_SERVER("server");

Options::Options():
	m_Personality(),
	m_IP(),
	m_Port()
{

}

bool Options::Process(std::vector<std::string> args)
{
	std::function<void()> PrintUsage = []()
	{
		std::cout << "Demonstration_Node " << PERSONALITY_SERVER << " <port>" << std::endl;
		std::cout << "Demonstration_Node " << PERSONALITY_CLIENT << " <server_ip> <server_port>" << std::endl;
	};

	if (args.size() < 3)
	{
		PrintUsage();
		return false;
	}

	m_Personality = args[1];

	if (m_Personality == PERSONALITY_SERVER)
	{
		std::stringstream ss;
		ss << args[2];
		ss >> std::dec >> m_Port;
	}
	else if (m_Personality == PERSONALITY_CLIENT)
	{
		if (args.size() < 4)
		{
			PrintUsage();
			return false;
		}

		m_IP = args[2];

		std::stringstream ss;
		ss << args[3];
		ss >> std::dec >> m_Port;
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
	return m_Personality;
}

std::int32_t Options::Port()
{
	return m_Port;
}

std::string Options::IP()
{
	return m_IP;
}