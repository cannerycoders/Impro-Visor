#!/usr/bin/env node

import {Part} from "../data/part.js";
import {IUnit} from "../data/iunit.js";
import {Constants} from "../constants.js";

let p = new Part();
p.setMetre(4, 4);

let uSize = Constants.BEAT;
for(let i=0;i<10;i++)
{
    p.addUnit(new IUnit(i*uSize, uSize));
}

p.dumpUnits();

console.log("\ninsert+stomp --------------------- ");
p.setUnit(400, new IUnit(0, uSize*7));
p.dumpUnits();
