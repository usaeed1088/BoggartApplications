#pragma once

#include <string>

class Options
{
public:
	static const std::string PERSONALITY_CLIENT;
	static const std::string PERSONALITY_SERVER;

private:
	static std::string s_Personality;
	static std::int32_t s_Port;
	static std::string s_IP;

private:
	Options();

public:
	static bool Process(int argc, char* argv[]);

	static std::string Personality();
	static std::int32_t Port();
	static std::string IP();
};