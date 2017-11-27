namespace DisplayFeature {  
    export let viewCircles: ViewCircles = null;
    export class ViewCircles { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;
        }

        public static init(control: JQuery): ViewCircles {
            if (DisplayFeature.viewCircles == null)
                DisplayFeature.viewCircles = new ViewCircles(control);

            return DisplayFeature.viewCircles;
        }
    }
}