namespace View {
    export class Configuration extends View {  
        private configurationData: ConfigurationData = null;
        private configurationDongle: ConfigurationDongle = null;
        private configurationBackground: ConfigurationBackground = null;
        private configurationAppSettings: ConfigurationAppSettings = null;
        private configurationLogging: ConfigurationLogging = null;
        
        public constructor(control: JQuery) {
            super(control);

            this.control.addClass(Var.Style.View.configuration);     
            
            this.configurationData = new ConfigurationData(this.control);
            this.configurationDongle = new ConfigurationDongle(this.control);
            this.configurationBackground = new ConfigurationBackground(this.control);
            this.configurationAppSettings = new ConfigurationAppSettings(this.control);
            this.configurationLogging = new ConfigurationLogging(this.control);
        }
    }
    
    viewList.push(Configuration);
}