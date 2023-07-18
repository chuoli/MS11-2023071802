
#ifndef AD_NTC_H_
#define AD_NTC_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef AD_NTC_SRC
#define EXTERN
#else
#define EXTERN extern
#endif

#define E_NTC_SHORT  1
#define E_NTC_OPEN   2
#define TPRREF( T )  ( S16 )( T * 256.0 )

typedef struct tag_TPR
{
    S16 val;
    U8 err;
    U8 cnt;
} TPR;

#define AD_NTC_CH    1
EXTERN TPR Temp;

#define AD_NTC2_CH   5
EXTERN TPR Temp2;

#define AD_VBAT_CH   0
#define AD_IM_CH     3
#define AD_VF_CH     4

EXTERN void AD_NTC_Init( void );
EXTERN void NTC_Check( void );
EXTERN void Voltage_Check( void );
EXTERN U16 BAT_GetVoltage( void );
EXTERN U16 VF_GetVoltage( void );
EXTERN U16 IM_GetCurrent( void );

#endif

