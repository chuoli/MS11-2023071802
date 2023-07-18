
#ifndef Main_App_H_
#define Main_App_H_

#ifdef EXTERN
#undef EXTERN
#endif

#ifdef Main_App_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"

#define DDD   2
#define SRAM 1
#define XRAM 21
#define CROM  const

#define __DI( )      DI()
#define __EI( )      EI()
#define __NOP( )     NOP()
#define __HALT( )    HALT( )
#define __STOP( )    STOP( )

#define __WDTC( )    R_WDT_Restart( )

#define __NOP_X1( )     do { __NOP( ); } while(0)
#define __NOP_X2( )     do { __NOP( ); __NOP( ); } while(0)
#define __NOP_X3( )     do { __NOP( ); __NOP( ); __NOP( ); } while(0)
#define __NOP_X4( )     do { __NOP( ); __NOP( ); __NOP( ); __NOP( ); } while(0)
#define __NOP_X5( )     do { __NOP( ); __NOP( ); __NOP( ); __NOP( ); __NOP( ); } while(0)


//#define _SuoShi
#ifdef _SuoShi
#define SECOND  1UL
#define MINUTE  1UL
#define HOUR    60UL
#else
#define SECOND  1UL
#define MINUTE  60UL
#define HOUR    3600UL
#endif

EXTERN void App_Init( void );
EXTERN void App_Run( void );

#endif
