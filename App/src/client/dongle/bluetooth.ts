namespace Dongle {
    declare let bluetoothSerial: any;
    
    export let bluetooth: Bluetooth = null;

    export class Bluetooth { 
        private isConnected: boolean = false;

        private constructor() {
            bluetoothSerial.enable((devices: any) => {
                console.log(devices);
            }, (error: any) => {
                console.log(error);
            });

            bluetoothSerial.list((list: any) => {
                console.log(list);
            }, (error: any) => {
                console.log(error);
            });           
            
        }

        public static init(): Bluetooth {
            if (Dongle.bluetooth == null)
                Dongle.bluetooth = new Bluetooth();

            return Dongle.bluetooth;
        }

        public IsConnected(): boolean {
            return this.isConnected;
        }
    }
}