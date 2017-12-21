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

    let isSwipeEnabled = true;

    export function enableSwipe() {
        isSwipeEnabled = true;
    }

    export function disableSwipe() {
        isSwipeEnabled = false;
    }

    export function handleSwipe(callback: (direction: number) => void) {
        let xDown: any = null;                                                        
        let yDown: any = null;   
    
        document.addEventListener(Settings.isMobile ? "touchstart" : "mousedown", (evt: any) => {  
            if (Settings.isMobile) {           
                xDown = evt.touches[0].clientX;                                      
                yDown = evt.touches[0].clientY;   
            }
            else {
                xDown = evt.clientX;
                yDown = evt.clientY;
            }  
        });  

        if (Settings.isMobile == false) {
            document.addEventListener("mouseup", (evt: any) => {
                xDown = null;
                yDown = null;
            });
        }
    
        document.addEventListener(Settings.isMobile ? "touchmove" : "mousemove", (evt: any) => { 
            if (!xDown || !yDown || !isSwipeEnabled)
                return;
        
            let xUp;                                  
            let yUp;        
    
            if (Settings.isMobile) {           
                xUp = evt.touches[0].clientX;                                      
                yUp = evt.touches[0].clientY;   
            }
            else {
                xUp = evt.clientX;
                yUp = evt.clientY;
            } 
        
            let xDiff = xDown - xUp;
            let yDiff = yDown - yUp;
            if (Math.abs(xDiff) + Math.abs(yDiff) > Settings.swipeDistance) {
                if (Math.abs(xDiff) > Math.abs(yDiff)) {
                    if (xDiff > 0) {
                        // Swipe to left
    
                        callback(1);
                    } else {
                        // Swipe to right
    
                        callback(-1)
                    } 
                }
    
                xDown = null;
                yDown = null;
            }
        }, false);
    }
}