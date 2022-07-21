export class VolumeSymbol
{
    static sMaxVolume = 127;

    static Make(str)
    {
        return new VolumeSymbol(str);
    }

    constructor(v = VolumeSymbol.sMaxVolume)
    {
        if(typeof(v) == "string")
            v = parseInt(v);
        this.volume = this.clipToRange(v);
    }

    clipToRange(v)
    {
        return v > sMaxVolume ? sMaxVolume : (v < 0 ? 0 : v);
    }
}