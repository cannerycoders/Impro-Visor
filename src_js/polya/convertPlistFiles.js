#!/usr/bin/env node

// nb: to run within node, this file must have a file extension
// our converted is intended to be run manually from this directory

import PlistParser from "./parser.js";
import yaml from "js-yaml";
import path from "path";
import fs from "fs";
import glob from "glob";

let patlist = [
    "../../fractals/*.fractal",
    "../../grammars/*.grammar",
    "../../leadsheets/*.ls",
    "../../leadsheets/*/*.ls",
    "../../leadsheets/*/*/*.ls",
    "../../vocab/My.*",  
];

async function convertFile(ifile)
{
    return new Promise((resolve, reject) =>
    {
        let parser = new PlistParser;
        console.log(`${ifile.replace(/\\/g, "/")}`);
        parser.load(ifile)
        .then((plist) =>
        {
            if(plist.length == 0)
            {
                // reject("Problem reading " + ifile);
                console.warn("WARNING problem reading " + ifile);
                resolve(0);
            }
            else
            {
                let ofile = path.join("../data", ifile.slice(6)); // remove "../../"
                try
                {
                    fs.mkdirSync(path.dirname(ofile), {recursive: true});
                }
                catch(err) {}
                let opts =
                {
                    noArrayIndent: false,
                    flowLevel: 1, /* higher numbers means more (shorter) lines */
                };
                let str = yaml.dump(plist, opts);
                console.log(`    -> ${ofile.replace(/\\/g, "/")} ${plist.length} ${str.length}`);
                let fd = fs.openSync(ofile, "w");
                let comment = "# Converted from Impro-Visor src on " + 
                                new Date().toISOString() + "\n" +
                            "# Impro-Visor is GPL-3, copyright 2005-2017 Robert Keller and Harvey Mudd College\n" +
                            "# see: https://github.com/Impro-Visor/Impro-Visor\n";
                fs.writeSync(fd, comment);
                fs.writeSync(fd, str);
                fs.close(fd); // NB: async
                resolve(0);
            }
        }).
        catch((err) =>
        {
            console.warn(err);
        });
    });
}

for(let pat of patlist)
{
    for(let ifile of glob.sync(pat))
    {
        await convertFile(ifile);
    }
}


