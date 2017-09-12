#pragma once

#include <string>
#include <vector>

class Options
{
public:
	static const std::string PERSONALITY_CLIENT;
	static const std::string PERSONALITY_SERVER;

private:
	std::string m_Personality;
	std::int32_t m_Port;
	std::string m_IP;
	
public:
	Options();

	bool Process(std::vector<std::string> args);

	std::string Personality();
	std::int32_t Port();
	std::string IP();
};