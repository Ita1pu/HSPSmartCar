namespace Dialog {
    export class Button {
        private control: JQuery = null;

        private text: string = null;
        private callback: () => void = null
        private isDangerous: boolean;

        public constructor(text: string, callback: () => void, isDangerous: boolean = false) {
            this.text = text;
            this.callback = callback;
            this.isDangerous = isDangerous;
        }

        public draw(control: JQuery) {
            this.control = $("<div>").text(this.text).appendTo(control);

            if (this.isDangerous) {
                this.control.addClass(Var.Style.Dialog.Button.dangerous);
            }

            this.control.click(this.callback);
        }
    }

    export class Dialog {
        private background: JQuery = null;
        private control: JQuery = null;

        private texts: string[] = null;
        private buttons: Button[] = null;

        private closeCallback: () => void = null;

        public constructor(texts: string[], buttons: Button[] = null, closeCallback: () => void = null) {
            this.texts = texts;
            this.buttons = buttons;
            this.closeCallback = closeCallback;

            this.background = $("<div>", { class: Var.Style.Dialog.background }).appendTo($(document.body));

            this.control = $("<div>", { class: Var.Style.dialog }).appendTo(this.background);
            
            let body = $("<div>", { class: Var.Style.Dialog.body }).appendTo(this.control);
            let buttonDiv = $("<div>", { class: Var.Style.Dialog.buttonDiv }).appendTo(this.control);

            if (this.closeCallback != null) {
                let closeButton = $("<div>", { class: Var.Style.Dialog.closeButton }).appendTo(this.control);

                closeButton.click(this.closeCallback);
            }
            
            for (let text of texts) {
                $("<div>").text(text).appendTo(body);
            }

            if (buttons != null) {
                for (let button of buttons) {
                    button.draw(buttonDiv);
                }
            }

            this.centerScreen();

            View.disableSwipe();
        }

        public destroy() {
            this.background.remove();

            View.enableSwipe();
        }

        private centerScreen() {
            this.control.css("left", "calc(50% - (" + this.control.width() + "px / 2) - 2px)");
            this.control.css("top", "calc(50% - (" + this.control.height() + "px / 2))");
        }  
    }
}