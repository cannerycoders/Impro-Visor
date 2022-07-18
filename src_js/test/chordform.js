#!/usr/bin/env node

import {ChordForm} from "../data/chordForm.js";

console.log("chordForm -------");

let cf = new ChordForm(["chord", 
    ["name", "CM#5add9"], 
    ["pronounce", "C", "major", "sharp", "five", "add", "9"], 
    ["key", "c"], 
    ["family", "augmented"], 
    ["spell", "c8", "e8", "g#8", "d8"], 
    ["color", "b8"], 
    ["priority", "g#8", "d8", "e8", "c8"], 
    ["approach", 
        ["c8", "c#8", "b8"], 
        ["e8", "eb8", "f8"], 
        ["g#8", "g8", "a8"], 
        ["d8", "eb8", "c#8"]], 
    ["voicings", 
        ["closed-1", ["type", "closed"], ["notes", "e-8", "g#-8", "d8"], 
           ["extension"]], 
        ["closed-2", ["type", "closed"], ["notes", "d-8", "e-8", "g-8"], ["extension"]], 
        ["open-1", ["type", "open"], ["notes", "e-8", "d8", "g#8"], ["extension"]], 
        ["open-2", ["type", "open"], ["notes", "e-8", "g#-8", "d8"], ["extension"]]
    ], 
    ["extensions", "CM9#5"], 
    ["scales", 
        ["C", "lydian", "augmented"], 
        ["C", "augmented"], 
        ["B", "minor", "six", "pentatonic"], 
        ["E", "flat", "six", "pentatonic"], 
        ["C", "chromatic"]
    ], ["avoid"], ["substitute"]]);

console.log("chord form for " + cf.name);
console.log(JSON.stringify(cf, null, 2));
