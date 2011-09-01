/**
 Make a simple console like app ...
**/
#include <Console.h>
#include <Draw.h>

#ifdef LIBXENON

#include <debug.h>
#include <xenos/xenos.h>

#include <diskio/ata.h>
#include <diskio/dvd.h>
extern "C" {
#include <input/input.h>
}
#include <console/console.h>
#include <diskio/diskio.h>
#include <usb/usbmain.h>
#include <time/time.h>
#include <xenon_soc/xenon_power.h>
#include <dirent.h>
#endif


int main() {
#ifdef LIBXENON
    usb_init();
    usb_do_poll();
    xenon_ata_init();
    dvd_init();
#else
    ZLX::Console App;
#endif
#ifdef WIN32
    App.Create("font\\font", 0xFF0000FF, 0xFFFFFF00);
#else
    App.Create("game:\\Verdana_16", 0xFF0000FF, 0xFFFFFF00);
#endif
    App.Format("LZX Pre 0.1");
    App.Format("Ced2911 %s", "dddd");
    App.Format("Test v...");

    while (1) {
    }

    return 0;
}
