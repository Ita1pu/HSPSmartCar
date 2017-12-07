namespace Logging {
    export class ErrorLogEntry {
        private time: Date;
        private msg: string[];
        private url: string | undefined;
        private line: number | undefined;
        private col: number | undefined;
        private error: Error | undefined;

        private static readonly invalidXmlCharsPattern = 
            /[\u0000-\u0008\u000b\u000c\u000e-\u001f\ufffe\uffff]/g;

        private static readonly entityMap: {
            [index: string]: string
        } = {
            "&": "&amp;",
            "<": "&lt;",
            ">": "&gt;",
            '"': '&#34;',
            "'": '&#39;'
        };

        constructor(msg: string[], url?: string, line?: number, col?: number, error?: Error) {
            this.time = new Date();
            this.msg = msg;
            this.url = url;
            this.line = line;
            this.col = col;
            this.error = error;
        }

        public getHtmlMessage(): string {
            try {
                let msg = "Time: " + this.escapeXml(this.time!.toString());

                if (this.msg != null)
                {
                    for (let i = 0; i < this.msg.length; i++) {
                        if (i == 0) {
                            msg += "<br />Message: " + this.escapeXml(this.msg[i])
                        }
                        else {
                            msg += "<br />" + this.escapeXml(this.msg[i])
                        }
                        
                    }
                }

                if (this.line != null || this.col != null)
                    msg += "<br />Line: " + this.line + " Col: " + this.col;

                if (this.error != null)
                    msg += "<br />Error: " + this.escapeXml(this.error && this.error.message);

                return msg;
            }
            catch (e) {
                return "[E] " + this.escapeXml(e);
            }
        }

        public getMessage(): string[] {
            return this.msg;
        }

        private escapeXml(string: string | null | undefined): string | null | undefined {
            // First, replace invalid XML 1.0 characters
            // then replace special XML characters.
            return typeof string == "undefined" || string === null ? string
                : string.replace(ErrorLogEntry.invalidXmlCharsPattern, " ")
                    .replace(/[&<>"']/g, s => ErrorLogEntry.entityMap[s]);
        }
    }

    let errors : ErrorLogEntry[] = [];

    export function push(error: ErrorLogEntry | string) {  
        if (Settings.isDebug)  
            console.log(error);   

        if (typeof error == "string") {
            errors.push(new Logging.ErrorLogEntry([ error ]));
        }
        else {
            errors.push(error);
        }
    }
}