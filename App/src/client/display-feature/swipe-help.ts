namespace DisplayFeature {  
    export let swipeHelp: SwipeHelp = null;
    export class SwipeHelp { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;

            control.text("swipeeeeeeee")
        }

        public static init(control: JQuery): SwipeHelp {
            if (DisplayFeature.swipeHelp == null)
                DisplayFeature.swipeHelp = new SwipeHelp(control);

            return DisplayFeature.swipeHelp;
        }
    }
}