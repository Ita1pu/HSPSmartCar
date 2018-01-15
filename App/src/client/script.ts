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

    "dialog/dialog",

    "view/view",
    "view/configuration/configuration-appSettings",
    "view/configuration/configuration-backend",
    "view/configuration/configuration-data",
    "view/configuration/configuration-dongle",
    "view/configuration/configuration-logging",
    "view/configuration/configuration",
    "view/simple-list",

    "dongle/bluetooth",

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