import {Constants} from "../constants.js";


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
export class Style
{
    static AllStyles = {};
    static OrderedStyles = [];
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

    static MakeStyle(plist)
    {
    }

    /* -------------------------------------------------------- */
    constructor()
    {
        this.bassDefinedRules = {};
        this.chordDefinedRules = {};
        this.drumDefinedRules = {};
    }

}

export default Style;