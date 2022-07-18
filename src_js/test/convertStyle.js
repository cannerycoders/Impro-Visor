#!/usr/bin/env node

// nb: to run within node, this file must have a file extension
// our converted is intended to be run manually from this directory

import PlistParser from "../polya/parser.js";
import yaml from "js-yaml";
import path from "path";
import fs from "fs";
import glob from "glob";

let patlist = [
    ["../../styles/cha-cha-cha.sty", 2],
];

const sVers = "vers 0.1.0";

async function convertFile(ifile, flow)
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
                console.log(JSON.stringify(plist, null, 2));
                resolve(0);
            }
        }).
        catch((err) =>
        {
            console.warn(err);
        });
    });
}

for(let [pat, flow] of patlist)
{
    for(let ifile of glob.sync(pat))
    {
        await convertFile(ifile, flow);
    }
}


