import {Constants} from "../constants.js";
import {MelodyPart} from "./melodyPart.js";
import {MelodyPartAccompanied} from "./melodyPartAccompanied.js";
import {ChordPart} from "./melodyPart.js";

/**
 * The Score class is representative of a musical score, containing several
 * parallel Parts, including melodies and chord progressions.
 * A Score contains several Parts stored in a PartList.
 * It contains information about the total Score, such as volume, tempo,
 * and title.  Parts should be added with the addPart method.
 */
export class Score
{
    constructor()
    {
        this.partlist = [];
    }

}