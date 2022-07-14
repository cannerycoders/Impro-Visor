const restCC = "r".charCodeAt(0);
const sharpCC = "#".charCodeAt(0);
const flatCC = "b".charCodeAt(0);
const plusCC = "+".charCodeAt(0);
const minusCC = "-".charCodeAt(0);
const uCC = "u".charCodeAt(0);

export class Constants
{
    /* we use getters to express const-ness... */
    /*
    static get RESTSTR() { return "r"; } 
    static get RESTCHAR() { return restCC;  }
    static get FLATCHAR() { return flatCC; }
    static get SHARPCHAR() { return sharpCC; }
    static get PLUSCHAR() { return plusCC; }
    static get MINUSCHAR() { return minusCC; }
    static get UCHAR() { return uCC; }
    */
    
    static RESTSTR = "r";
    static RESTCHAR = "r".charCodeAt(0);
    static SHARPCHAR = "#".charCodeAt(0);
    static FLATCHAR = "b".charCodeAt(0);
    static PLUSCHAR = "+".charCodeAt(0);
    static MINUSCHAR = "-".charCodeAt(0);
    static UCHAR = "u".charCodeAt(0);
    
    static BEAT = 120; // slots in a beat
    static WHOLE = 4 * this.BEAT;  // 480, "ticks"
    static THIRD_BEAT = 40;
    static HALF_BEAT = 60;
    static SIXTH_BEAT = 20;
    static DEFAULT_DURATION = this.HALF_BEAT;	// eighth note value

    constructor()

    {}
}

export default Constants;