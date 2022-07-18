#!/usr/bin/env node

import {Key} from "../data/key.js";

console.log("keys -------");
console.log(Key.getKeys().sort());

console.log("getKey('a'): " + JSON.stringify(Key.getKey("a")));

console.log("invalidNotes? " + Key.invalidNotes(["c8", "e8", "g8", "b8", "d8"]));
