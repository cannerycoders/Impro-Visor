import {VoicingSettings} from "./voicingSettings.js";

/**
 * This class is the actual voicing calculator for ChordPattern
 * and appears to only be relevant when operating in CUSTOM/"fluid"
 * style mode.  NB: this is *not* used by BassPattern.
 * 
 * Parameters can be loaded via .fv files.
 * 
 * Instructions: Initialize with priorities in constructor.
 * Chord notes, hand bounds, and number of notes per hand should be set with 
 * each new chord. The number of notes/hand, and hand bounds should have some 
 * degree of randomness.  Call calculate to actually calculate chord tones, 
 * and then get the integer array of midi notes to be played.
 *
 * The way this class works: all available notes are individually weighted, 
 * and array lists are created containing n duplicates of each note in each 
 * hand's range where n is the weight of the note. Notes are picked randomly 
 * from the list.  Once a note is picked, the notes around it and exactly an 
 * octave (and multiple octaves) above and below are multiplied by the 
 * multiplier settings and new array lists are generated.  The process 
 * is repeated until the array list is empty or the number of desired 
 * notes is reached.
 */
export class VoicingGenerator
{
    /* ----------------------------------------------------------------------- */
    constructor(init={})
    {
        this.allMidiValues = []; // int
        this.allMidiValues.length = 128;
        this.color = []; // int (set by style/stylePatterns)
        this.priority = []; // int (set by style/stylePatterns)
        this.leftHand = []; // int
        this.rightHand = []; // int
        this.allLeftValues = [];
        this.allRightValues = [];
        this.previousVoicing = [];
        this.numNotesLeft = 0;
        this.numNotesRight = 0;
        this.root = 0;

        this.vs = new VoicingSettings();
    }

    initVoicingSettings(avs) // aka: getVoicingSettings(avs)
    {
        this.vs.init(avs);
    }

    // invoked by stylePatterns/ChordPattern.java for "custom voicing" (fluid)
    initHandSettings(hm) // aka: getHandSettings
    {
        this.vs.hm.init(hm);
        this.numNotesLeft = hm.getNumLeftNotes(); // random
        this.numNotesRight = hm.getNumRightNotes(); // random
    }

    setColor(clist)  // controlled by style
    {
        this.color = clist;
    }

    setPriority(priorityList)  // controlled by style
    {
        this.priority = priorityList;
    }

    setRoot(r)
    {
        this.root = r;
    }

    setPreviousVoicing(pv)
    {
        this.previousVoicing = pv;
    }

