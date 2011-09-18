#include <Browser.h>
#include <Hw.h>
#include "action.h"



using namespace ZLX;

Browser App;

char FUSES[256];
int main() {
    
    Hw::SystemInit(INIT_SOUND|INIT_VIDEO|INIT_USB);
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