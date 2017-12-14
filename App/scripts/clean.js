var gulp = require("gulp");
var clean = require("gulp-clean");

module.exports = (callback = null) => {
    var stream = gulp.src(
        ["src/**/*.js", "src/**/*.css", "src/client/variables.scss", "build"], 
        {read: false});

    if (callback != null)
        stream.on("end", () => { callback(); });

    stream = stream.pipe(clean({force: true}));    
}