    /**
     * generates a voicing based on current parameters and stores it 
     * in the chord array accessible by get chord. 
     */
    calculate()
    {
        this.allLeftValues = [];
        this.allRightValues = [];
        this.leftHand = [];
        this.rightHand = [];
        
        let noteToAdd;
        let start = 0;
        let vs = this.vs;
        let hm = this.vs.hm;
        
        if(vs.voiceAll)
        {
            //System.out.println("Driving VAN");
            //enable only chord notes
            for(let i=0; i<this.allMidiValues.length; i++)
                this.allMidiValues[i] = 0;

            for(let i=0; i<this.priority.length; i++)
            {
                this.setupNote(this.priority[i], 
                        Math.floor(vs.maxPriority*10 
                            - i*10*vs.priorityMultiplier));
            }
            if(vs.rootless)
                this.setupNote(vs.root, 0);
            
            //do usual calculations, modded to ensure all notes happen
            for(let i=0;i<this.priority.length; i++)
            {
                this.setupAllLeftValues();   
                if(this.allLeftValues.length)
                {
                    if(i<this.priority.length)
                    {
                        let ix = Math.random()*this.allLeftValues.length;
                        noteToAdd = this.allLeftValues[ix];
                        this.leftHand.push(noteToAdd);
                        this.allMidiValues[noteToAdd] = 0;
                        if(this.allMidiValues[noteToAdd+1]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd+1] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd-1]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd-1] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd+2]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd+2] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd-2]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd-2] *= this.halfStepReducer;
                        this.multiplyNotes(noteToAdd, 0);
                    }
                }
                this.setupAllRightValues();
                if(!this.allRightValues.isEmpty())
                {
                    if(i<this.priority.length)
                    {
                        let ix = Math.floor(Math.random()*allRightValues.size());
                        noteToAdd = this.allRightValues[ix];
                        this.rightHand.push(noteToAdd);
                        this.allMidiValues[noteToAdd]=0;
                        if(this.allMidiValues[noteToAdd+1]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd+1] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd-1]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd-1] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd+2]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd+2] *= vs.halfStepReducer;
                        if(this.allMidiValues[noteToAdd-2]*vs.halfStepReducer>0)
                            this.allMidiValues[noteToAdd-2] *= vs.halfStepReducer;
                        this.multiplyNotes(noteToAdd,0);
                    }
                }
            }
            if(this.rightHand.length<this.leftHand.length)
                start = this.rightHand.length;
            else
                start = this.leftHand.length;
        }
        
        // begin normal algorithm
        this.initAllMidiValues();
        if(vs.rootless)
            this.setupNote(this.root, 0);
        for(let i of this.leftHand)
            this.allMidiValues[i] = 0;
        for(let i of this.rightHand)
            this.allMidiValues[i] = 0;
        if(this.previousVoicing.length)
            this.weightPreviousVoicing();

        console.debug((this.numNotesLeft+this.numNotesRight)+" #notes exp.");
        console.debug(this.hm.rightHandLowerLimit+" lower right bound");

        for(let i = start; i<this.numNotesLeft || i<this.numNotesRight; i++)
        {
            this.setupAllLeftValues();   
            if(this.allLeftValues.length)
            {
                if(this.leftHand.length<this.numNotesLeft)
                {
                    let il = Math.floor(Math.random()*this.allLeftValues.length);
                    noteToAdd = this.allLeftValues[il];
                    this.leftHand.push(noteToAdd);
                    this.allMidiValues[noteToAdd]=0;
                    this.allMidiValues[noteToAdd+1] *= vs.halfStepReducer;
                    this.allMidiValues[noteToAdd-1] *= vs.halfStepReducer;
                    this.allMidiValues[noteToAdd+2] *= vs.fullStepReducer;
                    this.allMidiValues[noteToAdd-2] *= vs.fullStepReducer;
                    this.multiplyNotes(noteToAdd, vs.repeatMultiplier);
                    for(let j=0; j<vs.leftMinInterval; j++)
                    {
                        this.allMidiValues[noteToAdd+j]=0;
                        this.allMidiValues[noteToAdd-j]=0;
                    }
                }
                else
                    console.debug("LH EMPTY: Req:"+this.numNotesLeft+" act:");
            }
            setupAllRightValues();
            if(this.allRightValues.length)
            {
                if(this.rightHand.length<this.numNotesRight)
                {
                    let ix = Math.floor(Math.random()*allRightValues.size());
                    noteToAdd=allRightValues[ix];
                    this.rightHand.push(noteToAdd);
                    this.allMidiValues[noteToAdd]=0;
                    this.allMidiValues[noteToAdd+1] *= vs.halfStepReducer;
                    this.allMidiValues[noteToAdd-1] *= vs.halfStepReducer;
                    this.allMidiValues[noteToAdd+2] *= vs.fullStepReducer;
                    this.allMidiValues[noteToAdd-2] *= vs.fullStepReducer;
                    this.multiplyNotes(noteToAdd, vs.repeatMultiplier);
                    for(let j=0; j<vs.rightMinInterval; j++)
                    {
                        this.allMidiValues[noteToAdd+j]=0;
                        console.debug("noteToAdd+i: "+(noteToAdd+i));
                        this.allMidiValues[noteToAdd-j]=0;
                    }
                }
            
            }
            else
                console.debug("RH EMPTY");
           // System.out.println("calculate called");
            if(vs.invertM9)
            {
                this.invertM9th(this.leftHand, this.leftHand);
                this.invertM9th(this.leftHand, this.rightHand);
                this.invertM9th(this.rightHand, this.rightHand);
            }
        }
    }

    /**
     * returns int array with all notes in chord.
     */
    getChord()
    {
        let chord = [];
        chord.push(...this.rightHand);
        chord.push(...this.leftHand);
        return chord;
    }

    /**
     * this is for voice leading, makes it likelier to hit notes in or near 
     * the last voicing.
     */
    weightPreviousVoicing()
    {
        console.assert(this.previousVoicing);
        let vs = this.vs;
        for(let n of this.previousVoicing)
        {
            this.allMidiValues[n] = Math.floor(this.allMidiValues[n] * vs.previousVoicingMultiplier);
            this.allMidiValues[n+1] = Math.floor(this.allMidiValues[n+1] * vs.halfStepAwayMultiplier);
            this.allMidiValues[n-1] = Math.floor(this.allMidiValues[n-1] * vs.halfStepAwayMultiplier);
            this.allMidiValues[n-2] = Math.floor(this.allMidiValues[n-2] * vs.fullStepAwayMultiplier);
            this.allMidiValues[n+2] = Math.floor(this.allMidiValues[n+2] * vs.fullStepAwayMultiplier);
        }
    }

    /**
     * sets up the midi values for a new chord 
     */
    initAllMidiValues()
    {
        //start with everything at zero
        for(let i=0; i<this.allMidiValues.length; i++)
            this.allMidiValues[i]=0;
        
        let vs = this.vs;
        for(let i=0; i<this.color.length; i++)
        {
            this.setupNote(this.color[i], vs.leftColorPriority*10);
            this.setupNote(this.color[i], vs.rightColorPriority*10, 
                          vs.hm.rightHandLowerLimit);
        }
        for(let p=0; p<this.priority.length; p++)
        {
            this.setupNote(this.priority[p], 
                    Math.floor(vs.maxPriority*10 - p*10*vs.priorityMultiplier));
        }
    }

    /**
     * sets up left array list
     */
    setupAllLeftValues() 
    {
        let hm = this.vs.hm;
        this.allLeftValues = [];
        for(let i=hm.leftHandLowerLimit; i<=hm.leftHandUpperLimit; i++)
        {
            for(let j=0; j<this.allMidiValues[i]; j++)
                this.allLeftValues.push(i);
        }
    }
    /**
     * sets up right array list
     */
    setupAllRightValues() 
    {
        let hm = this.vs.hm;
        this.allRightValues = [];
        for(let i=hm.rightHandLowerLimit; i<=hm.rightHandUpperLimit; i++)
        {
            for(let j=0; j<this.allMidiValues[i]; j++)
                this.allRightValues.push(i);
        }
    }

    /**
     * Sets up all of a certain note to a certain value in all octaves above the note start
     * @param midiValue the note (gets converted to mod12)
     * @param priority  the value to set up the note to
     * @param start the note from which to start setting up the note
     */
    setupNote(midiValue, priority, start)
    {
        if(start == undefined) start = midiValue;
        midiValue = midiValue%12;
        for(let i=start; i<this.allMidiValues.length; i++)
        {
            if(i%12 == midiValue)
                this.allMidiValues[i] = priority;
        }
    }
    /**
     * checks 2 lists for a minor 9th between them, and flips the interval to a 
     * maj. 7th. the lists may be the same list and we must take to to make
     * modifications "in-place".
     * @param list1
     * @param list2 
     */
    invertM9th(list1, list2)
    {   
//        System.out.println("invm9");
        let list3 = [];
        for(let i of list1)
        {
            let added = false;
            let list4 = [];
            for(let j of list2)
            {
                //System.out.println("invoked, i:"+ i+", j:"+j);
                if(j-i==13)
                {
                    if(added)
                        list3.pop(); // list3.remove(list3.size()-1);
                    list3.push(i+1);
                    added = true;
                    list4.push(j-1);
                    // console.debug("Inverted m9 i"+i+" "+j);
                }
                else
                {
                    list4.push(j);
                    if(!added)
                    {
                        list3.push(i);
                        added = true;
                    }
                }
            }
            list2.length = 0;
            list2.push(...list4);
        }
        list1.length = 0;
        list1.push(...list3);
        
//        for(int i: list1)
//            System.out.print(i+" ");
//        System.out.println("list1");
//        for(int j: list2)
//            System.out.print(j+" ");
//        System.out.println("list2");
    }

    /**
     *  generates array with notes in LH
     * @return int array
     */
    getLeftHand()
    {
        return this.leftHand.slice(); // clone new int[leftHand.size()];
        /*
        for(int i=0; i<leftHand.size(); i++)
        {
            leftArray[i]=leftHand.get(i);
        }
        return leftArray;
        */
    }

    /**
     * generates array with notes in RH
     * @return int array
     */
    getRightHand()
    {
        return this.rightHand.slice(); // clone
    }

    /**
     * returns int array with all notes in chord.
     */
    getChord()
    {
        let chord = [];
        chord.push(...this.rightHand);
        chord.push(...this.leftHand);
        return chord;
    }
}

export default VoicingGenerator;