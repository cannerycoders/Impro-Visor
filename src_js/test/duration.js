#!/usr/bin/env node

import {Duration} from "../data/duration.js";
import {Notes} from "./notes.js";
import {Constants} from "../constants.js";

let whole = Constants.WHOLE;
let half = whole/2;
let third = whole/3;
let quarter = whole/4;
let eighth = whole/8;

console.log("WHOLE", whole, "quarter", quarter, "eighth", eighth, 
            "default", Constants.DEFAULT_DURATION);

for(let [d, x] of [
                   ["c#+1", whole], 
                   ["c8.", eighth*1.5], 
                   ["d4", quarter], 
                   ["e2.", half*1.5], 
                   ["r1+2+3+4", whole+half+third+quarter],
                   ["f4+4/3+8/3", quarter + quarter*2/3 + eighth*2/3],
                   ["r1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1", whole*26],
                ])
{
    console.log(d, Duration.durationFromLeadsheet(d), `(${x})`);
}

let answer = whole/2*1.5 + eighth*2/3 + eighth/4;
let dstr = "2.+8/3+32";
let dur = Duration.getDuration(dstr);

console.log(`${dstr} => ${dur} (${answer})`)
console.assert(dur == answer);

for(let n of Notes)
{
    console.log(n, Duration.durationFromLeadsheet(n));
}
