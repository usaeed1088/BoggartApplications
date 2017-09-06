#include <Lib/Boggart/Boggart.h>
#include <Lib/Instantiator.h>

#include "Messages/Chat/Chat.h"
#include "Messages/Generic/Generic.h"

#include "../../Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.h"

#include "Options/Options.h"

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

std::string Query(std::string query)
{
	std::cout << query << " : " << std::flush;
	std::string response;
	std::getline(std::cin, response);
	return response;
}

int main(int argc, char* argv[])
{
	if (!Options::Process(argc, argv))
	{
		return 0;
	}

	std::string name = Query("Give me a name, please? ");
	std::cout << name << ". Seriously?" << std::endl;

	Boggart::BoggartPtr boggart(new Boggart::Boggart(name));

	std::shared_ptr<Boggart::Transport::TransportBase> transport = nullptr;

	if (Options::Personality() == Options::PERSONALITY_CLIENT)
	{
		transport = std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::TCP::Client(name, Options::IP(), Options::Port()));
	}
	else if (Options::Personality() == Options::PERSONALITY_SERVER)
	{
		transport = std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::TCP::Server(name, Options::Port()));
	}

	std::function<void(std::string, std::string, std::vector<unsigned char>)> OnUserIOMessage =
		[boggart, name](std::string source, std::string destination, std::vector<unsigned char> data)
	{
		std::shared_ptr<UserIOMessage> message(new UserIOMessage(data));
		if (!message->Valid())
		{
			return;
		}

		if (message->Destination() == name) // If it''s for me?
		{
			if (message->Command() == UserIOMessage::Command_Subscribe)
			{
				boggart->SubscribeMessage(message->Topic(), std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			}
			else if (message->Command() == UserIOMessage::Command_Send)
			{
				Boggart::Message::IMessagePtr message(new GenericMessage(message->Topic(), message->Data()));
				boggart->Send(Boggart::IPC::DestinationAny, message);
			}
		}
		else // If it's for someone else
		{
			boggart->Send(message->Destination(), message);
		}
		
	};

	boggart->InjectTransport(transport);

	boggart->SubscribeMessage(UserIOMessage::TypeString(), OnUserIOMessage);

	Boggart::Boggart::Start();

	return 0;
}
