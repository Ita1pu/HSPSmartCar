namespace Dialog {
    export class Button {
        private control: JQuery = null;

        private text: string = null;

        public constructor(text: string) {
            this.text = text;
        }

        public draw(control: JQuery) {
            this.control = $("<div>").text(this.text).appendTo(control);
        }
    }

    export class Dialog {
        private background: JQuery = null;
        private control: JQuery = null;

        private texts: string[] = null;
        private buttons: Button[] = null;

        public constructor(texts: string[], buttons: Button[] = null, showCloseButton: boolean = true) {
            this.texts = texts;
            this.buttons = buttons;

            this.background = $("<div>", { class: Var.Style.Dialog.background }).appendTo($(document.body));

            this.control = $("<div>", { class: Var.Style.dialog }).appendTo(this.background);
            
            let body = $("<div>", { class: Var.Style.Dialog.body }).appendTo(this.control);
            let buttonDiv = $("<div>", { class: Var.Style.Dialog.buttonDiv }).appendTo(this.control);

            if (showCloseButton) {
                let closeButton = $("<div>", { class: Var.Style.Dialog.closeButton }).appendTo(this.control);
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
        }

        public destroy() {

        }

        private centerScreen() {
            this.control.css("left", "calc(50% - (" + this.control.width() + "px / 2))");
            this.control.css("top", "calc(50% - (" + this.control.height() + "px / 2))");
        }  
    }
}