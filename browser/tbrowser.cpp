#include <Browser.h>
#include "action.h"
extern "C"{
#include <xenon_nand/xenon_sfcx.h>
void enet_quiesce();
}
ZLX::Browser App;

int main() {

    sfcx_init();
    enet_quiesce();
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