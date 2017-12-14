/// <reference path="../typings/index.d.ts" />

import * as path from "path";
import * as express from "express";
import * as http from "http";
import * as ws from "ws";

import * as log from "./logging/logging"
import * as server from "./server/server";

let app = express();

app.use((req: express.Request, res: express.Response, next: express.NextFunction) => {
    log.info("Express served static", 
        { "method": req.method, "orginalUrl": req.originalUrl, "query": req.query }, 
        log.Color.Green);

    next();
},
express.static(path.join(__dirname, '../')),
(req: express.Request, res: express.Response, next: express.NextFunction) => {
    log.warning("Express static service caused an page not found error.",
        { "req": req, "res": res }, log.Color.Green);
        
    res.status(404).json("Page not found.");

    next();
},
(err: any, req: express.Request, res: express.Response, next: express.NextFunction) => {
    log.error("Express static service caused an internal server error.",
        {"err": err, "req": req, "res": res }, log.Color.Green);

    res.status(500).json("Internal server error.");
});

let httpServer: http.Server = app.listen(80);

let wss = new ws.Server({ server: httpServer });
server.main(wss);






