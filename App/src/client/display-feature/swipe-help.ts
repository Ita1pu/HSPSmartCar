namespace DisplayFeature {  
    export let swipeHelp: SwipeHelp = null;
    export class SwipeHelp { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;

            this.control.addClass(Var.Style.swipeHelp);
            
            // Hide Swipe Help after x seconds.
            setTimeout(() => {
                this.hide();
            }, 3000);
        }

        public static init(control: JQuery): SwipeHelp {
            if (DisplayFeature.swipeHelp == null)
                DisplayFeature.swipeHelp = new SwipeHelp(control);

            return DisplayFeature.swipeHelp;
        }

        public hide() {
            this.control.hide();
        }
    }
}