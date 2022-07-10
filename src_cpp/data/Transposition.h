#pragma once

/**
 * Transposition is a an object for packaging and communicating transpositions
 * of chord, bass, and melody.
 */
class Transposition
{
private:
    int m_bass;
    int m_chord;
    int m_melody;

public:
    Transposition()
    {
        m_bass = 0;
        m_chord = 0;
        m_melody = 0;
    }

    Transposition(int b, int c, int m)
    {
        m_bass = b;
        m_chord = c;
        m_melody = m;
    }

    int getBassTransposition()
    {
        return m_bass;
    }

    int getChordTransposition()
    {
        return m_chord;
    }

    int getMelodyTransposition()
    {
        return m_melody;
    }
    
    Transposition newBassTransposition(int value)
    {
        return Transposition(value, m_chord, m_melody);
    }
        
    Transposition newChordTransposition(int value)
    {
        return Transposition(m_bass, value, m_melody);
    }
    
    Transposition newMelodyTransposition(int value)
    {
        return Transposition(m_bass, m_chord, value);
    }
    
    std::string toString()
    {
        return std::to_string(m_bass) + " " + 
               std::to_string(m_chord) + " " + 
               std::to_string(m_melody);
    }
};
