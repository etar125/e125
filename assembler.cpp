#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include <sstream>

using byte = unsigned char;

int ver = 0;
int subver = 1;
std::string logo = "___       __\n|    | /\\ |_\n|-- /|  /  /\n|__  | /_ /\nAssembler v";

std::string trim(const std::string& str)
{
    if (str.size() == 0)
        return str;
    if (str[0] != ' ' && str[str.size() - 1] != ' ')
        return str;
    
    int start = 0;
    int end = str.size() - 1;
    
    while (start <= end && str[start] == ' ')
        start++;

    int len = end - start + 1;
    
    return str.substr(start, len);
}

std::vector<std::string> split(const std::string& str)
{
	std::vector<std::string> result;
	std::string temp;
	for(char sym : str)
	{
		if(sym != ' ') temp += sym;
		else
		{
			result.push_back(temp);
			temp = "";
		}
	}
	if(temp != "")
		result.push_back(temp);
	return result;
}

int main(int argc, char *argv[])
{
	std::string file = "main.asm";
	std::string output = "main.txt";
	int appstart = 4;
	bool nw = false; // no write
	bool nl = false; // no logo
	bool pr = false; // print result
	if(argc > 1)
	{
		std::string ar = argv[1];
		if (ar == "--help" || ar == "--ver" || ar == "--v")
		{
			std::cout << "e125 assembler v" << ver << "." << subver << std::endl << std::endl;
			std::cout << "файл [опции]" << std::endl;
			std::cout << "--help --ver --v   Показывает информацию" << std::endl << std::endl;
			std::cout << "--nw   Не записавать результат в файл" << std::endl;
			std::cout << "--nl   Не выводить логотип" << std::endl;
			std::cout << "--pr   Вывести результат" << std::endl << std::endl;
			std::cout << "--o   Файл для записи" << std::endl;
			std::cout << "--a   Задаёт начало программы" << std::endl;
			return 0;
		}
		else
		{
			for(int i = 1; i < argc; i++)
			{
				std::string arg = argv[i];
				if(arg == "--nw") nw = true;
				else if(arg == "--nl") nl = true;
				else if(arg == "--pr") pr = true;

				else if(arg == "--o")
				{
					i++;
					output = arg;
				}
				else if(arg == "--a")
				{
					i++;
					appstart = std::stoi(arg);
				}

				else { file = arg; }
			}
		}
	}
	if(!nl)
	{
		std::cout << logo << ver << "." << subver << std::endl << std::endl;
	}

	std::ifstream fin(file);
	std::ofstream fout(output);
	std::string str;

	if(!fin)
	{
		std::cout << "Не найден файл " << file << std::endl;
		return 0;
	}

	std::vector<std::string> result;

	int count = appstart;

	while (std::getline(fin, str))
	{
		str = trim(str);

		if(str[0] == ';') continue;
		else
		{
			std::vector<std::string> ins = split(str);

			if(ins[0] == "mov")
			{
				int com;
				int adr = std::stoi(ins[1]);
				int val;

				if(ins[2].substr(0, 1) == "$")
				{
					com = 10;
					val = std::stoi(ins[2].substr(1, ins[2].size() - 1));
				}
				else
				{
					com = 11;
					val = std::stoi(ins[2]);
				}

				std::string temp = "";

				temp = "LoadToMemory(" + std::to_string(com) + "," + std::to_string(count) + ");";
				count++;
				result.push_back(temp);

				temp = "LoadToMemory(" + std::to_string(adr) + "," + std::to_string(count) + ");";
				count++;
				result.push_back(temp);

				temp = "LoadToMemory(" + std::to_string(val) + "," + std::to_string(count) + ");";
				count++;
				result.push_back(temp);
			}
		}
	}

	for(std::string s : result)
	{
		if(pr) std::cout << s << std::endl;
		if(!nw) fout << s << std::endl;
	}

	return 0;
}