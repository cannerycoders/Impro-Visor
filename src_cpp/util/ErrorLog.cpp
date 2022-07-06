#include "ErrorLog.h"

#include <iostream>
#include <stdio.h>

namespace ErrorLog
{
    Level Filter = DEBUG;
    const int MaxMsgLength = 4000;
    char buf[MaxMsgLength];
    char const *lastMsg = buf;

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