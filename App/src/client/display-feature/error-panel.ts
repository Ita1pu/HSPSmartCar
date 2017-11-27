namespace DisplayFeature {  
    export let errorPanel: ErrorPanel = null;

    export class ErrorPanel { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;
        }

        public static init(control: JQuery): ErrorPanel {
            if (DisplayFeature.errorPanel == null)
                DisplayFeature.errorPanel = new ErrorPanel(control);

            return DisplayFeature.errorPanel;
        }

        public showError(entry: Logging.ErrorLogEntry) {
            Logging.errors.push(entry);
        }
    }
}