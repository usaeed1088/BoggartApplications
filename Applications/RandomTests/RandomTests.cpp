#include <Lib/Boggart/Boggart.h>

#include <string>
#include <cstdint>
#include <thread>

#include <iostream>

const std::string SERVER_IP("127.0.0.1");
const std::int32_t SERVER_PORT(9990);

class ChatMessage : public Boggart::Message::MessageBase
{
private:
	std::string m_Message;

public:
	ChatMessage(std::string message) : MessageBase(std::string("chat")), m_Message(message) {}
	ChatMessage(const std::vector<unsigned char>& data) : MessageBase(data) { Decode(data); }

	std::string Message() { return m_Message; }

private:
	std::vector<unsigned char> OnEncode() override
	{
		return std::vector<unsigned char>(m_Message.begin(), m_Message.end());
	}

	bool OnDecode(const std::vector<unsigned char>& data) override
	{
		if (data.empty())
		{
			return false;
		}

		m_Message.clear();
		m_Message.insert(m_Message.end(), data.begin(), data.end());
		return true;
	}
};

class Subscriber : public Boggart::IPC::Subscribable
{
public:
	Subscriber() :Subscribable("Susbcriber") {}

	void OnMessage(std::string source, std::string destination, std::vector<unsigned char> data)
	{
		std::shared_ptr<ChatMessage> chatMessage = std::make_shared<ChatMessage>(data);
		std::cout << source << " -> " << destination << " :: " << chatMessage->Message() << std::endl;
	}
};

void SendMessage(Boggart::BoggartPtr boggart)
{
	std::shared_ptr<ChatMessage> chatMessage(new ChatMessage("Hey There"));
	boggart->IPC()->Send("Client2", chatMessage);
}

Boggart::BoggartPtr CreateBoggart(std::string id, std::shared_ptr<Boggart::Transport::TransportBase> transport)
{
	std::shared_ptr<Boggart::Boggart> boggart(new Boggart::Boggart("Mr.Server"));

	boggart->InjectIPC(std::shared_ptr<Boggart::IPC::IPCBase>(new Boggart::IPC::Asynchronous(id)));
	boggart->InjectLogger(std::shared_ptr<Boggart::Logger::LoggerBase>(new Boggart::Logger::Console()));
	boggart->InjectTransport(transport);
	boggart->InjectTimerManager(std::shared_ptr<Boggart::Timer::ManagerBase>(new Boggart::Timer::SoftTimerManager()));

	boggart->Logger()->EnableModule(std::string("ConnectionManager"));

	return boggart;
}

int main(int argc, char* argv[])
{
	// Setup Clients and a Server
	std::shared_ptr<Boggart::Transport::TransportBase> serverTransport(new Boggart::Transport::TCP::Server("Server", SERVER_PORT));
	Boggart::BoggartPtr server = CreateBoggart("Server", serverTransport);

	std::shared_ptr<Boggart::Transport::TransportBase> client1Transport(new Boggart::Transport::TCP::Client("Client1", SERVER_IP, SERVER_PORT));
	Boggart::BoggartPtr client1 = CreateBoggart("Client1", client1Transport);

	std::shared_ptr<Boggart::Transport::TransportBase> client2Transport(new Boggart::Transport::TCP::Client("Client2", SERVER_IP, SERVER_PORT));
	Boggart::BoggartPtr client2 = CreateBoggart("Client2", client2Transport);

	// Setup subscriber
	std::shared_ptr<Boggart::Transport::TransportBase> subscriberTransport(new Boggart::Transport::TCP::Client("Subscriber", SERVER_IP, SERVER_PORT));
	Boggart::BoggartPtr subscriber = CreateBoggart("Subscriber", subscriberTransport);
	std::shared_ptr<Subscriber> subscription(new Subscriber());
	subscriber->IPC()->SubscribeMessage(subscription, "chat", std::bind(&Subscriber::OnMessage, subscription, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	Boggart::Timer::IDevicePtr messageSendingTimer = subscriber->TimerManager()->Create
	(
		Boggart::Timer::Span_t(1000),
		Boggart::Timer::Type_t::Periodic,
		[client1, client2]() 
		{
			Boggart::BoggartPtr boggart;
			static int i = 0;
			if ((i++ % 2) == 0)
			{
				boggart = client1;
			}
			else
			{
				boggart = client2;
			}
			SendMessage(boggart);
		}
	);

	subscriber->TimerManager()->Start(messageSendingTimer);

	Boggart::Boggart::Start();

	return 0;
}