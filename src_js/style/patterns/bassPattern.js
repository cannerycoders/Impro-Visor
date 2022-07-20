import {Pattern} from "./pattern.js";
import {Duration} from "../../data/duration.js";
import {NoteSymbol} from "../../data/noteSymbol.js";
import {ChordForm} from "../../data/chordForm.js";
import {Key} from "../../data/Key.js";
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
    /**
     * constructor implements static makePattern
     * @param {Style} style 
     * @param {Array} plist 
     * @param {Object} definedMap - ie syle's bassPatternMap
     */
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

    /**
     * 
     * @returns total duration of this pattern
     */
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
        let result = []; // array of NoteSymbol
        let chordRoot = chord.getRootString(); 
        let chordForm = chord.getForm();
        let key = chordForm.getKey(chordRoot);
        let rise = PitchClass.findRise(chordRoot);

        // indicator for directional placement
        int indicator = STAY;

        int volume = 127;

        while( i.hasNext() )
        {
            int rule = i.next();
            String duration = j.next();
            String modifier = m.next();
            MelodySymbol melodySymbol;
    //System.out.println("applying bass rule " + rule + ", duration = " + duration + ", modifier = " + modifier);
            switch( rule )
            {
                case VOLUME:
                {
                    melodySymbol = new VolumeSymbol(duration);
                    //System.out.println("creating VolumeSymbol: " + melodySymbol);
                    break;
                }

                case PITCH: // Allow X for bass too, 
                // as a convenience in cutting and pasting in editor
                case BASS:
                {
                    melodySymbol = new NoteSymbol(chord.getBass());
                    break;
                }

                case NEXT:
                {
                    // FIX: This may be broken (octave jumps). Please check
                    NoteSymbol noteSymbol = new NoteSymbol(nextChord.getBass());
                    if( i.hasNext() )
                    {
                        melodySymbol = noteSymbol;
                    }
                    else
                    {
                        melodySymbol = new NoteSymbol(
                                noteSymbol.getPitchClass(),
                                noteSymbol.getOctave(),
                                Duration.getDuration0(duration));
                    }
                    Polylist L = Polylist.list(duration, melodySymbol);
                    basslineSegment.add(L);
                    return basslineSegment;
                }

                case CHORD:
                {
                    Polylist chordTones =
                            (Polylist) chordForm.getSpell(chordRoot, key);
                    if( chordTones.length() > 1 )
                    {
                        chordTones = lastNote.enhDrop(chordTones);
                    }
                    melodySymbol = (NoteSymbol) getRandomItem(chordTones);

                    break;
                }

                case SCALE:
                {
                    Polylist scales = (Polylist) chordForm.getScales();
                    if( scales == null || scales.isEmpty() )
                    {
                        Polylist chordTones =
                                (Polylist) chordForm.getSpell(chordRoot, key);
                        if( chordTones.length() > 1 )
                        {
                            chordTones = lastNote.enhDrop(chordTones);
                        }
                        melodySymbol = (NoteSymbol) getRandomItem(chordTones);
                        break;
                    }
                    Polylist scale = (Polylist) scales.first();

                    NoteSymbol tonic =
                            NoteSymbol.makeNoteSymbol((String) scale.first());

                    String scaleType =
                            Advisor.concatListWithSpaces(scale.rest());

                    ScaleForm scaleForm = Advisor.getScale(scaleType);

                    Polylist tones = scaleForm.getSpell(tonic);
                    tones = NoteSymbol.transposeNoteSymbolList(tones, rise);
                    tones = tones.reverse().rest().reverse();

                    Polylist seconds = getIntervals(2, tones, lastNote);
                    Polylist thirds = getIntervals(3, tones, lastNote);
                    tones = seconds.append(thirds);

                    if( tones.length() > 1 )
                    {
                        tones = lastNote.enhDrop(tones);
                    }
                    melodySymbol = (NoteSymbol) getRandomItem(tones);


                    break;
                }

                case APPROACH:
                {
                    NoteSymbol noteSymbol = new NoteSymbol(nextChord.getBass());
                    Polylist approach = Polylist.list(noteSymbol.transpose(1),
                                                    noteSymbol.transpose(-1));
                    if( approach.length() > 1 )
                    {
                        approach = lastNote.enhDrop(approach);
                    }

                    melodySymbol = (NoteSymbol) getRandomItem(approach);
                    break;
                }

                case REST:
                {
                    melodySymbol = NoteSymbol.makeNoteSymbol("r");
                    break;
                }

                case EQUAL:
                {
                    melodySymbol = new NoteSymbol(lastNote); 
                    break;
                }

                default:
                {                             // higher than 99 means flat/sharp
                    if( (rule > 0 && rule < 11) || rule > 99 )
                    {
                        Polylist scales = chordForm.getScales();

                        if( scales == null || scales.isEmpty() )
                        {
                            Polylist chordTones =
                                    chordForm.getSpell(chordRoot, key);
                            if( chordTones.length() > 1 )
                            {
                                chordTones = lastNote.enhDrop(chordTones);
                            }
                            melodySymbol = (NoteSymbol) getRandomItem(chordTones);
                            break;
                        }

                        Polylist scale = (Polylist) scales.first();

                        NoteSymbol tonic =
                                NoteSymbol.makeNoteSymbol((String) scale.first());

                        String scaleType =
                                Advisor.concatListWithSpaces(scale.rest());

                        ScaleForm scaleForm = Advisor.getScale(scaleType);

                        Polylist tones = scaleForm.getSpell(tonic);
                        tones = NoteSymbol.transposeNoteSymbolList(tones, rise);
                        tones = tones.reverse().rest().reverse();

                        // flattened notes
                        if( rule > FLATTEN && rule < FLATTEN + 11 )
                        {
                            rule = rule - FLATTEN;
                            NoteSymbol noteSymbol = getInterval(rule, tones);
                            melodySymbol = noteSymbol.transpose(-1);
                        }   // sharpened notes
                        else if( rule > SHARPEN && rule < SHARPEN + 11 )
                        {
                            rule = rule - SHARPEN;
                            NoteSymbol noteSymbol = getInterval(rule, tones);
                            melodySymbol = noteSymbol.transpose(1);
                        }
                        else
                        {
                            melodySymbol = getInterval(rule, tones);
                        }
                    }
                    else
                    {
                        melodySymbol = new NoteSymbol(chord.getBass());
                    }

                    break;
                }

            }
            
            if( melodySymbol != null )
            {
                if( melodySymbol instanceof NoteSymbol )
                {
                    NoteSymbol noteSymbol = (NoteSymbol) melodySymbol;

                    if( !noteSymbol.isRest() && rule != EQUAL )
                    {
                        // System.out.println("Original melodySymbol is " + melodySymbol.getMIDI() );

                        // Why -24??

                        noteSymbol = noteSymbol.transpose(-24);
                        //pitch = placePitchNear(melodySymbol, lastNote, style);

                        if( modifier.equals("U") )
                        {
                            noteSymbol = placePitchAbove(noteSymbol, lastNote);
                        }
                        else if( modifier.equals("D") )
                        {
                            noteSymbol = placePitchBelow(noteSymbol, lastNote);
                        }
                        else if( modifier.equals("DD") )
                        {
                            noteSymbol = placePitchOctaveBelow(noteSymbol, lastNote);
                        }
                        else
                        {
                            noteSymbol = placePitchNear(noteSymbol, lastNote, style);
                            noteSymbol = pressure(noteSymbol, style);
                        }
                    }

                    NoteSymbol note = new NoteSymbol(
                            noteSymbol.getPitchClass(),
                            noteSymbol.getOctave(),
                            Duration.getDuration0(duration),
                            volume);

                    basslineSegment.add(note);

                    if( !note.isRest() )
                    {
                        lastNote = note;
                    }
                }
                else if( melodySymbol instanceof VolumeSymbol )
                {
                    basslineSegment.add(melodySymbol);
                }
                else
                {
                    assert false;
                }
            }
        //System.out.println("rule = " + ruleTypes[rule] + " melodySymbol = " + melodySymbol);
        }
        
        return basslineSegment;

        return result;
    }

}

export default BassPattern;