namespace View {
    export class SimpleList extends View {
        public static lastInstance: SimpleList = null;

        private table: JQuery = null;

        private velocity: JQuery = null;
        private velocityMobile: JQuery = null;
        private rotSpeed: JQuery = null;
        private oilTemperatur: JQuery = null;
        private gps: JQuery = null;
        private gpsMobile: JQuery = null;

        private lastNotification: string = "";
        
        public constructor(control: JQuery) {
            super(control);

            SimpleList.lastInstance = this;

            this.control.addClass(Var.Style.View.simpleList);

            this.table = $("<table>", { class: Var.Style.View.SimpleList.table }).appendTo(this.control);

            this.velocity = this.addTableRow(S(Strings.View.SimpleList.velocity));
            this.velocityMobile = this.addTableRow(S(Strings.View.SimpleList.velocity, Strings.View.SimpleList.mobile));
            this.rotSpeed = this.addTableRow(S(Strings.View.SimpleList.rotSpeed));
            this.oilTemperatur = this.addTableRow(S(Strings.View.SimpleList.oilTemperatur));
            this.gps = this.addTableRow(S(Strings.View.SimpleList.gps));
            this.gpsMobile = this.addTableRow(S(Strings.View.SimpleList.gps, Strings.View.SimpleList.mobile));

            Mobile.gps.addCallback(this.onNewPosition);
            Dongle.bluetooth.addNotification(this.onNotification); 
        }

        public destroy() {
            super.destroy();

            Mobile.gps.removeCallback(this.onNewPosition);
            Dongle.bluetooth.removeNotification(this.onNotification);

            SimpleList.lastInstance = null;
        }

        private onNotification(buffer: Uint8Array, text: string) {
            let entries = (SimpleList.lastInstance.lastNotification + text).split(";");

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

                    if (pid == Settings.PIDs.latitude) {
                        let longitude = "-";
                        if (SimpleList.lastInstance.gps.text().split(", ").length == 2)
                           longitude = SimpleList.lastInstance.gps.text().split(", ")[1];
                            
                        SimpleList.lastInstance.gps.text((parseInt(value) / 1000000).toPrecision(6)  + ", " + longitude);
                    }
                    else if (pid == Settings.PIDs.longitude) {
                        let latitude = "-";
                        if (SimpleList.lastInstance.gps.text().split(", ").length == 2)
                            latitude = SimpleList.lastInstance.gps.text().split(", ")[0];

                        SimpleList.lastInstance.gps.text(latitude + ", " + (parseInt(value) / 1000000).toPrecision(6));
                    }
                    else if (pid == Settings.PIDs.coolantTemperatur) {
                        SimpleList.lastInstance.oilTemperatur.text((parseInt(value) - 40) + " °C");
                    }
                    else if (pid == Settings.PIDs.rpm) {
                        SimpleList.lastInstance.rotSpeed.text((parseInt(value) * 0.25) + " U/min");
                    }
                    else if (pid == Settings.PIDs.velocity) {
                        SimpleList.lastInstance.velocity.text(value + " Km/H");
                    }
                } 
            }            

            if (entries.length > 0)
                SimpleList.lastInstance.lastNotification = entries[entries.length - 1];
        }

        private onNewPosition(position: Position) {
            SimpleList.lastInstance.gpsMobile.text(
                position.coords.latitude.toPrecision(6) + ", " + 
                position.coords.longitude.toPrecision(6));

            if (position.coords.speed != null) 
                SimpleList.lastInstance.velocityMobile.text(position.coords.speed);
            else
                SimpleList.lastInstance.velocityMobile.text("0");
        }

        private addTableRow(text: string): JQuery {
            let row = $("<tr>", { class: Var.Style.View.SimpleList.Table.entry }).appendTo(this.table);
            let label = $("<td>", { class: Var.Style.View.SimpleList.Table.label }).text(text).appendTo(row);
            return ($("<td>", { class: Var.Style.View.SimpleList.Table.value }).appendTo(row));
        }
    }
    
    viewList.push(SimpleList);
}