/*********************************************************************************************
 * License: Copyright (C) 2022 Genbyte Technology Inc Limited Company. All Rights Reserved.
 * FilePath: \SOFT\APP\SystemCtrl.c
 * Company: GenByte
 * Author: Aescn.Pan
 * Date: 2023-03-06 09:01:49
 * LastEditors: Please set LastEditors
 * LastEditTime: 2023-07-08 22:26:59
 * Description: 
 *********************************************************************************************/

#define SystemCtrl_SRC_

#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
#include <stdio.h>
#include <string.h>
#include "Typedef.h"
#include "SystemCtrl.h"
#include "Main_App.h"
#include "AD_NTC.h"
#include "MathLib.h"
#include "Check.h"



void UpdateDataReady( void )
{
    S8 tempdiff;
    static U8 u8loaderrcnt = 0;
    static U8 u8loadoktim = 0;
    static U16 u16loaderrtim = 0;
    static U8 SysStopTime = 0;
    static U8 SetChangeTim = 0;
    static U8 ResponeErrTim = 0;
    static U16 u16doorerrtim = 0;
    static U8 u8tempdifftim = 0;
    static U8 u8VccHighTime = 0;
    static U8 u8VccLowTime = 0;

    //Set Temp update
    INCEX( SetChangeTim );

    if( (u8SetMode!=IVFWModReq) || (u8TempSetRq!=IVFTSetReq) )
    {
        u8SetMode = IVFWModReq;
        u8TempSetRq = IVFTSetReq;
        SetChangeTim = 0;
    }

    if( (u8TempSet!=u8TempSetRq) || (u8RunMode!=u8SetMode) )//&& (SetChangeTim>=2*2*SECOND)
    {
        switch( u8SetMode )
        {
        case RUN_COOL:
            if( (u8TempSetRq>=2) && (u8TempSetRq<=15) )
            {
                if( u8RunMode != u8SetMode )
                {
                    u8RunMode = u8SetMode;
                    u8DmosOffTime = 0;
                    u8CoolOnTime = 0;
                    u8HeatOnTime = 0;
                    u8Coefficient = 250;
                }
                u8TempSet = u8TempSetRq;
                u8SetDataErr = 0;
            }
            else
            {
                u8SetDataErr = 1;
            }
            break;

        case RUN_HEAT:
            if( (u8TempSetRq>=35) && (u8TempSetRq<=50) )
            {
                if( u8RunMode != u8SetMode )
                {
                    u8RunMode = u8SetMode;
                    u8DmosOffTime = 0;
                    u8CoolOnTime = 0;
                    u8HeatOnTime = 0;
                    u8Coefficient = 250;
                }
                u8TempSet = u8TempSetRq;
                u8SetDataErr = 0;
            }
            else
            {
                u8SetDataErr = 1;
            }
            break;

        default:
            break;
        }
    }

    //Run mode update
    if( u8PowerSta != IVFWOnOffReq )
    {
        u8PowerSta = IVFWOnOffReq;
    }

    if( !u8PowerSta )
    {
        u8RunMode = RUN_STOP;
        u8SetMode = RUN_STOP;
        IVFWOnOffReq = FALSE;
        RunErrFlag.All = 0;
    }

    if( u16PowerLinTime >= 5*SECOND )
    {
        u8SleepOrder = TRUE;
        u16PowerLinTime = 0;
    }

    if( u8SleepOrder )
    {
        u8DmosOn = FALSE;
        u8CoolOn = FALSE;
        u8HeatOn = FALSE;
        u8FanRun = E_FAN_INIT;
        u8InFanOn = FALSE;
        u8OutFanOn = FALSE;
        if( ++SysStopTime >= 1*2*SECOND )
        {
            u8SysWorkSta = SYS_WORK_STOP;
            SysStopTime = 0;
        }
    }
    else
    {
        SysStopTime = 0;
        if( u8SysWorkSta == SYS_WORK_STOP )
        {
            u8SysWorkSta = SYS_WORK_RUNS;
        }
    }

    //Load run status update
    if( u8DmosOn ) u8LoadRunSta = TRUE;
    else u8LoadRunSta = FALSE;

    //Lamp status update
    if( u8OutDoorSignl && (u8SysWorkSta==SYS_WORK_RUNS) )
    {
        u8LampOn = TRUE;
    }
    else u8LampOn = FALSE;

    //Send temp update
    u8SendTemp = (U8)(((float)s16SysTempVal/256 + 0.5) + 20);

    //Large temperature difference error check
    tempdiff = (S8)(s16SysTempVal/256 - u8TempSet);
    if( u8RunMode == RUN_STOP )
    {
        RunErrFlag.Err.TEMP_DIFF_ERR = 0;
        u8tempdifftim = 0;
    }
    else if( (ABS(tempdiff) > 15) && (u16LoadCurrent < 2000) )
    {
        if( ++u8tempdifftim >= 2*2*MINUTE )
        {
            RunErrFlag.Err.TEMP_DIFF_ERR = 1;
            u8tempdifftim = 0;
        }
    }
    else
    {
        RunErrFlag.Err.TEMP_DIFF_ERR = 0;
        u8tempdifftim = 0;
    }

    //Input voltage check
    if( BAT_GetVoltage() > 16000U )
    {
        if( ++u8VccHighTime >= 2*2*SECOND )
        {
            RunErrFlag.Err.BAT_HIGH_ERR = 1;
            u8VccHighTime = 0;
        }
    }
    else if( BAT_GetVoltage() < 15800U )
    {
        RunErrFlag.Err.BAT_HIGH_ERR = 0;
        u8VccHighTime = 0;
    }

    if( BAT_GetVoltage() < 9000U )//(16-9)*20%+9=10.4
    {
        if( ++u8VccLowTime >= 2*2*SECOND )
        {
            RunErrFlag.Err.BAT_LOW_ERR = 1;
            u8VccLowTime = 0;
        }
    }
    else if( BAT_GetVoltage() > 9200U )//(16-9)*30%+9=11.1
    {
        RunErrFlag.Err.BAT_LOW_ERR = 0;
        u8VccLowTime = 0;
    }

    //Load current check of cool&heat
    if( !u8LoadRunSta )
    {
        u16ImStopCurrent = IM_GetCurrent();
    }
    else if( u8CoolOn || u8HeatOn )
    {
        if( u16LoadCurrent < 2000 )
        {
            if( u8CoolOn && !RunErrFlag.Err.LOAD_ERR )
            {
                if( s16SysTempVal > (TPRREF(u8TempSet)+TPRREF(8)) )
                {
                    if( ++u16loaderrtim >= 3*2*MINUTE )
                    {
                        u16loaderrtim = 0;

                        if( ++u8loaderrcnt >= 3 )
                        {
                            u8loaderrcnt = 0;
                            RunErrFlag.Err.LOAD_ERR = 1;
                        }
                    }
                }
            }
            else if( u8HeatOn && !RunErrFlag.Err.LOAD_ERR )
            {
                if( s16SysTempVal < (TPRREF(u8TempSet)-TPRREF(10)) )
                {
                    if( ++u16loaderrtim >= 3*2*MINUTE )
                    {
                        u16loaderrtim = 0;

                        if( ++u8loaderrcnt >= 3 )
                        {
                            u8loaderrcnt = 0;
                            RunErrFlag.Err.LOAD_ERR = 1;
                        }
                    }
                }
            }
        }
        else
        {
            u8loaderrcnt = 0;

            if( RunErrFlag.Err.LOAD_ERR )
            {
                if( ++u8loadoktim >= 3*2*SECOND )
                {
                    u8loadoktim = 0;
                    RunErrFlag.Err.LOAD_ERR = 0;
                }
            }
        }
    }

    if( IM_GetCurrent() < u16ImStopCurrent ) u16LoadCurrent = 0;
    else u16LoadCurrent = IM_GetCurrent() - u16ImStopCurrent;

    //Door open time check
    if( u8OutDoorSignl )
    {
        if( ++u16doorerrtim >= 5*2*MINUTE )
        {
            u16doorerrtim = 0;
            RunErrFlag.Err.DOOR_ERR = 1;
        }
    }
    else
    {
        u16doorerrtim = 0;
        RunErrFlag.Err.DOOR_ERR = 0;
    }

    //Respone error update
    if( u8LinErrSta )
    {
        RunErrFlag.Err.RESPONE_ERR = 1;
        ResponeErrTim = 0;
    }
    else
    {
        if( ++ResponeErrTim >= 5*2*SECOND )
        {
            RunErrFlag.Err.RESPONE_ERR = 0;
            ResponeErrTim = 5*2*SECOND;
        }
    }
}

