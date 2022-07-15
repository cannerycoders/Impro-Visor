/**
 * base-class for Notes and chords
 */
export class IUnit
{
    constructor(begin=0, dur=1)
    {
        this.begin = begin;
        this.duration = dur; // aka rhythmValue
    }

    getBegin()
    {
        return this.begin;
    }

    getEnd()
    {
        return this.begin + this.duration;
    }

    getDuration()
    {
        return this.duration;
    }

    setBegin(begin)
    {
        this.begin = begin;
    }

    /**
     *  change unit by setting its begin while preserving end
     *  this necessarily changes duration.
     */
    clipBegin(b)
    {
        this.duration = this.getEnd() - b;
        console.assert(this.duration > 0);
        this.begin = b;
    }

    /**
     *  change unit by setting its end while preserving begin
     *  this necessarily changes duration.
     */
    clipEnd(e)
    {
        this.duration = e - this.begin;
        console.assert(this.duration > 0);
    }

    isRest() // virtual
    {
        return false;
    }

}

export default IUnit;
