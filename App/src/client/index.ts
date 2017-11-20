/// <reference path="../../typings/index.d.ts" />

// TODO Test compatibility of browers (e.g. IE)

window.onerror = (msg: string, url: string, line: number, col: number, error: Error) => {
    //errors.push(new AppErrorLogEntry([ msg ], url, line, col, error));

    //GUI.snackbar.showTooltip(msg, GUI.SnackbarCategory.Error);

    // Prevent firing the default error handler
    return !Settings.isDebug;
};

$(() => {
    let test = $("<div>").appendTo($(document.body)); 

    test.text("Hello World");
});