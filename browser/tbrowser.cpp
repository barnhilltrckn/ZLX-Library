#include <Browser.h>
#include "action.h"

ZLX::Browser App;

void BrowseriiAction(void *param) {
    printf("BrowseriiAction clicked\r\n");
    lpBrowserActionEntry _this = (lpBrowserActionEntry) param;
    _this->name = "Prout..";
}

int main() {


    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Browserii";
        action->action = BrowseriiAction;
        action->param = NULL;
        App.AddAction(action);
    }
    {
        lpBrowserActionEntry action = new BrowserActionEntry();
        action->name = "Browserii2";
        action->action = NULL;
        action->param = NULL;
        App.AddAction(action);
    }

    App.SetLaunchAction(ActionLaunchFile);
    App.Run("uda://ressources/");
}