void TempFilter( void )
{
    if( !Temp.err )
    {
        static U8 sampletime = 0;
        static U8 errtime = 0;
        static S32 tempTsum = 0;

        if( ((s16SysTempOld-Temp.val)>TPRREF(10)) || ((Temp.val-s16SysTempOld)>TPRREF(10)) )
        {
            if( ++errtime >= 5*2*SECOND )
            {
                s16SysTempOld = Temp.val;
                errtime = 0;
            }
        }
        else
        {
            s16SysTempOld = Temp.val;
            errtime = 0;
        }

        if( ++sampletime <= 20 )
        {
            tempTsum += s16SysTempOld;
        }
        else
        {
            sampletime = 0;
            tempTsum = tempTsum/20;

            if( u8Coefficient >= 8 )
            {
                u8Coefficient -= 3;
                if( u8RunMode == RUN_STOP )
                {
                    u8Coefficient = 5;
                }
                else if( u8RunMode == RUN_COOL )
                {
                    if( tempTsum < s16SysTempVal )
                    {
                        u8Coefficient = 5;
                    }
                }
                else if( u8RunMode == RUN_HEAT )
                {
                    if( tempTsum > s16SysTempVal )
                    {
                        u8Coefficient = 5;
                    }
                }
            }
            else
            {
                u8Coefficient = 5;
            }

            s16SysTempVal = (S16)((tempTsum-s16SysTempVal)/u8Coefficient)+s16SysTempVal;

            tempTsum = 0;
        }
    }
}


