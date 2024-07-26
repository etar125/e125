#pragma once

#include <string>
#include <vector>
#include "../tinyss/tinyss.hpp"

struct ext {
    std::string name;
    std::string desc;
    std::vector<std::string> code;
    tss ss;

    ext() {}

    ext(std::string _name) { ext::name = _name; }

    ext(std::string _name, std::string _desc, std::vector<std::string> _code) { ext::name = _name; ext::code = _code; ext::desc = _desc; }

    // void Run();

    void clear() {
        ext::ss.vars.clear();
        ext::ss.stack.clear();
    }
};
