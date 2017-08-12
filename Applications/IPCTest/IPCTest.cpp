#include <Lib/Logger/Console/Console.h>
#include <Lib/Transport/InProcess/InProcess.h>
#include <Lib/IPC/Asynchronous/Asynchronous.h>
#include <Lib/Timer/Manager/SoftTimerManager/SoftTimerManager.h>

#include <Lib/IPC/Messages/Request/Request.h>
#include <Lib/IPC/Messages/Request/Subtypes.h>

#include <Lib/IPC/Messages/Response/Response.h>
#include <Lib/IPC/Messages/Response/Subtypes.h>

#include <Lib/Instantiator.h>

#include <sstream>

Boggart::IPC::IIPCPtr CreateIPC(std::string id, Boggart::Logger::ILoggerPtr logger, Boggart::Timer::IManagerPtr timer)
{
	std::shared_ptr<Boggart::IPC::Asynchronous> ipc(new Boggart::IPC::Asynchronous(id));
	std::shared_ptr<Boggart::Transport::InProcess> transport(new Boggart::Transport::InProcess(id));

	transport->InjectDependency(logger);
	transport->InjectDependency(timer);

	ipc->InjectDependency(logger);
	ipc->InjectDependency(timer);
	ipc->InjectDependency(transport);

	transport->Open();
	ipc->Start();

	return ipc;
}

void ProcessOutgoing(std::vector<Boggart::IPC::IIPCPtr>* ipcs)
{
	static std::int32_t index = 0;
	++index >= ipcs->size() ? index = 0 : index;

	std::shared_ptr<Boggart::IPC::Request> request(new Boggart::IPC::Request(Boggart::IPC::RequestSubtypes::KeepAlive));

	ipcs->at(index)->Send(std::string("2"), std::static_pointer_cast<Boggart::Message::IMessage>(request));
}

class Subscriber : public Boggart::IPC::Subscribable
{
public:
	Subscriber() :Boggart::IPC::Subscribable("Subscriber") {}

	void OnMessage(Boggart::Logger::ILoggerPtr logger, Boggart::Message::IMessagePtr message)
	{
		std::stringstream ss;
		ss << "Received " << message->Type() << " From " << message->Source();

		logger->Log(Boggart::Logger::Level::Debug, "Test", "Main", ss.str().c_str());
	}
};

int main(int argc, char* argv[])
{
	std::shared_ptr<Boggart::Timer::SoftTimerManager> timerManager(new Boggart::Timer::SoftTimerManager());
	std::shared_ptr<Boggart::Logger::Console> logger(new Boggart::Logger::Console());

	timerManager->InjectDependency(logger);

	logger->EnableModule(std::string("ConnectionManager"));

	const std::int32_t N = 3;

	std::vector<Boggart::IPC::IIPCPtr> ipcs;
	for (std::int32_t i = 0; i < N; i++)
	{
		static char c = 'a'; std::string id; id += c; c++;
		ipcs.push_back(CreateIPC(id, logger, timerManager));
	}

	std::function<void(void)> processOutgoing = std::bind(ProcessOutgoing, &ipcs);

	Boggart::Timer::IDevicePtr outgoingProcessorTimer = timerManager->Create
	(
		Boggart::Timer::Span_t(1000),
		Boggart::Timer::Type_t::Periodic,
		processOutgoing
	);

	//timerManager->Start(outgoingProcessorTimer);

	std::shared_ptr<Subscriber> subscriber(new Subscriber());
	std::function<void(Boggart::Message::IMessagePtr)> subscribed = std::bind(&Subscriber::OnMessage, subscriber, logger, std::placeholders::_1);
	
	for (std::int32_t i = 0; i < ipcs.size(); i++)
	{
		//ipcs[i]->SubscribeMessage(subscriber, Boggart::IPC::Request::TypeString(), subscribed);
	}

	while (true)
	{
		timerManager->Process();
		PAL::Instantiator::APIFactory()->GetTranceInstance()->Sleep(10);
	}

	return 0;
}