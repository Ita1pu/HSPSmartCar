namespace View {  
    export let viewList: (typeof View)[] = [];

    export class View { 
        protected control: JQuery = null;   
        
        public constructor(control: JQuery) {
            this.control = control;
        }

        public destroy(): void {
            this.control.remove();
        };
    }    
}