import {NoteSymbol} from "./noteSymbol.js";
import {Key} from "./key.js";

export class ChordForm
{
    // [chord, 
    //      [name, CM9], 
    //      [pronounce, C, major, nine], 
    //      [key, c], 
    //      [family, major], 
    //      [spell, c8, e8, g8, b8, d8], 
    //      [color, a8, f#8], 
    //      [priority, b8, e8, d8, g8, c8], 
    //      [approach, [c8, c#8], [e8, eb8, f8], [g8, f#8, g#8], [a8, g#8, bb8], [d8, c#8, eb8], [b8, bb8, c8]] 
    //      [voicings, [left-hand-A, [type, closed], [notes, e-8, g-8, b-8, d8], [extension]], [left-hand-B, [type, closed], [notes, b-8, d8, e8, g8], [extension]], [left-hand-C, [type, closed], [notes, g-8, b-8, d8, e8], [extension]], [drop-2-A, [type, open], [notes, b-8, e8, g8, d+8], [extension]], [drop-2-B, [type, open], [notes, g-8, d8, e8, b8], [extension]], [drop-2-C, [type, open], [notes, e-8, b-8, d8, g8], [extension]], [drop-2-D, [type, open], [notes, d8, g8, b8, e+8], [extension]], [drop-3-A, [type, open], [notes, g-8, e8, b8, d+8], [extension]], [drop-3-B, [type, open], [notes, e-8, d8, g8, b8], [extension]], [drop-3-C, [type, open], [notes, b-8, g8, d+8, e+8], [extension]], [drop-2-3-A, [type, open], [notes, g-8, b-8, e8, d+8], [extension]], [drop-2-3-B, [type, open], [notes, e-8, g-8, d+8, b+8], [extension]], [drop-2-3-C, [type, open], [notes, d-8, e-8, b-8, g8], [extension]], [two-hand-A, [type, open], [notes, g8, d+8, b+8, e++8], [extension]], [two-hand-B, [type, open], [notes, b8, e+8, g+8, d++8], [extension]], [two-hand-C, [type, open], [notes, e-8, b-8, d8, g8], [extension]], [shout-A, [type, shout], [notes, e-8, g-8, b-8, d8], [extension, g++8, d+++8, g+++8]], [shout-B, [type, shout], [notes, b-8, e8, g8], [extension, g++8, d+++8, g+++8]]]
    //      [extensions, CM9#11, CM13], 
    //      [scales, [C, major], [C, lydian], [C, bebop, major], [C, major, pentatonic], [G, major, pentatonic], [C, major, blues], [C, chromatic]], 
    //      [avoid, c8], 
    //      [substitute, CM69, Em7, Am11]]
    constructor(plist)
    {
        console.assert(plist[0] == "chord");
        for(let i=1;i<plist.length;i++)
        {
            let p = plist[i];
            switch(p[0])
            {
            case "name":
                this.name = p.slice(1).join(" ");
                break;
            case "key":
                this.key = Key.getKey(p[1]);
                if(!this.key)
                {
                    console.warn("ChordForm unknown key " + p[1]);
                    this.key = Key.CKey;
                }
                break;
            case "family":
                this.family = p.slice(1).join(" ");
                break;
            case "same":
                this.same = p[1];
                break;
            case "spell":
                {
                    let s = p.slice(1);
                    if(Key.invalidNotes(s))
                        console.warn(`ChordForm ${this.name} bad notes in ` + s);
                    else
                        this.spell = NoteSymbol.makeList(s, 0);
                }
                break;
            case "priority":
                {
                    // [priority, b8, e8, d8, g8, c8], 
                    let x = p.slice(1);
                    if(Key.invalidNotes(x))
                        console.warn(`ChordForm ${this.name} bad priority in ` + x);
                    else
                        this.priority = NoteSymbol.makeList(x, 0);
                }
                break;
            case "avoid":
                {
                    let a = p.slice(1);
                    if(Key.invalidNotes(a))
                        console.warn(`ChordForm ${this.name} bad avoid notes in ` + a);
                    else
                        this.avoid = NoteSymbol.makeList(a, 0);
                }
                break;
            case "color":
                {
                    let c = p.slice(1);
                    if(Key.invalidNotes(c))
                        console.warn(`ChordForm ${this.name} bad color notes in ` + a);
                    else
                        this.color = NoteSymbol.makeList(c, 0);
                }
                break;
            case "approach":
                // [approach, [c8, c#8], [e8, eb8, f8], [g8, f#8, g#8], [a8, g#8, bb8], [d8, c#8, eb8], [b8, bb8, c8]] 
                {
                    let a = p.slice(1);
                    this.approach = [];
                    for(let i of a)
                    {
                        if(Key.invalidNotes(i))
                            console.warn(`ChordForm ${this.name} bad approach notess in ` + a);
                        else
                            this.approach.push(NoteSymbol.makeList(i, 0));
                    }
                }
                break;
            case "pronounce":
                this.pronounce = p.slice(1).join(" ");
                break;
            case "voicings":
                // [voicings, [left-hand-A, [type, closed], [notes, e-8, g-8, b-8, d8], [extension]], [left-hand-B, [type, closed], [notes, b-8, d8, e8, g8], [extension]], [left-hand-C, [type, closed], [notes, g-8, b-8, d8, e8], [extension]], [drop-2-A, [type, open], [notes, b-8, e8, g8, d+8], [extension]], [drop-2-B, [type, open], [notes, g-8, d8, e8, b8], [extension]], [drop-2-C, [type, open], [notes, e-8, b-8, d8, g8], [extension]], [drop-2-D, [type, open], [notes, d8, g8, b8, e+8], [extension]], [drop-3-A, [type, open], [notes, g-8, e8, b8, d+8], [extension]], [drop-3-B, [type, open], [notes, e-8, d8, g8, b8], [extension]], [drop-3-C, [type, open], [notes, b-8, g8, d+8, e+8], [extension]], [drop-2-3-A, [type, open], [notes, g-8, b-8, e8, d+8], [extension]], [drop-2-3-B, [type, open], [notes, e-8, g-8, d+8, b+8], [extension]], [drop-2-3-C, [type, open], [notes, d-8, e-8, b-8, g8], [extension]], [two-hand-A, [type, open], [notes, g8, d+8, b+8, e++8], [extension]], [two-hand-B, [type, open], [notes, b8, e+8, g+8, d++8], [extension]], [two-hand-C, [type, open], [notes, e-8, b-8, d8, g8], [extension]], [shout-A, [type, shout], [notes, e-8, g-8, b-8, d8], [extension, g++8, d+++8, g+++8]], [shout-B, [type, shout], [notes, b-8, e8, g8], [extension, g++8, d+++8, g+++8]]]
                this.voicings = {};
                for(let v of p.slice(1))
                {
                    console.assert(Array.isArray(v));
                    let o = {name: v[0]};
                    for(let sv of v.slice(1))
                    {
                        switch(sv[0])
                        {
                        case "type":
                            o.type = sv[1];
                            break;
                        case "notes":
                            o.notes = NoteSymbol.makeList(sv.slice(1), 0);
                            break;
                        case "extension":
                            o.extension = sv.length == 1 ? [] : NoteSymbol.makeList(sv.slice(1), 0);
                            break;
                        }
                    }
                    this.voicings[o.name] = o;
                }
                break;
            case "scales":
                //  [scales, [C, major], [C, lydian], [C, bebop, major], [C, major, pentatonic], [G, major, pentatonic], [C, major, blues], [C, chromatic]], 
                this.scales = p.slice(1).map((v) => v.join(" "));
                break;
            case "extensions":
                // [extensions, CM9#11, CM13], 
                this.extensions = p.slice(1);
                break;
            case "substitute":
                // [substitute, CM69, Em7, Am11]]
                this.substitutions = p.slice(1);
                break;
            }
        }
    }

}

export default ChordForm;