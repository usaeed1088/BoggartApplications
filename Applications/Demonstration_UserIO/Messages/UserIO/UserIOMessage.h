#pragma once

#include <Lib/Message/MessageBase.h>

class UserIOMessage : public Boggart::Message::MessageBase
{
public:
	static const std::string Command_Subscribe;
	static const std::string Command_Send;

private:
	static const std::string s_Type;

public:
	static std::string TypeString();

private:
	std::string m_Command;
	std::string m_Topic;
	std::string m_Data;

public:
	UserIOMessage(std::string command, std::string topic, std::string message);
	UserIOMessage(const std::vector<unsigned char>& data);

	std::string Command();
	std::string Topic();
	std::string Data();

private:
	std::vector<unsigned char> OnEncode() override;
	bool OnDecode(const std::vector<unsigned char>& data) override;
};