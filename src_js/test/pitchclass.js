#!/usr/bin/env node

import {PitchClass} from "../data/pitchclass.js";

for(let x of ["bb", "f", "c", "g", "d", "a", "cb", "e#", "f#", "fs"])
{
    let pc = PitchClass.getPitchClass(x);
    let nm = pc.name;
    let index = pc.index;
    console.log(`${x}: ${index} ${nm}`);
}

console.log("pitchclass by semitone: " + JSON.stringify(PitchClass.pitchClassBySemitone));
console.log("upTranpositions: " + JSON.stringify(PitchClass.upTranspositions));
