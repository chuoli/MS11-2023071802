
#ifndef SystemCtrl_H_
#define SystemCtrl_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef SystemCtrl_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define IN_DOOR_SIGL( )    ( P13.7 )

enum SYS_WORK_STA
{
    SYS_WORK_INIT,
    SYS_WORK_RUNS,
    SYS_WORK_STOP,
    SYS_WORK_TEST
};
EXTERN U8 u8SysWorkSta;
EXTERN U16 u16SelfVerifyStep;
EXTERN U8 u8PowerSta;

enum SYS_RUN_MODE
{
    RUN_STOP,
    RUN_COOL,
    RUN_HEAT
};
EXTERN U8 u8SetMode;
EXTERN U8 u8RunMode;
EXTERN U8 u8RunSta;
EXTERN U8 u8LoadRunTime;

EXTERN U16 u16FanVoltage;
EXTERN U16 u16LoadCurrent;
EXTERN U16 u16ImStopCurrent;

EXTERN U16 u16CoolOffTime;
EXTERN U16 u16HeatOffTime;
EXTERN U8  u8CoolOnTime;
EXTERN U8  u8HeatOnTime;
EXTERN U8  u8DmosOffTime;

EXTERN U8 u8LoadRunSta;
EXTERN U8 u8CoolSta;
EXTERN U8 u8CoolOn;
EXTERN U8 u8HeatSta;
EXTERN U8 u8HeatOn;
EXTERN U8 u8DmosOn;
EXTERN U8 u8LampOn;

EXTERN U8 u8SendTemp;
EXTERN S16 s16SysTempVal;
EXTERN S16 s16SysTempOld;
EXTERN U8 u8Coefficient;

EXTERN U8 u8TempSet;
EXTERN U8 u8TempSetRq;
EXTERN U8 IVFTSetReq;
EXTERN U8 IVFWModReq;
EXTERN U8 IVFWOnOffReq;
EXTERN U8 VCCDLZCUComFltSts;

EXTERN U8 u8SetDataErr;
EXTERN U8 u8SleepOrder;
EXTERN U8 u8WakeUpSta;

EXTERN U8 u8OutDoorSignl;

EXTERN U8 u8LinSta;
EXTERN U8 u8LinErrSta;
EXTERN U16 u16PowerLinTime;
EXTERN U16 u16ModeLinTime;
EXTERN U16 u16TempLinTime;
EXTERN U8  u8DiagTime;

EXTERN U8  u8OutFanOn;
EXTERN U16 u16OutFanCnt;
EXTERN U8  u8InFanOn;
EXTERN U16 u16InFanCnt;
EXTERN U8 u8FanRun;
#define E_FAN_INIT   0
#define E_PWM_FULL   1
#define E_PWM_FALL   0x10
#define E_PWM_RISE   0x20
#define STANDVOLTAGE 12000UL

typedef union
{
    U16 All;
    struct
    {
        U8 DOOR_ERR : 1;
        U8 LOAD_ERR : 1;
        U8 FAN_IN_ERR : 1;
        U8 FAN_OUT_ERR : 1;
        U8 TEMP_DIFF_ERR : 1;
        U8 BAT_LOW_ERR : 1;
        U8 BAT_HIGH_ERR : 1;
        U8 RESPONE_ERR : 1;
    } Err;
} E_Flag;

EXTERN E_Flag RunErrFlag;

typedef struct
{
    U8  runsta;
    U8  systemp;
    U8  settemp;
    U8  door;
    U8  lamp;
    U8  err;
    U16 current;
} DataParam;
EXTERN DataParam LinParamBak;

EXTERN U8 SerialNumber[8];
#define SN_Address  0xFFF0

#define FAN_DRV_DUTY( D )  PWM_FAN_DUTY( D )

EXTERN void (*p_fun)( );

EXTERN void Data_Init( void );
EXTERN void DoorCheck( void );
EXTERN void FanSpeedCheck( void );
EXTERN void Fan_Ctrl( void );
EXTERN void SystemCtrl( void );

#endif

