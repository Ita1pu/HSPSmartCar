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

gulp.task('ReleaseClient', (callback) => { 
	releaseClient(() => {
		gutil.log("Successfully completed!");
	});
});

gulp.task('Release', (callback) => { 
	releaseClient(() => {
		gulp.src('./build/script.js')
			.pipe(gulp.dest('./www/'));
			
		gulp.src('./build/style.css')
			.pipe(gulp.dest('./www/'));
		
		gutil.log("Successfully completed!");
	});
});
