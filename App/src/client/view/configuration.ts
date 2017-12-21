namespace View {
    export class Configuration extends View {         
        public constructor(control: JQuery) {
            super(control);

            this.control.addClass(Var.Style.View.configuration);     
            
            this.initDataTransfer();
            this.initConnectionToDongle();
            this.initConnectionToBackground();
            this.initAppSettings();
            this.initLogging();
        }

        private initDataTransfer() {
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
            });

            upload.click(() => {
                Logging.push("Upload was clicked!");
            });

            clearBuffer.click(() => {
                Logging.push("Clear Buffer was clicked!");

                // Neben button clearbuffer noch die größe des Buffers anzeigen |###---------| 43MB
            });
        }

        private initConnectionToDongle() {
            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.connectionToDongle).appendTo(this.control);

            let connect = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.connect).appendTo(this.control);

            let test = $("<div>", { class: Var.Style.View.Configuration.button + " " + 
                    Var.Style.View.Configuration.Button.test })
                .text(Strings.View.Configuration.Button.test).appendTo(this.control);
            $("<div>").prependTo(test);

            let disconnect = $("<div>", { class: Var.Style.View.Configuration.button })
                .text(Strings.View.Configuration.Button.disconnect).appendTo(this.control);

            connect.click(() => {
                Logging.push("Dongle -> Connect was clicked!");

                // wenn mit anderen verbinden wollen und buffer not empty
                // -> Please upload or clear buffer before connecting to another Dongle
            });

            test.click(() => {
                Logging.push("Dongle -> Test was clicked!");

                test.addClass(Var.Style.View.Configuration.Button.Test.bad);
            });

            disconnect.click(() => {
                Logging.push("Dongle -> Disconnect was clicked!");

                let dialog = new Dialog.Dialog(
                    [ "Disconnect from Dongle and clear all Bluetooth settings?" ],
                    [ new Dialog.Button("Delete", () => { 

                        // TODO if buffer not empty
                        // -> cancel with error = Please upload or clear buffer before disconnecting from Dongle

                        // TODO disconnect from dongle

                        // TODO clear bluetooth settings

                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }

        private initConnectionToBackground() {
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
            });

            test.click(() => {
                Logging.push("Background -> Test was clicked!");

                test.addClass(Var.Style.View.Configuration.Button.Test.progress);
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

        private initAppSettings() {
            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.appSettings).appendTo(this.control);

            let velocityUnit = $("<div>", { class: Var.Style.View.Configuration.switch })
                .appendTo(this.control);

            let velocityUnitMpH = $("<div>")
                .text(Strings.View.Configuration.Switch.VelocityUnit.MpH).appendTo(velocityUnit);

            let velocityUnitKmH = $("<div>")
                .text(Strings.View.Configuration.Switch.VelocityUnit.KmH).appendTo(velocityUnit);

            let swipeHelp = $("<div>", { class: Var.Style.View.Configuration.switch })
                .appendTo(this.control);

            let swipeHelpDisable = $("<div>")
                .text(Strings.View.Configuration.Switch.SwipeHelp.disbale).appendTo(swipeHelp);

            let swipeHelpEnable = $("<div>")
                .text(Strings.View.Configuration.Switch.SwipeHelp.enable).appendTo(swipeHelp);

            let velocityUnitVal = Store.get(Settings.Store.velocityUnit);
            if (velocityUnitVal == null || velocityUnitVal == Settings.Store.VelocityUnit.KmH) {
                velocityUnitKmH.addClass(Var.Style.View.Configuration.Switch.enabled);
            }
            else {
                velocityUnitMpH.addClass(Var.Style.View.Configuration.Switch.enabled);
            }

            let swipeHelpVal = Store.get(Settings.Store.showSwipeHelp);
            if (swipeHelpVal == null || swipeHelpVal == Settings.Store.ShowSwipeHelp.true) {
                swipeHelpEnable.addClass(Var.Style.View.Configuration.Switch.enabled);
            }
            else {
                swipeHelpDisable.addClass(Var.Style.View.Configuration.Switch.enabled);
            }

            let onVelocityUnit = () => {
                let velocityUnitVal = Store.get(Settings.Store.velocityUnit);
                if (velocityUnitVal == null || velocityUnitVal == Settings.Store.VelocityUnit.KmH) {
                    Store.set(Settings.Store.velocityUnit, Settings.Store.VelocityUnit.MpH);

                    velocityUnitKmH.removeClass(Var.Style.View.Configuration.Switch.enabled);
                    velocityUnitMpH.addClass(Var.Style.View.Configuration.Switch.enabled);
                }
                else {
                    Store.set(Settings.Store.velocityUnit, Settings.Store.VelocityUnit.KmH);

                    velocityUnitKmH.addClass(Var.Style.View.Configuration.Switch.enabled);
                    velocityUnitMpH.removeClass(Var.Style.View.Configuration.Switch.enabled);
                }
            };
            velocityUnitMpH.click(onVelocityUnit);
            velocityUnitKmH.click(onVelocityUnit);

            let onSwipeHelp = () => {
                let swipeHelpVal = Store.get(Settings.Store.showSwipeHelp);
                if (swipeHelpVal == null || swipeHelpVal == Settings.Store.ShowSwipeHelp.true) {
                    Store.set(Settings.Store.showSwipeHelp, Settings.Store.ShowSwipeHelp.false);

                    swipeHelpEnable.removeClass(Var.Style.View.Configuration.Switch.enabled);
                    swipeHelpDisable.addClass(Var.Style.View.Configuration.Switch.enabled);
                }
                else {
                    Store.set(Settings.Store.showSwipeHelp, Settings.Store.ShowSwipeHelp.true);

                    swipeHelpEnable.addClass(Var.Style.View.Configuration.Switch.enabled);
                    swipeHelpDisable.removeClass(Var.Style.View.Configuration.Switch.enabled);
                }
            };
            swipeHelpEnable.click(onSwipeHelp);
            swipeHelpDisable.click(onSwipeHelp);
        }

        private initLogging() {
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
    
    viewList.push(Configuration);
}