
#ifndef MyTimer_H_
#define MyTimer_H_

#ifdef EXTERN
#undef EXTERN
#endif

#ifdef MyTimer_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#include "Typedef.h"

EXTERN U8 ResetSta;

typedef struct
{
    U8 T1MS     :1;
    U8 T10MS    :1;
    U8 T20MS    :1;
    U8 T40MS    :1;
    U8 T50MS    :1;
    U8 T100MS   :1;
    U8 T500MS   :1;
    U8 T1S      :1;
}Flag_T;
EXTERN Flag_T flagT;

EXTERN void Timer_Init( void );
EXTERN void Timer2_Interrupt( void );
EXTERN void Timer3_Interrupt( void );
EXTERN void sysTimer( void );

#endif
