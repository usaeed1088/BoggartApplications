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
	Display("You can: ");

	Display("Subscribe (sub)");
	Display("Send Data (send)");

	Display("");

	Display("Example: sub <topic>");
	Display("Example: send <topic> \"<data>\"");
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
	std::vector<std::string> parsedData(3);
	int index = 0;

	// Raw parser
	std::string temp;
	for (auto c : response)
	{
		if (c == ' ')
		{
			if (temp.empty())
			{
				continue;
			}

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
		parsedData[index++] = temp;
	}

	std::string command = parsedData[0];
	std::string topic = parsedData[1];
	std::string data = parsedData[2];

	Display("Command: " + command);
	Display("Topic: " + topic);
	Display("Data: " + data);

	if (s_CommandHandlers.find(command) != s_CommandHandlers.end()) // Found?
	{
		s_CommandHandlers[command](topic, data);
	}
	else
	{
		Display("Invalid Command: " + command);
		Display("Command handler not found");
	}
}