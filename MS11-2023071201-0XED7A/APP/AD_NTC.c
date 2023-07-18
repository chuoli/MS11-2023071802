/*********************************************************************************************
 * License: Copyright (C) 2022 Genbyte Technology Inc Limited Company. All Rights Reserved.
 * FilePath: \SOFT\APP\AD_NTC.c
 * Company: GenByte
 * Author: Aescn.Pan
 * Date: 2023-02-28 10:38:19
 * LastEditors: Please set LastEditors
 * LastEditTime: 2023-06-25 10:09:32
 * Description: 
 *********************************************************************************************/

#define AD_NTC_SRC

#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"

#include <string.h>

#include "Typedef.h"
#include "AD_NTC.h"
#include "Main_App.h"
#include "MathLib.h"
#include "SystemCtrl.h"
#define AD_OF_NTC( R )  ( (U16)(10.0 / (10.0+R) * 1024 + 0.5) )

U16 const ad_PB_3435[ ] =
{
    AD_OF_NTC( 208.5977), // -40
    AD_OF_NTC( 197.0831), // -39
    AD_OF_NTC( 186.2746), // -38
    AD_OF_NTC( 176.1249), // -37
    AD_OF_NTC( 166.5899), // -36
    AD_OF_NTC( 157.6288), // -35
    AD_OF_NTC( 149.2037), // -34
    AD_OF_NTC( 141.2715), // -33
    AD_OF_NTC( 133.8234), // -32
    AD_OF_NTC( 126.8053), // -31
    AD_OF_NTC( 120.1967), // -30
    AD_OF_NTC( 113.9715), // -29
    AD_OF_NTC( 108.1053), // -28
    AD_OF_NTC( 102.5754), // -27
    AD_OF_NTC( 97.3605 ), // -26
    AD_OF_NTC( 92.441  ), // -25
    AD_OF_NTC( 87.7985 ), // -24
    AD_OF_NTC( 83.4159 ), // -23
    AD_OF_NTC( 79.2771 ), // -22
    AD_OF_NTC( 75.3673 ), // -21
    AD_OF_NTC( 71.6725 ), // -20
    AD_OF_NTC( 68.1798 ), // -19
    AD_OF_NTC( 64.8769 ), // -18
    AD_OF_NTC( 61.7526 ), // -17
    AD_OF_NTC( 58.7962 ), // -16
    AD_OF_NTC( 55.9977 ), // -15
    AD_OF_NTC( 53.348  ), // -14
    AD_OF_NTC( 50.8383 ), // -13
    AD_OF_NTC( 48.4605 ), // -12
    AD_OF_NTC( 46.2069 ), // -11
    AD_OF_NTC( 44.0704 ), // -10
    AD_OF_NTC( 42.0443 ), // -9
    AD_OF_NTC( 40.1223 ), // -8
    AD_OF_NTC( 38.2985 ), // -7
    AD_OF_NTC( 36.5675 ), // -6
    AD_OF_NTC( 34.9239 ), // -5
    AD_OF_NTC( 33.3629 ), // -4
    AD_OF_NTC( 31.88   ), // -3
    AD_OF_NTC( 30.4709 ), // -2
    AD_OF_NTC( 29.1314 ), // -1
    AD_OF_NTC( 27.8579 ), // 0
    AD_OF_NTC( 26.6528 ), // 1
    AD_OF_NTC( 25.5072 ), // 2
    AD_OF_NTC( 24.4179 ), // 3
    AD_OF_NTC( 23.3818 ), // 4
    AD_OF_NTC( 22.3961 ), // 5
    AD_OF_NTC( 21.4579 ), // 6
    AD_OF_NTC( 20.5647 ), // 7
    AD_OF_NTC( 19.7141 ), // 8
    AD_OF_NTC( 18.9038 ), // 9
    AD_OF_NTC( 18.1316 ), // 10
    AD_OF_NTC( 17.3957 ), // 11
    AD_OF_NTC( 16.694  ), // 12
    AD_OF_NTC( 16.0248 ), // 13
    AD_OF_NTC( 15.3863 ), // 14
    AD_OF_NTC( 14.7771 ), // 15
    AD_OF_NTC( 14.1956 ), // 16
    AD_OF_NTC( 13.6404 ), // 17
    AD_OF_NTC( 13.1102 ), // 18
    AD_OF_NTC( 12.6036 ), // 19
    AD_OF_NTC( 12.1196 ), // 20
    AD_OF_NTC( 11.657  ), // 21
    AD_OF_NTC( 11.2147 ), // 22
    AD_OF_NTC( 10.7917 ), // 23
    AD_OF_NTC( 10.3871 ), // 24
    AD_OF_NTC( 10.0    ), // 25
    AD_OF_NTC( 9.6295  ), // 26
    AD_OF_NTC( 9.2749  ), // 27
    AD_OF_NTC( 8.9353  ), // 28
    AD_OF_NTC( 8.61    ), // 29
    AD_OF_NTC( 8.2985  ), // 30
    AD_OF_NTC( 7.9999  ), // 31
    AD_OF_NTC( 7.7138  ), // 32
    AD_OF_NTC( 7.4395  ), // 33
    AD_OF_NTC( 7.1765  ), // 34
    AD_OF_NTC( 6.9242  ), // 35
    AD_OF_NTC( 6.6822  ), // 36
    AD_OF_NTC( 6.4499  ), // 37
    AD_OF_NTC( 6.227   ), // 38
    AD_OF_NTC( 6.0131  ), // 39
    AD_OF_NTC( 5.8076  ), // 40
    AD_OF_NTC( 5.6103  ), // 41
    AD_OF_NTC( 5.4208  ), // 42
    AD_OF_NTC( 5.2387  ), // 43
    AD_OF_NTC( 5.0636  ), // 44
    AD_OF_NTC( 4.8954  ), // 45
    AD_OF_NTC( 4.7337  ), // 46
    AD_OF_NTC( 4.5781  ), // 47
    AD_OF_NTC( 4.4285  ), // 48
    AD_OF_NTC( 4.2846  ), // 49
    AD_OF_NTC( 4.1461  ), // 50
    AD_OF_NTC( 4.0126  ), // 51
    AD_OF_NTC( 3.884   ), // 52
    AD_OF_NTC( 3.7603  ), // 53
    AD_OF_NTC( 3.6411  ), // 54
    AD_OF_NTC( 3.5263  ), // 55
    AD_OF_NTC( 3.4157  ), // 56
    AD_OF_NTC( 3.3091  ), // 57

    AD_OF_NTC( 3.2064  ), // 58
    AD_OF_NTC( 3.1074  ), // 59
    AD_OF_NTC( 3.012   ), // 60
    AD_OF_NTC( 2.92    ), // 61
    AD_OF_NTC( 2.8312  ), // 62
    AD_OF_NTC( 2.7456  ), // 63
    AD_OF_NTC( 2.663   ), // 64
    AD_OF_NTC( 2.5832  ), // 65
    AD_OF_NTC( 2.5063  ), // 66
    AD_OF_NTC( 2.432   ), // 67
    AD_OF_NTC( 2.3603  ), // 68
    AD_OF_NTC( 2.2911  ), // 69
    AD_OF_NTC( 2.2242  ), // 70
    AD_OF_NTC( 2.1596  ), // 71
    AD_OF_NTC( 2.0972  ), // 72
    AD_OF_NTC( 2.0369  ), // 73
    AD_OF_NTC( 1.9787  ), // 74
    AD_OF_NTC( 1.9223  ), // 75
    AD_OF_NTC( 1.8679  ), // 76
    AD_OF_NTC( 1.8152  ), // 77
    AD_OF_NTC( 1.7643  ), // 78
    AD_OF_NTC( 1.715   ), // 79
    AD_OF_NTC( 1.6674  ), // 80
    AD_OF_NTC( 1.6213  ), // 81
    AD_OF_NTC( 1.5767  ), // 82
    AD_OF_NTC( 1.5335  ), // 83
    AD_OF_NTC( 1.4918  ), // 84
    AD_OF_NTC( 1.4513  ), // 85
    AD_OF_NTC( 1.4121  ), // 86
    AD_OF_NTC( 1.3742  )  // 87
};

