namespace View {
    export class ConfigurationBackground {
        private control: JQuery

        public constructor(control: JQuery) {
            this.control = control;

            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.connectionToBackground).appendTo(this.control);

            let connect = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.connect).appendTo(this.control);

            let test = $("<div>", { class: Var.Style.View.Configuration.button + " " + 
                    Var.Style.View.Configuration.Button.test })
                .text(Strings.View.Configuration.Button.test).appendTo(this.control);
            $("<div>").prependTo(test);

            let disconnect = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.disconnect).appendTo(this.control);

            connect.click(() => {
                Logging.push("Background -> Connect was clicked!");

                // TODO connect
            });

            test.click(() => {
                Logging.push("Background -> Test was clicked!");

                test.addClass(Var.Style.View.Configuration.Button.Test.progress);

                // TODO Test
            });

            disconnect.click(() => {
                Logging.push("Background -> Disconnect was clicked!");

                let dialog = new Dialog.Dialog(
                    [ "Delete all connection parameters for the Backend?" ],
                    [ new Dialog.Button("Delete", () => { 

                        // TODO delete all connection parameters for the backend.

                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }
    }
}