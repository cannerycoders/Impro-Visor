#pragma once

#include "Part.h"

/**
 * An extension of the Part class that contains only Chord objects.
 * This is useful to separate simple chord drawing (names only,) from
 * the more complex melody drawing.
 */
class ChordPart : public Part
{
public: 
    using Ptr = std::shared_ptr<ChordPart>;

    ChordPart() {}

};