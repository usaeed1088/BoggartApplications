#include "Generic.h"

#include <Message/Utility/Utility.h>

GenericMessage::GenericMessage(std::string type, std::string payload) :
	MessageBase(type),
	m_Payload(payload)
{

}

GenericMessage::GenericMessage(const std::vector<unsigned char>& data) :
	MessageBase(data)
{
	if (Decode(data))
	{
		m_Valid = true;
	}
}

std::string GenericMessage::Payload()
{
	return m_Payload;
}

std::vector<unsigned char> GenericMessage::OnEncode()
{
	std::vector<unsigned char> data;

	Boggart::Message::Utility::EncodeString(m_Payload, data);

	return data;
}

bool GenericMessage::OnDecode(const std::vector<unsigned char>& data)
{
	if (data.empty())
	{
		return false;
	}

	std::vector<unsigned char> _data = data;

	Boggart::Message::Utility::DecodeString(m_Payload, _data);

	return true;
}