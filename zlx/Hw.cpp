/* 
 * File:   Hw.cpp
 * Author: cc
 *
 * Created on 17 septembre 2011, 13:35
 */


#include <debug.h>
#include <xenos/xenos.h>

#include <diskio/ata.h>
extern "C" {
#include <input/input.h>
#include <xenon_nand/xenon_sfcx.h>
#include <network/network.h>
#include <ppc/timebase.h>
#include <time/time.h>
}
#include <console/console.h>
#include <diskio/diskio.h>
#include <usb/usbmain.h>
#include <time/time.h>
#include <xenon_soc/xenon_power.h>
#include <xenon_smc/xenon_smc.h>

#include "Hw.h"
#include "posix/input.h"

namespace ZLX 
{
    static int init_flags = 0;
    
    void Hw::SystemInit(int flags){
        if(init_flags==0){
            xenon_make_it_faster(XENON_SPEED_FULL);
            
            if(flags & INIT_SOUND){
                
            }
            if(flags & INIT_VIDEO){
                
            }
            if(flags & INIT_USB){
                usb_init();
            }
            if(flags & INIT_NETWORK){
                network_init();
            }
            
            
            sfcx_init();
            init_flags = flags;
        }
    }
    void Hw::SystemPool(){
        if(init_flags & INIT_USB){
            usb_do_poll();
        }
        if(init_flags & INIT_NETWORK){
            network_poll();
        }
    }
}