static U8 adCnt = 0;
static U8 adErr = 0;
static U16 adMin = 0;
static U16 adMax = 0;
static U16 adSum = 0;

static U16 vbatVoltage = 0;
static U16 vfVoltage = 0;
static U16 imVoltage = 0;
static U16 imCurrent = 0;
static U16 vbatAdList[ 4 ];
static U16 vbatRefAdList[ 4 ];
static U16 vfAdList[ 4 ];
static U16 vfRefAdList[ 4 ];
static U16 imAdList[ 4 ];
static U16 imRefAdList[ 4 ];
static U8  vbatAdIndex = 0xFF;
static U8  vfAdIndex = 0xFF;
static U8  imAdIndex = 0xFF;

void AD_NTC_Init( void )
{
    Temp.val = 0;
    Temp.cnt = 0;
    Temp.err = 0;

    Temp2.val = 0;
    Temp2.cnt = 0;
    Temp2.err = 0;
    
    vbatAdIndex = 0xFF;
    memset( vbatAdList, 0, sizeof( vbatAdList ) );
    memset( vbatRefAdList, 0, sizeof( vbatRefAdList ) );

    vfAdIndex = 0xFF;
    memset( vfAdList, 0, sizeof( vfAdList ) );
    memset( vfRefAdList, 0, sizeof( vfRefAdList ) );

    imAdIndex = 0xFF;
    memset( imAdList, 0, sizeof( imAdList ) );
    memset( imRefAdList, 0, sizeof( imRefAdList ) );
}

