#define _Check_SRC

#include "Main_App.h"
#include <stdio.h>
#include <string.h>
#include "Typedef.h"
#include "MathLib.h"
#include "AD_NTC.h"
#include "SystemCtrl.h"
#include "Uart.h"
#include "Check.h"

void SelfVerify(void)
{
    U16 vbat,vcc,vfan,im;
    S8 temp,temp2;
    int u8datasize;
    U8 ifanSta,ofanSta,u8dataget[ SIZE_BUF_TX ];
    
    if( u8SysWorkSta != SYS_WORK_TEST )
    {
        u16SelfVerifyStep = FALSE;
        return;
    }

    if( !u8DmosOn )
    {
        u16ImStopCurrent = IM_GetCurrent();
    }
    if( IM_GetCurrent() < u16ImStopCurrent ) u16LoadCurrent = 0;
    else u16LoadCurrent = IM_GetCurrent() - u16ImStopCurrent;
    
    u8LampOn = FALSE;
    u8CoolOn = FALSE;
    u8HeatOn = FALSE;
    u8DmosOn = FALSE;
    u8InFanOn = FALSE;
    u8OutFanOn = FALSE;

    if( u16SelfVerifyStep == 1 )
    {
        u8LampOn = TRUE;
        FAN_DRV_DUTY( PWM_STOP );
    }
    else if( u16SelfVerifyStep == 2 )
    {
        u8InFanOn = TRUE;
        FAN_DRV_DUTY( PWM_80_CYCLE );
        if( u16InFanCnt >= 20 ) ifanSta = TRUE;
        else ifanSta = FALSE;
    }
    else if( u16SelfVerifyStep == 3 )
    {
        u8OutFanOn = TRUE;
        FAN_DRV_DUTY( PWM_80_CYCLE );
        if( u16OutFanCnt >= 20 ) ofanSta = TRUE;
        else ofanSta = FALSE;
    }
    else if( u16SelfVerifyStep == 4 )
    {
        u8OutFanOn = TRUE;
        FAN_DRV_DUTY( PWM_80_CYCLE );
        vfan = VF_GetVoltage();
    }
    else if( u16SelfVerifyStep == 5 )
    {
        u8CoolOn = TRUE;
        u8DmosOn = TRUE;
        FAN_DRV_DUTY( PWM_STOP );
    }
    else if( u16SelfVerifyStep == 6 )
    {
        u8HeatOn = TRUE;
        u8DmosOn = TRUE;
        FAN_DRV_DUTY( PWM_STOP );
    }
    else if( u16SelfVerifyStep == 7 )
    {
        u8HeatOn = TRUE;
        u8DmosOn = TRUE;
        FAN_DRV_DUTY( PWM_STOP );
        im = u16LoadCurrent;
    }
    else if( u16SelfVerifyStep == 8 )
    {
        FAN_DRV_DUTY( PWM_STOP );
        vbat = BAT_GetVoltage();
    }
    else if( u16SelfVerifyStep == 9 )
    {
        FAN_DRV_DUTY( PWM_STOP );
        temp = (S8)(Temp.val/256);
    }
    else if( u16SelfVerifyStep == 10 )
    {
        FAN_DRV_DUTY( PWM_STOP );
        temp2 = (S8)(Temp2.val/256);
    }
    else if( u16SelfVerifyStep == 13 )
    {
        FAN_DRV_DUTY( PWM_STOP );
        u8dataget[0] = SerialNumber[0];
        u8dataget[1] = SerialNumber[1];
        u8dataget[2] = SerialNumber[2];
        u8dataget[3] = SerialNumber[3];
        u8dataget[4] = SerialNumber[7];
        u8dataget[5] = SerialNumber[6];
        u8dataget[6] = SerialNumber[5];
        u8dataget[7] = SerialNumber[4];
    }
    else if( u16SelfVerifyStep == 14 )
    {
        FAN_DRV_DUTY( PWM_STOP );
        u8SleepOrder = TRUE;
        u8SysWorkSta = SYS_WORK_STOP;
    }
    else
    {
        FAN_DRV_DUTY( PWM_STOP );
    }

    switch( u16SelfVerifyStep )
    {
        case 0://GO STEP:-N|00
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:MS11_V1.7",(int)u16SelfVerifyStep);
            break;
        case 1://GO STEP:-O|01
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:LAMP - ON",
                        (int)u16SelfVerifyStep );
            break;
        case 2:////GO STEP:-P|02
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:INFAN - %d",
                        (int)u16SelfVerifyStep, (int)ifanSta );
            break;
        case 3://GO STEP:-Q|03
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:OUTFAN - %d",
                        (int)u16SelfVerifyStep, (int)ofanSta );
            break;
        case 4://GO STEP:04
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:VFAN_MV - %d",
                        (int)u16SelfVerifyStep, (int)vfan );
            break;
        case 5://GO STEP:-R|05
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:COOL - ON",
                        (int)u16SelfVerifyStep );
            break;
        case 6://GO STEP:-S|06
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:HEAT - ON",
                        (int)u16SelfVerifyStep );
            break;
        case 7://GO STEP:-T|07
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:IM_MA - %d",
                        (int)u16SelfVerifyStep, (int)im );
            break;
        case 8://GO STEP:-U|08
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:VBAT_MV - %d",
                        (int)u16SelfVerifyStep, (int)vbat );
            break;
        case 9://GO STEP:09
            if( Temp.err )
            {
                u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:NTC1 - err",
                            (int)u16SelfVerifyStep );
            }
            else
            {
                u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:NTC1 - %d",
                            (int)u16SelfVerifyStep,(int)temp );
            }
            break;
        case 10://GO STEP:10
            if( Temp2.err )
            {
                u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:NTC2 - err",
                            (int)u16SelfVerifyStep );
            }
            else
            {
                u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:NTC2 - %d",
                            (int)u16SelfVerifyStep,(int)temp2 );
            }
            break;
        case 11://GO STEP:11
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:DOOR - %d",
                        (int)u16SelfVerifyStep, (int)u8OutDoorSignl );
            break;
        case 12://GO STEP:12
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:LIN - %d",
                        (int)u16SelfVerifyStep, (int)u8LinSta );
            break;

        case 13://GO STEP:13
            break;

        default:
            u8datasize = sprintf((char*)&u8dataget[0],"STEP%02d:no step",
                        (int)u16SelfVerifyStep );
            break;
    }
    
    if( u16SelfVerifyStep == 13 ) u8datasize = 8;
    if( u8SysWorkSta == SYS_WORK_TEST )
    {
        MyUART_Puts( &UartToCar, &u8dataget[0], (U16)u8datasize );
    }

}


