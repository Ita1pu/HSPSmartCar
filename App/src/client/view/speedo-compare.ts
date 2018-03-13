namespace View {
    declare let RadialGauge: any;

    export class SpeedoCompare extends View {
        private velocity: JQuery = null;
        private velocityMobile: JQuery = null;

        public constructor(control: JQuery) {
            super(control);

            this.control.addClass(Var.Style.View.speedoCompare);

            if(this.control.width() > this.control.height() / 2) {
                this.control.width(this.control.height() / 2);
                this.control.css("left", window.innerWidth / 2 - this.control.width() / 2);
            }

            this.velocity = this.drawVelocity(S(Strings.View.SpeedoCompare.car));
            this.velocityMobile = this.drawVelocity(S(Strings.View.SpeedoCompare.mobile));

            // <--- DEBUG
            DisplayFeature.noConnectionPanel.hide();

            setTimeout(() => {
                this.velocity.value = 33.2;
                this.velocityMobile.value = 36.8;
            }, 1000);
            // <--- END

            // Dongle.bluetooth.addNotification(this.onNotification); // TODO wieder rein
        }

        public destroy() {
            super.destroy();

            //Dongle.bluetooth.removeNotification(this.onNotification); // TODO wieder rein
        }

        private onNotification(buffer: Uint8Array, text: string) {
            Logging.push("onData: " +  buffer + " - " + JSON.stringify(buffer) + " - " + text); // TODO only debug  
            this.control.html(this.control.html() + "<br />" + text); // TODO only debug
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