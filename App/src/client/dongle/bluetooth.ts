
// TODO falls read or write fehlschlägt und dannach isConnected = false => NoConnectionPanel anzeigen (nur wenn selectedPanel != 0)

namespace Dongle {
    declare let ble: any;
    
    export let bluetooth: Bluetooth = null;

    export class Bluetooth { 
        private constructor() {
            this.connect();
        }

        public static init(): Bluetooth {
            if (Dongle.bluetooth == null)
                Dongle.bluetooth = new Bluetooth();

            return Dongle.bluetooth;
        }

        public IsConnected(success: () => void, failure: () => void) {
            this.enable(() => {      
                let deviceId = Store.get(Settings.Store.deviceId);

                if (deviceId != null)
                    ble.isConnected(deviceId, success, failure);
                else
                    failure();
            }, 
            () => {
                failure();
            });
        }

        public connect(deviceId: string = null, success: () => void = null, failure: () => void = null) {
            Logging.push("Bluetooth connecting...");

            if (deviceId == null) 
                deviceId = Store.get(Settings.Store.deviceId);
            if (deviceId == null) {
                Logging.push("Bluetooth not connected, deviceId is null!");

                if (failure != null)
                    failure();

                return;
            }               

            this.enable(() => {            
                this.IsConnected(() => {
                    Logging.push("Bluetooth already connected, disconnecting...");

                    success();                
                }, () => {
                    Logging.push("Bluetooth connecting to: '" + deviceId + "'...");

                    ble.connect(deviceId, (device: any) => {
                        Logging.push("Bluetooth connect succeeded!");

                        DisplayFeature.noConnectionPanel.hide();

                        if (success != null)
                            success();
                    }, (error: any) => {
                        Logging.push("Bluetooth connect failed: " + error);

                        if (failure != null)
                            failure();
                    });
                });  
            }, 
            () => {
                if (failure != null)
                    failure();
            });
        }
    
        public disconnect() {
            let deviceId = Store.get(Settings.Store.deviceId);

            if (deviceId != null)
                ble.disconnect(deviceId, () => {}, () => {});
        }

        public startScan(callback: (device: any) => void, failure: () => void) {   
            this.enable(() => {
                ble.startScan([], callback, failure);
            }, () => {
                failure();
            }); 
        }

        public stopScan() {
            ble.stopScan();
        }

        private enable(success: () => void, failure: () => void) {
            ble.isEnabled(
                () => {
                    Logging.push("Bluetooth already enabled!");

                    success();
                }, 
                () => {
                ble.enable(() => {
                    Logging.push("Bluetooth enabled!");

                    success();
                }, (error: any) => {
                    Logging.push("Bluetooth error: " + error);

                    failure();
                });
            })
        }
    }
}