import yaml from "js-yaml";
import fs from "fs";

export default class Advisor
{
    constructor()
    {
        /* associated with vocabulary file --- */
        this.scales = {};
        this.chords = {};
        this.extensions = {}; // for each chord, the chords it extends
        this.invertedExtension = {}; // for each chord, chords that extend it
        this.cells = {}; // collections of notes: "plain", "idiom", "rhythm"
        this.licks = {}; // "plain", "quote", "brick"
        this.styles = [];

        /* associated with rules file ---- */
        this.rules = [];
        this.marks = [];
    }

    loadVocabulary(vocfile)
    {
    }

    loadRules(rulesfile)
    {
    }

}