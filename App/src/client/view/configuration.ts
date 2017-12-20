
/*

Configuration

Data Trasfer
- Download 
- Upload
- Clear Buffer

App Settings
- MpH or KmH
- Enable / Disable Swipe Help

Connection to Dongle
(show if connected or not and show current mac address)
- Browse Dongles
- Diconnect from Device
// if connect to new dongle abfrage to disconnect form old device
// if disconnect form old divice and buffer not empty abfrage that all old data will be whiped
//    if user dont want that he needs to upload the buffered data first

Connetion to Backend
(show if parameters are right or not)
- Test Connection to Backend
- URL to Backend
- Login Parameter (User/Password/Token for Backend)
- Disconnect / Logout form Backend

Logging (Default eingeklappt)
- einfache Textbox mit lognachrichten


+-----------------------------------+
|          Data Transfer            |
+------+------------------+---------+
       |     Download     |
       +------------------+
       |      Upload      |
       +------------------+
       |   Clear Buffer   |
+------+------------------+---------+
|       Connection to Dongle        |
+------+------------------+---------+
       |     Connect      |                Nach connect auf click OK, test ausführe
       +------------------+
       |  °   Test        |                Grüner / Roter Punkt ob erfolgreich oder nicht
       +------------------+
       |    Disconnect    |
+------+------------------+---------+
|      Connection to Backend        |
+------+------------------+---------+
       |     Connect      |                Nach connect auf click OK, test ausführen
       +------------------+
       |  °   Test        |                Grüner / Roter Punkt ob erfolgreich oder nicht
       +------------------+
       |    Disconnect    |
+------+------------------+---------+
|          App Settings             |
+------+------------------+---------+
       |    MpH or KmH    |                Kein Button sondern On/Off Switch
       +------------------+
       | Enable / Disable |                Kein Button sondern On/Off Switch
       |    Swipe Help    |
+------+------------------+---------+
|            Logging                |
+------+------------------+---------+
       |       ...        |
       |       ...        |
       |       ...        |
       |       ...        |
       |       ...        |
       +------------------+

*/

/*

TODO

Dialog (z.B. für config nachfrage ob dannach gleich upload to backend)
oder für Connect to backend dialog (da kommen alle parameter und am schluss kann man ok oder abbrechen)

*/

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
            });

            test.click(() => {
                Logging.push("Dongle -> Test was clicked!");

                test.addClass(Var.Style.View.Configuration.Button.Test.bad);
            });

            disconnect.click(() => {
                Logging.push("Dongle -> Disconnect was clicked!");
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