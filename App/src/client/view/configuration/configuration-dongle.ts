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

                let dialog = new Dialog.Dialog(
                    [ "" ],
                    [ new Dialog.Button("Cancel", () => { 
                        Dongle.bluetooth.stopScan();
                        dialog.destroy(); 
                    }) ], 
                    () => { 
                        Dongle.bluetooth.stopScan();
                        dialog.destroy(); 
                    });

                dialog.maximize();

                let dialogBody = dialog.getBody();
                dialogBody.html("");

                let dialogDiv = $("<div>", { class: Var.Style.View.Configuration.Dongle.dialog }).appendTo(dialogBody);

                Dongle.bluetooth.startScan((device: any) => {
                    Logging.push("Bluetooth device found: " + device.id + " | " + device.name); 

                    if (device.name == null || typeof device.name === typeof undefined || device.name == "")
                        device.name = "---";

                    let entry = $("<div>", { class: Var.Style.View.Configuration.Dongle.Dialog.entry }).appendTo(dialogDiv);
                    let nameDiv = $("<div>", { class:  Var.Style.View.Configuration.Dongle.Dialog.Entry.name }).text(device.name).appendTo(entry);
                    let idDiv = $("<div>", { class:  Var.Style.View.Configuration.Dongle.Dialog.Entry.id }).text(device.id).appendTo(entry);
                    let rssiDiv = $("<div>", { class:  Var.Style.View.Configuration.Dongle.Dialog.Entry.rssi }).text(device.rssi).appendTo(entry);
                    
                    entry.click(() => {
                        Logging.push("Dongle -> Entry was clicked: " + device.id + " | " +  device.name);

                        Store.set(Settings.Store.deviceId, device.id);
                        Dongle.bluetooth.stopScan();
                        dialog.destroy(); 
                        test.click();
                    });
                },
                () => {
                    Logging.push("Bluetooth browse failed!");
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
                    Dongle.bluetooth.connect(Store.get(Settings.Store.deviceId), 
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

                        Store.clear(Settings.Store.deviceId);

                        dialog.destroy();
                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }
    }
}