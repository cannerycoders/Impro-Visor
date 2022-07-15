import fs from "fs";
import yaml from "js-yaml";
import Terminals from "./terminals.js";
import Note from "../data/note.js";
import Constants from "../constants.js";

const sTraceLevel = 0;

class WeightedRule
{
    constructor(l)
    {
        //          
        // rule is ["rule", [rulenm ..], [RHS...], weight]
        // RHS is a polylist of symbols. If it's a RULE, 
        // some expressions ie: 
        //  (- Z 1) to evaluate.
        console.assert(l.length == 4);
        this.type = l[0];
        this.lhs = l[1][0];
        this.rhs = l[2];
        this.weight = l[3];
    }

    GetType()
    {
        return this.type;
    }

    GetKey()
    {
        return this.lhs;
    }

    GetValue()
    {
        return this.rhs;
    }

    GetWeight()
    {
        return this.weight;
    }

};

const sRuleTags = new Set([
    "startsymbol", "terminals", "base", "rule",
    "parameter", "comment"
]);
const sBuiltinVars = new Set([
    "brick", "muse", "chord-family", "expectancy",
    "syncopation", "high", "medium", "low",
    0, 1
]);
const sOperators = new Set(["+", "-", "*", "/"]);
const sSpecialForms = new Set([
    "builtin", "splice", "literal"
]);
const tWrapper =
{
    FILL: "fill", 
    NONE: "none", 
    SHARE: "share", 
    UNSHARE: "unshare", 
    UNSHAREALL: "unshareall",
    MUSE_SWITCH: "muse-switch", 
    MUSE_DEFAULT: "default" // muse keywords
};
const sWrappers = new Set(Object.values(tWrapper));

