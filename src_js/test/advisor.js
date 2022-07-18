#!/usr/bin/env node

import {Advisor} from "../data/advice/advisor.js";
import {Constants} from "../constants.js";

let a = new Advisor();

a.loadVocabulary("../yaml/vocab/My.voc");
console.log("scales -------");
for(let s of a.getKnownScales().sort())
    console.log(`  ${s.slice(2)}`);
