#include <Lib/Boggart/Boggart.h>
#include <Lib/Instantiator.h>

#include "../../Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.h"

#include "Options/Options.h"

#include <iostream>
#include <map>

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

void OnUserIOMessage(Boggart::BoggartPtr boggart, std::string source, std::string destination, std::vector<unsigned char> data)
{
	std::shared_ptr<UserIOMessage> message(new UserIOMessage(data));
	if (!message->Valid() || message->Destination() != boggart->Name()) // If message is invalid or is not for me
	{
		return;
	}

	if (message->Command() == UserIOMessage::Command_Subscribe)
	{
		boggart->SubscribeMessage(message->Topic(), std::bind(OnMessage, boggart->Name(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	else if (message->Command() == UserIOMessage::Command_Send)
	{
		std::string userData = message->Data(); std::vector<unsigned char> payload(userData.begin(), userData.end());
		Boggart::Message::IMessagePtr imessage(new Boggart::Message::Type::Generic(message->Topic(), payload));
		boggart->Send(Boggart::IPC::DestinationAny, imessage);
	}
}

bool SetupForPC(std::shared_ptr<Options> options)
{
	// Needs one TCP Server
	std::string name = Query("Give me a name, please? ");
	std::cout << name << "??? Seriously... ???" << std::endl;

	std::shared_ptr<Boggart::Transport::TransportBase> transport(new Boggart::Transport::TCP::Server(name, options->TCPPort()));

	static Boggart::BoggartPtr boggart(new Boggart::Boggart(name));
	boggart->InjectTransport(transport);
	boggart->SubscribeMessage(UserIOMessage::TypeString(), std::bind(OnUserIOMessage, boggart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return true;
}

bool SetupForVM(std::shared_ptr<Options> options)
{
	// Needs one TCP Client
	// Needs one UDP Server
	std::string name = Query("Give me a name, please? ");
	std::cout << name << "??? Seriously... ???" << std::endl;

	// Setup bridged boggart
	std::shared_ptr<Boggart::Transport::TransportBase> tcpClient(new Boggart::Transport::TCP::Client(name, options->IP(), options->TCPPort()));
	std::shared_ptr<Boggart::Transport::TransportBase> udpServer(new Boggart::Transport::UDP::Server(name, options->UDPPort()));
	std::shared_ptr<Boggart::Transport::TransportBase> inProc(new Boggart::Transport::InProcess(name));

	std::shared_ptr<Boggart::Transport::Bridge> bridge(new Boggart::Transport::Bridge());
	bridge->InjectTransport(tcpClient);
	bridge->InjectTransport(udpServer);
	bridge->InjectTransport(inProc);

	static Boggart::BoggartPtr bridgedBoggart(new Boggart::Boggart(name + "Bridged"));
	bridgedBoggart->InjectTransport(bridge);

	// Setup normal boggart
	std::shared_ptr<Boggart::Transport::TransportBase> transport(new Boggart::Transport::InProcess(name));
	static Boggart::BoggartPtr boggart(new Boggart::Boggart(name));
	boggart->InjectTransport(transport);

	boggart->SubscribeMessage(UserIOMessage::TypeString(), std::bind(OnUserIOMessage, boggart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return true;
}

bool SetupForPi(std::shared_ptr<Options> options)
{
	// Needs one TCP Server
	std::string name = Query("Give me a name, please? ");
	std::cout << name << "??? Seriously... ???" << std::endl;

	std::shared_ptr<Boggart::Transport::TransportBase> transport(new Boggart::Transport::UDP::Client(name, options->IP(), options->UDPPort()));

	static Boggart::BoggartPtr boggart(new Boggart::Boggart(name));
	boggart->InjectTransport(transport);
	boggart->SubscribeMessage(UserIOMessage::TypeString(), std::bind(OnUserIOMessage, boggart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	return true;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	std::map<std::string, std::function<bool(std::shared_ptr<Options>)>> PersonalitySetupLUT;

	PersonalitySetupLUT[Options::PERSONALITY_PC] = std::bind(SetupForPC, std::placeholders::_1);
	PersonalitySetupLUT[Options::PERSONALITY_VM] = std::bind(SetupForVM, std::placeholders::_1);
	PersonalitySetupLUT[Options::PERSONALITY_PI] = std::bind(SetupForPi, std::placeholders::_1);

	std::shared_ptr<Options> options(new Options());
	if (!options->Process(args))
	{
		return 0;
	}

	PersonalitySetupLUT[options->Personality()](options);

	Boggart::Boggart::Start();

	return 0;
}
