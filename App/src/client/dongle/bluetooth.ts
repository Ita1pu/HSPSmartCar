namespace Dongle {
    declare let ble: any;
    
    export let bluetooth: Bluetooth = null;

    export class Bluetooth { 
        private notificationCallbacks: ((buffer: Uint8Array, text: string) => void)[] = null;

        private constructor() {
            this.notificationCallbacks = [];

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

                    let timer = setTimeout(() => {
                        this.stopScan();

                        Logging.push("Bluetooth connect failed - scan timeout!");

                        if (failure != null)
                            failure();
                    }, 5000);

                    this.startScan((device: any) => {
                        if (device.id == deviceId) {
                            this.stopScan();
                            clearTimeout(timer);

                            ble.connect(deviceId, () => {
                                Logging.push("Bluetooth connect succeeded!");

                                ble.startNotification(deviceId, "ffe0", "ffe1", (data: any) => {                
                                    var buffer = new Uint8Array(data);  
                                    var text = String.fromCharCode.apply(null, new Uint8Array(data));
            
                                    for (let notificationCallback of this.notificationCallbacks) {
                                        notificationCallback(buffer, text);
                                    }
                                }, (error: any) => {
                                    Logging.push("Bluetooth notification error: " + error);
            
                                    if (failure != null)
                                        failure();
                                });

                                DisplayFeature.noConnectionPanel.hide();
        
                                if (success != null)
                                    success();
                            }, (error: any) => {
                                Logging.push("Bluetooth connect failed: " + error);
        
                                if (failure != null)
                                    failure();
                            });
                        }
                    }, () => {
                        Logging.push("Bluetooth connect - scanning failed");
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

        public addNotification(callback: (buffer: Uint8Array, text: string) => void) {
            this.notificationCallbacks.push(callback);
        }

        public removeNotification(callback: (buffer: Uint8Array, text: string) => void) {
            let index = this.notificationCallbacks.indexOf(callback);

            if (index > -1) {
                this.notificationCallbacks.splice(index, 1);
            }
        }

        public removeNotifications() {
            this.notificationCallbacks = [];
        }

        private enable(success: () => void, failure: () => void) {
            ble.isEnabled(
                () => {
                    //Logging.push("Bluetooth already enabled!");

                    success();
                }, 
                () => {
                ble.enable(() => {
                    Logging.push("Bluetooth enabled!");

                    success();
                }, (error: any) => {
                    Logging.push("Bluetooth enable error: " + error);

                    failure();
                });
            })
        }
    }
}