#include <iostream>
#include <string>
#include <vector>
#include "main.hpp"

std::vector<std::string> ssplit(std::string source, char separator)
{
	std::vector<std::string> result;
	std::string temp;
	bool in = false;
	bool no = false;
	for(char s : source)
	{
		if(s == '\\' && !no) no = true;
		else if(s == '"' && !no) in = true;
		else if(s != separator || in)
			temp += s;
		else
		{
			result.push_back(temp);
			temp = "";
		}
		if(no) no = false;
	}
	result.push_back(temp);
	return result;
}

int main(int argc, char *argv[])
{
	Logo();
	Init();

	std::string str;
	std::string command;
	std::vector<std::string> args;

	while(true)
	{
		std::cout << green << "[" << list[current].name << "]: " << reset;
		getline(std::cin, str);
		std::vector<std::string> res = ssplit(str, ' ');
		command = res[0];
		res.erase(res.begin());
		args = res;
		//if (args.size() >= 1) std::cout << "command: " << command << " args[0]: \"" << args[0] << "\"\n";
		if(command == "exit") return 0;
		else Do(command, args);
	}

	return 0;
}