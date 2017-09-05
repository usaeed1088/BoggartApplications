#include "Chat.h"

const std::string ChatMessage::s_Type("chat");

std::string ChatMessage::TypeString()
{
	return s_Type;
}

ChatMessage::ChatMessage(std::string message) :
	MessageBase(s_Type),
	m_Message(message) 
{

}

ChatMessage::ChatMessage(const std::vector<unsigned char>& data) :
	MessageBase(data) 
{ 
	if (Decode(data))
	{
		m_Valid = true;
	}
}

std::string ChatMessage::Message() 
{ 
	return m_Message; 
}

std::vector<unsigned char> ChatMessage::OnEncode()
{
	return std::vector<unsigned char>(m_Message.begin(), m_Message.end());
}

bool ChatMessage::OnDecode(const std::vector<unsigned char>& data)
{
	if (data.empty())
	{
		return false;
	}

	m_Message.clear();
	m_Message.insert(m_Message.end(), data.begin(), data.end());
	return true;
}