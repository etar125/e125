#pragma once

#include <string>
#include <vector>
#include "../tinyss/tinyss.hpp"

struct Extension
{
    std::string name;
    std::string desc;
    std::vector<std::string> code;
    tss ss;

    Extension();

    Extension(std::string _name);

    Extension(std::string _name, std::string _desc, std::vector<std::string> _code);

    // void Run();

    void Clear();
};