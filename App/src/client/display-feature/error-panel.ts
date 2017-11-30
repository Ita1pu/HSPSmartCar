namespace DisplayFeature {  
    export let errorPanel: ErrorPanel = null;

    export class ErrorPanel { 
        private static readonly DEFAULT_TEXT = 
            "<br /><br />See \"Configuration → Logging\" for more informations!";

        protected control: JQuery = null; 
        
        private imageDiv: JQuery = null;
        private textDiv: JQuery = null;

        private lastErrorNum: number = 0;

        private constructor(control: JQuery) {
            this.control = control;

            this.control.addClass(Var.Style.errorPanel);

            this.imageDiv = $("<div>", { class: Var.Style.ErrorPanel.image}).appendTo(this.control);
            this.textDiv = $("<div>", { class: Var.Style.ErrorPanel.text}).appendTo(this.control);
        }

        public static init(control: JQuery): ErrorPanel {
            if (DisplayFeature.errorPanel == null)
                DisplayFeature.errorPanel = new ErrorPanel(control);

            return DisplayFeature.errorPanel;
        }

        public showError(msg: Logging.ErrorLogEntry | string) {
            let myError = ++this.lastErrorNum;

            let entry: Logging.ErrorLogEntry;
            if (typeof msg == "string") {
                Logging.errors.push(new Logging.ErrorLogEntry([ msg ]));
            
                this.textDiv.text(msg);
            }
            else {
                Logging.errors.push(msg);

                this.textDiv.text(msg.getMessage()[0]);
            } 

            this.textDiv.html(this.textDiv.html() + ErrorPanel.DEFAULT_TEXT);
            
            this.control.show();

            setTimeout(() => {
                if (myError == this.lastErrorNum) {
                    this.control.fadeOut(2000);
                }
            }, 5000);
        }
    }
}