namespace View {
    declare let RadialGauge: any;
    declare let LinearGauge: any;

    export class Speedo extends View {
        public static lastInstance: Speedo = null;

        private velocity: JQuery = null;
        private rotSpeed: JQuery = null;
        private oilTemperatur: JQuery = null

        private lastNotification: string = "";

        public constructor(control: JQuery) {
            super(control);   
            
            Speedo.lastInstance = this;

            this.control.addClass(Var.Style.View.speedo);

            if(this.control.width() > this.control.height() / 2) {
                this.control.width(this.control.height() / 2);
                this.control.css("left", window.innerWidth / 2 - this.control.width() / 2);
            }

            this.drawVelocity();
            this.drawRotSpeed();
            this.drawOilTemperatur();

            Dongle.bluetooth.addNotification(this.onNotification); 
        }

        public destroy() {
            super.destroy();

            Dongle.bluetooth.removeNotification(this.onNotification); 

            Speedo.lastInstance = null;
        }

        private onNotification(buffer: Uint8Array, text: string) {
            let entries = (Speedo.lastInstance.lastNotification + text).split(";");

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

                    if (pid == Settings.PIDs.coolantTemperatur) {
                        Speedo.lastInstance.oilTemperatur.value = parseInt(value);
                    }
                    else if (pid == Settings.PIDs.rpm) {
                        Speedo.lastInstance.rotSpeed.value = parseInt(value) / 100;
                    }
                    else if (pid == Settings.PIDs.velocity) {
                        Speedo.lastInstance.velocity.value = parseInt(value);
                    }
                } 
            }            

            if (entries.length > 0)
                Speedo.lastInstance.lastNotification = entries[entries.length - 1];
        }

        private drawVelocity() {
            this.velocity = new RadialGauge({
                renderTo: document.createElement('canvas'),
                width: this.control.width(),
                height: this.control.height() / 2,
                units: "Km/h",
                title: false,
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
            
            this.control.append(this.velocity.options.renderTo);            
            this.velocity.draw();
        }

        private drawRotSpeed() {
            this.rotSpeed = new RadialGauge({
                renderTo: document.createElement('canvas'),
                width: this.control.width(),
                height:  this.control.height() / 2,
                units: "rpmx100",
                title: false,
                value: 0,
                minValue: 0,
                maxValue: 80,
                startAngle: 180,
                ticksAngle: 180,
                majorTicks: [
                    '0','10','20','30','40','50','60','70','80'
                ],
                minorTicks: 2,
                strokeTicks: false,
                highlights: [
                    { from: 50, to: 70, color: 'rgba(255,255,0,.15)' },
                    { from: 70, to: 80, color: 'rgba(255,30,0,.25)' }
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
                valueBox: false,
                animationRule: 'linear',
                animationDuration: 100
            });
            
            this.control.append(this.rotSpeed.options.renderTo);            
            this.rotSpeed.draw();

            $(this.rotSpeed.canvas.element).css("position", "absolute");
            $(this.rotSpeed.canvas.element).css("left", (-0.35 * this.control.width()) + "px");
            $(this.rotSpeed.canvas.element).css("bottom", "0px");
        }

        private drawOilTemperatur() {
            this.oilTemperatur = new LinearGauge({
                renderTo: document.createElement('canvas'),
                width: 0.5 * this.control.width(),
                height:  this.control.height() / 2,
                units: "°C",
                minValue: 0,
                startAngle: 90,
                ticksAngle: 180,
                valueBox: false,
                maxValue: 240,
                majorTicks: [
                    "0",
                    "40",
                    "80",
                    "120",
                    "160",
                    "200",
                    "240"
                ],
                minorTicks: 2,
                strokeTicks: true,
                highlights: [
                    {
                        "from": 180,
                        "to": 240,
                        "color": "rgba(255,30,0,.25)"
                    }
                ],
                colorPlate: "transparent",
                colorMajorTicks: '#f5f5f5',
                colorMinorTicks: '#ddd',
                colorTitle: '#fff',
                colorUnits: '#ccc',
                colorNumbers: '#eee',
                colorNeedle: 'rgba(240, 128, 128, 1)',
                colorNeedleEnd: 'rgba(255, 160, 122, .9)',
                borderShadowWidth: 0,
                borders: false,
                needleType: "arrow",
                needleWidth: 5,
                animationDuration: 100,
                animationRule: "linear",
                tickSide: "left",
                numberSide: "left",
                needleSide: "left",
                barStrokeWidth: 0,
                barBeginCircle: false,
                barWidth: 0
            });

            this.control.append(this.oilTemperatur.options.renderTo);            
            this.oilTemperatur.draw();

            $(this.oilTemperatur.canvas.element).css("position", "absolute");
            $(this.oilTemperatur.canvas.element).css("right", (-0.1 * this.control.width()) + "px");
            $(this.oilTemperatur.canvas.element).css("bottom", "0px");
        }
    }
    
    viewList.push(Speedo);
}