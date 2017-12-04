
/*

Configuration

Data Trasfer
- Download 
- Upload
- Clear Buffer

App Settings
- MpH or KmH
- Enable / Disable Swipe Help

Connection to Dongle
(show if connected or not and show current mac address)
- Browse Dongles
- Diconnect from Device
// if connect to new dongle abfrage to disconnect form old device
// if disconnect form old divice and buffer not empty abfrage that all old data will be whiped
//    if user dont want that he needs to upload the buffered data first

Connetion to Backend
(show if parameters are right or not)
- Test Connection to Backend
- URL to Backend
- Login Parameter (User/Password/Token for Backend)
- Disconnect / Logout form Backend

Logging (Default eingeklappt)
- einfache Textbox mit lognachrichten

*/

namespace View {
    export class Configuration extends View {         
        public constructor(control: JQuery) {
            super(control);

            this.control.text("CONFIGURATION!!!!");

            //DisplayFeature.errorPanel.showError("Hello Error!");
        }
    }
    
    viewList.push(Configuration);
}