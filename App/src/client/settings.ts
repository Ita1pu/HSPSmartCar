const Settings = {
    defaultLanguage: "en",

    isDebug: true,

    isMobile: false,

    defaultView: 1,

    swipeDistance: 100,

    gpsTimeout: 500,

    Backend: {
        clientId: "smartcar_app",
        secret: "tZop4xbZ9wV"
    },

    PIDs: {
        latitude: "240",
        longitude: "241",
        rpm: "12",
        velocity: "13",
        coolantTemperatur: "5"
    },

    Store: {
        selectedView: "selectedView",
        showSwipeHelp: "showSwipeHelp",
        ShowSwipeHelp: {
            true: "true",
            false: "false"
        },
        velocityUnit: "velocityUnit",
        VelocityUnit: {
            MpH: "MpH",
            KmH: "KmH"
        },
        temperaturUnit: "temperaturUnit",
        TemperaturUnit: {
            Celsius: "Celsius",
            Kelvin: "Kelvin"
        },
        deviceId: "deviceId",
        Backend: {
            url: "url",
            identityPort: "identityPort",
            apiPort: "apiPort",
            username: "username",
            password: "password",
            token: "token"
        }
    }
};