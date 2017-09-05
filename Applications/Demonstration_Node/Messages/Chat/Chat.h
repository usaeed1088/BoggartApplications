#pragma once

#include <Lib/Message/MessageBase.h>

class ChatMessage : public Boggart::Message::MessageBase
{
private:
	static const std::string s_Type;

public:
	static std::string TypeString();

private:
	std::string m_Message;

public:
	ChatMessage(std::string message);
	ChatMessage(const std::vector<unsigned char>& data);

	std::string Message();

private:
	std::vector<unsigned char> OnEncode() override;
	bool OnDecode(const std::vector<unsigned char>& data) override;
};