#include "Duration.h"
#include "PitchClass.h"
#include "Constants.h"

using namespace Constants;
namespace Duration
{
    const int default_numerator = 8; // eighth note

    int durationFromLeadsheet(std::string &s)
    {
        size_t len = s.length();
        if( len == 0 )
            return 0;

        char c = s[0];

        if(c == RESTCHAR)
        {
            int duration = getDuration(s.substr(1));
            return duration;
        }

        if(!PitchClass::isValidPitchStart(c))
            return 0;

        int index = 1;

        bool natural = true;
        bool sharp = false;

        std::string noteBase;
        noteBase.push_back(c);

        if(index < len)
        {
            char second = s[1];
            if( second == SHARP || second == FLAT )
            {
                index++;
                noteBase.push_back(second);
                natural = false;
                sharp = (second == SHARP);
            }
        }

        // Check for any octave shifts specified in the notation

        bool more = true;
        while(index < len && more)
        {
            char c = s[index];
            if(c == PLUS || c == MINUS || c == 'u')
            {
                index++;
                break;
            }
            else
                more = false;
        }
        return getDuration(s.substr(index));
    }

    int getDuration0(std::string &item)
    {
        return 0;
    }
    int getDuration(std::string &item)
    {
        return 0;
    }
    bool isDuration(std::string &item)
    {
        return false;
    }
};