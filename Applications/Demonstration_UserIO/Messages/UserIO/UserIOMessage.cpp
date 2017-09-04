#include "UserIOMessage.h"

#include <Message/Utility/Utility.h>

const std::string UserIOMessage::Command_Subscribe("sub");
const std::string UserIOMessage::Command_Send("send");

const std::string UserIOMessage::s_Type("userio");

std::string UserIOMessage::TypeString()
{
	return s_Type;
}

UserIOMessage::UserIOMessage(std::string command, std::string topic, std::string data) :
	MessageBase(s_Type),
	m_Command(command),
	m_Topic(topic),
	m_Data(data)
{

}

UserIOMessage::UserIOMessage(const std::vector<unsigned char>& data) :
	MessageBase(data)
{
	if (Decode(data))
	{
		m_Valid = true;
	}
}

std::string UserIOMessage::Command()
{
	return m_Command;
}

std::string UserIOMessage::Topic()
{
	return m_Topic;
}

std::string UserIOMessage::Data()
{
	return m_Data;
}

std::vector<unsigned char> UserIOMessage::OnEncode()
{
	std::vector<unsigned char> data;

	Boggart::Message::Utility::EncodeString(m_Command, data);
	Boggart::Message::Utility::EncodeString(m_Topic, data);
	Boggart::Message::Utility::EncodeString(m_Data, data);

	return data;
}

bool UserIOMessage::OnDecode(const std::vector<unsigned char>& data)
{
	if (data.empty())
	{
		return false;
	}

	std::vector<unsigned char> _data = data;

	Boggart::Message::Utility::DecodeString(m_Command, _data);
	Boggart::Message::Utility::DecodeString(m_Topic, _data);
	Boggart::Message::Utility::DecodeString(m_Data, _data);
	
	return true;
}