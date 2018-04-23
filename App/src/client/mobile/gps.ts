namespace Mobile {    
    export let gps: GPS = null;

    export class GPS { 
        private currentPositionCallbacks: ((position: Position) => void)[] = null;

        private watchID: number = null;

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

            if (this.watchID == null) {
                // IF it the watch not already started -> Start it.
                Logging.push("GPS watch started!");

                this.getCurrentPosition();
            }
        }

        public removeCallback(callback: (position: Position) => void) {
            let index = this.currentPositionCallbacks.indexOf(callback);

            if (index > -1) {
                this.currentPositionCallbacks.splice(index, 1);
            }

            if (this.currentPositionCallbacks.length == 0 && this.watchID != null) {
                // IF there are no more callbacks left and the watch is still running -> Stop it.
                Logging.push("GPS watch stopped!");

                navigator.geolocation.clearWatch(this.watchID);
                this.watchID = null;
            }
        }

        private getCurrentPosition() {
            this.watchID = navigator.geolocation.watchPosition((position: Position) => {
                //Logging.push("GPS: " + position.coords.latitude + ", " + position.coords.longitude + " | " + position.coords.speed);

                for (let notificationCallback of this.currentPositionCallbacks) {
                    notificationCallback(position);
                }
            }, (error: PositionError) => {
                Logging.push("GPS error: " + error.message);
            }, { maximumAge: 10000, timeout: 5000, enableHighAccuracy: true });


            /*navigator.geolocation.getCurrentPosition((position: Position) => {
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
            });*/
        }
    }
}