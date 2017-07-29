#include <Boggart/Lib/IPC/Asynchronous/Asynchronous.h>
#include <Boggart/Lib/Logger/Console/Console.h>
#include <Boggart/Lib/Transport/InProcess/InProcess.h>
#include <Boggart/Lib/Timer/Manager/SoftTimerManager/SoftTimerManager.h>

#include <PAL/Lib/PAL.h>

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

void ProcessIncoming(std::vector<Boggart::Transport::ITransportPtr>& transports)
{
	for (Boggart::Transport::ITransportPtr transport : transports)
	{
		std::vector<unsigned char> data = transport->Receive();
		if (!data.empty())
		{
			std::cout << transport->Id() << " Received ";
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
	std::shared_ptr<Boggart::Logger::Console> logger(new Boggart::Logger::Console());
	std::shared_ptr<Boggart::Timer::SoftTimerManager> timerManager(new Boggart::Timer::SoftTimerManager());	
	timerManager->InjectDependencies(logger, nullptr);

	std::vector<Boggart::Transport::ITransportPtr> transports;

	std::function<void(void)> f = std::bind(ProcessOutgoing, &transports);

	Boggart::Timer::IDevicePtr outgoingProcessingTimer =
		timerManager->Create(
			Boggart::Timer::Span_t(1000),
			Boggart::Timer::Type_t::Periodic,
			f);
	timerManager->Start(outgoingProcessingTimer);

	const std::int32_t N = 5;

	char c = '1';
	for (int i = 0; i < N; i++)
	{
		std::string id;
		id += c++;
		transports.push_back(CreateTransport(id, logger, timerManager));
	}

	while (true)
	{
		ProcessIncoming(transports);
		timerManager->Process();
		PAL::PAL::Instance()->Sleep(100);
	}

	return 0;
}