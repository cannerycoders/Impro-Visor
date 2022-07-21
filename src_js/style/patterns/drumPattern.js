import {Pattern} from "./pattern.js";
import {DrumRule, DrumRuleRep} from "./drumRule.js";

/* a drumPattern is a collection of drumRules.
 * a drumRule binds an instrument to a "ruleset"
 * eg: [drum-pattern, 
 *        [drum, Ride_Cymbal_1, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8, X8], 
 *        [drum, Claves, R4, X4+8, X4+8, X4+8, X4+8, X4], [weight, 10]]
 * 
 * from swing-TadDameron.sty:
 *     [drum-pattern, [pattern-name], 
 *        [drum, Ride_Cymbal_1, [name], 
 *           [rules, X8, X8, X4, X4, X4]], 
 *        [drum, Closed_Hi-Hat, [name], 
 *           [rules, R4, X4, R4, X4]], 
 *        [drum, Acoustic_Snare, [name], 
 *            [rules, R2, V10, X8/3, V20, X8/3, V30, X8/3, V40, X8/3, V50, X8/3, V60, X8/3]]
 */
export class DrumPattern extends Pattern
{
    static sDefaultDrumPatternDuration = 480;
    static sRuleTypes = [ "X", "R", "V" ];
    static sRuleCodes = this.sRuleTypes.map((v) => v.charCodeAt(0));
    static sRuleIndices =
    {
        strike: 0,
        rest: 1,
        volume: 2
    };

    /**
     * constructor implements static makePattern
     * @param {Style} style 
     * @param {Array} plist 
     * @param {Object} definedRules - ie style's bassChordMap
     */
    constructor(style, plist, definedRules)
    {
        super(style);
        this.patternName = "";
        this.drums = []; // of DrumRuleRep
        this.definedRules = definedRules;
        this.makePattern(plist);
    }

    getDrums() { return this.drums; }
    getName() { return this.patternName; }
    setDefinedRules(r) { this.definedRules = r; }

    makePattern(plist)
    {
        for(let i=0;i<plist.length;i++)
        {
            let item = plist[i];
            console.assert(Array.isArray(item));
            switch(item[0])
            {
            case "name":
                {
                    if(item.length > 1)
                        this.patternName = item.slice(1).join(" ");
                    else 
                        this.setError("Unrecognized name type in drum pattern: " + rest);
                    break;
                }
            case "drum": // a single drum "rule" in the pattern
                {
                    let rep = new DrumRuleRep(item.slice(1), this.definedRules);
                    this.addRule(rep);
                    break;
                }
                
            case "weight": // weight of entire pattern
                {
                    this.weight = Number(item[1]); // in parent
                    break;
                }
            }
        }
    }

    /** 
     * @param {DrumRuleRep} rep
     */
    addRule(rep)
    {
        for(let i=0;i<this.drums.length;i++)
        {
            let e = this.drums[i];
            if(e.getInstrument() == rep.getInstrument())
            {
                this.drums[i] = rep;
                return;
            }
        }
        this.drums.push(rep);
    }

}
