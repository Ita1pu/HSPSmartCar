
export enum Color {
    Gray = <any>"\x1B[37m",
    Green = <any>"\x1B[32m",
    Purple = <any>"\x1B[35m",
    Black = <any>"\x1B[38m",
    Red = <any>"\x1B[31m",
    Yellow = <any>"\x1B[33m",
    Blue = <any>"\x1B[34m",
    Cyan = <any>"\x1B[36m"
}

export enum Type {
    Debug = <any>"Debug",
    Info = <any>"Info",
    Warning = <any>"Warning",
    Error = <any>"Error",
}

export function debug(msg: string, attachment: any = null, color: Color = Color.Black) {
    log(Type.Debug, msg, attachment, color);
}

export function info(msg: string, attachment: any = null, color: Color = Color.Black) {
    log(Type.Info, msg, attachment, color);
}

export function warning(msg: string, attachment: any = null, color: Color = Color.Black) {
    log(Type.Warning, msg, attachment, color);
}

export function error(msg: string, attachment: any = null, color: Color = Color.Black) {
    log(Type.Error, msg, attachment, color);
}

function log(type: Type, msg: string, attachment: any = null, color: Color = Color.Black) {
    let outStr: string = "";
    outStr += Color.Gray.toString() + (new Date()).toISOString() + " ";

    if (type == Type.Debug)        
        outStr += Color.Purple.toString() + type + " ";
    else if (type == Type.Info)        
        outStr += Color.Blue.toString() + type + " ";
    else if (type == Type.Warning)
        outStr += Color.Yellow.toString() + type + " ";
    else if (type == Type.Error)
        outStr += Color.Red.toString() + type + " ";

    outStr += color.toString() + msg;   

    if (attachment != null) {
        outStr += "\n   " + Color.Gray.toString(); 
        let test = decycle(attachment);
        console.log(outStr, JSON.stringify(decycle(attachment)));
    }
    else {
        console.log(outStr);
    }    
}

function decycle(object: any): any {
    /*var objects = new Map();

    return (function derez(value: any, path: string): any {
        var old_path; 
        var nu: any;  

        if (typeof value === "object" && value !== null && !(value instanceof Boolean) && 
            !(value instanceof Date) && !(value instanceof Number) && 
            !(value instanceof RegExp) && !(value instanceof String)) {

            old_path = objects.get(value);

            if (old_path !== undefined) {
                return {$ref: old_path};
            }

            objects.set(value, path);


            if (value.buffer instanceof ArrayBuffer) { // TODO sauber proggen, alles in einer zeile bei debug console evtl mit json viewer
                nu = [];

                for (let i = 0; i < value.length; i++) {
                    nu[i] = derez(value[i], path + "[" + i + "]");   
                    if (i > 99)
                        break;
                }
            } else {
                nu = {};
                Object.keys(value).forEach(function (name) {
                    nu[name] = derez(
                        value[name],
                        path + "[" + JSON.stringify(name) + "]"
                    );
                });
            }
            return nu;
        }
        return value;
    }(object, "$"));*/
}