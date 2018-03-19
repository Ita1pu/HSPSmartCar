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
             // TODO not working, not in same scope
            
            Logging.push("onData: " +  buffer + " - " + JSON.stringify(buffer) + " - " + text); // TODO only debug  
            this.control.html(this.control.html() + "<br />" + text); // TODO only debug

            // <--- DEBUG
            SimpleList.lastInstance.velocity.text("30 Km/H");
            SimpleList.lastInstance.rotSpeed.text("40");
            SimpleList.lastInstance.oilTemperatur.text("40 °C");
            SimpleList.lastInstance.gps.text("41.40338, 2.17403");
            // <--- END
        }

        private onNewPosition(position: Position) {
            SimpleList.lastInstance.gpsMobile.text(position.coords.latitude + ", " + position.coords.longitude);

            if (position.coords.speed != null) 
                SimpleList.lastInstance.velocityMobile.text(position.coords.speed);
            else
                SimpleList.lastInstance.velocityMobile.text("Not supported!");
        }

        private addTableRow(text: string): JQuery {
            let row = $("<tr>", { class: Var.Style.View.SimpleList.Table.entry }).appendTo(this.table);
            let label = $("<td>", { class: Var.Style.View.SimpleList.Table.label }).text(text).appendTo(row);
            return ($("<td>", { class: Var.Style.View.SimpleList.Table.value }).appendTo(row));
        }
    }
    
    viewList.push(SimpleList);
}