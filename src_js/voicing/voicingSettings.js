import {HandManager} from "./handManager.js";
import yaml from "js-yaml";
import fs from "fs";

/**
 * VoicingSettings and HandManager combine to characterize
 * the behavior of VoicingGenerator.
 */
export class VoicingSettings
{
    /**
     * @param colorPriority-the weight the color notes should have. Make it between 0 and a big-ish number
     * @param maxPriority-the maximum weight the priority notes should have. should be greater than the max number of priority notes*the priority multiplier.
     * @param previousVoicingMultiplier- amount to multiply the weight of the notes in the previous voicing's weightings by. Default: 1. Make greater than 1 for voice leading.
     * @param halfStepAwayMultiplier-amount to multiply the weight of the notes half a step away from the previous chord Default: 1. Make greater than 1 for voice leading.
     * @param fullStepAwayMultiplier-amount to multiply the weight of the notes a full step away from the previous chord. Default: 1. Make greater than 1 for voice leading.
     * @param priorityMultiplier - amount of weight to remove from notes as priority decreases. Default 0 for equal probability.
     * @param repeatMultiplier - the amount to reduce(or increase) the priority of notes already selected for the chord in other octaves. for reduction, make between 0 and 1. Default 1.
     */
    static sDefaults =
    {
        previousVoicingMultiplier: 4,
        halfStepAwayMultiplier: 3,
        fullStepAwayMultiplier: 2,
        repeatMultiplier: .3,
        halfStepReducer: 0,
        fullStepReducer: .7,

        leftColorPriority: 0,
        rightColorPriority: 0, 
        maxPriority: 6,
        priorityMultiplier: .667,

        invertM9: false,
        voiceAll: false,
        rootless: false,
        leftMinInterval: 0,
        rightMinInterval: 0,
    };

    static sFieldMap =
    {
        "LH-lower-limit": "hm:leftHandLowerLimit",
        "RH-lower-limit": "hm:rightHandLowerLimit",
        "LH-upper-limit": "hm:leftHandUpperLimit",
        "RH-upper-limit": "hm:rightHandUpperLimit",
        "LH-spread": "hm:leftHandSpread",
        "RH-spread": "hm:rightHandSpread",
        "LH-min-notes": "hm:leftHandMinNotes",
        "LH-max-notes": "hm:leftHandMaxNotes",
        "RH-min-notes": "hm:rightHandMinNotes",
        "RH-max-notes": "hm:rightHandMaxNotes",
        "pref-motion": "hm:preferredMotion",
        "pref-motion-range": "hm:preferredMotionRange",
        "prev-voicing-multiplier": "previousVoicingMultiplier",
        "half-step-multiplier": "halfStepAwayMultiplier",
        "full-step-multiplier": "fullStepAwayMultipler",
        "LH-color-priority": "leftColorPriority",
        "RH-color-priority": "rightColorPriority",
        "max-priority": "maxPriority",
        "priority-multiplier": "priorityMultiplier",
        "repeat-multiplier": "repeatMultiplier",
        "half-step-reducer": "halfStepReducer",
        "full-step-reducer": "fullStepReducer",
        "left-min-interval": "leftMinInterval",
        "right-min-interval": "rightMinInterval",
        "invert-9th": "invertM9",
        "voice-all": "voiceAll",
        "rootless": "rootless",
        "comment": null
    };

    constructor()
    {
        for(let k in VoicingSettings.sDefaults)
            this[k] = VoicingSettings.sDefaults[k];

        this.hm = new HandManager();
    }

    init(other)
    {
        for(let k in VoicingSettings.sDefaults)
            this[k] = other[k];
        this.hm.init(other.hm);
    }

    load(file)
    {
        let ruleList = yaml.load(fs.readFileSync(file, "utf8"));
        for(let l of ruleList)
        {
            let key = l[0];
            let val = l[1];
            if(val == "true")
                val = true;
            else
            if(val == "false" || val == "off")
                val = false;
            let field = VoicingSettings.sFieldMap[key];
            if(field == null)
                continue;
            else
            if(field)
            {
                if(field.startsWith("hm:"))
                    this.hm[field.slice(3)] = val;
                else
                    this[field] = val;
            }
            else
                console.warn("VoicingSettings unexpected value " + key);
        }
    }

    serialize()
    {
        return JSON.stringify(this);
    }
}