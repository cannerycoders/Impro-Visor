import {Scale} from "../../data/scale.js";
import {ChordForm} from "../../data/chordform.js";

import yaml from "js-yaml";
import fs from "fs";

export class Advisor
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

    loadVocabulary(file)
    {
        let voc = yaml.load(fs.readFileSync(file, "utf8"));
        if(!voc || !Array.isArray(voc))
        {
            console.warn("Invalid vocabulary file " + file);
            return null;
        }
        for(let i of voc)
        {
            switch(i[0])
            {
            case "scale":
                {
                    let s = new Scale(i);
                    this.scales[s.name] = s;
                }
                break;
            case "chord":
                {
                    let c = new ChordForm(i);
                    this.chords[c.name] = c;
                }
                break;
            }
        }
    }

    getKnownScales()
    {
        return Object.keys(this.scales);
    }

    loadRules(rulesfile)
    {
    }

}

export default Advisor;