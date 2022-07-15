import {Constants} from "../constants.js";
/**
 * The Part class is representative of an arbitrarily long melody or chord part, 
 * played by a specific instrument at a particular volume, etc.
 * A Part contains a sequence of Units (Chords, Notes/Rests) stored in 
 * this.unitList is a sparse fashion.  See discussion below on the slots versus 
 * unitList approaches.   Part is subclassed by ChordPart, MelocyPart, etc.
 * Here we provide shared unitList management and timing utilities.
 * 
 * Units should be added using the setUnit method, which will automatically
 * adjust rhythm values.  Due to the combination of explicit size and the
 * sparse unitList, rests can be both explict and implicitly present.
 * An empty unitList is serialized a sufficient rests to fill out the size.
 * 
 * Since this implementation targets lick-generation, support for 
 * editing (cut/copy/paste) is weak/unimplemented.
 */
const sDefaultVolume = 85;
const sDefaultMetre = [4, 4];
const sDefaultBeatValue = 100;
const sDefaultInstrument = 1; // GM: piano
const sDefaultSwing = .67;
const sDefaultKeySig = 0;
const sDefaultSize = 0;  // measured in numslots 

export class Part
{
    // on parts
    //   contains units (chords or notes) and so can't represent
    //   eg held-arpeggios (though a chord implementation could manage 
    //   that one scenario).  This may not be a big limitation because
    //   score has-a list of parts (whose use could get MPE/smarmy).
    //   fgrid has-a (sparse) list per midi-key so doesn't require
    //     a separate notion of chord.
    // on slots versus unitList:
    // slots: 
    //   - slots.length represents the part-length
    //   - a unit's position is implied by its index in slots.
    //   - imposes time quantization
    //   - requires exhaustive slot maintenance (iterating over all slots)
    //   - isn't sparse but relatively efficient since js arrays elements 
    //      can be empty
    //   - what's playing query requires interation ~O(nslots)
    //   - separate management of numUnits required
    //   - units have only duration
    //   - "requires" part-iterator to walk units
    // unitlist:
    //   - separate management/enforcement of size
    //   - sparse interval managment/maintenance
    //   - what's playing query requires interation ~O(nevents)
    //   - trivial numUnits
    //   - units have position and duration
    //   - when timeline is sparsely populated, rests are inferred
    //   
    constructor()
    {
        this.unitList = []; // vs slots
        this.volume = sDefaultVolume;
        this.keySig = sDefaultKeySig;
        this.beatValue = sDefaultBeatValue;
        this.measureLength = 0; // valid after setMetre
        this.swing = sDefaultSwing;
        this.instrument = sDefaultInstrument;
        this.size = sDefaultSize;
        this.metre = [0, 0];
        this.setMetre(sDefaultMetre[0], sDefaultMetre[1]);
    }

    dumpUnits()
    {
        console.log("Part size " + this.size);
        for(let u of this.unitList)
        {
            console.log(u.getBegin(), u.getEnd());
        }
    }

    setMetre(top, bottom)
    {
        this.metre[0] = top;
        this.metre[1] = bottom;
        this.beatValue = Constants.WHOLE / this.metre[1];
        this.measureLength = this.beatValue * this.metre[0];
    }

    getBars()
    {
        return Math.ceil(this.size / this.measureLength);
    }

    /**
     * Returns the number of active measures, meaning ones that contain the start of
     * a chord or non-rest note
     *
     * @return int the end time of the Part
     */
    getActiveBars()
    {
        if(!this.unitList.length)
            return 0;
        else
            return Math.ceil(this.unitList[this.unitList.length-1].getEnd() / this.measureLength);
    }

    /**
     * Adds a Unit to the end of the Part, extending the length as it goes. Note
     * that 0 duration units should not be added, as they will cause this to fail.
     *
     * @param unit the Unit to add
     */
    addUnit(unit)
    {
        let dur = unit.getDuration();
        if(dur <= 0) return;

        let begin = 0;
        if(this.unitList.length)
            begin = this.unitList[this.unitList.length-1].getEnd();
        unit.setBegin(begin);
        this.size = unit.getEnd();

        this.unitList.push(unit);

        // This tends to reduce the number of accidentals in the notation.
        if(unit.fixupKey)
            unit.fixupKey(this.keySig);
    }

    /**
     * Inserts unit into unitList at the specified unitIndex.
     * When an intersection occurs
     *
     * @param unitIndex the slotindex (time) for the Unit in
     * @param unit the Unit to set
     */
    setUnit(unitIndex, unit)
    {
        if(unitIndex >= this.size || unitIndex < 0)
            return; // shouldn't happen, but can.

        //Trace.log(0, "setting Unit at " + unitIndex + " to " + (unit == null ? null : unit.toLeadsheet()));
        // if we want to set it to empty, we are effectively deleting it
        if(unit == null)
        {
            this.delUnit(unitIndex);	
            return;
        }

        unit.setBegin(unitIndex);
        if(this.unitList.length == 0)
        {
            // trivial first add
            this.unitList.push(unit);
            return;
        }

        // here we insert unit so that its begin is properly sorted.
        // then we invoke makeConsistent to handle edge conditions.
        // neighbors accordingly
        let last;
        for(let i=0;i<this.unitList.length;i++)
        {
            last = this.unitList[i];
            if(unitIndex <= last.getBegin())
            {
                this.unitList.splice(i, 0/*delete none*/, unit);
                this.makeConsistent();
                return;
            }
            else
            if(unitIndex < last.getEnd())
            {
                last.clipEnd(unitIndex);
                this.unitList.splice(i+1, 0/*delete none*/, unit);
                this.makeConsistent();
                return;
            }
        }
        // unitIndex after the last begin
        if(last.getEnd() > unitIndex)
            last.clipEnd(unitIndex);
        this.unitList.push(unit);
    }

    makeConsistent()
    {
        let ctx = { lastEnd: null };
        let newList = this.unitList.filter((el) =>
        {
            if(ctx.lastEnd == null) 
            {
                ctx.lastEnd = el.getEnd();
                return true; // always keep the first
            }
            if(ctx.lastEnd >= el.getEnd()) 
            {
                // el is filtered, lastEnd still abides
                return false;
            }
            else
            if(ctx.lastEnd > el.getBegin())
            {
                // clip uNow, but don't filter it. 
                el.clipBegin(ctx.lastEnd);
            }
            ctx.lastEnd = el.getEnd();
            return true;
        });
        this.unitList = newList;
        if(this.size != ctx.lastEnd)
        {
            console.log(`Part sizechange ${this.size} -> ${ctx.lastEnd}`);
            this.size = ctx.lastEnd;
        }
    }

    newSetUnit(unitIndex, unit)
    {
        return this.setUnit(unitIndex, unit);
    }

    /**
     *  delete the unit which starts at t / unitIndex
     *  since each unit has an absolute position we needn't extend
     *  duratations of neighbors.
     */
    delUnit(t)
    {
        this.unitList.filter((el) =>
        {
            if(t == el.getBegin())
                return false;
            else
                return true;
        });
    }
}

export default Part;