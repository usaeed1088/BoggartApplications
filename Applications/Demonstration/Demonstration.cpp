#include <Lib/Boggart/Boggart.h>
#include <Lib/Instantiator.h>

#include "Messages/Chat/Chat.h"
#include "Messages/Generic/Generic.h"

#include "../../Applications/Demonstration_UserIO/Messages/UserIO/UserIOMessage.h"

#include <iostream>

void OnMessage(std::string source, std::string destination, std::vector<unsigned char> data)
{
	ChatMessage message(data);
	if (!message.Valid())
	{
		return;
	}

	std::string format = PAL::Instantiator::APIFactory()->GetTimeInstance()->TimeFormat();
	std::string timestamp = PAL::Instantiator::APIFactory()->GetTimeInstance()->HumanReadableTimestamp(format, false);

	std::cout << timestamp << " " << source << " -> " << destination << " : " << message.Message() << std::endl;
}

int main(int argc, char* argv[])
{
	Boggart::BoggartPtr boggart(new Boggart::Boggart("some_boggart"));

	std::function<void(std::string, std::string, std::vector<unsigned char>)> OnUserIOMessage =
		[boggart](std::string source, std::string destination, std::vector<unsigned char> data)
	{
		UserIOMessage message(data);
		if (!message.Valid())
		{
			return;
		}

		if (message.Command() == UserIOMessage::Command_Subscribe)
		{
			boggart->SubscribeMessage(message.Topic(), std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}
		else if (message.Command() == UserIOMessage::Command_Send)
		{
			Boggart::Message::IMessagePtr message(new GenericMessage(message.Topic(), message.Data()));
			boggart->Send(Boggart::IPC::DestinationAny, message);
		}
	};

	boggart->SubscribeMessage(UserIOMessage::TypeString(), OnUserIOMessage);

	std::shared_ptr<Boggart::Transport::TransportBase> server(new Boggart::Transport::TCP::Server("some_boggart", 9998));
	boggart->InjectTransport(server);

	Boggart::Boggart::Start();

	return 0;
}