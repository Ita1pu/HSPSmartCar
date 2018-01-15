namespace View {
    export class ConfigurationAppSettings {
        private control: JQuery

        public constructor(control: JQuery) {
            this.control = control;

            $("<div>", { class: Var.Style.View.Configuration.header })
                .text(Strings.View.Configuration.Header.appSettings).appendTo(this.control);

            let velocityUnit = $("<div>", { class: Var.Style.View.Configuration.switch })
                .appendTo(this.control);

            let velocityUnitMpH = $("<div>")
                .text(Strings.View.Configuration.Switch.VelocityUnit.MpH).appendTo(velocityUnit);

            let velocityUnitKmH = $("<div>")
                .text(Strings.View.Configuration.Switch.VelocityUnit.KmH).appendTo(velocityUnit);

            let swipeHelp = $("<div>", { class: Var.Style.View.Configuration.switch })
                .appendTo(this.control);

            let swipeHelpDisable = $("<div>")
                .text(Strings.View.Configuration.Switch.SwipeHelp.disbale).appendTo(swipeHelp);

            let swipeHelpEnable = $("<div>")
                .text(Strings.View.Configuration.Switch.SwipeHelp.enable).appendTo(swipeHelp);

            let velocityUnitVal = Store.get(Settings.Store.velocityUnit);
            if (velocityUnitVal == null || velocityUnitVal == Settings.Store.VelocityUnit.KmH) {
                velocityUnitKmH.addClass(Var.Style.View.Configuration.Switch.enabled);
            }
            else {
                velocityUnitMpH.addClass(Var.Style.View.Configuration.Switch.enabled);
            }

            let swipeHelpVal = Store.get(Settings.Store.showSwipeHelp);
            if (swipeHelpVal == null || swipeHelpVal == Settings.Store.ShowSwipeHelp.true) {
                swipeHelpEnable.addClass(Var.Style.View.Configuration.Switch.enabled);
            }
            else {
                swipeHelpDisable.addClass(Var.Style.View.Configuration.Switch.enabled);
            }

            let onVelocityUnit = () => {
                let velocityUnitVal = Store.get(Settings.Store.velocityUnit);
                if (velocityUnitVal == null || velocityUnitVal == Settings.Store.VelocityUnit.KmH) {
                    Store.set(Settings.Store.velocityUnit, Settings.Store.VelocityUnit.MpH);

                    velocityUnitKmH.removeClass(Var.Style.View.Configuration.Switch.enabled);
                    velocityUnitMpH.addClass(Var.Style.View.Configuration.Switch.enabled);
                }
                else {
                    Store.set(Settings.Store.velocityUnit, Settings.Store.VelocityUnit.KmH);

                    velocityUnitKmH.addClass(Var.Style.View.Configuration.Switch.enabled);
                    velocityUnitMpH.removeClass(Var.Style.View.Configuration.Switch.enabled);
                }
            };
            velocityUnitMpH.click(onVelocityUnit);
            velocityUnitKmH.click(onVelocityUnit);

            let onSwipeHelp = () => {
                let swipeHelpVal = Store.get(Settings.Store.showSwipeHelp);
                if (swipeHelpVal == null || swipeHelpVal == Settings.Store.ShowSwipeHelp.true) {
                    Store.set(Settings.Store.showSwipeHelp, Settings.Store.ShowSwipeHelp.false);

                    swipeHelpEnable.removeClass(Var.Style.View.Configuration.Switch.enabled);
                    swipeHelpDisable.addClass(Var.Style.View.Configuration.Switch.enabled);
                }
                else {
                    Store.set(Settings.Store.showSwipeHelp, Settings.Store.ShowSwipeHelp.true);

                    swipeHelpEnable.addClass(Var.Style.View.Configuration.Switch.enabled);
                    swipeHelpDisable.removeClass(Var.Style.View.Configuration.Switch.enabled);
                }
            };
            swipeHelpEnable.click(onSwipeHelp);
            swipeHelpDisable.click(onSwipeHelp);
        }
    }
}