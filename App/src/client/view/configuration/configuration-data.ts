namespace View {
    export class ConfigurationData {
        private control: JQuery

        public constructor(control: JQuery) {
            this.control = control;

            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.dataTransfer).appendTo(this.control);

            let download = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.DataTransfer.download).appendTo(this.control);

            let upload = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.DataTransfer.upload).appendTo(this.control);

            let clearBuffer = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.DataTransfer.clearBuffer).appendTo(this.control);

            download.click(() => {
                Logging.push("Download was clicked!");

                // TODO download
            });

            upload.click(() => {
                Logging.push("Upload was clicked!");

                // TODO upload
            });

            clearBuffer.click(() => {
                Logging.push("Clear Buffer was clicked!");

                // TODO Neben button clearbuffer noch die größe des Buffers anzeigen |###---------| 43MB

                let dialog = new Dialog.Dialog(
                    [ "Delete all buffered Data?" ],
                    [ new Dialog.Button("Delete", () => { 

                        // TODO clear buffer

                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }
    }
}