import * as ws from "ws";

import * as log from "../logging/logging"

let _webSocketServer: ws.Server = null;
let _glbClientCnt: number = 0;

let _client: ws = null;

let i = 0;

export function main(webSocketServer: ws.Server) {
    log.info("Started!");

    _webSocketServer = webSocketServer;

    _webSocketServer.on("connection", (client: ws): void => {  
        client["customID"] = ++_glbClientCnt + "-" + _webSocketServer.clients.length + "-" + (new Date()).getTime();

        log.info("New Connection", { "client": client });

        client.on("message", (data: any, flags: any): void => {
            onMessage(client, data, flags);
        });
    });

    setInterval(() => {
        sendMessage("Hello Client!");

        if (_client != null && _client.readyState == ws.OPEN) {
            _client.send(JSON.stringify({"gui/footer/version": [1, 2, i++] }));
        }
    }, 1000);
}

function onMessage(client: ws, data: any, flags: any): void {
    log.info("New message from client", { "customID": client["customID"], "flags": flags, "data": data }); 

    _client = client;
}

function sendMessage(data: any): void {
    for (let client of _webSocketServer.clients) {
        client.send(data);
    }
}







