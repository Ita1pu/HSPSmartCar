var gulp = require('gulp');
var foreach = require('gulp-foreach');
var gutil = require('gulp-util');
var fs  = require('fs');

var clean = require("./clean.js");
var compileTypescript = require("./compile-typescript.js");
var compileSass = require("./compile-sass.js");
var releaseClient = require("./release-client.js");

gulp.task('Clean', (callback) => { clean(callback); });

gulp.task('Debug', (callback) => {
    compileTypescript(() => {
        compileSass.generateVariables(() => {        
            compileSass.compile(() => { 
                callback();
            });
        });
    });
});

gulp.task('Watch', ['Debug'], () => {
    gulp.watch(['src/**/*.ts'], (file) => {
        console.log("File changed: " + file.path + "...");

        if (file.path.endsWith('variables.ts')) {
            compileTypescript(() => {
                compileSass.generateVariables(() => {                
                    compileSass.compile(() => {
                        console.log("Compile done!");
                    });
                });                
            });
        }
        else {
            compileTypescript(() => {
                console.log("Compile done!");
            });
        }
    });

    gulp.watch(['src/client/**/*.scss'], (file) => {
        if (file.path.endsWith('variables.ts') == false) {
            console.log("File changed: " + file.path + "...");

            compileSass.compile(() =>  {
                console.log("Compile done!");
            });
        }
    });
});

gulp.task('Release', (callback) => { 
    var stream = gulp.src(['src/**/*.ts']);

    var hasChanges = false;

    stream = stream.pipe(foreach((stream, file) => {
        try {
            var tsStats = fs.statSync(file.path);
            var jsStats = fs.statSync(file.path.substring(0, file.path.length - 2) + "js");
            
            if (tsStats.mtime > jsStats.mtime) {
                gutil.log("File has been changed: " + file.path);
                hasChanges = true;
            }
        }
        catch(ex) {
            gutil.log("File was not compiled: " + file.path);
            hasChanges = true;
        }

        return stream;
    }));
    
    stream.on("end", () => {
        if (hasChanges) {
            releaseClient(() => {
                gutil.log("Successfully completed!");
            });    
        } 
        else {
            gutil.log("Nothing to do!");
        }
    });

    return stream;
});
