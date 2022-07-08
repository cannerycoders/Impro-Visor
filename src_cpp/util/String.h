#pragma once
#include <string>
#include <algorithm>
#include <cctype>

namespace UtString
{

inline std::string toLower(std::string const &s)
{
    std::string l(s);
    std::transform(l.begin(), l.end(), l.begin(),
            [](unsigned char c){ return std::tolower(c); });
    return l;
}

inline std::string toUpper(std::string const &s)
{
    std::string l(s);
    std::transform(l.begin(), l.end(), l.begin(),
            [](unsigned char c){ return std::toupper(c); });
    return l;
}

};
