namespace DisplayFeature {  
    export let noConnectionPanel: NoConnectionPanel = null;
    export class NoConnectionPanel { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;

            // if no connection to bluetooth and view != configuration
            // noConnectionDiv in the middle of the content and in red (content disabled...)
            // let noConnectionDiv = $("<div>").text("No connection to Dongle.").appendTo($(document.body)); 

        }

        public static init(control: JQuery): NoConnectionPanel {
            if (DisplayFeature.noConnectionPanel == null)
                DisplayFeature.noConnectionPanel = new NoConnectionPanel(control);

            return DisplayFeature.noConnectionPanel;
        }
    }
}