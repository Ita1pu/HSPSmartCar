const Strings = {
    ErrorPanel: {
        defaultText: "See \"Configuration → Logging\" for more informations!"
    },
    noConnectionPanel: "No connection to dongle!"
}

let currentLanguage: string = null;

function S(...keys: any[]): string {
    //if (Config.data != null && Config.hasChanged() == false) 
        currentLanguage = Settings.defaultLanguage;

    let res = "";

    for (let key of keys) {
        if (res != "")
            res += " ";

        if (typeof key[currentLanguage] != typeof undefined) // Config.data != null && 
            res += key[currentLanguage];
        else if (typeof key[Settings.defaultLanguage] != typeof undefined)
            res += key[Settings.defaultLanguage];
        else
            res += key;
    }

    return res;
}
