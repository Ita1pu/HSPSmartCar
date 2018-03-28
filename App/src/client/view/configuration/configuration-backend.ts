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

                let urlInput: JQuery;
                let apiPortInput: JQuery;
                let identityPortInput: JQuery;
                let usernameInput: JQuery;
                let passwordInput: JQuery;

                let dialog = new Dialog.Dialog(
                    [ "" ],
                    [ new Dialog.Button("Save", () => {
                        Store.set(Settings.Store.Backend.url, urlInput.val());
                        Store.set(Settings.Store.Backend.apiPort, apiPortInput.val());
                        Store.set(Settings.Store.Backend.identityPort, identityPortInput.val());
                        Store.set(Settings.Store.Backend.username, usernameInput.val());
                        Store.set(Settings.Store.Backend.password, passwordInput.val());
                        
                        dialog.destroy();

                        test.removeClass(Var.Style.View.Configuration.Button.Test.good);
                        test.removeClass(Var.Style.View.Configuration.Button.Test.bad);
            
                        test.addClass(Var.Style.View.Configuration.Button.Test.progress);
                        
                        this.connect(() => {
                            test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                            test.removeClass(Var.Style.View.Configuration.Button.Test.bad);
            
                            test.addClass(Var.Style.View.Configuration.Button.Test.good);
                        }, () => {
                            test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                            test.removeClass(Var.Style.View.Configuration.Button.Test.good);
            
                            test.addClass(Var.Style.View.Configuration.Button.Test.bad);
                        });
                    }), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });

                dialog.maximize();

                let dialogBody = dialog.getBody();
                dialogBody.html("");

                let table = $("<table>", { class: Var.Style.View.Configuration.Backend.Dialog.table }).appendTo(dialogBody);

                // url
                let urlTr = $("<tr>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.entry }).appendTo(table);
                $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.label }).text("URL:").appendTo(urlTr);
                let urlDiv = $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.input }).appendTo(urlTr);
                urlInput = $("<input type='text'>").text(Store.get(Settings.Store.Backend.url)).appendTo(urlDiv);
                urlInput.val(Store.get(Settings.Store.Backend.url));

                // apiPort
                let apiPortTr = $("<tr>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.entry }).appendTo(table);
                $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.label }).text("API Port:").appendTo(apiPortTr);
                let apiPortDiv = $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.input }).appendTo(apiPortTr);
                apiPortInput = $("<input type='text'>").text(Store.get(Settings.Store.Backend.apiPort)).appendTo(apiPortDiv);
                apiPortInput.val(Store.get(Settings.Store.Backend.apiPort));

                // identityPort
                let identityPortTr = $("<tr>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.entry }).appendTo(table);
                $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.label }).text("Identity Port:").appendTo(identityPortTr);
                let identityPortDiv = $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.input }).appendTo(identityPortTr);
                identityPortInput = $("<input type='text'>").text(Store.get(Settings.Store.Backend.identityPort)).appendTo(identityPortDiv);
                identityPortInput.val(Store.get(Settings.Store.Backend.identityPort));

                // username
                let usernameTr = $("<tr>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.entry }).appendTo(table);
                $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.label }).text("Username:").appendTo(usernameTr);
                let usernameDiv = $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.input }).appendTo(usernameTr);
                usernameInput = $("<input type='text'>").text(Store.get(Settings.Store.Backend.username)).appendTo(usernameDiv);
                usernameInput.val(Store.get(Settings.Store.Backend.username));

                // password
                let passwordTr = $("<tr>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.entry }).appendTo(table);
                $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.label }).text("Password:").appendTo(passwordTr);
                let passwordDiv = $("<td>", { class: Var.Style.View.Configuration.Backend.Dialog.Table.Entry.input }).appendTo(passwordTr);
                passwordInput = $("<input type='password'>").text(Store.get(Settings.Store.Backend.password)).appendTo(passwordDiv);
                passwordInput.val(Store.get(Settings.Store.Backend.password));
            });

            test.click(() => {
                Logging.push("Background -> Test was clicked!");

                test.removeClass(Var.Style.View.Configuration.Button.Test.good);
                test.removeClass(Var.Style.View.Configuration.Button.Test.bad);

                test.addClass(Var.Style.View.Configuration.Button.Test.progress);

                this.connect(() => {
                    test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                    test.removeClass(Var.Style.View.Configuration.Button.Test.bad);
    
                    test.addClass(Var.Style.View.Configuration.Button.Test.good);
                }, () => {
                    test.removeClass(Var.Style.View.Configuration.Button.Test.progress);
                    test.removeClass(Var.Style.View.Configuration.Button.Test.good);
    
                    test.addClass(Var.Style.View.Configuration.Button.Test.bad);
                });
            });

            disconnect.click(() => {
                Logging.push("Background -> Disconnect was clicked!");

                let dialog = new Dialog.Dialog(
                    [ "Delete all connection parameters for the Backend?" ],
                    [ new Dialog.Button("Delete", () => { 
                        Store.clear(Settings.Store.Backend.apiPort);
                        Store.clear(Settings.Store.Backend.identityPort);
                        Store.clear(Settings.Store.Backend.password);
                        Store.clear(Settings.Store.Backend.token);
                        Store.clear(Settings.Store.Backend.url);
                        Store.clear(Settings.Store.Backend.username);

                        dialog.destroy(); 
                    }, true), new Dialog.Button("Cancel", () => { dialog.destroy(); }) ], 
                    () => { dialog.destroy(); });
            });
        }

        private connect(succesCallback: () => void, errorCallback: () => void) {
            Logging.push("Backend connecting...");

            let params = $.param({
                username: Store.get(Settings.Store.Backend.username), 
                password: Store.get(Settings.Store.Backend.password),
                scope: "openid",
                grant_type: "password",
                client_id: Settings.Backend.clientId,
                client_secret: Settings.Backend.secret
            });
        
            $.ajax({
                url: "http://" + Store.get(Settings.Store.Backend.url) + ":" + Store.get(Settings.Store.Backend.identityPort) + "/connect/token",
                method: "POST",
                dataType: "json",
                jsonp: false,
                contentType: "application/x-www-form-urlencoded",
                data: params,
                processData: false,
                timeout: 5000,
                success: function (response) {
                    Logging.push("Backend connected successfully: " + JSON.stringify(response));

                    // TODO save token

                    succesCallback();
                },
                error: (request: JQueryXHR, status: string, error: string) => {
                    Logging.push("Backend connection failed: " + status + " | " + error);

                    errorCallback();
                }
            }); 
        }
    }
}