U16 ADC_Convert( U8 chn )
{
    U8 n;
    U16 x = 0;
    
    ADIF = 0;
    ADCE = 1;
    __NOP_X2( );
    ADS = chn;
    __NOP_X2( );
    ADS = chn;
    __NOP_X2( );
    ADCS = 1;
    n = 0;
    
    while ( !ADIF )
    {
        if( ++n >= 250 ) break;
        __WDTC( );
    }
    
    x = ADCR;
    ADS = 0;
    ADCS = 0;
    ADIF = 0;
    
    return ( x >> 6 );
}

#if 0
U16  ADC_Convert( U8 chn )
{
    U8 t;
    volatile U16 adv;

    R_ADC_Stop();
    ADS = chn;
    R_ADC_Set_OperationOn();
    R_ADC_Start();
    t = 0;
    __NOP( );
    __NOP( );
    __NOP( );
    while( ADCS )
    {
        if ( ++t >= 250 ) break;
        __WDTC( );
    }

    R_ADC_Get_Result(&adv);

    return ( adv );
}
#endif

BOOL ADC_Collect( U8 chn, U8 rev )
{
    U16 ad;
    U8 N;

    ad = ADC_Convert( chn );

    if( rev ) ad = 1024 - ad;

    N = adCnt % 10;

    if( N == 0 )
    {
        adMin = ad;
        adMax = ad;
        adSum = ad;

        if( ad > 1024-123 ) adErr = E_NTC_SHORT;
        else if( ad < 47 ) adErr = E_NTC_OPEN;
        else adErr = 0;
    }
    else
    {
        adSum += ad;

        if( ad < adMin ) adMin = ad;

        if( adMax < ad ) adMax = ad;

        if( adErr ==0 )
        {
            if( ad > 1024-123 ) adErr |= E_NTC_SHORT;
            else if( ad < 47 ) adErr |= E_NTC_OPEN;
        }
    }

    if( N == 9 )
    {
        adSum = ( adSum - adMin - adMax +4 ) >> 3;
        return 1;
    }

    return 0;
}

BOOL V_Collect( U8 chn )
{
    U16 ad;
    U8 N;

    ad = ADC_Convert( chn );

    N = adCnt % 10;

    if( N == 0 )
    {
        adMin = ad;
        adMax = ad;
        adSum = ad;
    }
    else
    {
        adSum += ad;

        if( ad < adMin ) adMin = ad;

        if( ad > adMax ) adMax = ad;
    }

    if( N == 9 )
    {
        adSum = ( adSum - adMin - adMax + 4 ) >> 3;
        return 1;
    }

    return 0;
}


