
#ifndef _MATH_LIB_H_
#define _MATH_LIB_H_

#ifdef  EXTERN
#undef  EXTERN
#endif
#ifdef  _MATH_LIB_SRC_
#define EXTERN
const U8 FILT_T[16] =
{
    0x00, 0x02, 0x04, 0x86, 0x00, 0x82, 0x84, 0xDE,
    0x60, 0x0A, 0x0C, 0x8E, 0x08, 0x8A, 0x8C, 0x8E
};
#else
#define EXTERN extern
extern const U8 FILT_T[16];
#endif

#define IDR_INPUT( V, C )   do{ if( C ) V++; V = FILT_T[ V & 0XF ]; } while(0)
#define IDR_FILT            0X08
#define IDR_EDGE_RISE       0X10
#define IDR_EDGE_FALL       0X20
#define IDR_EDGE_BOTH       0X40

#define IN_FAN_FBK( )      ( P12.5 )
#define OUT_FAN_FBK( )     ( P8.4 )

#define COOL_ON( )       do{ P3.0 = 1; }while( 0 )
#define COOL_OFF( )      do{ P3.0 = 0; }while( 0 )

#define HEAT_ON( )       do{ P1.7 = 1; }while( 0 )
#define HEAT_OFF( )      do{ P1.7 = 0; }while( 0 )

#define DMOS_ON( )       do{ P8.5 = 1; }while( 0 )
#define DMOS_OFF( )      do{ P8.5 = 0; }while( 0 )

#define IFAN_ON( )       do{ P1.0 = 1; }while( 0 )
#define IFAN_OFF( )      do{ P1.0 = 0; }while( 0 )

#define OFAN_ON( )       do{ P1.1 = 1; }while( 0 )
#define OFAN_OFF( )      do{ P1.1 = 0; }while( 0 )

#define LAMP_ON( )       do{ P6.3 = 1; }while( 0 )
#define LAMP_OFF( )      do{ P6.3 = 0; }while( 0 )

#define LIN_ON( )        do{ P1.2 = 1; }while( 0 )
#define LIN_OFF( )       do{ P1.2 = 0; }while( 0 )

#define HALL_ON( )       do{ P4.1 = 1; }while( 0 )
#define HALL_OFF( )      do{ P4.1 = 0; }while( 0 )

#define NTC_ON( )        do{ P1.6 = 1; }while( 0 )
#define NTC_OFF( )       do{ P1.6 = 0; }while( 0 )

#define VBAT_ON( )       do{ P12.0 = 1; }while( 0 )
#define VBAT_OFF( )      do{ P12.0 = 0; }while( 0 )

#define PWM_FAN_DUTY( D )   TDR05 = ( D )

#define PWM_CYCLE         ( _095F_TAU_TDR00_VALUE + 1U )
#define PWM_STOP          ( 0U )
#define PWM_80_CYCLE      ( 1920U )


#define API_MATH_BIT_SCAN       1
#define API_MATH_MULU16         1
#define API_MATH_DIVU32         1
#define API_MATH_MODU32         0
#define API_MATH_SQRT32         0
#define API_MATH_LPF            0
#define API_MATH_PID            0
#define API_MATH_LOOK_UP_REV    0

typedef union
{
    S32     D;
    #if     BIG_ENDIAN
    struct  { S16 H; S16 L; }   W;
    #else
    struct  { S16 L; S16 H; }   W;
    #endif
}   SDW_t;

typedef struct
{
    S16     ri;
    S16     ro;
}   LPF_t;

typedef struct
{
    S16     kp;
    S16     ki;
    S16     kd;
    SDW_t   ri;
    SDW_t   ro;
}   PID_t;

#if     API_MATH_BIT_SCAN
EXTERN  U8 const FILT2[ 16 ]
#ifdef  _MATH_LIB_SRC_
=   {
        0x00, 0x02, 0x04, 0x06, 0x00, 0x02, 0x04, 0x5E,
        0x60, 0x0A, 0x0C, 0x0E, 0x08, 0x0A, 0x0C, 0x0E
    }
#endif
;
#define BIT_SCAN( X, C )        do { if ( C ) X |= 1; X = FILT2[ X & 0X0F ]; } while(0)
#define BIT_INIT_0000( X )      do { X = 0X00; } while(0)
#define BIT_INIT_0001( X )      do { X = 0X02; } while(0)
#define BIT_INIT_1110( X )      do { X = 0X0C; } while(0)
#define BIT_INIT_1111( X )      do { X = 0X0E; } while(0)
#define BIT_DATA_0000( X )      ( X == 0X00 )
#define BIT_DATA_1111( X )      ( X == 0X0E )
#define BIT_DATA_LAST( X )      ( X & 0X02 )
#define BIT_DATA_FILT( X )      ( X & 0X08 )
#define BIT_EDGE_RISE( X )      ( X & 0X10 )
#define BIT_EDGE_FALL( X )      ( X & 0X20 )
#define BIT_EDGE_BOTH( X )      ( X & 0X40 )
#endif

#define MULU8( a, b )           mulu( ( U8 )( a ), ( U8 )( b ) )
#define DIVU16( a, b )          divuw( ( U16 )( a ), ( U8 )( b ) )
#define MODU16( a, b )          moduw( ( U16 )( a ), ( U8 )( b ) )
#define MULU32( a, b )          ( ( U32 )( a ) * ( U32 )( b ) )
#define MULS32( a, b )          ( ( S32 )( a ) * ( S32 )( b ) )

#if     API_MATH_MULU16
EXTERN  U32  MULU16( U16 a, U16 b );
#endif
#if     API_MATH_DIVU32
EXTERN  U16  DIVU32( U32 a, U16 b );
#endif

EXTERN void MCU_Sleep( void );
EXTERN void MCU_WakeUp( void );
EXTERN void Refresh( void );

#endif

