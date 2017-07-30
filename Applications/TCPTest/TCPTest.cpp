#include <Transport/TCP/Client/Client.h>
#include <Transport/TCP/Server/Server.h>

#include <Logger/Console/Console.h>
#include <Timer/Manager/SoftTimerManager/SoftTimerManager.h>

#include <PAL/Lib/Instantiator.h>

#include <memory>
#include <string>
#include <cstdint>
#include <iostream>

Boggart::Transport::ITransportPtr CreateClient(std::string server, std::int32_t port, std::string id, Boggart::Logger::ILoggerPtr logger, Boggart::Timer::IManagerPtr timerManager)
{
	std::shared_ptr<Boggart::Transport::TCP::Client> client(new Boggart::Transport::TCP::Client(id, server, port));
	client->InjectDependencies(logger, timerManager);
	client->Open();
	return client;
}

Boggart::Transport::ITransportPtr CreateServer(std::int32_t port, std::string id, Boggart::Logger::ILoggerPtr logger, Boggart::Timer::IManagerPtr timerManager)
{
	std::shared_ptr<Boggart::Transport::TCP::Server> server(new Boggart::Transport::TCP::Server(id, port));
	server->InjectDependencies(logger, timerManager);
	server->Open();
	return server;
}

void ProcessServers(std::vector<Boggart::Transport::ITransportPtr>* servers)
{
	for (std::size_t i = 0; i < servers->size(); i++)
	{
		std::vector<unsigned char> data = servers->at(i)->Receive();
		if (data.empty())
		{
			continue;
		}

		// TODO: Print vector and id
		servers->at(i)->Send(data);
	}
}

void ProcessClients(std::vector<Boggart::Transport::ITransportPtr>* clients)
{
	for (std::size_t i = 0; i < clients->size(); i++)
	{
		std::vector<unsigned char> data = clients->at(i)->Receive();
		if (data.empty())
		{
			continue;
		}

		// TODO: Print vector and id
		std::cout << clients->at(i)->Id() << std::endl;
	}
}

void DataGenerator(std::vector<Boggart::Transport::ITransportPtr>* clients)
{
	static std::int32_t index = 0;
	index++; index >= clients->size() ? index = 0 : index;

	std::string data("1234567890");
	std::vector<unsigned char> vector(data.begin(), data.end());
	clients->at(index)->Send(vector);
}

int main(int argc, char* argv[])
{
	const std::int32_t N_CLIENTS = 5;
	const std::int32_t N_SERVERS = 2;

	std::shared_ptr<Boggart::Logger::Console> logger(new Boggart::Logger::Console());
	std::shared_ptr<Boggart::Timer::SoftTimerManager> timerManager(new Boggart::Timer::SoftTimerManager());

	timerManager->InjectDependencies(logger);

	std::vector<Boggart::Transport::ITransportPtr> servers;
	std::vector<Boggart::Transport::ITransportPtr> clients;

	for (std::int32_t i = 0; i < N_SERVERS; i++)
	{
		static std::int32_t port = 9990;
		static char c = 'a'; std::string id;	id += c++;
		servers.push_back(CreateServer(port, id, logger, timerManager));

		for (std::int32_t j = 0; j < N_CLIENTS; j++)
		{
			std::string id_; id_ += c++;
			clients.push_back(CreateClient(std::string("127.0.0.1"), port, id_, logger, timerManager));
		}

		port++;
	}

	std::function<void(void)> serverProcessor = std::bind(ProcessServers, &servers);
	std::function<void(void)> clientProcessor = std::bind(ProcessClients, &clients);
	std::function<void(void)> dataGenerator = std::bind(DataGenerator, &clients);

	Boggart::Timer::IDevicePtr serverProcessorTimer = timerManager->Create
	(
		Boggart::Timer::Span_t(50),
		Boggart::Timer::Type_t::Periodic,
		serverProcessor
	);

	Boggart::Timer::IDevicePtr clientProcessorTimer = timerManager->Create
	(
		Boggart::Timer::Span_t(50),
		Boggart::Timer::Type_t::Periodic,
		clientProcessor
	);

	Boggart::Timer::IDevicePtr dataGeneratorTimer = timerManager->Create
	(
		Boggart::Timer::Span_t(1000),
		Boggart::Timer::Type_t::Periodic,
		dataGenerator
	);

	timerManager->Start(serverProcessorTimer);
	timerManager->Start(clientProcessorTimer);
	timerManager->Start(dataGeneratorTimer);

	while (true)
	{
		timerManager->Process();
		PAL::Instantiator::APIFactory()->GetTranceInstance()->Sleep(10);
	}

	return 0;
}