namespace Mobile {    
    export let gps: GPS = null;

    export class GPS { 
        private currentPositionCallbacks: ((position: Position) => void)[] = null;

        private running: boolean = false;

        private constructor() {
            this.currentPositionCallbacks = [];
        }

        public static init(): GPS {
            if (Mobile.gps == null)
                Mobile.gps = new GPS();

            return Mobile.gps;
        }

        public addCallback(callback: (position: Position) => void) {
            this.currentPositionCallbacks.push(callback);

            if (this.running == false) {
                this.running = true;
                this.getCurrentPosition();
            }
        }

        public removeCallback(callback: (position: Position) => void) {
            let index = this.currentPositionCallbacks.indexOf(callback);

            if (index > -1) {
                this.currentPositionCallbacks.splice(index, 1);
            }
        }

        private getCurrentPosition() {
            navigator.geolocation.getCurrentPosition((position: Position) => {
                for (let notificationCallback of this.currentPositionCallbacks) {
                    notificationCallback(position);
                }

                if (this.currentPositionCallbacks.length != 0)
                    setTimeout(() => { this.getCurrentPosition() }, Settings.gpsTimeout);
                else
                    this.running = false;
            }, (error: PositionError) => {
                Logging.push("GPS error: " + error.message);

                if (this.currentPositionCallbacks.length != 0)
                    setTimeout(() => { this.getCurrentPosition() }, Settings.gpsTimeout);
                else
                    this.running = false;
            }, {
                enableHighAccuracy: true,
                timeout: Infinity,
                maximumAge: 10000
            });
        }
    }
}