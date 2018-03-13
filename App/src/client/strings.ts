const Strings = {
    ErrorPanel: {
        defaultText: "See \"Configuration → Logging\" for more informations!"
    },
    noConnectionPanel: "No connection to dongle!",
    View: {
        Configuration: {
            Header: {
                dataTransfer: "Data Transfer",
                connectionToDongle: "Connection to Dongle",
                connectionToBackground: "Connection to Backend",
                appSettings: "App Settings",
                logging: "Logging"
            },
            Button: {
                DataTransfer: {
                    download: "Download from Dongle",
                    upload: "Upload to Backend",
                    clearBuffer: "Clear Buffer"
                },
                connect: "Connect",
                test: "Test",
                disconnect: "Disconnect",
                load: "Load"
            },
            Switch: {
                VelocityUnit: {
                    MpH: "Mp/H",
                    KmH: "Km/H"
                },
                TemperaturUnit: {
                    Celsius: "Celsius",
                    Kelvin: "Kelvin"
                },
                SwipeHelp: {
                    enable: "Enable Swipe Help",
                    disbale: "Disable Swipe Help"
                }
            }
        },
        SimpleList: {
            velocity: "Velocity",
            rotSpeed: "rot. Speed",
            oilTemperatur: "Oil Temp.",
            gps: "GPS",
            mobile: " (mob.)"
        },
        Speedo: {
            velocity: "Velocity",
            oil: "Oil",
            rotSpeed: "rot. Speed"
        },
        SpeedoCompare: {
            car: "Car",
            mobile: "Mobile"
        }
    }
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
