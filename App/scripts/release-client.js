var compileTypescript = require("./compile-typescript.js");
var compileSass = require("./compile-sass.js");
var releaseClient = require("./release-client.js");
var tsc = require("gulp-typescript");

var gulp = require('gulp');
var gutil = require('gulp-util');
var concat = require('gulp-concat');
var addsrc = require('gulp-add-src');
var replace = require('gulp-replace');
var rename = require("gulp-rename");
var uglifyjs = require('gulp-uglifyjs');
var jsObfuscator = require('gulp-js-obfuscator');
var insert = require('gulp-insert');
var gzip = require('gulp-gzip');

var fs = require("fs");

var varNames = [];
var counter = [0];

var rootNameStr = ["呦", "呧", "周", "呩", "呪", "呫", "呬", "呭", "呮", "呯", "呰", "呱", "呲", "味", "呴", "呵", "呶", "呷", "呸", 
"呹", "呺", "呻", "呼", "命", "呾", "呿", "咀", "咁", "咂", "咃", "咄", "咅", "咆", "咇", "咈", "咉", "咊", "咋", "和", "咍", "咎",
"咏", "咐", "咑", "咒", "咓", "咔", "咕", "咖", "咗", "咘", "咙", "咚", "咛", "咜", "咝", "咞", "咟", "咠", "咡", "咢", "咣", "咤", 
"咥", "咦", "咧", "咨", "咩", "咪", "咫", "咬", "咭", "咮", "咯", "咰", "咱", "咲", "咳", "咴", "咵", "咶", "咷", "咸", "咹", "咺", 
"咻", "咼", "咽", "咾", "咿", "哀", "品", "哂", "哃", "哄", "哅", "哆", "哇", "哈", "哉", "哊", "哋", "哌", "响", "哎", "哏", "哐", 
"哑", "哒", "哓", "哔", "哕", "哖", "哗", "哘", "哙", "哚", "哛", "哜", "哝", "哞", "哟", "哠", "員", "哢", "哣", "哤", "哥", "哦", 
"哧", "哨", "哩", "哪", "哫", "哬", "哭", "哮", "哯", "哰", "哱", "哲", "哳", "哴", "哵", "哶", "哷", "哸", "哹", "哺", "哻", "哼", 
"哽", "哾", "哿", "唀", "唁", "唂", "唃", "唄", "唅", "唆", "唇", "唈", "唉", "唊", "唋", "唌", "唍", "唎", "唏", "唐", "唑", "唒", 
"唓", "唔", "唕", "唖", "唗", "唘", "唙", "唚", "唛", "唜", "唝", "唞", "唟", "唠", "唡", "唢", "唣", "唤", "唥", "唦", "唧", "唨", 
"唩", "唪", "唫", "唬", "唭", "售", "唯", "唰", "唱", "唲", "唳", "唴", "唵", "唶", "唷", "唸", "唹", "唺", "唻", "唼", "唽", "唾", 
"唿", "啀", "啁", "啂", "啃", "啄", "啅", "商", "啇", "啈", "啉", "啊", "啋", "啌", "啍", "啎", "問", "啐", "啑", "啒", "啓", "啔", 
"啕", "啖", "啗", "啘", "啙", "啚", "啛", "啜", "啝", "啞", "啟", "啠", "啡", "啢", "啣", "啤", "啥", "啦", "啧", "啨", "啩", "啪", 
"啫", "啬", "啭", "啮", "啯", "啰", "啱", "啲", "啳", "啴", "啵", "啶", "啷", "啸", "啹", "啺", "啻", "啼", "啽", "啾", "啿", "喀", 
"喁", "喂", "喃", "善", "喅", "喆", "喇", "喈", "喉", "喊", "喋", "喌", "喍", "喎", "喏", "喐", "喑", "喒", "喓", "喔", "喕", "喖", 
"喗", "喘", "喙", "喚", "喛", "喜", "喝", "喞", "喟", "喠", "喡", "喢", "喣", "喤", "喥", "喦", "喧", "喨", "喩", "喪", "喫", "喬", 
"喭", "單", "喯", "喰", "喱", "喲", "喳", "喴", "喵", "営", "喷", "喸", "喹", "喺", "喻", "喼", "喽", "喾", "喿", "嗀", "嗁", "嗂", 
"嗃", "嗄", "嗅", "嗆", "嗇", "嗈", "嗉", "嗊", "嗋", "嗌", "嗍", "嗎", "嗏", "嗐", "嗑", "嗒", "嗓", "嗔", "嗕", "嗖", "嗗", "嗘", 
"嗙", "嗚", "嗛", "嗜", "嗝", "嗞", "嗟", "嗠", "嗡", "嗢", "嗣", "嗤", "嗥", "嗦", "嗧", "嗨", "嗩", "嗪", "嗫", "嗬", "嗭", "嗮", 
"嗯", "嗰", "嗱", "嗲", "嗳", "嗴", "嗵", "嗶", "嗷", "嗸", "嗹", "嗺", "嗻", "嗼", "嗽", "嗾", "嗿", "嘀", "嘁", "嘂", "嘃", "嘄", 
"嘅", "嘆", "嘇", "嘈", "嘉", "嘊", "嘋", "嘌", "嘍", "嘎", "嘏", "嘐", "嘑", "嘒", "嘓", "嘔", "嘕", "嘖", "嘗", "嘘", "嘙", "嘚", 
"嘛", "嘜", "嘝", "嘞", "嘟", "嘠", "嘡", "嘢", "嘣", "嘤", "嘥", "嘦", "嘧", "嘨", "嘩", "嘪", "嘫", "嘬", "嘭", "嘮", "嘯", "嘰", 
"嘱", "嘲", "嘳", "嘴", "嘵", "嘶", "嘷", "嘸", "嘹", "嘺", "嘻", "嘼", "嘽", "嘾", "嘿", "噀", "噁", "噂", "噃", "噄", "噅", "噆", 
"噇", "噈", "噉", "噊", "噋", "噌", "噍", "噎", "噏", "噐", "噑", "噒", "噓", "噔", "噕", "噖", "噗", "噘", "噙", "噚", "噛", "噜", 
"噝", "噞", "噟", "噠", "噡", "噢", "噣", "噤", "噥", "噦", "噧", "器", "噩", "噪", "噫", "噬", "噭", "噮", "噯", "噰", "噱", "噲", 
"噳", "噴", "噵", "噶", "噷", "噸", "噹", "噺", "噻", "噼", "噽", "噾", "噿", "嚀", "嚁", "嚂", "嚃", "嚄", "嚅", "嚆", "嚇", "嚈", 
"嚉", "嚊", "嚋", "嚌", "嚍", "嚎", "嚏", "嚐", "嚑", "嚒", "嚓", "嚔", "嚕", "嚖", "嚗", "嚘", "嚙", "嚚", "嚛", "嚜", "嚝", "嚞", 
"嚟", "嚠", "嚡", "嚢", "嚣", "嚤", "嚥", "嚦", "嚧", "嚨", "嚩", "嚪", "嚫", "嚬", "嚭", "嚮", "嚯", "嚰", "嚱", "嚲", "嚳", "嚴", 
"嚵", "嚶", "嚷", "嚸", "嚹", "嚺", "嚻", "嚼", "嚽", "嚾", "嚿", "囀", "囁", "囂", "囃", "囄", "囅", "囆", "囇", "囈", "囉", "囊", 
"囋", "囌", "囍", "囎", "囏", "囐", "囑", "囒", "囓", "囔", "囕", "囖"];
//var followNameStr = ["̐", "̓", "̾", "͍", "ͩ", "̘", "̱", "͎"];
followNameStr = rootNameStr;        

