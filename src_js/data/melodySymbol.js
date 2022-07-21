import "./noteSymbol.js";
import "./volumeSymbol.js";

const sNoteCodes = ["a", "b", "c", "d", "e", "f", "g", "r"]
                    .filter((v) => v.charCodeAt(0));

export class MelodySymbol 
{
    // factory for VolumeSymbol and NoteSymbol, invoked by leadsheet
    // currently we don't use as base-class.
    static Make(str)
    {
        if(str[0] == 'v')
            return VolumeSymbol.Make(str);
        else
        if(sNoteCodes.indexOf(str.charCodeAt(0)) != -1)
            return NoteSymbol.Make(str);
        else 
            return null;
    }

    static IsNoteSymbol(o) { return o.isRest !== undefined; }
    static IsVolumeSymbol(o) { return o.volume !== undefined; }
}