#include <Boggart/Lib/Transport/InProcess/InProcess.h>

#include <Boggart/Lib/Logger/Console/Console.h>
#include <Boggart/Lib/Timer/Manager/SoftTimerManager/SoftTimerManager.h>

#include <PAL/Lib/Instantiator.h>

#include <iostream>

Boggart::Transport::ITransportPtr CreateTransport(std::string id, Boggart::Logger::ILoggerPtr logger, Boggart::Timer::IManagerPtr timers)
{
	std::shared_ptr<Boggart::Transport::InProcess> transport(new Boggart::Transport::InProcess(id));
	transport->InjectDependencies(logger, timers);
	transport->Open();
	return transport;
}

void PrintVector(std::vector<unsigned char> data)
{
	std::cout << "Data : [";
	for (unsigned char c : data)
	{
		std::cout << std::hex << (std::int16_t)c << " ";
	}
	std::cout << "]" << std::endl;
}

void ProcessIncoming(std::vector<Boggart::Transport::ITransportPtr>* transports)
{
	for (std::int32_t i = 0; i<transports->size(); i++)
	{
		std::vector<unsigned char> data = transports->at(i)->Receive();
		if (!data.empty())
		{
			std::cout << transports->at(i)->Id() << " Received ";
			PrintVector(data);
		}
	}
}

void ProcessOutgoing(std::vector<Boggart::Transport::ITransportPtr>* transports)
{
	static std::int32_t index = 0;
	++index >= transports->size() ? index = 0 : index;

	std::string string("1234567890");
	std::vector<unsigned char> data(string.begin(), string.end());

	std::cout << transports->at(index)->Id() << " Sending ";
	PrintVector(data);
	transports->at(index)->Send(data);
}

int main(int argc, char* argv[])
{
	Boggart::Logger::ILoggerPtr logger(new Boggart::Logger::Console());
	std::shared_ptr<Boggart::Timer::SoftTimerManager> timerManager(new Boggart::Timer::SoftTimerManager());

	timerManager->InjectDependencies(logger);

	std::vector<Boggart::Transport::ITransportPtr> transports;

	std::function<void(void)> processOutgoing = std::bind(ProcessOutgoing, &transports);
	std::function<void(void)> processIncoming = std::bind(ProcessIncoming, &transports);

	Boggart::Timer::IDevicePtr outgoingProcessingTimer =
		timerManager->Create
		(
			Boggart::Timer::Span_t(1000),
			Boggart::Timer::Type_t::Periodic,
			processOutgoing
		);

	Boggart::Timer::IDevicePtr incomingProcessingTimer =
		timerManager->Create
		(
			Boggart::Timer::Span_t(100),
			Boggart::Timer::Type_t::Periodic,
			processIncoming
		);

	timerManager->Start(outgoingProcessingTimer);
	timerManager->Start(incomingProcessingTimer);

	const std::int32_t N = 2;

	char c = '1';
	for (int i = 0; i < N; i++)
	{
		std::string id;
		id += c++;
		transports.push_back(CreateTransport(id, logger, timerManager));
	}

	while (true)
	{
		timerManager->Process();
		PAL::Instantiator::APIFactory()->GetTranceInstance()->Sleep(10);
	}

	return 0;
}