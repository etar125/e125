#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "tinyss/tinyss.hpp"
#include "extension.hpp"

Extension::Extension() {}
Extension::Extension(std::string _name, std::string _desc, std::vector<std::string> _code) { Extension::name = _name; Extension::code = _code; Extension::desc = _desc; }

void tss::gfunc(std::string name)
{
    if(name == "print") std::cout << (char)(std::stoi(tss::stack[0]));
}

void Extension::Run()
{
	TSSException b = Extension::ss.docode(Extension::code);
}

void Extension::Clear()
{
    Extension::ss.vars.clear();
    Extension::ss.stack.clear();
}