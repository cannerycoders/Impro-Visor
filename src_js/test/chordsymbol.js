#!/usr/bin/env node

import {ChordSymbol} from "../data/ChordSymbol.js";
import {Advisor} from "../data/advice/advisor.js";

let a = new Advisor();
a.loadVocabulary("../yaml/vocab/My.voc");

let chord = "CM#5add9";
for(let c of [chord, chord+"/d#"])
{
    let x = ChordSymbol.makeChordSymbol(c);
    console.log(c, "--------------------");
    console.log(JSON.stringify(x, null, 2));
    console.log("\n");
}
