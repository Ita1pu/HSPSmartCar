/// <reference path="../../typings/index.d.ts" />

let files: string[] = [
    
    "settings",
    
    "variables",
    "strings",
    "store",

    "logging/logging",

    "display-feature/error-panel",
    "display-feature/no-connection-panel",
    "display-feature/swipe-help",
    "display-feature/view-circles",

    "view/view",
    "view/configuration",
    "view/simple-list",

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