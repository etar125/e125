#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "../tinyss/tinyss.hpp"
#include "extension.hpp"

ext::ext() {}
ext::ext(std::string _name) { ext::name = _name; }
ext::ext(std::string _name, std::string _desc, std::vector<std::string> _code) { ext::name = _name; ext::code = _code; ext::desc = _desc; }

// void ext::Run()
// {
// 	TSSException b = ext::ss.docode(ext::code);
// }

void ext::clear()
{
    ext::ss.vars.clear();
    ext::ss.stack.clear();
}
