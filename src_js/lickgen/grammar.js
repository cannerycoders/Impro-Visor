import fs from "fs";
import yaml from "js-yaml";

export default class Grammar
{
    constructor ()
    {
        this.rules = null;
        this.startSymbol = null;
        this.currentSlot = -1;
        this.chordSlot = -1;
    }

    load(file)
    {
        try 
        {
            this.rules = yaml.load(fs.readFileSync(file, "utf8"));
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