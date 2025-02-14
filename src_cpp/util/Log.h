#pragma once

#include <cstdarg>

namespace Log
{
    enum Level
    {
        SEVERE=0,
        WARNING=1,
        COMMENT=2,
        DEBUG=3,
        TRACE=4,
    };
    extern Level Filter;
    extern char const *LastMsg;

    void Log(Level l, char const *fmt, ...);
    void Log(Level l, char const *fmt, va_list vap);

};