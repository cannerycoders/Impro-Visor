import {NoteSymbol} from "./noteSymbol.js";

export class Key
{
    /**
     * table of all Keys organized as a line of fifths
    */
    static sKeys = [
        new Key(0, "gb", 6),
        new Key(1, "db", 11),
        new Key(2, "ab", 4),
        new Key(3, "eb", 9),
        new Key(4, "bb", 2),
        new Key(5, "f", 7),
        new Key(6, "c", 0),
        new Key(7, "g", 5),
        new Key(8, "d", 10),
        new Key(9, "a", 3),
        new Key(10, "e", 8),
        new Key(11, "b", 1),
        new Key(12, "f#", 6),
        new Key(13, "c#", 11)
    ];

    // ['a', 'b', 'c'].reduce((a, v) => ({ ...a, [v]: v}), {}) 
    static sKeyMap = this.sKeys.reduce((a, v) => {
        a[v.name] = v;
        return a;
    }, {});

    static CKey = this.sKeys[6];

    static getKeys() { return Object.keys(this.sKeyMap); }

    static getKey(nm) { return this.sKeyMap[nm]; }

    static invalidNotes(nlist)
    {
        if(nlist.length == 0) return true;
        for(let n of nlist)
        {
            if(!NoteSymbol.isValidNote(n))
                return true;
        }
        return false; // means all valid
    }

    /* --------------------------------------------------------------------- */
    constructor(index, name, cpos)
    {
        this.index = index;
        this.name = name;
        this.cpos = cpos;
    }

}

export default Key;