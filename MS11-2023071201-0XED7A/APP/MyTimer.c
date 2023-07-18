/*********************************************************************************************
 * License: Copyright (C) 2022 Genbyte Technology Inc Limited Company. All Rights Reserved.
 * FilePath: \SOFT\APP\MyTimer.c
 * Company: GenByte
 * Author: Aescn.Pan
 * Date: 2023-03-06 09:01:49
 * LastEditors: Please set LastEditors
 * LastEditTime: 2023-07-08 22:06:05
 * Description: 
 *********************************************************************************************/

#define MyTimer_SRC_

#include "r_cg_timer.h"
#include "Typedef.h"
#include "MyTimer.h"
#include "SystemCtrl.h"
#include "Check.h"

void Timer_Init( void )
{
    R_TAU0_Channel0_Start( );
    R_TAU0_Channel2_Start( );
    R_TAU0_Channel3_Start( );
}

void Timer2_Interrupt( void )//1ms
{
    static U8 t10ms = 0;
    
    flagT.T1MS = 1;
    if( ++t10ms >= 10U )
    {
        static U8 t50ms = 0;
        static U8 t100ms = 0;
        static U8 t500ms = 0;

        t10ms = 0;
        flagT.T10MS = 1;
        if( ++t50ms >= 5U )
        {
            t50ms = 0;
            flagT.T50MS = 1;
        }
        if( ++t100ms >= 10U )
        {
            t100ms = 0;
            flagT.T100MS = 1;
        }
        if( ++t500ms >= 50U )
        {
            t500ms = 0;
            flagT.T500MS = 1;
        }
    }
}

void Timer3_Interrupt( void )//200us
{
    FanSpeedCheck( );
}

void sysTimer( void )
{
    if( flagT.T1S )
    {
        flagT.T1S = 0;
        DECEX( u16CoolOffTime );
        DECEX( u16HeatOffTime );
        INCEX( u16PowerLinTime );
        INCEX( u16ModeLinTime );
        INCEX( u16TempLinTime );
        DECEX( u8DiagTime );
    }
    else
    {
        flagT.T1S = 1;
    }
}

