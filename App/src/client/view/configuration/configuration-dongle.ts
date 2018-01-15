// TODO nochmals testen (connection hat schonmal geklappt)

namespace View {
    export class ConfigurationDongle {
        private control: JQuery

        public constructor(control: JQuery) {
            this.control = control;

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

                // TODO show dialog
                let dialog = new Dialog.Dialog(
                    [ "" ],
                    [ new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });

                let dialogBackground = dialog.getBackground();
                let dialogBody = dialog.getBody();
                let dialogButtonDiv = dialog.getButtonDiv();

                dialogBody.height(dialogBackground.height() - dialogButtonDiv.height() - 50);
                dialog.centerScreen();

                dialogBody.html("");

                // TODO refresh button

                // TODO loading bubble einblenden

                Dongle.bluetooth.browse((device: any) => {
                    Logging.push("Bluetooth device found: " + device.id + " | " + device.name); 
                    
                    // TODO css
                    // TODO if no name show <unknown>

                    let entry = $("<div>", { class: "" }).appendTo(dialogBody);
                    let nameDiv = $("<div>", { class: ""}).text(device.name).appendTo(entry);
                    let idDiv = $("<div>", { class: "" }).text(device.id).appendTo(entry);
                    
                    entry.click(() => {
                        // TODO alles andere disablen und darüber noch einen loading bubble anzeigen

                        Logging.push("Dongle -> Entry was clicked: " + device.id + " | " +  device.name);

                        let address_or_uuid = device.address;

                        if (typeof address_or_uuid === "undefined") {
                            address_or_uuid = device.uuid;
                        }

                        Store.set(Settings.Store.address_or_uuid, address_or_uuid);
                        Dongle.bluetooth.connect(address_or_uuid, () => {
                            test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                            test.removeClass(Var.Style.View.Configuration.Button.Test.bad);
            
                            test.addClass(Var.Style.View.Configuration.Button.Test.good);

                            dialog.destroy();
                        }, () => {
                            test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                            test.removeClass(Var.Style.View.Configuration.Button.Test.good);
            
                            test.addClass(Var.Style.View.Configuration.Button.Test.bad);

                            dialog.destroy();
                        });
                    });
                }, 
                () => {
                    Logging.push("Bluetooth browse succeeded!");
                    
                    // TODO loading bubble ausblenden
                },
                () => {
                    Logging.push("Bluetooth browse failed!");

                    // TODO loading bubble ausblenden
                });
            });

            test.click(() => {
                Logging.push("Dongle -> Test was clicked!");

                test.removeClass(Var.Style.View.Configuration.Button.Test.good);
                test.removeClass(Var.Style.View.Configuration.Button.Test.bad);

                test.addClass(Var.Style.View.Configuration.Button.Test.progress);

                Dongle.bluetooth.IsConnected(() => {
                    test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                    test.addClass(Var.Style.View.Configuration.Button.Test.good);
                },
                () => {
                    Dongle.bluetooth.connect(Store.get(Settings.Store.address_or_uuid), 
                    () => {
                        test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                        test.addClass(Var.Style.View.Configuration.Button.Test.good);
                    }, 
                    () => {
                        test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                        test.addClass(Var.Style.View.Configuration.Button.Test.bad);
                    });
                });
            });

            disconnect.click(() => {
                Logging.push("Dongle -> Disconnect was clicked!");

                let dialog = new Dialog.Dialog(
                    [ "Disconnect from Dongle and clear all Bluetooth settings?" ],
                    [ new Dialog.Button("Delete", () => {                        
                        Dongle.bluetooth.disconnect();

                        Store.clear(Settings.Store.address_or_uuid);

                        dialog.destroy();
                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }
    }
}