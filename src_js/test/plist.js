#!/usr/bin/env node

import {PlistParser} from "../polya/parser.js";

function testString(block=false)
{
    let str = `// a comment
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
    `;
    if(block)
    {
        return PlistParser.ParsePlist(str);
    }
    else
    {
        let p = new PlistParser();
        p.parse(str);
    }
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

console.log("testString: " + JSON.stringify(testString(true/*block*/)));

