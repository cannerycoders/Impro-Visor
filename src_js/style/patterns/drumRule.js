import {GeneralMidi} from "../../midi/GM.js";
import {PlistParser} from "../../polya/parser.js";
import {DrumPattern} from "./drumPattern.js";
import {Duration} from "../../data/duration.js";

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
 *        [weight, 5]]
 */

/**
 * Element represents one element of a rule.
 * It consists of a type and a suffix.
 * For example, if the rule is X4+8, then the type is 'X' and the suffix is "4+8".
 * For X and R types, the suffix represents a duration string.
 * For V types, the suffix represents a volume setting 0-127.
 */
class Element
{
    constructor(etyp, suffix)
    {
        this.elementType = etyp;
        this.suffix = suffix;
    }
    getType() { return this.elementType; }
    getSuffix() { return this.suffix; }
    toString() { return `${this.elmementType} ${this.suffix}` }

}

export class DrumRuleRep
{
    static makeDrumRuleRep(str)
    {
        let plist = PlistParser.ParsePlist(str);
        // XXX: do we need to shed the outer list container
        return new DrumRuleRep(plist);
    }

    /* ----------------------------------------------------------- */
    constructor(plist, definedRules)
    {
        this.drumNumber = -1;
        this.elements = [];
        this.definedRules = definedRules;
        this.ruleAsList = [];
        this.ruleName = "";
        this.errorMessage = null;
        if(plist)
            this.initPlist(plist);
    }

    initPlist(plist)
    {
        if(typeof(plist) == "string")
            plist = PlistParser.ParserPlist(plist);
        let first = plist[0];
        if(typeof(first) == "number")
            this.drumNumber = Math.floor(first);
        else
        if(typeof(first) == "string")
            this.drumNumber = GeneralMidi.FindDrumId(first);
        else
            console.warn("drumRuleRep expects number or string first arg " + plist);
        this.ruleAsList = plist.slice(1);
        const ri = DrumPattern.sRuleIndices;
        for(let r of this.ruleAsList)
        {
            if(Array.isArray(r))
            {
                switch(r[0])
                {
                case "name":
                    {
                        let rest = r.slice(1);
                        if(rest.length)
                            this.ruleName = rest.join(" ");
                    }
                    break;
                case "rules":
                    {
                        for(let x of r.slice(1))
                        {
                            console.assert(typeof(x) === "string");

                            let type = x[0];
                            let suffix = x.slice(1);
                            switch(type)
                            {
                            case "X":
                            case "R":
                                {
                                    let dur = Duration.getDuration0(suffix);
                                    if(dur <= 0)
                                    {
                                        console.warn("The duration in " + x
                                                    + " is invalid");
                                    }
                                }
                                break;
                            case "V":
                                // need error checking on suffix?
                                break;
                            default:
                                console.warn("Each pattern element must begin with one "
                                            + "of 'X', 'R', or 'V', but this one begins with '" 
                                            + type + "': " + r);
                                continue;
                            }
                            this.elements.push(new Element(type, suffix));
                        }
                    }
                    break;
                case "use":
                default:
                    console.warn("DrumRuleRep unhandled token " + r[0]);
                    break;
                }
            }
            else
            {
                console.assert(typeof(r) == "string");
                let type = r[0];
                let tcode = type.charCodeAt(0);
                let suffix = r.slice(1);
                switch(tcode)
                {
                case ri.strike:
                case ri.rest:
                    {
                        let dur = Duration.getDuration0(suffix);
                        if(dur <= 0)
                            console.warn("The duration in " + x + " is invalid");
                    }
                    break;
                case ri.volume:
                    // need to error check volum
                    break;
                default:
                    break;
                }
                this.elements.push(new Element(type, suffix));
            }
        }
    }

    getInstrument() { return this.drumNumber; }
    getElements() { return this.elements; }

    /**
     * Get the duration, in slots
     * @return 
     */
    getDuration()
    {
        let duration = 0;
        for(let el of this.elements)
        {
            switch(el.getType())
            {
            case DrumPattern.DRUM_STRIKE:
                // fall through
            case DrumPattern.DRUM_REST:
                duration += Duration.getDuration(el.getSuffix());
                break;
                    
            case DrumPattern.DRUM_VOLUME:
                // ignore volume
                break;
            }
        }
        return duration;
    }

    toString()
    {
        this.fixRuleList();
        let buffer = "";
        buffer = `(drum ${this.drumNumber} `;
        buffer += this.serializeRule(this.ruleAsList);
        buffer += ")";
        return buffer;
    }

    serializeRule(plist)
    {
        return JSON.stringify(place).replace(/{/g, "(").replace(/}/g, ")");
    }

    fixRuleList()
    {
        let first = this.ruleAsList[0];
        if(Array.isArray(first))
        {
            first = first[0];
            let item = first.slice(1);
            switch(first)
            {
            case "use":
            {
                if(typeof(item[0]) == "string")
                {
                    let name = item[0];
                    let buffer = `(name ${name})`;

                    let ruleDefinitions = this.getDefinedRules();
                    let rule = ruleDefinitions[name];
                    buffer += this.serializeRule(rule);
                    this.ruleAsList = PlistParser.ParsePlist(buffer);
                }
                break;
            }
            default:
                break;
            }
        }
    }

    forStyleMixer()
    {
        let result = `${this.drumNumber}`;
        for(let e of this.elements)
        {
            result += " ";
            result += e.toString();
        }
        return result;
    }

    getStatus()
    {
        return this.errorMessage == null;
    }

    getErrorMessage()
    {
        return this.errorMessage || "";
    }

    setError(error)
    {
        this.errorMessage = error;
    }

    getName() { return this.ruleName; }
    setName(name)
    {
        this.ruleName = name;
    }

    getDefinedRules() { return this.definedRules; }
    setDefinedRules(map)
    {
        this.definedRules = map;
    }

}

/**
 * Added after the fact, a "DrumRule" is the part of a DrumPattern 
 * corresponding to a single percussion instrument.
 * This is temporarily hacked for the spreadsheet table view, but ultimately 
 * should be replaced with DrumRuleRep
 */
export class DrumRule
{
    constructor(pattern, inst=3)
    {
        this.pattern = pattern; // string
        this.instrument = inst;
        this.name = "";
    }

    setName(nm) { this.name = nm; }

    getPattern() { return this.pattern; }
    getInstrument() { return this.instrument; }
    getName() { return this.name; }
}

export default DrumRuleRep;