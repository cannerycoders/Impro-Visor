#!/usr/bin/env node

import {Style} from "../style/style.js";
import {Constants} from "../constants.js";


for(let s of ["airflow.sty", "swing-TadDameron.sty"])
{

    let sty = Style.LoadStyle(`../yaml/styles/${s}`);
    console.log(`Loaded style: ${s} ${sty.name}`);
    let bp = sty.getBP();
    console.log("  bass-patterns: " + bp.length);

    let cp = sty.getCP();
    console.log("  chord-patterns: " + cp.length);

    let dp = sty.getDP();
    console.log("  drum-patterns: " + dp.length);
}
