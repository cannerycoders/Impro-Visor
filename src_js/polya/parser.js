import fs from "fs";
import Stream from "stream";
import yaml from "js-yaml";
import path from "path";

const tEsc = "escape";
const tQuote = "quote";
const tInit = "init";
const tBegin = "begin";
const tEnd = "end";
const tSymbol = "sym";
const tNumber = "num";
const tSpace = "space";
const tEOL = "eol";
const tCaptureBlock = "grabblock";
const tCaptureLine = "grabline";
const tCommentBlock = "cblock";
const tCommentEOL = "ceol";
const tString = "string";
const tStringBlock = "stringblock";
const tSlash = "/";
const tStar = "*";
const tSection = "section";
const tPhrase = "phrase";
const tPart = "part";
const tJSON = "json";

class tokenizer
{
    constructor(stream, onToken, onDone)
    {
        this.rstream = stream;
        this.onToken = onToken;
        this.onDone = onDone;
        this.parseCtx = 
        {
            pending: [],
            state: tInit,
            escape: false,
            tokenType: null,
            processed: [],
            lastChar: null,
            reset: function (force)
            {
                // we are called once a token has been produced.
                // One transition, from symbol to comment, requires
                // that we don't reset to init state
                if(force || (this.state != tCommentEOL && this.state != tCommentBlock))
                    this.state = tInit;
                this.escape = false;
                this.tokenType = null;
                this.lastChar = null; // for comment-blocks
                this.processed.length = 0;
            }
        };
        this.charTypes = 
        {
            "\\": tEsc,
            "\"": tQuote,
            "(": tBegin,
            ")": tEnd,
            " ": tSpace,
            "\n": tEOL,
            "\r": tEOL,
        };

        this.rstream.on("readable", () =>
        {
            let chunk;
            while(null !== (chunk = stream.read()))
                this.processChunk(chunk);
        });
        this.rstream.on("end", () =>
        {
            if(this.parseCtx.state == tCaptureBlock ||
               this.parseCtx.state == tCaptureLine)
            {
                let tok = {
                    type: tStringBlock,
                    value: this.parseCtx.processed.join("")
                };
                this.onToken(tok, this.parseCtx);
                this.parseCtx.reset();
            }
            this.finish();
        });
    }
    /* deliver as many tokens as we find in the active
     * chunklist.  A token may exist across multiple 
     * chunks and getToken is responsible for bookkeeping.
     */
    processChunk(chunk)
    {
        this.parseCtx.pending.push(...chunk.split(""));
        let tok;
        while((tok = this.getToken(this.parseCtx)))
        {
            this.onToken(tok, this.parseCtx);
        }
    }

    /** getToken may be called in an intermediate parsing state
     *  due to chunking.  We only re-init parseCtx after
     *  a token is discovered.
     */
    getToken(ctx)
    {
        let ch;
        let tok;
        while((ch = ctx.pending.shift()))
        {
            let ctype = this.charTypes[ch];
            if(ctx.escape)
            {
                ctype = undefined; // signals to process normally
                ctx.escape = false;
            }
            else
            if(ctype == tEsc)
            {
                ctx.escape = true;
                continue;
            }

            if(ctx.state == tCaptureBlock)
            {
                // a block is defined as all characters up to the next begin
                if(ctype == tBegin)
                {
                    ctx.pending.unshift(ch); // putback
                    tok = {
                        type: tStringBlock,
                        value: ctx.processed.join("")
                    };
                    break;
                }
                else
                    ctx.processed.push(ch);
            }
            else
            if(ctx.state == tCaptureLine)
            {
                if(ctype == tEOL)
                {
                    ctx.pending.unshift(ch); // putback
                    tok = {
                        type: tString,
                        value: ctx.processed.join("")
                    };
                    break;
                }
                else
                    ctx.processed.push(ch);
            }
            else
            if(ctx.state == tCommentEOL)
            {
                if(ctype == tEOL)
                    ctx.reset(true/*force*/);
                // else eat chars including eol
                // and continue
            }
            else
            if(ctx.state == tCommentBlock)
            {
                if(ch == tSlash && ctx.lastChar == tStar)
                {
                    ctx.reset(true/*force*/); // resets lastChar
                }
                else
                    ctx.lastChar = ch;
                // else eat chars including end of block
                // and continue
            }
            else
            if(ctx.state == tQuote)
            {
                if(ctype == tQuote) // endquote
                {
                    tok = {
                        type: tString,
                        value: ctx.processed.join("")
                    };
                    break;
                }
                else
                    ctx.processed.push(ch); // accum
            }
            else
            if(ctx.state == tSymbol)
            {
                // symbols are connected 'til space, quote, parent
                if(ctype == undefined)
                {
                    if(ctx.lastChar == tSlash)
                    {
                        if(ch == tSlash)
                        {
                            ctx.state = tCommentEOL;
                        }
                        else
                        if(ch == tStar)
                        {
                            ctx.state = tCommentBlock;
                        }
                        if(ctx.state != tSymbol)
                        {
                            ctx.processed.pop(); // lastChar
                            if(ctx.processed.length)
                            {
                                // console.log(`detected symbol+comment ${ctx.processed}`);
                                tok = {
                                    type: tSymbol,
                                    value: ctx.processed.join("")
                                };
                                break;
                            }
                        }
                        else
                            ctx.processed.push(ch); // accum
                    }
                    else
                        ctx.processed.push(ch); // accum
                    ctx.lastChar = ch;
                }
                else
                {
                    if(ctype != tSpace && ctype != tEOL)
                        ctx.pending.unshift(ch); // putback
                    tok = {
                        type: tSymbol,
                        value: ctx.processed.join("")
                    };
                    break;
                }
            }
            else
            if(ctype == tQuote)
            {
                ctx.state = tQuote; // eat '"', enter string-capture
            }
            else
            if(ctype == undefined) // ie: not-space, eol
            {
                console.assert(ctx.processed.length == 0)
                ctx.lastChar = ch;
                ctx.state = tSymbol
                ctx.processed.push(ch);
            }
            else
            if(ctype == tBegin || ctype == tEnd)
            {
                tok = {
                    type: ctype
                };
                break;
            }
            // here spaces are dropped purposefully
        }
        if(tok)
        {
            if(tok.type == tSymbol)
            {
                let n = Number(tok.value);
                if(!isNaN(n) && n != null)
                {
                    tok.type = tNumber;
                    tok.value = n;
                }
            }
            ctx.reset();
        }
        return tok;
    }