function getVarName(input) {  
    if (typeof(varNames[input]) != "undefined")
            return varNames[input]; 

    var str = rootNameStr[counter[0]];
    for (var i = 1; i < counter.length; i++)
        str += followNameStr[counter[i]];
    
    var increased = false;
    for(var i = 0; i < counter.length; i++) {
        if (counter[i] < rootNameStr.length - 1) {
            counter[i]++;
            increased = true;
            break;
        }
        else { counter[i] = 0; }
    }
    
    if (increased == false)
        counter.push(0);

    varNames[input] = str;

    return varNames[input]; 
}

function renameVariables(callback = null) {
    var stream = gulp.src(['src/client/variables.js']);

    stream = stream.pipe(replace(/[:][ ]["][^"]*["]/g, (input) => 
        { return ": \"" + getVarName(input.split("\"")[1]) + "\""; })) 

    stream = stream.pipe(gulp.dest((f) => { return f.base; }));

    varNames = [];
    counter = [0];

    if (callback != null)
        stream.on("end", () => { callback(); });
}

function obfuscate(callback = null) {
    var stream = gulp.src([]);

    var scriptContent  = fs.readFileSync("src/client/script.js", "utf8");
    var split = scriptContent.split("\"");

    for (var i = 3; i < split.length - 1; i+=2) {
        if (split[i] != "variables")
            stream = stream.pipe(addsrc.append('src/client/' + split[i] + '.ts'));
    }

    var tsProject = tsc.createProject("tsconfig.json", { "outFile": "bundle.js" });  
    stream = stream.pipe(tsProject());

    stream = stream.pipe(addsrc.prepend('src/client/variables.js'))   
    stream = stream.pipe(addsrc.prepend('node_modules/jquery/dist/jquery.min.js'))   
    stream = stream.pipe(addsrc.prepend('src/client/authentication/cryptojs.js'))   
    stream = stream.pipe(concat('script.js'));    

    stream = stream.pipe(insert.prepend('(function () {'));
    stream = stream.pipe(insert.append('})();'));

    stream = stream.pipe(jsObfuscator({
        compact: false,
        disableConsoleOutput: true,
        rotateStringArray: true,
        selfDefending: true,
        stringArray: true,
        unicodeEscapeSequence: true
    }));

    stream = stream.pipe(uglifyjs('script.js', { mangle: false }));

    stream = stream.pipe(replace(/_0x[0123456789abcdefx]*/g, (input) => { return getVarName(input); }));

    stream = stream.pipe(gulp.dest("build/tmp/client"));

    if (callback != null)
        stream.on("end", () => { callback(); });
}

function merge(callback = null) {
    var stream = gulp.src(['build/tmp/client/script.js'])

    stream = stream.pipe(insert.prepend('<!DOCTYPE html><html xmlns="http://www.w3.org/1999/xhtml"><head><title>HSPSmartCar</title><meta http-equiv="Content-Type" content="text/html;charset=UTF-8" /><meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" /><script type="text/javascript">'))
    stream = stream.pipe(insert.append('</script><style type="text/css">'))
    stream = stream.pipe(addsrc.append('src/client/style.css')) 
    stream = stream.pipe(concat('client.html'))                                   
    stream = stream.pipe(insert.append('</style></head><body></body></html>'))
    stream = stream.pipe(gulp.dest("build/"))
    stream = stream.pipe(gzip())
    stream = stream.pipe(rename({ extname: ".gz" }))

    stream = stream.pipe(gulp.dest("build/"))

    if (callback != null)
        stream.on("end", () => { 
            var scriptStream = gulp.src(['build/tmp/client/script.js']);
            scriptStream = scriptStream.pipe(gulp.dest("build/"));

            scriptStream.on("end", () => { 
                var styleStream = gulp.src(['src/client/style.css']);
                styleStream = styleStream.pipe(gulp.dest("build/"));

                styleStream.on("end", () => { 
                    callback();
                });
            });            
         });
}

module.exports = (callback = null) => {  
    gutil.log('Compile typescript...');
    compileTypescript(() => { 
        gutil.log('Rename variables...');
        renameVariables(() => { 
            gutil.log('Gernerate sass variables...');
            compileSass.generateVariables(() => { 
                gutil.log('Obfuscate...');
                obfuscate(() => {
                    gutil.log('Compile sass...');
                    compileSass.release(() => {
                        gutil.log('Merge files...');
                        merge(() => {
                            if (callback != null)
                                callback();
                        });
                    }); 
                }); 
            });
        });
    });
}