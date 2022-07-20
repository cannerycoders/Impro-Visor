 import {Advisor} from "./advice/advisor.js";
 import {Constants} from "../constants.js";
 import {PitchClass} from "./pitchclass.js";
 import {Key} from "./key.js";
 import {ChordForm} from "./chordForm.js";

 /**
  * A ChordSymbol is an object representing a chord in lightweight form,
  * from which a Chord can be created given a rhythmValue or duration and
  * possibly other parameters. 
  * It is like a pre-parsed string version of a chord from a leadsheet.
  * The parsed components are:
  *
  * - The type of chord, such as M7, 7, 7#5, etc.
  * - The root of the chord
  * - The bass note (defined to be the root if not otherwise specified).
  * - The polychord component, which is another ChordSymbol over which this chord is placed.
  */
export class ChordSymbol
{
    static makeChordSymbol(nm)
    {
        if(nm == Constants.NOCHORD)
            return new ChordSymbol(nm);
        
        let match = ChordForm.sParseChord.exec(nm);
        if(!match) return null;

        let rootPC = PitchClass.getPitchClass(match[1]);
        if(!rootPC) return null;

        let ctype = match[2];

        // (3) handle slash and polychords
        // as mutually exclusive.
        let bassPC, polybaseCS;
        if(match[3])
        {
            if(match[3][0] == "/")
            {
                bassPC = PitchClass.getPitchClass(match[3].slice(1));
                if(!bassPC) return null;
            }
            else
            {
                // we have a polychord
                polybaseCS = this.makeChordSymbol(match[3].slice(1));
                bassPC = polybaseCS.getBass();
            }
        }
        else
            bassPC = rootPC;

        return new ChordSymbol(rootPC, ctype, bassPC, polybaseCS);
    }
    static capitalize(nm)
    {
        if(nm.length)
        {
            return nm[0].toUpperCase() + nm.slice(1);
        }
        else
            return nm;
    }

    /* ------------------------------------------------------------------- */
    /**
     * ChordSymbol constructor
     * @param {PitchClass} root 
     * @param {string} type 
     * @param {PitchClass} bass 
     * @param {ChordSymbol} polybase 
     */
    constructor(root, type, bass, polybase)
    {
        if(root == Constants.NOCHORD)
        {
            this.name = root;
            this.type = Constants.NOCHORD;
            this.root = PitchClass.cClass;
            this.base = PitchClass.cClass;
            this.rootString = Constants.CROOT;
            return;
        }
        this.rootString = ChordSymbol.capitalize(root.toString());
        this.type = type;
        this.root = root; // pitchclass
        this.bass = bass; // pitchclass
        this.polybase = polybase;
        if(polybase)
        {
            this.name = `${this.rootString}\\${polybase.toString()}`;
        }
        else
        {
            let cbass = ChordSymbol.capitalize(bass.toString());
            this.name = `${this.rootString}/${cbass}`;
        }
        this.chordForm = null;
        this.voicing = [];
        this.extension = [];
        this.checkValidity(); // initializes chordForm
    }

    getName() { return this.name; }
    getFamily() { return this.chordForm ? this.chordForm.getFamily() : "other"; }
    getRoot() { return this.root; } // pitchclass
    getRootString() { return this.rootString; } 
    getType() { return this.type; }
    getChordForm() { return this.chordForm; }
    getBass() { return this.bass; }
    getPolybase() { return this.polybase; }
    toString() { return this.name; }
    isNOCHORD() { return this.type == Constants.NOCHORD; }
    isSlashChord() { return this.root.enharmonic(this.bass); }
    enhChord(other) { 
        return this.type == other.type &&
                this.root.enharmonic(other.root);
    }
    enhMember(cslist)
    {
        for(let v of cslist)
        {
            if(this.enhChord(v)) 
                return true;
        }
        return false;
    }

    getTypeIndex(note)
    {
        if(!this.chordForm)
            return Constants.NoteTypes.ChordTone;
        else
            return this.chordForm.getTypeIndex(note, -this.root.getSemitones());
    }

    /**
     * Returns the ChordSymbol's generic quality, accounting for polychords
     * and slash chord
     * @return a String of the ChordSymbol's quality.
     */
    getQuality() 
    {
        if (this.polybase != null)
            return this.polybase.getQuality();
        
        let quality = this.name;
        if(quality.length() > 1 && 
            (quality.charCodeAt(1) == Constants.FLATCHAR || 
             quality.charCodeAt(1) == Constants.SHARPCHAR))
        {
            quality = quality.substring(2);
        }
        else
            quality = quality.substring(1);
        
        if(this.isSlashChord())
            quality = quality.split("/")[0];

        return quality;
    }

    getVoicing() { return this.voicing; }
    setVoicing(v)
    {
        this.voicing = v;
    }

    getExtension() { return this.extension; }
    setExtension(x) 
    { 
        this.extension = x; 
    }

    /**
     * Check whether chord is known in the list of all chords.  If it is, set 
     * this.chordForm for future access.
     */
    checkValidity()
    {
        let Cversion = "C" + this.type;
        this.chordForm = Advisor.getChordForm(Cversion);
        if(!this.chordForm)
            return false;
        if(this.polybase)
            return this.polybase.checkValidity();
        return true;
    }
}