    finish()
    {
        // no more to read, finish processing chunks
        let err = 0;
        if(this.parseCtx.state != tInit)
        {
            console.warn("invalid parse session\n" + JSON.stringify(this.parseCtx));
        }
        this.onDone(err);
    }
}

export default class PlistParser
{
    constructor()
    {
    }

    load(file)
    {
        return new Promise((resolve, reject) =>
        {
            this.begin();
            let stream = fs.createReadStream(file);
            stream.setEncoding("utf8");
            this.tokenizer = new tokenizer(stream, 
                                this.getTokenHandler(),
                                (err) =>
                                {
                                    resolve(err ? {} : this.plist);
                                });
        });
    }

    parse(str)
    {
        this.begin();
        let stream = Stream.Readable.from(str);
        this.tokenizer = new tokenizer(stream, 
                            this.getTokenHandler(),
                            this.onDone.bind(this));
    }

    getTokenHandler()
    {
        // may implement different handlers
        return this.onTokenPlist.bind(this);
    }

    begin()
    {
        this.plist = [];
        this.stack = [this.plist];
        this.processMode = null;
    }

    onTokenPlist(tok, parseCtx)
    {
        switch(tok.type)
        {
        case tBegin:
            {
                let newList = [];
                this.stack[0].push(newList);
                this.stack.unshift(newList); // push_front
            }
            break;
        case tEnd:
            this.stack.shift(); // pop_front
            break;
        case tSymbol:
        case tString:
        case tNumber:
            if(this.stack[0] != this.plist)
            {
                // a contained list...
                this.stack[0].push(tok.value);
            }
            else
            {
                // handle the leadsheet case where we have outerscoped
                // non-list tokens comprising the section or part "body".  
                // Currently we create a new 'body' sublist which contains
                // a multiline string.
                let last = this.plist[this.plist.length-1];
                if(last)
                {
                    let lastType = last[0];
                    let lastEnd = last[last.length-1];
                    let lastEndType;
                    if(Array.isArray(lastEnd))
                    {
                        lastEndType = lastEnd[0];
                        console.assert(lastEndType != "body");
                    }
                    switch(lastType)
                    {
                    case tSection:
                    case tPart:
                    case tPhrase:
                        {
                            let body = ["body", [tok.value+" "]];
                            last.push(body); // add to end of current section/part
                            this.stack.unshift(body);
                            parseCtx.state = tCaptureBlock;
                        }
                        break;
                    default:
                        // console.warn("Unhandled root-level symbol following " + last[0]);
                        this.stack[0].push(tok.value);
                        break;
                    }
                }
            }
            break;
        case tStringBlock:
            console.assert(this.stack[0][0] == "body");
            this.stack[0][1] = this.stack[0][1] + tok.value;
            this.stack.shift(); // body
            console.assert(this.stack[0] == this.plist);
            break;
        default:
            console.log("unhandled " + tok.type);
        }
    }

    onDone(err)
    {
        if(!err)
        {
            if(this.target == tJSON)
                console.log(JSON.stringify(this.plist));
            else
            {
                let opts =
                {
                    noArrayIndent: false,
                    flowLevel: 2, /* lower numbers means more (shorter) lines */

                };
                console.log(yaml.dump(this.plist, opts));
            }
        }
    }
}

function testString()
{
    let p = new PlistParser();
    p.parse(`
        // a comment
        // another comment
        (rhythm-cluster-filename default.cluster)
        (my-rhythms-file "My Little.rhythms")
        (default-aux-instrument 57)
        /* a
         * block comment
         */
        (audio-in-latency 1.0)
        (advice-cache-size 10)
        (cache-enabled true) (default-load-stave/*comment*/ 1)
        (my-list (one 2 3))
    `);
}

function testFile()
{
    let p = new PlistParser();
    p.load("../../vocab/My.voc")
    .then((plist) =>
    {
        let opts =
        {
            noArrayIndent: false,
            flowLevel: 2, /* lower numbers means more (shorter) lines */
        };
        console.log(yaml.dump(plist, opts));
    });
}

function testLeadsheet()
{
    let p = new PlistParser();
    p.load("../../leadsheets/_test.ls")
    .then((plist) =>
    {
        let opts =
        {
            noArrayIndent: false,
            flowLevel: 2, /* lower numbers means more (shorter) lines */
        };
        console.log(yaml.dump(plist, opts));
    });
}

function testGrammar()
{
    let p = new PlistParser();
    p.load("../../grammars/ParkerMotif.grammar")
    .then((plist) =>
    {
        let opts =
        {
            noArrayIndent: false,
            flowLevel: 2, /* lower numbers means more (shorter) lines */
        };
        console.log(yaml.dump(plist, opts));
    });
}


if(path.basename(process.argv[1]) == "parser.js")
{
    // testFile();
    // testString();
    // testLeadsheet();
    testGrammar();
}
