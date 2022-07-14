#!/usr/bin/env node

import {Terminals} from "../lickgen/terminals.js";
import {Constants} from "../constants.js";

for(let [d, x] of [
                   ["A1", true],  // Approach
                   [["A1"], true],  // Approach, wrapped
                   ["C8.", true], // Chord
                   ["H", true],  // helpful
                   ["X", true],  // Arbitrary
                   ["L", true],  // Color
                   ["R", true],  // Rest
                   ["S", true],  // Scale
                   ["Y", true],  // Outside
                   ["a", false],  // invalid
                   [["slope", 1, 2, 3], true],
                   [["triadic", 1, 2], true],
                   [["triadic", 1, 2, 3], false],
                   [["X", 1, 2], true], // ScaleDegree
                   [["X", 1, 2, 3], false], // ScaleDegree
                ])
{
    console.log(d, Terminals.isTerminal(d), x);
}
