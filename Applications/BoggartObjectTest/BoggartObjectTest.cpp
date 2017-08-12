#include <Lib/Boggart/Boggart.h>

const std::string IP = "127.0.0.1";
const std::int32_t Port = 9998;

std::shared_ptr<Boggart::Boggart> CreateServer(std::string id)
{
	std::shared_ptr<Boggart::Boggart> boggart(new Boggart::Boggart("Mr.Server"));

	boggart->InjectIPC(std::shared_ptr<Boggart::IPC::IPCBase>(new Boggart::IPC::Asynchronous(id)));
	boggart->InjectLogger(std::shared_ptr<Boggart::Logger::LoggerBase>(new Boggart::Logger::Console()));
	boggart->InjectTransport(std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::TCP::Server(IP, Port)));
	boggart->InjectTimerManager(std::shared_ptr<Boggart::Timer::ManagerBase>(new Boggart::Timer::SoftTimerManager()));

	boggart->Logger()->EnableModule(std::string("ConnectionManager"));

	return boggart;
}

std::shared_ptr<Boggart::Boggart> CreateClient(std::string id)
{
	std::shared_ptr<Boggart::Boggart> boggart(new Boggart::Boggart("Mr.Server"));

	boggart->InjectIPC(std::shared_ptr<Boggart::IPC::IPCBase>(new Boggart::IPC::Asynchronous(id)));
	boggart->InjectLogger(std::shared_ptr<Boggart::Logger::LoggerBase>(new Boggart::Logger::Console()));
	boggart->InjectTransport(std::shared_ptr<Boggart::Transport::TransportBase>(new Boggart::Transport::TCP::Client(id, IP, Port)));
	boggart->InjectTimerManager(std::shared_ptr<Boggart::Timer::ManagerBase>(new Boggart::Timer::SoftTimerManager()));

	boggart->Logger()->EnableModule(std::string("ConnectionManager"));
	boggart->Logger()->EnableCategory(std::string("Transport"));

	return boggart;
}

int main(int argc, char* argv[])
{
	auto server = CreateServer("23");

	auto client1 = CreateClient("90");
	auto client2 = CreateClient("93");

	server->InjectBoggart(client1);
	server->InjectBoggart(client2);

	server->Start();

	return 0;
}