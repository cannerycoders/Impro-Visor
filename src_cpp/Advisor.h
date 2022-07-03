#pragma once

#include "polya/Polylist.h"

/**
 * Contains a set of rules read and processed during construction that can
 * be queried for Advice.
 */
class Advisor
{
public:
    void SetRules(std::shared_ptr<Polylist::list>);

private:
    std::shared_ptr<Polylist::list> cells[2]; // indexed by cell flavor
    std::shared_ptr<Polylist::list> licks[3]; // indexed by lick flavor

    /**
     * the table listing, for each chord, the chords it extends
     * (either directly, or indirectly)
     */
    std::shared_ptr<Polylist::list> extensionsTable;
    std::shared_ptr<Polylist::list> invertedExtensionsTable;
};