#include "Options.h"

#include <sstream>
#include <functional>
#include <iostream>

const std::string Options::PERSONALITY_PC("PC");
const std::string Options::PERSONALITY_VM("VM");
const std::string Options::PERSONALITY_PI("PI");

Options::Options():
	m_Personality(),
	m_TCPPort(),
	m_UDPPort(),
	m_IP()
{

}

Options::Options(std::string personality, std::int32_t tcpPort, std::int32_t udpPort, std::string ip):
	m_Personality(personality),
	m_TCPPort(tcpPort),
	m_UDPPort(udpPort),
	m_IP(ip)
{

}

bool Options::Process(std::vector<std::string> args)
{
	std::function<void()> PrintUsage = []()
	{
		std::cout << "Demonstration_Node " << PERSONALITY_PC << " <port>" << std::endl;
		std::cout << "Demonstration_Node " << PERSONALITY_VM << " <server_ip> <server_port> <udp_port>" << std::endl;
		std::cout << "Demonstration_Node " << PERSONALITY_PI << " <udp_host_ip> <udp_host_port>" << std::endl;
	};

	if (args.size() < 3)
	{
		PrintUsage();
		return false;
	}

	m_Personality = args[1];

	if (m_Personality == PERSONALITY_PC)
	{
		std::stringstream ss;
		ss << args[2];
		ss >> std::dec >> m_TCPPort;
	}
	else if (m_Personality == PERSONALITY_VM)
	{
		if (args.size() < 5)
		{
			PrintUsage();
			return false;
		}

		m_IP = args[2];

		std::stringstream ss;
		ss << args[3];
		ss >> std::dec >> m_TCPPort;

		std::stringstream ss2;
		ss2 << args[4];
		ss2 >> std::dec >> m_UDPPort;
	}
	else if (m_Personality == PERSONALITY_PI)
	{
		if (args.size() < 4)
		{
			PrintUsage();
			return false;
		}

		m_IP = args[2];

		std::stringstream ss;
		ss << args[3];
		ss >> std::dec >> m_UDPPort;
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

std::int32_t Options::TCPPort()
{
	return m_TCPPort;
}

std::int32_t Options::UDPPort()
{
	return m_UDPPort;
}

std::string Options::IP()
{
	return m_IP;
}