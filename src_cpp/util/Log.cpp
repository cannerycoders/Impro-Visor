#include "Log.h"

#include <iostream>
#include <stdio.h>

namespace Log
{
    Level Filter = DEBUG;
    const int MaxMsgLength = 4000;
    char buf[MaxMsgLength];
    char const *LastMsg = buf;

    void Log(Level level, char const *fmt, ...)
    {
        va_list vap;
        va_start(vap, fmt);
        Log(level, fmt, vap);
        va_end(vap);
    }

    void Log(Level level, char const *fmt, va_list vap)
    {
        if(level < Filter)
        {
            vsnprintf_s(buf, MaxMsgLength, fmt, vap);
            std::cerr << LastMsg << "\n";
        }
    }
};