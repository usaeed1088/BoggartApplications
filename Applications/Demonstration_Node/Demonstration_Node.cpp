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
	Boggart::BoggartPtr boggart(new Boggart::Boggart("some_boggart_client"));

	std::shared_ptr<Boggart::Transport::TransportBase> client(new Boggart::Transport::TCP::Client("some_boggart_client", "127.0.0.1", 9998));
	boggart->InjectTransport(client);

	boggart->SubscribeMessage(ChatMessage::TypeString(), std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	Boggart::Boggart::Start();

	return 0;
}
