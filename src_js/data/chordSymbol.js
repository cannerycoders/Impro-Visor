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
    /**
     * Creates a ChordSymbol with the specified name.
     * @param name          a String containing the chord name
     */
    static Make(name)
    {

    }

    /* ------------------------------------------------------------------- */
    /**
     * ChordSymbol constructor
     * @param {PitchClass} root 
     * @param {string} type 
     * @param {PitchClass} bass 
     * @param {ChordSymbol} polybass 
     */
    constructor(root, type, bass, polybass)
    {
        this.name;
        this.type;
        this.chordForm;
        this.voicing
        this.extension;
        this.rootString;
        this.root; // pitchclass
        this.bass; // pitchclass
        this.polybase; // polychord component, if any (chord over which this chord is placed)
    }
}