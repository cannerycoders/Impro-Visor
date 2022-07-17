#!/usr/bin/env node

import {VoicingSettings} from "../voicing/voicingSettings.js";

let v = new VoicingSettings();
v.load("../yaml/voicings/Closed-High.fv");
console.log(v.serialize());
