namespace DisplayFeature {  
    export let noConnectionPanel: NoConnectionPanel = null;
    export class NoConnectionPanel { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;

            this.control.addClass(Var.Style.noConnectionPanel);

            let panel = $("<div>", { class: Var.Style.NoConnectionPanel.panel }).appendTo(this.control);
            let imageDiv = $("<div>", { class: Var.Style.NoConnectionPanel.Panel.image }).appendTo(panel);
            let textDiv = $("<div>", { class: Var.Style.NoConnectionPanel.Panel.text }).appendTo(panel);

            textDiv.text(S(Strings.noConnectionPanel));

            panel.css("top", "calc(50% - (" + imageDiv.height() + "px / 2))");
        }

        public static init(control: JQuery): NoConnectionPanel {
            if (DisplayFeature.noConnectionPanel == null)
                DisplayFeature.noConnectionPanel = new NoConnectionPanel(control);

            return DisplayFeature.noConnectionPanel;
        }

        public show() {
            this.control.show();
        }

        public hide() {
            this.control.hide();
        }
    }
}