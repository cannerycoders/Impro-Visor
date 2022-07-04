#include "util/Preferences.h"
#include <iostream>

int main(int argc, char** argv) 
{
    Preferences::get().write(std::cerr);
    std::cerr << Preferences::DEFAULT_VOCAB_FILE
        << Preferences::get().getPreference(Preferences::DEFAULT_VOCAB_FILE)
        << "\n";
    return 0;
}