#pragma once
#include "MelodySymbol.h"
#include <string>

class VolumeSymbol : public MelodySymbol
{
public:
    using VSPtr = std::shared_ptr<VolumeSymbol>;
    static VSPtr makeVolumeSymbol(std::string const &s)
    {
        try
        {
            size_t pos;
            long v = std::stol(s, &pos);
            if(pos == s.size()) 
                return std::make_shared<VolumeSymbol>(v) ;
        }
        catch(std::exception const& )
        {
        }
        return VSPtr();
    };

    enum
    {
        k_maxVolume = 127
    };

    VolumeSymbol(int vol)
    {
        m_volume = clipToRange(vol);
    }

    int getVolume() { return m_volume; }

private:
    int clipToRange(int v)
    {
        return v > k_maxVolume ? k_maxVolume : v < 0 ? 0 : v;
    }
    int m_volume;
};