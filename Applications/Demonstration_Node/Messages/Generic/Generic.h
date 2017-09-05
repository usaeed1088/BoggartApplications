#pragma once

#include <Lib/Message/MessageBase.h>

class GenericMessage : public Boggart::Message::MessageBase
{
private:
	std::string m_Payload;

public:
	GenericMessage(std::string type, std::string payload);
	GenericMessage(const std::vector<unsigned char>& data);

	std::string Payload();

private:
	std::vector<unsigned char> OnEncode() override;
	bool OnDecode(const std::vector<unsigned char>& data) override;
};