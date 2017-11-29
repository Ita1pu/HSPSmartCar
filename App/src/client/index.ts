/// <reference path="../../typings/index.d.ts" />

window.onerror = (msg: string, url: string, line: number, col: number, error: Error) => {

    DisplayFeature.errorPanel.showError(
        new Logging.ErrorLogEntry([ msg ], url, line, col, error));

    // Prevent firing the default error handler
    return !Settings.isDebug;
};

$(() => {
    DisplayFeature.NoConnectionPanel.init($("<div>").appendTo($(document.body)));
    DisplayFeature.ViewCircles.init($("<div>").appendTo($(document.body))); 

    let swipeHelp = Store.get(Settings.Store.showSwipeHelp);
    if (swipeHelp == null || Boolean(swipeHelp) == true)
        DisplayFeature.SwipeHelp.init($("<div>").appendTo($(document.body)));

    DisplayFeature.ErrorPanel.init($("<div>").appendTo($(document.body))); 

    let selectedView: View.View = null;
    let selectedViewNum: number = Settings.defaultView;

    try {
        selectedViewNum = parseInt(Store.get(Settings.Store.selectedView));

        if (isNaN(selectedViewNum) || selectedViewNum < 0 || selectedViewNum > View.viewList.length) {
            selectedViewNum = Settings.defaultView;
            Store.set(Settings.Store.selectedView, Settings.defaultView.toString());
        }
    }
    catch (ex) {
        selectedViewNum = Settings.defaultView;
    }

    selectedView = new View.viewList[selectedViewNum]($("<div>").prependTo($(document.body)));
    DisplayFeature.viewCircles.select(selectedViewNum);

    let xDown: any = null;                                                        
    let yDown: any = null;   

    document.addEventListener('touchstart', (evt: any) => {             
        xDown = evt.touches[0].clientX;                                      
        yDown = evt.touches[0].clientY;                                      
    });  

    document.addEventListener('touchmove', (evt: any) => {
        if (!xDown || !yDown)
            return;
    
        let xUp = evt.touches[0].clientX;                                    
        let yUp = evt.touches[0].clientY;
    
        let xDiff = xDown - xUp;
        let yDiff = yDown - yUp;
        if (Math.abs(xDiff) + Math.abs(yDiff) > 150) {
            if (Math.abs(xDiff) > Math.abs(yDiff)) {
                let newSelectedViewNum = selectedViewNum;

                if (xDiff > 0) {
                    // Swipe to left

                    if (newSelectedViewNum < View.viewList.length - 1)
                        newSelectedViewNum++;
                } else {
                    // Swipe to right

                    if (newSelectedViewNum > 0)
                        newSelectedViewNum--;
                } 
                if (newSelectedViewNum != selectedViewNum) {   
                    selectedViewNum = newSelectedViewNum;                 
                    selectedView.destroy();

                    selectedView = new View.viewList[selectedViewNum]($("<div>").prependTo($(document.body)));
                    DisplayFeature.viewCircles.select(selectedViewNum);
                }
            }

            xDown = null;
            yDown = null;
        }
    }, false);
});