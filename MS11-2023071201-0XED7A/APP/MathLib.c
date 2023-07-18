/*********************************************************************************************
 * License: Copyright (C) 2022 Genbyte Technology Inc Limited Company. All Rights Reserved.
 * FilePath: \SOFT\APP\MathLib.c
 * Company: GenByte
 * Author: Aescn.Pan
 * Date: 2023-03-06 09:01:49
 * LastEditors: Please set LastEditors
 * LastEditTime: 2023-05-10 09:18:14
 * Description: 
 *********************************************************************************************/

#define  _MATH_LIB_SRC_

#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
#include "Typedef.h"
#include "MathLib.h"
#include "Main_App.h"
#include "RLIN_driver.h"
#include "SystemCtrl.h"
#include "Uart.h"

#if  API_MATH_MULU16
U32  MULU16( U16 a, U16 b )//a*b
{
    U32 MDL,MDH;
    U32 MDB;
    DWORD reg;

    MDL = a;
    MDH = b;
    MDB = ( U32 )( MDL*MDH );
    __NOP_X2( );
    reg.W.H = ( U16 )( MDB >> 16 );
    reg.W.L = ( U16 )( MDB );
    return reg.D;
}
#endif

#if  API_MATH_DIVU32
U16  DIVU32( U32 a, U16 b )//a/b
{
    U32 MDA;
    U16 MDB,MDC;

    MDA = a;
    MDB = b;
    MDC = ( U16 )( MDA/MDB );
    __NOP_X2( );
    return MDC;
}
#endif

void Delay( void )
{
    U8 cnt = 10;

    while( cnt-- )
    {
        __NOP_X5( );
        __NOP_X5( );
        __NOP_X5( );
        __NOP_X5( );
    }
}

void MCU_Sleep( void )
{
    R_TAU0_Channel0_Stop( );
    R_TAU0_Channel2_Stop( );
    R_TAU0_Channel3_Stop( );
    R_UART0_Stop( );
    R_ADC_Stop( );
    ADCE = 0;
    COOL_OFF( );
    HEAT_OFF( );
    DMOS_OFF( );
    LAMP_OFF( );
    __NOP_X3( );
    LCUC0 = 0x01;
    EGP1 &= ~0X08;
    EGN1 |= 0X08;
    LIN_OFF( );
    PER2  &= ~0x04;
    __NOP_X3( );
    HALL_OFF( );
    NTC_OFF( );
    VBAT_OFF( );
    __WDTC( );
    __NOP_X3( );
    u8SleepOrder = FALSE;
    __STOP( );
    Delay( );
    if( !P1.4 ) u8WakeUpSta = 1;
}

void MCU_WakeUp( void )
{
    EGP1 &= ~0x08;
    EGN1 &= ~0x08;
    RLIN_Slave_Init( );
    LIN_ON( );
    HALL_ON( );
    NTC_ON( );
    VBAT_ON( );

    ADCE = 1;
    
    R_TAU0_Channel0_Start( );
    R_TAU0_Channel2_Start( );
    R_TAU0_Channel3_Start( );
    MyUART_Init( );
}

void Refresh( void )
{
    if( u8CoolOn ) COOL_ON( );
    else COOL_OFF( );

    if( u8HeatOn ) HEAT_ON( );
    else HEAT_OFF( );

    if( u8DmosOn ) DMOS_ON( );
    else DMOS_OFF( );

    if( u8InFanOn ) IFAN_ON( );
    else IFAN_OFF( );

    if( u8OutFanOn ) OFAN_ON( );
    else OFAN_OFF( );

    if( u8LampOn ) LAMP_ON( );
    else LAMP_OFF( );
}

