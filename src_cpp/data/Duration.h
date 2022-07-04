#pragma once

#include <string>

namespace Duration
{
    extern const int default_numerator;

    int durationFromLeadsheet(std::string &s);
    int getDuration0(std::string &item);
    int getDuration(std::string &item);
    bool isDuration(std::string &item);

};