void DoorCheck( void )//10ms
{
    static U8 u8DoorOpenTime = 0;
    static U16 u16DoorCloseTime = 0;
    static U8 idrkey;
    
    IDR_INPUT( idrkey, IN_DOOR_SIGL() );
    if( idrkey & IDR_FILT )
    {
        u16DoorCloseTime = 0;
        if( ++u8DoorOpenTime >= 20 )
        {
            u8DoorOpenTime = 20;
            u8OutDoorSignl = TRUE;
        }
    }
    else
    {
        u8DoorOpenTime = 0;
        if( u8SysWorkSta == SYS_WORK_TEST )
        {
            if( ++u16DoorCloseTime >= 20 )
            {
                u16DoorCloseTime = 20;
                u8OutDoorSignl = FALSE;
            }
        }
        else
        {
            if( ++u16DoorCloseTime >= 500 )
            {
                u16DoorCloseTime = 500;
                u8OutDoorSignl = FALSE;
            }
        }
    }
}

void FanSpeedCheck( void )//200us
{
    static U8 idrIn,idrOut;

    IDR_INPUT( idrIn, IN_FAN_FBK() );
    IDR_INPUT( idrOut, OUT_FAN_FBK() );

    if( idrIn & IDR_EDGE_RISE )
    {
        if( u16InFanCnt <= 0xFFF0 ) u16InFanCnt++;
    }

    if( idrOut & IDR_EDGE_RISE )
    {
        if( u16OutFanCnt <= 0xFFF0 ) u16OutFanCnt++;
    }
}

void FanErrCheck( void )
{
    static U16 u16OutFanTime,u16InFanTime;
    static U8  InFanErrCnt,OutFanErrCnt;

    if( u8SysWorkSta == SYS_WORK_TEST ) return;

    //INFAN 3600RPM, OUTFAN 2000RPM

    if( u8InFanOn )
    {
        if( ++u16InFanTime >= 5*100*SECOND )
        {
            u16InFanTime = 0;
            if( u16InFanCnt < 125 )//((1500RPM*2RISE)/12)/2=125RISE(5S)
            {
                if( ++InFanErrCnt >= 3 )
                {
                    InFanErrCnt = 3;
                    RunErrFlag.Err.FAN_IN_ERR = 1;
                }
            }
            else
            {
                InFanErrCnt = 0;
                RunErrFlag.Err.FAN_IN_ERR = 0;
            }

            u16InFanCnt = 0;
        }
    }
    else
    {
        u16InFanCnt = 0;
        u16InFanTime = 0;
        InFanErrCnt = 0;
    }

    if( u8OutFanOn )
    {
        if( ++u16OutFanTime >= 5*100*SECOND )
        {
            u16OutFanTime = 0;
            if( u16OutFanCnt < 125 )//((1500RPM*2RISE)/12)/2=125RISE(5S)
            {
                if( ++OutFanErrCnt >= 3 )
                {
                    OutFanErrCnt = 3;
                    RunErrFlag.Err.FAN_OUT_ERR = 1;
                }
            }
            else
            {
                OutFanErrCnt = 0;
                RunErrFlag.Err.FAN_OUT_ERR = 0;
            }

            u16OutFanCnt = 0;
        }
    }
    else
    {
        u16OutFanCnt = 0;
        u16OutFanTime = 0;
        OutFanErrCnt = 0;
    }
}