namespace View {
    export class ConfigurationLogging {
        private control: JQuery

        public constructor(control: JQuery) {
            this.control = control;

            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.logging).appendTo(this.control);

            let load = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.load).appendTo(this.control);    

            let logging = $("<div>", { class: Var.Style.View.Configuration.logging }).appendTo(this.control);

            load.click(() => {
                logging.html("");

                for (let logEntry of Logging.getErrors()) {
                    $("<div>").html(logEntry.getHtmlMessage()).appendTo(logging);
                }
            }) 
        }
    }
}