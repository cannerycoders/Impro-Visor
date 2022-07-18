import {Constants} from "../constants.js";
import {VoicingGenerator} from "../voicing/voicingGenerator.js";
import {NoteSymbol} from "../data/noteSymbol.js";
import {BassPattern} from "./patterns/bassPattern.js";
import {DrumPattern} from "./patterns/drumPattern.js";
import {ChordPattern} from "./patterns/chordPattern.js";
import yaml from "js-yaml";
import fs from "fs";

/**
 * An object that contains patterns and parameters for generating an
 * accompaniment.  Contains functions to create a Style from text, 
 * output a Style to text, and, given a ChordPart, arrange patterns 
 * to construct an accompaniment.
 * @see         Pattern
 * @see         BassPattern
 * @see         DrumPattern
 * @see         ChordPattern
 * @see         ChordPart
 */

const sAllStyles = {};
const sOrderedStyles = [];
const sUsePreviousStyle = "*";
const sCustomVoicing = "fluid";
const sNullStyle = "";
const sDefaultDrumPatternDuration = 480;
const sChordInstrument = 1; // General MIDI
const sBassInstrument = 33; 

export class Style
{
    static sUsePreviousStyle = "*";

    /*keywords used in style specs */
    static sKeywords = 
    [
        "name", "bass-pattern", "bass-high",
        "bass-low", "bass-base", "swing",
        "drum-pattern", "chord-pattern",
        "chord-high", "chord-low", "chord-base",
        "use-extensions", "no-style",
        "voicing-type", "comments",
        "comp-swing", "define-rule", "bass",
        "chord", "drum", "voicing-name", "interpolate",
        "interpolable", "substitute"
    ];

    static getStyle(nm)
    {
        return sAllStyles[nm];
    }

    static setStyle(nm, style)
    {
        sAllStyles[nm] = style;
    }

    static numStyles()
    {
        return Object.keys(sAllStyles).length;
    }

    static getNth(n)
    {
        return sAllStyles[Object.keys(sAllStyles)[n]];
    }

    static LoadStyle(file)
    {
        let sty = yaml.load(fs.readFileSync(file, "utf8"));
        if(!sty || sty[0] != "style")
        {
            console.warn("Invalid style file " + file);
            return null;
        }
        else
        {
            let s = new Style();
            for(let i=1;i<sty.length;i++)
            {
                let l = sty[i];
                switch(l[0])
                {
                case "name":
                    s.name = l[1];
                    break;
                case "bass-high":
                    s.bassHigh = NoteSymbol.make(l[1]);
                    break;
                case "bass-low":
                    s.bassLow = NoteSymbol.make(l[1]);
                    break;
                case "bass-base":
                    s.bassBase = NoteSymbol.make(l[1]);
                    break;
                case "swing":
                    s.swing = Number(l[1]);
                    break;
                case "comp-swing":
                    s.accompanimentSwing = Number(l[1]);
                    break;
                case "voicing-type":
                    s.voicingType = l[1];
                    break;
                case "chord-high":
                    s.chordHigh = NoteSymbol.make(l[1]);
                    break;
                case "chord-low":
                    s.chordLow = NoteSymbol.make(l[1]);
                    break;
                case "chord-base":
                    s.chordBase = NoteSymbol.makeList(l.slice(1)); // unused
                    break;
                case "comments":
                    s.comments = l.slice(1).join(" ");
                    break;
                case "bass-pattern":
                    s.addBassPattern(l.slice(1));
                    break;
                case "drum-pattern":
                    s.addDrumPattern(l.slice(1));
                    break;
                case "chord-pattern":
                    s.addChordPattern(l.slice(1));
                    break;
                }
            }
            sAllStyles[s.name] = s;
            return s;
        }
    }

    static GetStyle(nm)
    {
        return sAllStyles[nm];
    }

    /* -------------------------------------------------------- */
    constructor()
    {
        this.bassDefinedRules = {};
        this.chordDefinedRules = {};
        this.drumDefinedRules = {};
        this.name = sDefaultStyleName;
        this.vgen = new VoicingGenerator();
        this.noStyle = false; // whether to use "no-style" behavior
        this.comments = "";
        this.swing = .5;
        this.accompanimentSwing = .5;
        this.voicingType = "closed";
        this.voicingFileName = "default.fv";
        this.useExtensions = false; // auto-extend chords
        this.chordBase = NoteSymbol.makeList(["c-", "e-", "g-"]);
        this.chordLow = NoteSymbol.make("c-"); // lower range of chord prog
        this.chordHigh = NoteSymbol.make("a"); // upper range of chord prog
        this.bassBase = NoteSymbol.make("g---"); // unused
        this.bassLow = NoteSymbol.make("g---");
        this.bassHigh = NoteSymbol.make("g-");
        this.chordPatterns = []; // of ChordPattern
        this.bassPatterns = []; // of BassPattern
        this.drumPatterns = []; // of DrumPattern
        this.interpolations = [];
        this.interpolables = [];
        this.substitutions = [];
        this.definedInterpolations = [];
        this.definedInterpolables = [];
        this.definedSubstitutions = [];

        // save rules defined outside the patterns
        this.bassDefinedRules = {};
        this.chordDefinedRules = {};
        this.drumDefinedRules = {};
    }

    usePreviousStyle()
    {
        return this.name == sUsePreviousStyle;
    }

    hasCustomVoicing()
    {
        return this.voicingType == sCustomVoicing;
    }

    addBassPattern(plist)
    {
        new BassPattern(this, plist, this.bassPatterns); // adds itself to our map
    }

    addDrumPattern(plist)
    {
    }

    addChordPattern(plist)
    {
    }

    getBP() { return this.bassPatterns; }
    getCP() { return this.chordPatterns; }
    getDP() { return this.drumPatterns; }

}

export default Style;