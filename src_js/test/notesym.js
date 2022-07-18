#!/usr/bin/env node

import {NoteSymbol} from "../data/noteSymbol.js";

let ns =  NoteSymbol.make("bb+++4+4+4+4");
console.log(JSON.stringify(ns, null, 2));

if(0)
{
    let syms = NoteSymbol.makeList(["bb+++", "f--", "c", "g", "d", "a", "cb---", "e#", "f#+++", "fs"]);
    console.log(JSON.stringify(syms, null, 2));
}
