#!/usr/bin/env node

import {Advisor} from "../data/advice/advisor.js";
import {ChordForm} from "../data/chordForm.js";
import {Constants} from "../constants.js";

let a = new Advisor();

a.loadVocabulary("../yaml/vocab/My.voc");
console.log("scales -------");
for(let s of a.getKnownScales().sort())
    console.log(`  ${s.slice(2)}`);

console.log("chords -------");
for(let s of a.getKnownChords().sort())
{
    let match = ChordForm.sParseChord.exec(s);
    console.log(`  ${s} : ${match[1]} ${match[2]} ${match[3]||""}`);
}

console.log("Extensions of Cm7b5 " + a.getSubExtensions("Cm7b5"));

