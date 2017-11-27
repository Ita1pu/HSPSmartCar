namespace View {
    export class SimpleList extends View {         
        public constructor(control: JQuery) {
            super(control);

            this.control.text("SIMPLE LIST!!!!");
        }
    }
    
    viewList.push(SimpleList);
}