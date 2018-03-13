namespace View {
    export class SimpleList extends View {
        private table: JQuery = null;

        private velocity: JQuery = null;
        private velocityMobile: JQuery = null;
        private rotSpeed: JQuery = null;
        private oilTemperatur: JQuery = null;
        private gps: JQuery = null;
        private gpsMobile: JQuery = null;
        
        public constructor(control: JQuery) {
            super(control);

            this.control.addClass(Var.Style.View.simpleList);

            this.table = $("<table>", { class: Var.Style.View.SimpleList.table }).appendTo(this.control);

            this.velocity = this.addTableRow(S(Strings.View.SimpleList.velocity));
            this.velocityMobile = this.addTableRow(S(Strings.View.SimpleList.velocity, Strings.View.SimpleList.mobile));
            this.rotSpeed = this.addTableRow(S(Strings.View.SimpleList.rotSpeed));
            this.oilTemperatur = this.addTableRow(S(Strings.View.SimpleList.oilTemperatur));
            this.gps = this.addTableRow(S(Strings.View.SimpleList.gps));
            this.gpsMobile = this.addTableRow(S(Strings.View.SimpleList.gps, Strings.View.SimpleList.mobile));

            // <--- DEBUG
            DisplayFeature.noConnectionPanel.hide();

            this.velocity.text("30 Km/H");
            this.velocityMobile.text("30 Km/H");
            this.rotSpeed.text("40");
            this.oilTemperatur.text("40 °C");
            this.gps.text("41.40338, 2.17403");
            this.gpsMobile.text("41.40338, 2.17403");
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

        private addTableRow(text: string): JQuery {
            let row = $("<tr>", { class: Var.Style.View.SimpleList.Table.entry }).appendTo(this.table);
            let label = $("<td>", { class: Var.Style.View.SimpleList.Table.label }).text(text).appendTo(row);
            return $("<td>", { class: Var.Style.View.SimpleList.Table.value }).appendTo(row);
        }
    }
    
    viewList.push(SimpleList);
}