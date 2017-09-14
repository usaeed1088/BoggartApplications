#pragma once

#include <string>
#include <vector>

class Options
{
public:
	static const std::string PERSONALITY_PC;
	static const std::string PERSONALITY_VM;
	static const std::string PERSONALITY_PI;

private:
	std::string m_Personality;
	std::int32_t m_TCPPort;
	std::int32_t m_UDPPort;
	std::string m_IP;
	
public:
	Options();
	Options(std::string personality, std::int32_t tcpPort, std::int32_t udpPort, std::string ip);

	bool Process(std::vector<std::string> args);

	std::string Personality();
	std::int32_t TCPPort();
	std::int32_t UDPPort();
	std::string IP();
};