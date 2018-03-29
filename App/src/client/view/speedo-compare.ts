namespace View {
    declare let RadialGauge: any;

    export class SpeedoCompare extends View {
        public static lastInstance: SpeedoCompare = null;

        private velocity: JQuery = null;
        private velocityMobile: JQuery = null;

        private lastNotification: string = "";

        public constructor(control: JQuery) {
            super(control);

            SpeedoCompare.lastInstance = this;

            this.control.addClass(Var.Style.View.speedoCompare);

            if(this.control.width() > this.control.height() / 2) {
                this.control.width(this.control.height() / 2);
                this.control.css("left", window.innerWidth / 2 - this.control.width() / 2);
            }

            this.velocity = this.drawVelocity(S(Strings.View.SpeedoCompare.car));
            this.velocityMobile = this.drawVelocity(S(Strings.View.SpeedoCompare.mobile));

            Mobile.gps.addCallback(this.onNewPosition);
            Dongle.bluetooth.addNotification(this.onNotification);
        }

        public destroy() {
            super.destroy();

            Mobile.gps.removeCallback(this.onNewPosition);
            Dongle.bluetooth.removeNotification(this.onNotification);

            SpeedoCompare.lastInstance = null;
        }

        private onNotification(buffer: Uint8Array, text: string) {
            let entries = (SpeedoCompare.lastInstance.lastNotification + text).split(";");

            for (let entry of entries) {
                let pidValue = entry.split(":");

                if (pidValue.length == 2) {
                    let value = pidValue[1];

                    let pids = pidValue[0].split("#");

                    let pid;
                    if (pids.length == 2) {
                        pid = pids[1];
                    }
                    else {
                        pid = pids[0];
                    }

                    if (pid == Settings.PIDs.velocity) {
                        SpeedoCompare.lastInstance.velocity.value = parseInt(value);
                    }
                } 
            }            

            if (entries.length > 0)
                SpeedoCompare.lastInstance.lastNotification = entries[entries.length - 1];
        }

        private onNewPosition(position: Position) {
            if (position.coords.speed != null) 
                SpeedoCompare.lastInstance.velocityMobile.value = position.coords.speed;
            else
                SpeedoCompare.lastInstance.velocityMobile.value = 0;
        }

        private drawVelocity(title: string) {
            let velocity = new RadialGauge({
                renderTo: document.createElement('canvas'),
                width: this.control.width(),
                height: this.control.height() / 2,
                units: "Km/h",
                title: title,
                value: 0,
                minValue: 0,
                maxValue: 220,
                majorTicks: [
                    '0','20','40','60','80','100','120','140','160','180','200','220'
                ],
                minorTicks: 2,
                strokeTicks: false,
                highlights: [
                    { from: 0, to: 50, color: 'rgba(0,255,0,.15)' },
                    { from: 50, to: 100, color: 'rgba(255,255,0,.15)' },
                    { from: 100, to: 150, color: 'rgba(255,30,0,.25)' },
                    { from: 150, to: 200, color: 'rgba(255,0,225,.25)' },
                    { from: 200, to: 220, color: 'rgba(0,0,255,.25)' }
                ],
                borders: false,
                colorPlate: 'transparent',
                colorMajorTicks: '#f5f5f5',
                colorMinorTicks: '#ddd',
                colorTitle: '#fff',
                colorUnits: '#ccc',
                colorNumbers: '#eee',
                colorNeedle: 'rgba(240, 128, 128, 1)',
                colorNeedleEnd: 'rgba(255, 160, 122, .9)',
                valueBox: true,
                animationRule: 'linear',
                animationDuration: 100
            });
            
            this.control.append(velocity.options.renderTo);            
            velocity.draw();

            return velocity;
        }
    }
    
    viewList.push(SpeedoCompare);
}