import fs from "fs";
import yaml from "js-yaml";
import Terminals from "terminals.js";

class WeightedRule
{
    constructor(l)
    {
        // rule is ["rule", [Q0 ..], [Q1, ...], weight]
        console.assert(l.length == 4);
        this.lhs = l[1][0];
        this.rhs = l[2][0];
        this.weight = l[3];
    }

    GetKey()
    {
        return this.lhs;
    }

    GetWeight()
    {
        return this.weight;
    }

};

export default class Grammar
{
    constructor ()
    {
        this.reset();
    }

    reset()
    {
        this.parameters = {};
        this.rules = {}; // keyed by name (LHS), value is list of WeightedRules
        this.startSymbol = null;
        this.currentSlot = -1;
        this.chordSlot = -1;
    }

    load(file)
    {
        this.reset();
        try 
        {
            let ruleList = yaml.load(fs.readFileSync(file, "utf8"));
            for(let l of ruleList)
            {
                switch(l[0])
                {
                case "startsymbol":
                    console.assert(!this.startSymbol);
                    this.startSymbol = l[1];
                    break;
                case "parameter":
                    {
                        // auto-fill
                        // avoid-repeats
                        // chord-tone-decay
                        // chord-tone-weight
                        // color-tone-weight
                        // expectancy-constant
                        // expectancy-multiplier
                        // leap-prob
                        // min, max-duration
                        // min, max-interval
                        // min, max-pitch
                        // rectify
                        // rest-prob
                        // scale-root
                        // scale-tone-weight
                        // scale-type
                        // syncopation-constant
                        // syncopation-multiplier
                        // syncopation-type
                        // use-grammar
                        // use-syncopation
                        let pnm = l[1][0];
                        let pval = l[1].slice(1);
                        this.parameters[pnm] = pval;
                    }
                    break;
                case "base":
                    break;
                case "rule":
                    {
                        let r = new WeightedRule(l);
                        let k = r.GetKey();
                        let klist = this.rules[k];
                        if(!klist)
                        {
                            klist = [];
                            this.rules[k] = klist;
                        }
                        klist.push(r);
                    }
                    break;
                default:
                    console.warn("Grammar: unknown rule element " + l[0]);
                }
            }
        } 
        catch(e) 
        {
            console.warn(e);
        }
    }

    generate()
    {
        // default: noOutlines, noCritic, noAbstractMelody
        let abstractMelody = this.generateRhythmFromGrammar(startSlot, nSlots);
        let lick = this.generateLick(abstractMelody, startStop, nSlots-1);

    }


    // 0, 17280, false, false, 1920
    run(startSlot, totalSlots, doTrades, first, tradingQuantum)

}