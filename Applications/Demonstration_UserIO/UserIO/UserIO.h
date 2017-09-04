#pragma once

#include <map>
#include <string>
#include <functional>

class UserIO
{
public:
	typedef std::function<void(std::string, std::string)> Handler;

private:
	static std::map<std::string, Handler> s_CommandHandlers;

private:
	UserIO();

public:
	~UserIO();

	static void RegisterCommandHandler(std::string command, Handler handler);

	static std::string Query(std::string query);

	static void Process();

private:
	static void PrintMenu();

	static void Display(std::string data);

	static void ProcessResponse(std::string response);
};