#if 0
void Fan_Ctrl( void )//10ms
{
    static U16 u16FanPwm,u16OutFanTime,u16InFanTime;
    static U8  InFanErrCnt,OutFanErrCnt;

    if( u8SysWorkSta == SYS_WORK_TEST ) return;
    
    u16FanVoltage = VF_GetVoltage( );
    
    if( u8LoadRunSta )
    {
        switch( u8FanRun )
        {
        case E_FAN_INIT:
            if( BAT_GetVoltage( ) <= 12000 )
            {
                u8FanRun = E_PWM_FULL;
            }
            else u8FanRun = E_PWM_FALL;
            break;

        case E_PWM_FULL:
            FAN_DRV_DUTY( PWM_CYCLE );
            u8FanRun = E_PWM_RISE;
            break;

        case E_PWM_FALL:
            u16FanPwm = PWM_80_CYCLE;
            u8FanRun++;
            break;
        case E_PWM_FALL+1:
            if( u16FanVoltage > 12000 )
            {
                if(u16FanPwm>=20) u16FanPwm -= 10;
                FAN_DRV_DUTY( u16FanPwm );
            }
            else u8FanRun = E_PWM_RISE;
            break;

        case E_PWM_RISE:
            if( u16FanVoltage < 12000 )
            {
                if(u16FanPwm<PWM_CYCLE) u16FanPwm += 5;
                if(u16FanPwm>PWM_CYCLE) u16FanPwm = PWM_CYCLE;
                FAN_DRV_DUTY( u16FanPwm );
            }
            else u8FanRun = E_PWM_FALL+1;
            break;

        default:
            break;
        }

        if( u8OutDoorSignl ) u8InFanOn = FALSE;
        else u8InFanOn = TRUE;
        u8OutFanOn = TRUE;
    }
    else
    {
        FAN_DRV_DUTY( PWM_STOP );
        u8FanRun = E_FAN_INIT;
        u8InFanOn = FALSE;
        u8OutFanOn = FALSE;
    }

    if( u8InFanOn )
    {
        if( ++u16InFanTime >= 5*100*SECOND)
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
        if( ++u16OutFanTime >= 5*100*SECOND)
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
#endif
int Kp = 8;
int Ki = 8;
U32 pid_pwm = 0;
U16 PIDVoltage( S32 DiffVoltage )
{
    
    static S32 Diff2,Diff1,Diff0;
    S32 pid,Delta;

    if( VF_GetVoltage( ) > 13000 )
    {
        pid_pwm /= 2;
    }

    Diff2 = Diff1;
    Diff1 = Diff0;
    Diff0 = DiffVoltage;

    Delta = Kp * (Diff1 - Diff0) + Ki * Diff0;
    pid = ( (S32)pid_pwm + Delta );

    if( pid > (S32)(2400*256UL) )
    {
        pid_pwm = (S32)(2400*256UL);
    }
    else if( pid < (S32)(0*256UL) )
    {
        pid_pwm = (S32)(0*256UL);
    }
    else
    {
        pid_pwm += Delta;
    }
 

    return (U16)(pid_pwm>>8);
}

void Fan_Ctrl( void )//10ms
{
    static S32 StandVoltage;
    S32 s16ErrVoltage,FanVoltage;
    U16 u16PwmValue;

    if( u8SysWorkSta == SYS_WORK_TEST ) return;
    
    u16FanVoltage = VF_GetVoltage( );
    FanVoltage = u16FanVoltage/10;
   
    if( u8LoadRunSta )
    {
        if( BAT_GetVoltage( ) >= 16000 )
        {
            StandVoltage = 0;
        }
        else if( BAT_GetVoltage( ) <= 15800 )
        {
            StandVoltage = 1200;
        }
    }
    else
    {
        StandVoltage = 0;
    }

    s16ErrVoltage = StandVoltage - FanVoltage;
    u16PwmValue = PIDVoltage( s16ErrVoltage );
    FAN_DRV_DUTY( u16PwmValue );

    if( u8LoadRunSta )
    {
        //FAN_DRV_DUTY( u16PwmValue );
        if( u8OutDoorSignl ) u8InFanOn = FALSE;
        else u8InFanOn = TRUE;
        u8OutFanOn = TRUE;
    }
    else
    {
        //FAN_DRV_DUTY( PWM_STOP );
        u8FanRun = E_FAN_INIT;
        u8InFanOn = FALSE;
        u8OutFanOn = FALSE;
    }
}

void Cool_Heat_Ctrl( void )
{

    switch( u8RunMode )
    {
    case RUN_STOP:
        u8CoolSta = FALSE;
        u8HeatSta = FALSE;
        u8RunSta = u8RunMode;
        break;

    case RUN_COOL:
        u8HeatSta = FALSE;
        u8RunSta = u8RunMode;

        if( s16SysTempVal >= (TPRREF(u8TempSet)+TPRREF(1)) )
        {
            u8CoolSta = TRUE;
        }
        else if( s16SysTempVal <= (TPRREF(u8TempSet)-TPRREF(1)) )
        {
            u8CoolSta = FALSE;
        }
         
        if( RunErrFlag.Err.FAN_OUT_ERR || RunErrFlag.Err.LOAD_ERR )
        {
            u8CoolSta = FALSE;
            u8RunSta = RUN_STOP;
        }

        if( RunErrFlag.Err.BAT_HIGH_ERR || RunErrFlag.Err.BAT_LOW_ERR )
        {
            u8CoolSta = FALSE;
        }
        break;

    case RUN_HEAT:
        u8CoolSta = FALSE;
        u8RunSta = u8RunMode;

        if( s16SysTempVal <= (TPRREF(u8TempSet)+TPRREF(1)) )
        {
            u8HeatSta = TRUE;
        }
        else if( s16SysTempVal >= (TPRREF(u8TempSet)-TPRREF(1)) )
        {
            u8HeatSta = FALSE;
        }

        if( RunErrFlag.Err.FAN_OUT_ERR || RunErrFlag.Err.LOAD_ERR || Temp.err )
        {
            u8HeatSta = FALSE;
            u8RunSta = RUN_STOP;
        }

        if( RunErrFlag.Err.BAT_HIGH_ERR || RunErrFlag.Err.BAT_LOW_ERR )
        {
            u8HeatSta = FALSE;
        }
        break;

    default:
        break;
    }
}

void Mos_Relay_Ctrl( void )
{
    switch( u8RunMode )
    {
    case RUN_STOP:
        u8DmosOn = FALSE;
        if( u8CoolOn || u8HeatOn )
        {
            if( ++u8DmosOffTime >= 2*2*SECOND )
            {
                u8DmosOffTime = 0;
                u8CoolOn = FALSE;
                u8HeatOn = FALSE;
            }
        }
        break;

    case RUN_COOL:
        if( u8HeatOn )
        {
            u8DmosOn = FALSE;
            if( ++u8DmosOffTime >= 2*2*SECOND )
            {
                u8DmosOffTime = 0;
                u8HeatOn = FALSE;
                u16HeatOffTime = 1*MINUTE;
            }
        }
        else
        {
            if( u8CoolSta )//turn on cool load
            {
                if( !u16HeatOffTime )
                {
                    u8CoolOn = TRUE;
                    if( !u8DmosOn )
                    {
                        if( ++u8CoolOnTime >= 2*2*SECOND )
                        {
                            u8CoolOnTime = 0;
                            u8DmosOn = TRUE;
                        }
                    }
                }
            }
            else//Turn off cool load
            {
                u8DmosOn = FALSE;
                if( u8CoolOn )
                {
                    if( ++u8DmosOffTime >= 2*2*SECOND )
                    {
                        u8DmosOffTime = 0;
                        u8CoolOnTime = 0;
                        u8CoolOn = FALSE;
                    }
                }
            }
        }
        break;

    case RUN_HEAT:
        if( u8CoolOn )
        {
            u8DmosOn = FALSE;
            if( ++u8DmosOffTime >= 2*2*SECOND )
            {
                u8DmosOffTime = 0;
                u8CoolOn = FALSE;
                u16CoolOffTime = 1*MINUTE;
            }
        }
        else
        {
            if( u8HeatSta )//turn on cool load
            {
                if( !u16CoolOffTime )
                {
                    u8HeatOn = TRUE;
                    if( !u8DmosOn )
                    {
                        if( ++u8HeatOnTime >= 2*2*SECOND )
                        {
                            u8HeatOnTime = 0;
                            u8DmosOn = TRUE;
                        }
                    }
                }
            }
            else//Turn off cool load
            {
                u8DmosOn = FALSE;
                if( u8HeatOn )
                {
                    if( ++u8DmosOffTime >= 2*2*SECOND )
                    {
                        u8DmosOffTime = 0;
                        u8HeatOnTime = 0;
                        u8HeatOn = FALSE;
                    }
                }
            }
        }
        break;

    default:
        break;
    }
}

void Read_SN( U32 StartAdd, U8 *pData )
{
    U8 Cnt;

    for( Cnt=0; Cnt<8; Cnt++ )
    {
        *pData = *(U8 __far *)(StartAdd + Cnt );
        pData++;
    }
}

void Data_Init( void )
{
    U32 SNData;

    IVFWOnOffReq = FALSE;
    IVFWModReq = RUN_STOP;
    //IVFTSetReq = 5;
    p_fun = UpdateDataReady;

    Read_SN( SN_Address, SerialNumber );

    SNData = ((U32)SerialNumber[7]<<24)|((U32)SerialNumber[6]<<16)|((U32)SerialNumber[5]<<8)|((U32)SerialNumber[4]);

    SerialNumber[4] = (U8)( (SNData%10) | (((SNData/10)%10)<<4) );
    SerialNumber[5] = (U8)( ((SNData/100)%10) | (((SNData/1000)%10)<<4) );
    SerialNumber[6] = (U8)( ((SNData/10000)%10) | (((SNData/100000)%10)<<4) );
    SerialNumber[7] = (U8)( ((SNData/1000000)%10) | (((SNData/10000000)%10)<<4) );
}


void SystemCtrl( void )
{
    static U8 timcnt = 0;
    
    switch( u8SysWorkSta )
    {
    case SYS_WORK_INIT:
        s16SysTempVal = Temp.val;
        s16SysTempOld = Temp.val;
        //UpdateDataReady( );
        (*p_fun)( );
        if( ++timcnt >= 4 )
        {
            timcnt = 0;
            u8SysWorkSta = SYS_WORK_RUNS;
        }
        break;

    case SYS_WORK_RUNS:
        TempFilter( );
        //UpdateDataReady( );
        (*p_fun)( );
        Cool_Heat_Ctrl( );
        Mos_Relay_Ctrl( );
        break;

    case SYS_WORK_STOP:
        (*p_fun)( );
        break;

    case SYS_WORK_TEST:
        SelfVerify( );
        break;
    }
}
