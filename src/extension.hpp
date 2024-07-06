#pragma once

#include <string>
#include <vector>
#include "../tinyss/tinyss.hpp"

struct ext
{
    std::string name;
    std::string desc;
    std::vector<std::string> code;
    tss ss;

    ext();

    ext(std::string _name);

    ext(std::string _name, std::string _desc, std::vector<std::string> _code);

    // void Run();

    void clear();
};
