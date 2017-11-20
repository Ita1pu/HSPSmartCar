var gulp = require("gulp");
var tsc = require("gulp-typescript");
var sourcemaps = require("gulp-sourcemaps");

var path = require("path");

const tsProject = tsc.createProject("tsconfig.json");

module.exports = (callback = null) => { 
    var stream = gulp.src(["src/**/*.ts"]);

    stream = stream.pipe(sourcemaps.init());
    
    stream = stream.pipe(tsProject());

    stream = stream.pipe(sourcemaps.write({
        // Return relative source map root directories per file.
        sourceRoot: function (file) {
            var sourceFile = path.join(file.cwd, file.sourceMap.file);
            return path.relative(path.dirname(sourceFile), file.cwd);
        }
    }))

    stream = stream.pipe(gulp.dest((f) => { return f.base; }));

    if (callback != null)
        stream.on("end", () => { callback(); });
}