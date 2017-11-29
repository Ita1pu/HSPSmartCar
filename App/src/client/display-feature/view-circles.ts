namespace DisplayFeature {  
    export let viewCircles: ViewCircles = null;
    export class ViewCircles { 
        protected control: JQuery = null;   

        private constructor(control: JQuery) {
            this.control = control;

            this.control.addClass(Var.Style.viewCircles);

            for (let i = 0; i < View.viewList.length; i++) {
                $("<div>", { 
                    id: Var.Style.ViewCircles.circle + "_" + i, 
                    class: Var.Style.ViewCircles.circle 
                }).appendTo(this.control);
            }

            this.control.css("left", "calc(50% - (" + this.control.width() + "px / 2))")
        }

        public static init(control: JQuery): ViewCircles {
            if (DisplayFeature.viewCircles == null)
                DisplayFeature.viewCircles = new ViewCircles(control);

            return DisplayFeature.viewCircles;
        }

        public select(number: number) {
            $("." + Var.Style.ViewCircles.circle)
                .removeClass(Var.Style.ViewCircles.Circle.selected);

            $("#" + Var.Style.ViewCircles.circle + "_" + number)
                .addClass(Var.Style.ViewCircles.Circle.selected);
        }
    }
}