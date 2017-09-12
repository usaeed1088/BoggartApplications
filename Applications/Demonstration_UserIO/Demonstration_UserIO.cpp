#include <Lib/Boggart/Boggart.h>
#include <Lib/Instantiator.h>

#include "UserIO/UserIO.h"
#include "../../Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.h"

#include <mutex>
#include <queue>
#include <thread>
#include <functional>

std::mutex lock;
std::queue<Boggart::Message::IMessagePtr> messageQueue;

void Task_UserIO()
{
	std::function<void(Boggart::Message::IMessagePtr)> PushMessage = [](Boggart::Message::IMessagePtr message)
	{
		std::lock_guard<std::mutex> guard(lock);
		messageQueue.push(message);
	};

	UserIO::Handler onSubscriptionCommand = [PushMessage](std::string destination, std::string topic, std::string data)
	{
		Boggart::Message::IMessagePtr message(new UserIOMessage(destination, UserIOMessage::Command_Subscribe, topic, data));
		PushMessage(message);
	};

	UserIO::Handler onSendCommand = [PushMessage](std::string destination, std::string topic, std::string data)
	{
		Boggart::Message::IMessagePtr message(new UserIOMessage(destination, UserIOMessage::Command_Send, topic, data));
		PushMessage(message);
	};

	UserIO::RegisterCommandHandler(UserIOMessage::Command_Subscribe, onSubscriptionCommand);
	UserIO::RegisterCommandHandler(UserIOMessage::Command_Send, onSendCommand);

	while (true)
	{
		UserIO::Process();
	}
}

void Task_Boggart()
{
	Boggart::BoggartPtr boggart(new Boggart::Boggart("user_io_boggart"));
	std::shared_ptr<Boggart::Timer::ManagerBase> timer(new Boggart::Timer::SoftTimerManager());

	Boggart::Timer::IDevicePtr sendTimer = timer->Create
	(
		Boggart::Timer::Span_t(50),
		Boggart::Timer::Type_t::Periodic,
		[boggart]()
		{
			if (messageQueue.empty())
			{
				return;
			}

			std::lock_guard<std::mutex> guard(lock);
			Boggart::Message::IMessagePtr message = messageQueue.front();
			boggart->Send(Boggart::IPC::DestinationAny, message);
			messageQueue.pop();
		}
	);

	timer->Start(sendTimer);

	std::shared_ptr<Boggart::Transport::TransportBase> client(new Boggart::Transport::UDP::Client("user_io_boggart", "127.0.0.1", 9998));

	boggart->InjectTimerManager(timer);
	boggart->InjectTransport(client);

	Boggart::Boggart::Start();
}

int main(int argc, char* argv[])
{
	std::thread task_userio(Task_UserIO);
	std::thread task_boggart(Task_Boggart);

	task_userio.join();
	task_boggart.join();

	return 0;
}