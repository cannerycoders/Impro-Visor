import {Pattern} from "./pattern.js";
import {ChordPatternVoiced} from "./chordPatternVoiced.js";

const sRuleTypes = [
    "X", "R", "V", 
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"
];
const sRuleCodes = ["X", "R", "V"].map(v => v.charCodeAt(0));
const sIRule = 
{
    STRIKE: 0,
    REST: 1,
    VOLUME: 2,
};
export class ChordPattern extends Pattern
{
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
        this.pushString = "";
        this.pushAmount = 0;
        this.definedRules = definedRules;
        this.rules = []; // of string
        this.durations = []; // of string

        if(!plist) return;
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
            case "push":
                if(p.length >= 1)
                {
                    // eg: [push, 8/3]
                    this.pushString = p.slice(1).join(" ");
                    this.pushAmount = Duration.getDuration(this.pushString);
                };
                break;
            case "rules":
                for(let r of p.slice(1))
                {
                    if(!Array.isArray(r))
                    {
                        let rule = r[0];
                        let dur = r.slice(1);
                        if(sRuleCodes.indexOf(rule.charCodeAt(0)) != -1)
                            this.addRule(rule, dur);
                        else
                            console.warn("ChordPattern unrecognized rule: " + rule);
                    }
                    else
                    {
                        // check to see if it is an S-expression
                        // eg (X 5 4)
                        if(r.length >= 3 && r[0] == "X")
                        {
                            let rule = `${r[1]}`; // toString
                            let duration = `${r[2]}`; // toString
                            this.addRule(rule, duration);
                        }
                        else
                            console.warn("ChordPattern unrecognized segment " + r);
                    }
                }
                break;
            case "weight":
                this.weight = Number(p[1]); // in parent class
                break;
            case "use": // apears to be unused, leave it unimplemented for now.
            default:
                console.warn("ChordPattern unexpected key " + p[0]);
                break;
            }
        }
    }
    getDefinedRules() { return this.definedRules; }
    setDefinedRules(map)
    {
        this.definedRules = map;
    }

    addRule(rule, dur)
    {
        this.rules.push(rule);
        this.durations.push(dur);
    }

    /**
     * Returns the duration, in slots
     */
    getDuration()
    {
        let duration = 0;
        for(let i=0;i<this.rules.length;i++)
        {
            let r = this.rules[i];
            if(r != "V") // Ignore volume in computing duration
            {
                let dur = this.durations[i];
                duration += Duration.getDuration(dur);
            }
        }
        return duration;
    }

    /**
     * Returns an ArrayList of exactly two chord patterns, splitting
     * up this pattern. But the second pattern will be null if empty.
     * @param desiredDuration
     * @return 
     */
    splitChordPattern(desiredDuration)
    {
        //System.out.println("Splitting for desired " + desiredDuration + " " + this);
        let prefix = new ChordPattern();
        let suffix = new ChordPattern();

        let rule;
        let dur;
        // copy first part of pattern to prefix
        let accumulatedDuration = 0;
        let difference = accumulatedDuration - desiredDuration;
        let ir = 0;
        for(ir = 0; ir < this.rules.length; ir++)
        {
            let rule = this.rules[ir];
            let dur = this.durations[ir];

            if( rule.equals(VOLUME_STRING) )
            {
                prefix.rules.push(rule);
                prefix.durations.push(dur);
            }
            else
            {
                let slots =  Duration.getDuration(dur);
                accumulatedDuration += slots;
                difference = accumulatedDuration - desiredDuration;
                if(difference <= 0)
                {
                    prefix.rules.add(rule);
                    prefix.durations.add(dur);
                }
                else
                if(difference > 0)
                {
                    // split a single rule
                    prefix.rules.add(rule);
                    prefix.durations.add(Note.getDurationString(slots-difference));
                    suffix.rules.add(rule);
                    suffix.durations.add(Note.getDurationString(difference));           
                }
            }
            if(difference < 0)
                break;
        }
    
        // copy remainder of pattern to suffix
        while( ir < this.rules.length)
        {
            suffix.rules.add(this.rules[ir]);
            suffix.durations.add(this.durations[ir]);                   
            ir++;
        }

        return [prefix, suffix];
    }
    /**
     * Realizes the Pattern into a sequencable Polylist.
     * @param chord     the ChordSymbol to voice
     * @param lastChord a Polylist containing the last chord voicing
     * @return A list that can be sequenced.  This has two elements.
     * 
     * The first element is another Polylist that contains
     * a sequence of chord voicings (each of which is a Polylist of
     * NoteSymbols, including possibly volume settings.)  
     * 
     * The second element is a MelodyPart containing
     * containing rests, each of which is a duration corresponding to
     * the voicings.
     */
    applyRules(chord, lastChord, style)
    {
        /*
        Iterator<String> i = rules.iterator();
        Iterator<String> j = durations.iterator();
        
        lastChord = BassPattern.filterOutStrings(lastChord);

        //System.out.println("applyRules in: Chord = " + chord + " previous Voicing = " + lastChord + ", rules = " + rules + ", durations = " + durations);

        String chordRoot = chord.getRootString();
        ChordForm chordForm = chord.getChordForm();
        Key key = chordForm.getKey(chordRoot);
        int rise = PitchClass.findRise(chordRoot);

        // FIXME: this is sort of a hacky way to do the durations since we
        // don't really have a proper way to store music with multiple voices
        MelodyPart durationMelody = new MelodyPart();

        LinkedList<Polylist> chordLine = new LinkedList<Polylist>();
        
        int volume = 127;

        while( i.hasNext() )
            {
            String rule = i.next();
            String duration = j.next();

            //System.out.println("     rule = " + rule + ", duration = " + duration);
            // Process the symbols in the pattern into notes and rests,
            // inserting volume indication when the volume changes.
            
            if( rule.equals(STRIKE_STRING) )
            {
                durationMelody.addNote(new Rest(Duration.getDuration(duration)));
                //System.out.println("durationMelody = " + durationMelody);
                Polylist voicing = findVoicing(chord, lastChord, style);

                if( voicing == null )
                {
                voicing = Polylist.nil;
                //break;
                }
                //System.out.println("voicing = " + voicing);
                chordLine.add(voicing.cons(new VolumeSymbol(volume)));
                lastChord = voicing; 
            }
            
            else if( rule.equals(REST_STRING) )
            {
                durationMelody.addNote(new Rest(Duration.getDuration(duration)));
                chordLine.add(Polylist.nil); // was NoteSymbol.makeNoteSymbol("r" + duration));
            }
            
            else if( rule.equals(VOLUME_STRING) )
            {
                // Volume will take effect when next chord voicing is appended.
                volume = Integer.parseInt(duration);
            }
            
            else
            {
            // This is used for single notes in the chord pattern, 
            // such as in the "La Bomba" pattern:
            // (X 1 4) (X 3 8) (X 5 8) (X 3 4) (X 6 8) (X 5 8)
                int interval = Integer.parseInt(rule);
                
                durationMelody.addNote(new Rest(Duration.getDuration(duration)));
                    
                // first, get the note that is the interval from the root
                Polylist scales = chordForm.getScales();
                    
                Polylist scale = (Polylist) scales.first();
                NoteSymbol tonic = NoteSymbol.makeNoteSymbol( (String) scale.first() );
                String scaleType = Advisor.concatListWithSpaces(scale.rest());
                ScaleForm scaleForm = Advisor.getScale(scaleType);
                    
                Polylist tones = scaleForm.getSpell(tonic);
                tones = NoteSymbol.transposeNoteSymbolList(tones, rise);

                tones = tones.reverse().rest().reverse();
                //System.out.println("The transposed notes are: " + tones);
                    
                // with the note symbol, we can get the chord base, which will
                // be used for the chord
                NoteSymbol noteSymbol = BassPattern.getInterval(interval, tones);
                PitchClass pitchClass = noteSymbol.getPitchClass();
                String noteBass = pitchClass.getChordBase();
                    
                String chordName = (String)noteBass.concat("Note");
                Chord newChord = new Chord(chordName);
                    
                //System.out.println("The new chord is " + newChord);
                    
                Polylist voicing = findVoicing(newChord.getChordSymbol(), lastChord, style);
                    
                // then add the voicing of the chord to the chord line
                if( voicing == null )
                {
                    voicing = Polylist.nil;
                }
                    
                //System.out.println("The voicing for this chord is: " + voicing);
                                
                chordLine.add(voicing.cons(new VolumeSymbol(volume)));
                lastChord = voicing;
                    
                //System.out.println("The duration melody is: " + durationMelody);
                //System.out.println("The chord line is: " + chordLine);
            }
        }

        ChordPatternVoiced result = new ChordPatternVoiced(chordLine, durationMelody);

        //System.out.println("applyRules: Chord = " + chord + ", rules = " + rules + ", durations = " + durations + ", result (chordline, durations) = " + result);
        return result;
        */
    }
}
