#include "UserIO.h"

#include <iostream>
#include <vector>
#include <string>

std::map<std::string, UserIO::Handler> UserIO::s_CommandHandlers;

UserIO::UserIO()
{

}

UserIO::~UserIO()
{

}

void UserIO::RegisterCommandHandler(std::string command, Handler handler)
{
	s_CommandHandlers[command] = handler;
}

void UserIO::Process()
{
	PrintMenu();
	std::string response = Query("Enter command");

	ProcessResponse(response);
}

void UserIO::PrintMenu()
{
	Display("You can execute for any Boggart: ");

	Display("Subscribe (sub)");
	Display("Send Data (send)");

	Display("");

	Display("Example: <destination> sub <topic>");
	Display("Example: <destination> send <topic> <data>");
}

std::string UserIO::Query(std::string query)
{
	std::cout << query << " : " << std::flush;
	std::string response;
	std::getline(std::cin, response);
	return response;
}

void UserIO::Display(std::string data)
{
	std::cout << data << std::endl;
}

void UserIO::ProcessResponse(std::string response)
{
	std::vector<std::string> parsedData(4);
	int index = 0;

	// Not a very well thought out implementation
	// But it works for the scope of this demonstration
	std::string temp;

	for (char c : response)
	{
		if ((index < 3) && (c == ' '))
		{
			parsedData[index++] = temp;
			temp.clear();
		}
		else
		{
			temp.push_back(c);
		}
	}

	if (!temp.empty())
	{
		parsedData[index] = temp;
	}
	
	std::string destination = parsedData[0];
	std::string command = parsedData[1];
	std::string topic = parsedData[2];
	std::string data = parsedData[3];

	Display("Destination: " + destination);
	Display("Command: " + command);
	Display("Topic: " + topic);
	Display("Data: " + data);

	if (s_CommandHandlers.find(command) != s_CommandHandlers.end()) // Found?
	{
		s_CommandHandlers[command](destination, topic, data);
	}
	else
	{
		Display("Invalid Command: " + command);
		Display("Command handler not found");
	}
}