U8 ADC_CalError( TPR* p )
{
    if( adErr == 0 )
    {
        if( p->cnt == 0 )
        {
            p->err = 0;
        }
        else p->cnt = 0; 
    }
    else
    {
        if( ++p->cnt >= 10 )
        {
            p->err = adErr;
        }
    }

    return ( p->err | p->cnt );
}

uint16_t LOOK_UP_REV( uint16_t val, uint16_t const *tab, uint8_t len )
{
    uint8_t a, b;
    uint16_t c;
    WORD r;

    if( val < tab[ 0 ] || len < 2 ) return 0;

    len--;

    if( val >= tab[ len ] )
    {
        r.B.H = len;
        r.B.L = 0;
        return r.W;
    }

    for( a = 0, b = 0x80; b != 0; b >>= 1 )
    {
        if( a + b >= len ) continue;

        if( val < tab[ a + b ] ) continue;

        a += b;
    }

    r.W = val - tab[ a ];
    c = tab[ a + 1 ] - tab[ a ];

    while( r.B.H != 0 )
    {
        r.W >>= 1;
        c >>= 1;
    }

    r.B.H = r.B.L;
    r.B.L = 0;
    r.B.L = ( uint8_t )( r.W / c );
    r.B.H = a;

    return r.W;
}

void NTC_Check( void )
{
    if( adCnt < 10 )
    {
        if( ADC_Collect( AD_NTC_CH, 1 ) )
        {
            if( !ADC_CalError( &Temp ) )
            {
                Temp.val =
                LOOK_UP_REV( adSum, ad_PB_3435, LENOF( ad_PB_3435 ) ) +
                TPRREF( -40 );
                if( u8RunMode == RUN_STOP )
                {
                        
                }
                else if( u8RunMode == RUN_COOL )
                {
                    Temp.val += TPRREF(5);
                }
                else if( u8RunMode == RUN_HEAT )
                {
                    Temp.val -= TPRREF(5);
                }
                
                if( Temp.val < TPRREF(-20) ) Temp.val = TPRREF( -20 );
                else if( Temp.val > TPRREF(75) ) Temp.val = TPRREF( 75 );
            }
        } 
    }
    else if( adCnt < 20 )
    {
        if( ADC_Collect( AD_NTC2_CH, 1 ) )
        {
            if( !ADC_CalError( &Temp2 ) )
            {
                Temp2.val =
                LOOK_UP_REV( adSum, ad_PB_3435, LENOF( ad_PB_3435 ) ) +
                TPRREF( -40 );
                if( Temp2.val < TPRREF(-20) ) Temp2.val = TPRREF( -20 );
                else if( Temp2.val > TPRREF(75) ) Temp2.val = TPRREF( 75 );
            }
        }
    }
    #if 0//Another method of voltage detection
    else if( adCnt < 30 )
    {
        if( V_Collect( AD_VBAT_CH ) )
        {
            vbatVoltage = DIVU32( MULU16( adSum, ( U16 )( 5000UL * 242UL / 22UL ) ), 1024 );
        }
    }
    else if( adCnt < 40 )
    {
        if( V_Collect( AD_VCC_CH ) )
        {
            vccVoltage = DIVU32( MULU16( adSum, ( U16 )( 5000UL * 242UL / 22UL ) ), 1024 );
        }
    }
    else if( adCnt < 50 )
    {
        if( V_Collect( AD_VF_CH ) )
        {
            vfVoltage = DIVU32( MULU16( adSum, ( U16 )( 5000UL * 267UL / 47UL ) ), 1024 );
        }
    }
    #endif

    if( ++adCnt >= 30 )
    {
        adCnt = 0;
    }
}