export class Grammar
{
    constructor()
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
        this.cache = {}; // for share/unshare
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
                // The BASE keyword stops all evaluation and variable 
                // substitution. If a lhs matches both a RULE and a 
                // BASE, it will always choose the BASE. This basically 
                // short-circuits any computation and provides an easy way
                // to find base cases.
                case "base":
                    // hrm: seems that Grammar.java is actually
                    // *ignoring* all base rules.
                    break;
                case "rule":
                    {
                        // >= 1 entries for a each rule
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

    /** the main entrypoint for Grammer objects.
     *  invoked from lickgen and tradingResponseInfo.
     *
     * @param startSlot
     * @param initialNumSlots
     * @param score
     * @param whetherToTrade
     * @param improVisorFirst when trading
     * @param tradingQuantum size of trading quantum (when trading)
     * @return the abstract melody, truncated to the desired number of slots.
     *  Truncation should not be necessary if everything was done right, but
     *  it seems that we go over sometime. Not sure why.
     */
    run(startSlot, initialNumSlots, score, whetherToTrade, 
        improVisorFirst, tradingQuantum)
    {
        this.currentSlot = startSlot;
        this.chordSlot = startSlot;
        this.score = score;

        if( !whetherToTrade )
        {
            improVisorFirst = true;
            tradingQuantum = initialNumSlots;
        }

        this.terminalBuffer = []; // applyRules will side-effect terminalBuffer 

        let totalSlotsToFill = initialNumSlots;
        let numSlotsToFill;
        let padding;
        while(totalSlotsToFill > 0)
        {
            if(improVisorFirst) // fill now, pad latter, if at all
            {
                numSlotsToFill = Math.min(tradingQuantum, totalSlotsToFill);
                totalSlotsToFill -= numSlotsToFill;
                padding = Math.min(tradingQuantum, totalSlotsToFill);
                totalSlotsToFill -= padding;
            }
            else // pad where user would play, then fill
            {
                padding = Math.min(tradingQuantum, totalSlotsToFill);
                totalSlotsToFill -= padding;
                numSlotsToFill = Math.min(tradingQuantum, totalSlotsToFill);
                totalSlotsToFill -= numSlotsToFill;
                if(padding > 0)
                {
                    let fillRests = Note.GetDurationString(padding);
                    this.terminalBuffer.push("R" + fillRests);
                }
            }
        }
        // Inner loop fills up to one quantum worth of abstract melody, or the
        // entire abstract melody if not trading.
        // Inner loop fills up to one quantum worth of abstract melody, or the
        // entire abstract melody if not trading.
        while(numSlotsToFill > 0)
        {
            let stack = this.addStart(numSlotsToFill);
            if(!stack)
                break;

            //System.out.println("\nTop Level");
            let result = this.outerFill(stack, numSlotsToFill);
            numSlotsToFill = result.numSlotsToFill;
            stack = result.stack;
            if( improVisorFirst && padding > 0 ) // Pad the user's part after
            {
                let fillRests = Note.getDurationString(padding);
                this.terminalBuffer.append("R" + fillRests);
            }
        }

        // Return the abstract melody, truncated to the desired number of slots.
        // Truncation should not be necessary if everything was done right, but
        // it seems that we go over sometime. Not sure why.
        return Terminals.truncateAbstractMelody(this.terminalBuffer.join(""), 
                                    initialNumSlots);
    }

    /**
     * Add the Start Symbol to list stack in order to start the grammar
     * expansion.
     */
    addStart(numSlots)
    {
        // nb: this bootstrapping behavior is slightly different than
        //  the reference implementation where the ordering of rules
        //  named by startSymbol matters (ie: it uses the first matching
        //  rule after the startSymbol is encountered without randomization).
        //  Here we draw the startrule according to the requested probability.
        //  (in Bach, there are two P_motif rules, each .5 probability).
        let rule = this.findRule(this.startSymbol); 
        if(!rule)
            console.warn("Grammar has no reles that match " + this.startSymbol);
        else
            return [rule, numSlots]; // stack
    }

    /**
     * outerFill refills the stack if numSlotsToFill > 0
     *
     * @param stack
     * @param numSlotsToFill
     * @return {stack, numSlotsToFill}
     */
    outerFill(origStack, numSlotsToFill)
    {
        let stack = origStack.slice(); // clone
        while(numSlotsToFill > 0)
        {
            if(stack.length == 0)
                stack = origStack.slice();
            
            let result = this.accumulateTerminals(stack, numSlotsToFill);
            numSlotsToFill = result.numSlotsToFill;
            stack = result.stack;

            result = this.applyRules(stack, numSlotsToFill);
            numSlotsToFill = result.numSlotsToFill;
            stack = result.stack;
        }
        return {stack, numSlotsToFill};
    }

    // Accumulate any terminal values at the beginning of stack.
    accumulateTerminals(stack, numSlotsToFill)
    {
        while(numSlotsToFill > 0 && stack.length)
        {
            let token = stack[0];
            if(!Terminals.isTerminal(token))
            {
                // no terminals here
                return {stack, numSlotsToFill};
            }
            if(Terminals.isWrappedTerminal(token))
            {
                numSlotsToFill = this.accumulateTerminal(token[0], numSlotsToFill);
            }
            else
            {
                numSlotsToFill = this.accumulateTerminal(token, numSlotsToFill);
            }
            stack.shift(); // popfront
        }
        return {stack, numSlotsToFill};
    }

    /**
     * Add terminal to the list of terminals generated, as long as the quota has
     * not been reached. Checks whether the terminal would go over quota, and
     * if so, does not add it, but set quotaReached indicator instead.
     *
     * @param terminal
     */
    accumulateTerminal(terminal, numSlotsToFill)
    {
        //System.out.println("accumulateTerminal " + numSlotsToFill + " " + terminal);
        let duration = Terminals.getDuration(terminal);
//        if( numSlotsToFill < duration )
//          {
//            //System.out.println("short slots = " + numSlotsToFill + " vs duration " + duration);
//            terminal = new Rest(numSlotsToFill);
//            duration = numSlotsToFill;
//          }
        this.terminalBuffer.push(terminal);
        this.chordSlot += duration;
        numSlotsToFill -= duration;
        return numSlotsToFill;
    }

    /**
     * Pop tokens off the stack. Any terminal tokens are pushed onto
     * accumulator. Rules are applied to non-terminals.
     *
     * @param stack
     * @return
     * @throws imp.lickgen.RuleApplicationException
     */
    applyRules(stack, numSlotsToFill)
    {
        if(stack.length == 0)
            return {stack, numSlotsToFill};

        //System.out.println("applyRules " + numSlotsToFill + " " + stack);
        let token = stack.shift(); // popfront

        // token is a non-terminal
        if( !Array.isArray(token))
            token = [token]; // wrap it

        let result = this.expandNonTerminal(token, numSlotsToFill);
        assert(result != null);

        numSlotsToFill = result.numSlotsToFill;
        let expansion = result.stack;
        stack = expansion.push(...stack);
        this.trace(stack);
        return {stack, numSlotsToFill};
    }

    expandNonTerminal(token, slotsToFill)
    {
        //System.out.println("expandNonTerminal " + slotsToFill + " " + token);
        let shareable = false;
        let unshare = false;
        switch(this.wrapperType(token))
        {
        case tWrapper.NONE:
            break;
                
        case tWrapper.FILL:
            {
                token.shift(); // popback, leave as a list
                let beats = token[0];
                console.assert(typeof(beats) == "number")

                // Convert to slots
                let slots = Math.floor(beats * Constants.BEAT);
                let innerToken = token.slice(1); // ie rest
                let result = this.outerFill(innerToken, slots);
                let stack = result.stack;
                stack.push([tWrapper.UNSHAREALL]); // stack.cons(Polylist.list(UNSHAREALL));
                return {stack, numSlotsToFill: slotsToFill - slots};
            }

        case tWrapper.MUSE_SWITCH: 
            {
                // analogous to switch statement rule, result is new rule.
                //  form: [muse-switch type [switchbody]]
                // MuseReceiver is an OSC-server that implements int getValue(int type); 
                //  (where type is the signal to "sample", brainwave and head-tilt)
                // the result selects a rule from "switchbody" list:
                //  [["default", "rule1"], [1, "rule2"], ...]
                token.shift(); // popback  = ((Polylist) token).rest();
                // token is now the list after muse-switch, which should be
                // two items:
                //    an argument to be evaluated (should return a Long)
                //
                //    an association list which gives a replacement 
                //    for each value that getMuseValue can return
                //    and a special default pair indicating the replacement in 
                //    case said value is not in the list
                
                let rawMuseArgument = token[0];
                
                //System.out.println("argument to muse-switch = " + rawArgument);
                
                let argValue = this.evaluate(rawMuseArgument);
                console.assert(typeof(argValue) === "number");
                    // XXX Provide a proper error message in case assertion fails.

                let alist = token[1]; // this is the switch-body list
                consol.eassert(Array.isArray(alist));
                
                //System.out.println("muse-switch alist = " + alist);
                
                // Make sure the Muse receiver is instantiated.
                let receiver = null; // ImproVisor.getMuseReceiver();
                if(receiver)
                {
                    // Get the value from muse, which generally depends on argValue
                    // Technically we don't need the receiver. part since it's
                    // static method, but doesn't hurt as a reminder.
                    let museValue = receiver.getMuseValue(argValue);
                
                    // Find the pair having the first component equal to museValue.
                    // These components should be Long (because of polya) constants,
                    // or the String MUSE_DEFAULT.
                
                    // If there is no match to museValue and no default specified,
                    // then we continue with no replacement (??).
                    // If any item in the putative association list is not a pair
                    // this thing might crash. REVISIT.
                    let def = null;
                    let replacement;
                    for(let b of alist)
                    {
                        if(b[0] == "default")
                            def = b[1];
                        else
                        if(b[0] == museValue)
                        {
                            replacement = b[1];
                            break;
                        }
                    }
                    if(!replacement)
                        replacement = def;
                    // XXX: this is an incomplete implementation with
                    // bogus error handling.
                    if(!replacement)
                        return this.applyRules([], slotsToFill);
                    else
                        return this.applyRules([replacement], slotsToFill);
                }
                else
                {
                    console.warn("Grammar: need to implement muse-receiver");
                }
            }

        case tWrapper.UNSHARE:
            unshare = true;
            // NOTE: Intentional fall-through
        case tWrapper.SHARE:
            shareable = true;
            token.shift(); // popfront
            {
                let find = token[0];
                let found = this.cache[find];
                if(found)
                {
                    if(unshare)
                        delete this.cache[find];
                    return {stack: found.shift(), numSlotsToFill: slotsToFill};
                }
            }
            break;
                
        case tWrapper.UNSHAREALL:
            {
                this.cache = {};
                return {stack: [], numSlotsToFill: slotsToFill};
            }
        } // switch

        let ruleToUse = this.findRule(token);
        if(ruleToUse == null)
            return null;

        let expansion = ruleToUse.GetValue();
        if(shareable && !unshare)
            this.cache[token] = expansion;

        if(sTraceLevel > 0)
            console.log("\nRULE: " + ruleToUse.GetKey());

        return {stack: expansion, numSlotsToFill: slotsToFill};
    }

    wrapperType(token)
    {
        if(Array.isArray(token))
        {
            switch(token.length)
            {
            case 1:
                if(token[0] == tWrapper.UNSHAREALL)
                    return tWrapper.UNSHAREALL;
                break;

            case 2:
                if(token[0] == tWrapper.SHARE)
                    return tWrapper.SHARE;
                if(token[0] ==  tWrapper.UNSHARE)
                    return tWrapper.UNSHARE;
                break;

            case 3:
                if(token[0] == tWrapper.FILL)
                    return tWrapper.FILL;
                if(token[0] == tWrapper.MUSE_SWITCH )
                    return tWrapper.MUSE_SWITCH;
                break;
              }
        }
        return tWrapper.NONE; // Not a wrapper
    }

    addRule(toAdd)
    {
        console.assert(!"Grammar.addRule implement me");
    }

    /* randomly selects a matching rule according to relative weights.
     */
    findRule(nm)
    {
        let rulelist = this.rules[nm];
        if(!rulelist)
        {
            console.warn(`Grammar ${nm} rule not found`);
            return null;
        }
        let result;
        let tot = rulelist._tot;
        if(tot == undefined)
        {
            tot = 0;
            for(let rule of rulelist)
                tot += rule.GetWeight();
            rulelist._tot = tot;

        }
        let rand = Math.random() * tot;
        let last = 0;
        for(let rule of rulelist)
        {
            let next = last + rule.GetWeight();
            if(rand >= last && rand < next)
            {
                result = rule;
                break;
            }
            else
                last = next;
        }
        return result;
    }

    /** performs arithmetic evaluation on its S-Expression argument.
     * (+ 3 4) evaluates to 7
     * (+ (/ 4 2) (* 7 3)) evaluates to 23
     *
     * As of 21 June 2012, evaluate can accept builtins, of the form
     * (builtin <some identifier>)
     * Right now, the only identifier is "expectancy". Evaluation of this identifier
     * is stubbed to return 1. Any other identifier will return 0.
     * Eventually, the value of the builtin identifier may change depending on the
     * then-current slot.
     *
     * Recursively evaluates until we get down to two values we can combine.
     */
    evaluate(toParse)
    {
        if(!Array.isArray(toParse))
            return toParse; // recursion complete, we've found number or string
        else
        if(toParse.length == 0)
            return null;
        else
        {
            let op = toParse[0];
            if(sOperators.has(op))
            {
                console.assert(toParse.length == 3);
                let a = this.evaluate(toParse[1]); 
                let b = this.evaluate(toParse[2]);
                switch(op)
                {
                case "-":
                    return a - b;
                case "+":
                    return a + b;
                case "/":
                    return a / b;
                case "*":
                    return a * b;
                default:
                    console.assert(!"can't happen");
                    return 0;
                }
            }
            else
            if(op === "builtin")
            {
                console.assert(toParse.length == 3);
                let a = this.evaluate(toParse[1]); 
                let b = this.evaluate(toParse[2]);
                return this.doBuiltin(a, b);
            }
            else
            {
                // recursive evaluation of all list members
                let L = toParse.slice(); // dup
                let result = [];
                while( L.length() )
                {
                    let first = L.shift(); // popfront
                    result.push(this.evaluate(first));

                    /*
                    p = Polylist.cons(evaluate(L.first()), p);
                    L = L.rest();
                    */
                }
                return result;
            }
        }
    } // end evaluate

    /**
     * Evaluates the arguments following the builtin operator. So far only
     * implemented for expectancy and syncopation
     *
     * @param arg1
     * @param arg2
     * @return
     */
    doBuiltin(a, b)
    {
        switch(a)
        {
        case "chord-family":
            if(Array.isArray(b))
            {
                let families = b;
                let chords = this.score.getChordProg();
                let currentChord = chords.getCurrentChord();
                if(!currentChord) return 0;
                let family = currentChord.getFamily();
                return families.indexOf(family) != -1;
            }
            else
                return 0;

        case "brick":
            if(typeof(b) !== "string")
                return 0;
            else
            {
                let brickName = b;
                this.score.ensureRoadmap();
                let chords = this.score.getChordProg();
                let currentBlock = chords.getBlockAtSlot(this.chordSlot);
                if(currentBlock == null)
                    return 0;
                let blockName = currentBlock.getDashedName();
                if(brickName == blockName)
                   return 1;
                else
                {
                    console.warn(`Grammar at slot ${this.chordSlot} ` +
                        `brickname ${brickName} doesn't match ${blockName}`);
                    return .1; // huh?
                }
            }

        default:
            console.warn("Grammar encountered unknown builtin " + a);
            return 0;
        }
    }
}

export default Grammar;