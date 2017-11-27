
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
(show if connected or not)
- Browse Dongles
- Diconnect from Device

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
        }
    }
    
    viewList.push(Configuration);
}