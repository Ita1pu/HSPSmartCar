/// <reference path="../../typings/index.d.ts" />

let files: string[] = [
    
    "settings",
    
    "variables",
    "strings",
    "store",

    "index"
    
];    
    
let i = 0;
var requireFile = () => {
    (<any>require)([files[i]], () => {
        i++;
        if (i < files.length)
            requireFile();
    }, () => {});
};

requireFile();