/**
 * facilitates hand movement by moving the hands between the lower and 
 * upper limits based on parameters set by the user or style.
 */
export class HandManager 
{
    static sDefaults =
    {
        leftHandLowerLimit: 46,
        leftHandUpperLimit: 67,
        leftHandSpread: 9,
        leftHandMinNotes: 1,
        leftHandMaxNotes: 2,
        leftHandLowestNote: 46,

        rightHandLowerLimit: 60,
        rightHandUpperLimit: 81,
        rightHandSpread: 9,
        rightHandMinNotes: 1,
        rightHandMaxNotes: 4,
        rightHandLowestNote: 60,

        //voice leading controls
        preferredMotion: 0,
        preferredMotionRange: 3,

    };

    constructor()
    {
        for(let k in HandManager.sDefaults)
        {
            this[k] = HandManager.sDefaults[k];
        }

        this.leftHandLowestNote = null;//used for calculating limits for current chord
        this.rightHandLowestNote = null;//used for calculating the limits for current chord
        this.resetHands();
    }

    init(other)
    {
        for(let k in other)
        {
            if(this[k] != undefined)
                this[k] = other[k];
        }
    }

    serialize()
    {
        return JSON.stringify(this);
    }

    /**
     * randomly generates a number of notes for the LH to play within range
     * @return a number of notes
     */
    getNumLeftNotes()
    {
        return Math.round(Math.random()*(this.leftHandMaxNotes-this.leftHandMinNotes)) 
                + this.leftHandMinNotes;
    }

    /**
     * randomly generate a number of notes for the RH to play within range
     * @return a number of notes
     */
    getNumRightNotes()
    {
        return Math.round(Math.random()*(this.rightHandMaxNotes-this.rightHandMinNotes)) 
                + this.rightHandMinNotes;
    }

    /**
     * moves hands between chords, ensuring that voicings are in ranges.
     */
    repositionHands()
    {
      //System.out.println("lll "+this.leftHandLowerLimit+"rll "+this.rightHandLowerLimit);
      //System.out.println("lul "+this.leftHandUpperLimit+"rul "+this.rightHandUpperLimit);
      //System.out.println("ll "+this.leftHandLowestNote+"rl "+this.rightHandLowestNote);
       this.leftHandLowestNote = Math.round(this.leftHandLowestNote +   
                                ((Math.random()*2.0*this.preferredMotionRange) 
                                    - this.preferredMotionRange) + 
                                this.preferredMotion);
       this.rightHandLowestNote = Math.round(this.rightHandLowestNote +
                                ((Math.random()*2.0*this.preferredMotionRange) 
                                    - this.preferredMotionRange) +
                                this.preferredMotion);
       if(this.leftHandLowestNote<this.leftHandLowerLimit)
           this.resetLH();
       if(this.rightHandLowestNote<this.rightHandLowerLimit)
           this.resetRH();
       if(this.leftHandLowestNote+this.leftHandSpread>this.leftHandUpperLimit)
           this.resetLH();
       if(this.rightHandLowestNote+this.rightHandSpread>this.rightHandUpperLimit)
           this.resetRH();
    }

    /**
     * sets hands to a starting position based on settings
     */
    resetHands()
    {
        if(this.preferredMotion>0)//to allow motion up
        {
            this.leftHandLowestNote = this.leftHandLowerLimit;
            this.rightHandLowestNote = this.rightHandLowerLimit;
        }
        else 
        if(this.preferredMotion<0)//to allow motion down
        {
            this.leftHandLowestNote = this.leftHandUpperLimit-this.leftHandSpread;
            this.rightHandLowestNote = this.rightHandUpperLimit-this.rightHandSpread;
        }
        else //start in the middle to be able to go both ways
        {
            this.leftHandLowestNote = (this.leftHandUpperLimit-this.leftHandSpread+this.leftHandLowerLimit)/2;
            this.rightHandLowestNote = (this.rightHandUpperLimit-this.rightHandSpread+this.rightHandLowerLimit)/2;
        }
        // console.debug("HandManager: both hands reset");
    }

    resetLH()
    {
        if(this.preferredMotion>0)//to allow motion up
        {
            this.leftHandLowestNote = this.leftHandLowerLimit;
            //rightHandLowestNote=rightHandLowerLimit;
        }
        else 
        if(this.preferredMotion<0)//to allow motion down
        {
            this.leftHandLowestNote = this.leftHandUpperLimit - this.leftHandSpread;
            //rightHandLowestNote=rightHandUpperLimit-rightHandSpread;
        }
        else //start in the middle to be able to go both ways
        {
            this.leftHandLowestNote = (this.leftHandUpperLimit - this.leftHandSpread
                                        + this.leftHandLowerLimit)/2;
            //rightHandLowestNote=(rightHandUpperLimit-rightHandSpread+rightHandLowerLimit)/2;
        }
    }

    resetRH()
    {
        if(this.preferredMotion>0)//to allow motion up
        {
            //leftHandLowestNote=leftHandLowerLimit;
            this.rightHandLowestNote = this.rightHandLowerLimit;
        }
        else 
        if(this.preferredMotion<0)//to allow motion down
        {
            //leftHandLowestNote=leftHandUpperLimit-leftHandSpread;
            this.rightHandLowestNote = this.rightHandUpperLimit - this.rightHandSpread;
        }
        else //start in the middle to be able to go both ways
        {
            //leftHandLowestNote=(leftHandUpperLimit-leftHandSpread+leftHandLowerLimit)/2;
            this.rightHandLowestNote = (this.rightHandUpperLimit - 
                    this.rightHandSpread+this.rightHandLowerLimit)/2;
        }
    }

}