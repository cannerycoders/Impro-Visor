import {Scale} from "../../data/scale.js";
import {ChordForm} from "../../data/chordform.js";
import {Key} from "../../data/key.js";

import yaml from "js-yaml";
import fs from "fs";

const sDepthLimit = 5; // to limit circularity in "same" scale or chord

export class Advisor
{
    static sSingleton = null;
    static Get()
    {
        return this.sSingleton;
    }

    static getChordForm(x)
    {
        return this.sSingleton.getChordForm(x);
    }

    /* --------------------------------------------------- */
    constructor()
    {
        /* associated with vocabulary file --- */
        this.scales = {};
        this.chords = {};
        this.extensions = {}; // for each chord, the chords it extends
        this.invertedExtension = {}; // for each chord, chords that extend it
        this.cells = {
            "plain": [],
            "idiom": [],
            "rhythm": []
        };
        this.licks = {
            "plain": [],
            "quote": [],
            "brick": [],
        }; 
        this.styles = [];
        this.rhythms = [];

        /* total vocabulary contents ... */
        this.rules = [];
        this.marks = []; // bool

        console.assert(Advisor.sSingleton == null);
        Advisor.sSingleton = this;
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
            case "style":
                break;
            case "cell":
            case "idiom":
            case "lick":
            case "quote":
            case "brick":
            case "rhythm":
            }
        }

        // ? build extensions

    }

    getKnownScales()
    {
        return Object.keys(this.scales);
    }

    getKnownChords()
    {
        return Object.keys(this.chords);
    }

    /**
     * Takes a chord name (in the key of C), looks it up in the chord list,
     * and returns a ChordForm containing information on that chord (or,
     * if there is a "same" chord, the information on that one).
     * @param chordName  the name of the chord to get information on
     * @return a ChordForm containing info on the chord
     */
    getChordForm(nm)
    {
        if(!nm) return null; //
        let depth = 0;
        while(depth < sDepthLimit)
        {
            let found = this.chords[nm];
            if(!found) return null;
            let same = found.getSame();
            if(!same)
                return found;
            else
            {
                nm = same;
                depth++;
            }
        }
        console.warn("Advisor chord 'same'-chain depth exceeded " + nm);
        return null;
    }

    getSubExtensions(chordName) // eg: Am7b5
    {
        let cnm = Key.makeCroot(chordName);
        let csym = this.chords[cnm];
        if(!csym)
        {
            console.log(`${chordName} => ${cnm} -> ${csym}`);
            return [];
        }
        else
        if(csym.extensions)
        {
            return csym.extensions;
        }

        /*
        vs
        let ext = this.m_extensions[cnm];
        */

    }

}

export default Advisor;