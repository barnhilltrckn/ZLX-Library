#include <Browser.h>
#include "action.h"
extern "C"{
#include <xenon_nand/xenon_sfcx.h>
#include <xenon_soc/xenon_power.h>
}
ZLX::Browser App;


char FUSES[256];
int main() {
    xenon_make_it_faster(XENON_SPEED_FULL);
    sfcx_init();
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Start HTTPD";
        action->action = ActionStartHttpd;
        action->param = NULL;
        App.AddAction(action);
    }
    
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Return to xell";
        action->action = ActionReturnToXell;
        action->param = NULL;
        App.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Boot tftp";
        action->action = ActionBootTFTP;
        action->param = NULL;
        App.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Dump nand";
        action->action = ActionDumpNand;
        action->param = NULL;
        App.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Shutdown";
        action->action = ActionShutdown;
        action->param = NULL;
        App.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Restart";
        action->action = ActionRestart;
        action->param = NULL;
        App.AddAction(action);
    }

    App.SetLaunchAction(ActionLaunchFile);
    App.Run("uda://ressources/");
}