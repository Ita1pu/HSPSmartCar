
// TODO falls read or write fehlschlägt und dannach isConnected = false => NoConnectionPanel anzeigen (nur wenn selectedPanel != 0)

namespace Dongle {
    declare let bluetoothSerial: any;
    
    export let bluetooth: Bluetooth = null;

    export class Bluetooth { 
        private isConnected: boolean = false;

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
                bluetoothSerial.isConnected(success, failure);
            }, 
            () => {
                failure();
            });
        }

        public connect(address_or_uuid: string = null, success: () => void = null, failure: () => void = null) {
            Logging.push("Bluetooth connecting...");

            if (address_or_uuid == null) 
                address_or_uuid = Store.get(Settings.Store.address_or_uuid);
            if (address_or_uuid == null) {
                Logging.push("Bluetooth not connected, address_or_uuid is null!");

                if (failure != null)
                    failure();

                return;
            }               

            this.enable(() => {            
                let con = () => {
                    Logging.push("Bluetooth connecting to: '" + address_or_uuid + "'...");

                    bluetoothSerial.connect(address_or_uuid, () => {
                        Logging.push("Bluetooth connect succeeded!");

                        DisplayFeature.noConnectionPanel.hide();

                        if (success != null)
                            success();
                    }, () => {
                        Logging.push("Bluetooth connect failed!");

                        if (failure != null)
                            failure();
                    });
                }

                bluetoothSerial.isConnected(() => {
                    Logging.push("Bluetooth already connected, disconnecting...");

                    this.disconnect();

                    Logging.push("Bluetooth disconnected, connecting now...");

                    con();                
                }, () => {
                    con();
                });  
            }, 
            () => {
                if (failure != null)
                    failure();
            });
        }
    
        public disconnect() {
            bluetoothSerial.disconnect();
        }

        public browse(callback: (device: any) => void, success: () => void, failure: () => void) {   
            this.enable(() => {
                bluetoothSerial.setDeviceDiscoveredListener(callback);
                bluetoothSerial.discoverUnpaired(success, failure);
            }, () => {
                failure();
            }); 
        }

        private enable(success: () => void, failure: () => void) {
            bluetoothSerial.isEnabled(
                () => {
                    Logging.push("Bluetooth already enabled!");

                    success();
                }, 
                () => {
                bluetoothSerial.enable(() => {
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