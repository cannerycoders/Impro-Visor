

/**
 * A Pattern is base class for accompaniment patterns that have
 * a certain duration and weight.
 * This simplifies the code for choosing a random pattern.
 */

export class Pattern
{
    /**
     * 
     * @param {Style} style 
     */
    constructor(style)
    {
        this.style = style;
        this.weight = 10;
        this.errorMsg = null;
    }

    getDuration() // override-me!
    {
        return 0;
    }

    getWeight() { return this.weight; }
    setWeight(w) { this.weight = w; }
    getStyle() { return this.style; }
}

export default Pattern;