void VBAT_Check( void )
{
    U16 adBat;
    U16 adRef;
    adBat = ADC_Convert( AD_VBAT_CH );
    adRef = ADC_Convert( 0x81 ); // Internal reference voltage output ( 1.45V ).
    if ( vbatAdIndex >= 4 )
    {
        vbatAdIndex = 0;
        vbatAdList[ 0 ] = adBat;
        vbatAdList[ 1 ] = adBat;
        vbatAdList[ 2 ] = adBat;
        vbatAdList[ 3 ] = adBat;
        vbatRefAdList[ 0 ] = adRef;
        vbatRefAdList[ 1 ] = adRef;
        vbatRefAdList[ 2 ] = adRef;
        vbatRefAdList[ 3 ] = adRef;
    }
    else
    {
        vbatAdIndex++;
        vbatAdIndex &= 0x03;
        vbatAdList[ vbatAdIndex ] = adBat;
        vbatRefAdList[ vbatAdIndex ] = adRef;
    }
    adBat = vbatAdList[ 0 ] + vbatAdList[ 1 ] + vbatAdList[ 2 ] + vbatAdList[ 3 ];
    adRef = vbatRefAdList[ 0 ] + vbatRefAdList[ 1 ] + vbatRefAdList[ 2 ] + vbatRefAdList[ 3 ];
    vbatVoltage = DIVU32( MULU16( adBat, ( U16 )( 1450UL * 242UL / 22UL ) ), adRef );
}

void VF_Check( void )
{
    U16 adBat;
    U16 adRef;
    adBat = ADC_Convert( AD_VF_CH );
    adRef = ADC_Convert( 0x81 ); // Internal reference voltage output ( 1.45V ).
    if ( vfAdIndex >= 4 )
    {
        vfAdIndex = 0;
        vfAdList[ 0 ] = adBat;
        vfAdList[ 1 ] = adBat;
        vfAdList[ 2 ] = adBat;
        vfAdList[ 3 ] = adBat;
        vfRefAdList[ 0 ] = adRef;
        vfRefAdList[ 1 ] = adRef;
        vfRefAdList[ 2 ] = adRef;
        vfRefAdList[ 3 ] = adRef;
    }
    else
    {
        vfAdIndex++;
        vfAdIndex &= 0x03;
        vfAdList[ vfAdIndex ] = adBat;
        vfRefAdList[ vfAdIndex ] = adRef;
    }
    adBat = vfAdList[ 0 ] + vfAdList[ 1 ] + vfAdList[ 2 ] + vfAdList[ 3 ];
    adRef = vfRefAdList[ 0 ] + vfRefAdList[ 1 ] + vfRefAdList[ 2 ] + vfRefAdList[ 3 ];
    vfVoltage = DIVU32( MULU16( adBat, ( U16 )( 1450UL * 267UL / 47UL ) ), adRef );
}

void IM_Check( void )
{
    U16 adBat;
    U16 adRef;
    adBat = ADC_Convert( AD_IM_CH );
    adRef = ADC_Convert( 0x81 ); // Internal reference voltage output ( 1.45V ).
    if ( imAdIndex >= 4 )
    {
        imAdIndex = 0;
        imAdList[ 0 ] = adBat;
        imAdList[ 1 ] = adBat;
        imAdList[ 2 ] = adBat;
        imAdList[ 3 ] = adBat;
        imRefAdList[ 0 ] = adRef;
        imRefAdList[ 1 ] = adRef;
        imRefAdList[ 2 ] = adRef;
        imRefAdList[ 3 ] = adRef;
    }
    else
    {
        imAdIndex++;
        imAdIndex &= 0x03;
        imAdList[ imAdIndex ] = adBat;
        imRefAdList[ imAdIndex ] = adRef;
    }
    adBat = imAdList[ 0 ] + imAdList[ 1 ] + imAdList[ 2 ] + imAdList[ 3 ];
    adRef = imRefAdList[ 0 ] + imRefAdList[ 1 ] + imRefAdList[ 2 ] + imRefAdList[ 3 ];
    imVoltage = DIVU32( MULU16( adBat, 1450UL ), adRef );
}


void Voltage_Check( void )
{
    VBAT_Check( );
    VF_Check( );
    IM_Check( );
}

U16 BAT_GetVoltage( void )
{
    return vbatVoltage;
}

U16 VF_GetVoltage( void )
{
    return vfVoltage;
}

U16 IM_GetCurrent( void )
{
    imCurrent = imVoltage*10;//mV/20/5mR=(mV/100)A=(mV*10)mA
    
    return imCurrent;
}


