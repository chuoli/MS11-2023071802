/*********************************************************************************************
 * License: Copyright (C) 2022 Genbyte Technology Inc Limited Company. All Rights Reserved.
 * FilePath: \SOFT\APP\Main_App.c
 * Company: GenByte
 * Author: Aescn.Pan
 * Date: 2023-03-06 09:01:49
 * LastEditors: Please set LastEditors
 * LastEditTime: 2023-05-26 15:31:48
 * Description: 
 *********************************************************************************************/

#define Main_App_SRC_

#include "Main_App.h"
#include "MyTimer.h"
#include "RLIN_driver.h"
#include "AD_NTC.h"
#include "SystemCtrl.h"
#include "Check.h"
#include "Uart.h"
#include "MathLib.h"


void App_Init( void )
{
    __DI( );
    ResetSta = RESF;
    while( ResetSta & 0x10 );//If reset by the watchdog, not run;
    Timer_Init( );
    AD_NTC_Init( );
    RLIN_Slave_Init( );
    MyUART_Init( );
    Data_Init( );
    __EI( );
}

void App_Run( void )
{
    __WDTC( );
    if( flagT.T1MS )
    {
        flagT.T1MS = 0;
        Voltage_Check( );
        Fan_Ctrl( );
    }
    if( flagT.T10MS )
    {
        flagT.T10MS = 0;
        NTC_Check( );
        DoorCheck( );
        FanErrCheck( );
        Refresh( );
    }
    if( flagT.T100MS )
    {
        flagT.T100MS = 0;
        CommunicationFun( );
    }
    if( flagT.T500MS )
    {
        flagT.T500MS = 0;
        SystemCtrl( );
        sysTimer( );
        LinDataRefreshFun( );
    }
    #if 1
    if( u8SysWorkSta == SYS_WORK_STOP )
    {
        MCU_Sleep( );
        __NOP_X5( );
        if( u8WakeUpSta )
        {
            MCU_WakeUp( );
            u8WakeUpSta = 0;
        }
        SystemCtrl( );
    }
    #endif
}

