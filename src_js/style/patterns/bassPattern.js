import {Pattern} from "./pattern.js";
import {Duration} from "../../data/duration.js";
import {Constants} from "../../constants.js";

const sRuleTypes = [
    "X", // Pitch: 0
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "B", // Bass: 11
    "C", // chord: 12
    "S", // scale: 13
    "A", // approach: 14
    "N", // next: 15,
    "R", // rest: 16
    "=", // equal 17,
    "U", // Up : 18,
    "D", // Down: 19
    "V", // volume: 20
];
const sBaseRuleCodes = ["A", "B", 'C', 'N', 'R', 'S', 'V', 'X', '='].map(v => v.charCodeAt(0));
const sFlatten = 100;
const sSharpen = 200;
const sFlat_Str = "b";
const sSharp_Str = "#";
const sNotePlus = "U";
const sNoteMinus = "d";
const sStay = 0;
const sBigRise = 6; // unused, "not-near"
const sMargin = 11; // internal bass can exceed limits by this
const sSoftMargin = 6;

export class BassPattern extends Pattern
{
    /* -------------------------------------------------------------- */
    constructor(style, plist, definedMap)
    {
        super(style);
        this.definedMap = definedMap;

        this.patternName = null; // optional
        this.rules = []; // indices into ruleTypes array
        this.durations = []; // string
        this.modifiers = []; // string

        // [bass-pattern, [name], [rules, B4, S4, C4, V90, A4], [weight, 10.0]
        // [bass-pattern, [rules, B4/3+16+64/3, R8/3+64/3, B8, [X, 5, 4+32/3], R8/3+32/3, A8, R16/3], [weight, 50]]
        console.assert(Array.isArray(plist));
        for(let p of plist)
        {
            switch(p[0])
            {
            case "name":
                if(p.length == 1)
                    this.patternName = null;
                else
                    this.patternName = p.slice(1).join(" ");
                break;
            case "rules":
                for(let r of p.slice(1))
                {
                    if(Array.isArray(r)) // a pitch
                    {
                        if(r.length >= 3 && r[0] == "X")
                        {
                            let rule = r[1]
                            let duration = r[2];
                            let modifier = r[3]; // may be undefined
                            this.addRule(rule, duration, modifier);
                        }
                        else
                            console.warn("BassPattern unrecognized segment " + r);
                    }
                    else
                    {
                        console.assert(typeof(r) == "string")
                        let rule = r[0];
                        let duration = r.slice(1);
                        let ruleCode = rule.charCodeAt(0);
                        if(sBaseRuleCodes.indexOf(ruleCode) != -1)
                            this.addRule(rule, duration);
                        else
                            console.warn("BassPattern unrecognized rule: " + rule);
                    }
                }
                break;
            case "weight":
                this.weight = Number(p[1]);
                break;
            case "use": // this theoretically allows for references by name
            default:
                console.log("BassPattern unexpected field in definition " + plist[0]);
                break;
            }
        }
    }

    addRule(rule, duration, modifier)
    {
        if(rule.length == 2) // eg b5, #4
        {
            let r = rule[1];
            let code = rule.charCodeAt(0);
            if(code == Constants.FLATCHAR)
                this.rules.push(sFlatten + sRuleTypes.indexOf(r));
            else
            if(code == Constants.SHARPCHAR)
                this.rules.push(sSharpen + sRuleTypes.indexOf(r));
            else
            {
                console.warn("BassPattern unknown rule " + rule);
                return;
            }
        }
        else
        {
            console.assert(rule.length == 1);
            this.rules.push(sRuleTypes.indexOf(rule));
        }
        this.durations.push(duration);
        this.modifiers.push(modifier);
    }

    getDuration() // override
    {
        let duration = 0;
        for(let dstr of this.durations)
            duration += Duration.getDuration0(dstr);
        return duration;
    }

    /**
     * Realizes the Pattern into a sequencable Polylist
     *
     * @param chord the ChordSymbol to use for the bassline
     * @param nextChord the ChordSymbol that comes next in the progression
     * @param lastNote a NoteSymbol containing the previous bassline note
     * @return A list of NoteSymbol objects that make up the bassline. 
     */
    applyRules(chord, nextChord, lastNote)
    {
        let result = [];
        let chordRoot = chord.getRootString();
        let chordForm = chord.getForm();
        let key = chordForm.getKey();


        return result;
    }
}

export default BassPattern;