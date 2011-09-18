/* 
 * File:   Hw.h
 * Author: cc
 *
 * Created on 17 septembre 2011, 13:36
 */

#ifndef HW_H
#define	HW_H

#include "zlx.h"

namespace ZLX 
{
    enum HW_INIT_FLAGS{
        INIT_SOUND = 0x1,
        INIT_VIDEO = 0x2,
        INIT_USB = 0x4,
        INIT_NETWORK = 0x8
    };

    class Hw{
    public:
        static void SystemInit(int flags);
        static void SystemPoll();
    };
}

#endif	/* HW_H */

