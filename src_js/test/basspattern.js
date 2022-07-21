#!/usr/bin/env node
import {Style} from "../style/style.js";
import {BassPattern} from "../style/patterns/bassPattern.js";
import {Advisor} from "../data/advice/advisor.js";
let a = new Advisor();
a.loadVocabulary("../yaml/vocab/My.voc");

let s = new Style();
let bp = new BassPattern();
bp.setStyle();
bp.setDefinedRules();
