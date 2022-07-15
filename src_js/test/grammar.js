#!/usr/bin/env node

import {Grammar} from "../lickgen/grammar.js";

let g = new Grammar();
let fn = "../yaml/grammars/Bach.grammar";
g.load(fn);
console.log(fn);
console.log("    parameters: " + JSON.stringify(g.parameters));
console.log("    startSymbol: " + g.startSymbol);
console.log("    rules: " + Object.keys(g.rules));

let score = null;
// g.run(0, 500, score, false/*notrade*/);
