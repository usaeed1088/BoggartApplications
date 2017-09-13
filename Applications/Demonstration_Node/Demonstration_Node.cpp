#include <Lib/Boggart/Boggart.h>
#include <Lib/Instantiator.h>

#include "Messages/Chat/Chat.h"

#include "../../Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.h"

#include "Options/Options.h"

#include <iostream>

void OnMessage(std::string me, std::string source, std::string destination, std::vector<unsigned char> data)
{
	Boggart::Message::Type::Generic message(data);
	if (!message.Valid())
	{
		return;
	}

	std::string format = PAL::Instantiator::APIFactory()->GetTimeInstance()->TimeFormat();
	std::string timestamp = PAL::Instantiator::APIFactory()->GetTimeInstance()->HumanReadableTimestamp(format, false);

	std::vector<unsigned char> payload = message.Payload();
	std::string string(payload.begin(), payload.end());

	std::cout << timestamp << " [" << me << "] received [" << message.Type() << "] from [" << source << "] : " << string << std::endl;
}

std::string Query(std::string query)
{
	std::cout << query << " : " << std::flush;
	std::string response;
	std::getline(std::cin, response);
	return response;
}

bool CreateBoggart(std::vector<std::string> args)
{
	std::shared_ptr<Options> options(new Options());
	if (!options->Process(args))
	{
		return false;
	}

	std::string name = Query("Give me a name, please? ");
	std::cout << name << ". Seriously?" << std::endl;

	Boggart::BoggartPtr boggart(new Boggart::Boggart(name));

	std::shared_ptr<Boggart::Transport::TransportBase> transport = nullptr;

	if (options->Personality() == Options::PERSONALITY_CLIENT)
	{
		transport = std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::UDP::Client(name, options->IP(), options->Port()));
	}
	else if (options->Personality() == Options::PERSONALITY_SERVER)
	{
		transport = std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::UDP::Server(name, options->Port()));
	}

	std::function<void(std::string, std::string, std::vector<unsigned char>)> OnUserIOMessage =
		[boggart, name, options](std::string source, std::string destination, std::vector<unsigned char> data)
	{
		std::shared_ptr<UserIOMessage> message(new UserIOMessage(data));
		if (!message->Valid() || message->Destination() != name) // If message is invalid or is not for me
		{
			return;
		}

		if (message->Command() == UserIOMessage::Command_Subscribe)
		{
			boggart->SubscribeMessage(message->Topic(), std::bind(OnMessage, name, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}
		else if (message->Command() == UserIOMessage::Command_Send)
		{
			std::string userData = message->Data(); std::vector<unsigned char> payload(userData.begin(), userData.end());
			Boggart::Message::IMessagePtr imessage(new Boggart::Message::Type::Generic(message->Topic(), payload));
			boggart->Send(Boggart::IPC::DestinationAny, imessage);
		}
	};

	boggart->InjectTransport(transport);
	boggart->SubscribeMessage(UserIOMessage::TypeString(), OnUserIOMessage);

	 return true;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	const std::string Port("9998");
	const std::string IP("127.0.0.1");

	if(!CreateBoggart(args))
	{
		return 0;
	}

	/*args[0] = ""; args[1] = "server"; args[2] = Port;
	CreateBoggart(args);

	args[0] = ""; args[1] = "client"; args[2] = IP; args[3] = Port;
	CreateBoggart(args);

	args[0] = ""; args[1] = "client"; args[2] = IP; args[3] = Port;
	CreateBoggart(args);

	args[0] = ""; args[1] = "client"; args[2] = IP; args[3] = Port;
	CreateBoggart(args);*/

	Boggart::Boggart::Start();

	return 0;
}
