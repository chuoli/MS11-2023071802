
#define Uart_SRC_

#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include <string.h>
#include "Typedef.h"
#include "Main_App.h"
#include "Uart.h"
#include "AD_NTC.h"
#include "SystemCtrl.h"

void MyUART_Init( void )
{
    UartToCar.rxdataindex = 0;
    UartToCar.getdataindex = 0;
    UartToCar.uartnum = 0;
    R_UART0_Start( );
    R_UART0_Receive( &UartToCar.rxdata, 1 );
}

U8 MyUART_Getc( UartStatus* uartsta, U8 *pdata )
{
    if( uartsta->getdataindex != uartsta->rxdataindex )
    {
        U16 index;

        *pdata = uartsta->getdata[ uartsta->getdataindex ];
        uartsta->getdata[ uartsta->getdataindex ] = 0;
        index = uartsta->getdataindex + 1;
        
        if( index >= SIZE_BUF_RX ) index = 0;
        
        uartsta->getdataindex = index;
        return TRUE;
    }
    return FALSE;
}

U16 MyUART_Gets( UartStatus* uartsta, U8 *pdata )
{
    U16 now_index = 0;
    U16 old_index = 0;
    U16 size = 0;
    now_index = uartsta->rxdataindex;
    old_index = uartsta->getdataindex;

    while( now_index != old_index )
    {
        *pdata = uartsta->getdata[ old_index ];
        old_index += 1;

        if( old_index >= SIZE_BUF_RX ) old_index = 0;

        uartsta->getdataindex = old_index;

        pdata++;
        size++;
        __WDTC( );
    }
    return size;
}

U16 MyUART_GetFrames( UartStatus* uartsta, U8 *pdata )
{
    U16 now_index = 0;
    U16 old_index = 0;
    U16 size = 0;
    U16 getsize = 0;
    now_index = uartsta->rxdataindex;
    old_index = uartsta->getdataindex;

    if( now_index > old_index )
    {
        getsize = now_index - old_index;
    }
    else if( now_index < old_index )
    {
        getsize = now_index + SIZE_BUF_RX - old_index;
    }
    else getsize = 0;

    if( uartsta->rxsize != getsize )
    {
        uartsta->rxsize = getsize;
        return size;
    }

    while( now_index != old_index )
    {
        *pdata = uartsta->getdata[ old_index ];
        old_index += 1;

        if( old_index >= SIZE_BUF_RX ) old_index = 0;

        uartsta->getdataindex = old_index;

        pdata++;
        size++;
        __WDTC( );

        if( getsize == size ) return size;
    }
    return size;
}

void Rx_Byte_Call_Back( UartStatus* uartsta, U8 data )
{
    U16 index = 0;
    uartsta->getdata[ uartsta->rxdataindex ] = data;
    index = uartsta->rxdataindex + 1;
    if( index >= SIZE_BUF_RX ) index = 0;
    if( index != uartsta->getdataindex )
    {
        uartsta->rxdataindex = index;
    }
}

void MyUART_TxCheck( UartStatus* uartsta )
{
    U16 size = 0;
    U16 now_index = 0;
    U16 old_index = 0;

    if( uartsta->txbusy )
    {
        if( ++uartsta->txbusy >= 5U )
        {
            uartsta->txbusy = FALSE;
        }
        return;
    }

    now_index = uartsta->txdataindex;
    old_index = uartsta->putdataindex;

    if( now_index == old_index ) return;

    if( now_index > old_index )
    {
        size = now_index - old_index;
        uartsta->putdataindex = now_index;
    }
    else
    {
        size = SIZE_BUF_TX - old_index;
        uartsta->putdataindex = 0;
    }

    if( uartsta->uartnum == 0 )
    {
        uartsta->txbusy = TRUE;
        R_UART0_Send( &( uartsta->putdata[ old_index ] ), size );
    }
}

BOOL MyUART_TxByte( UartStatus* uartsta )
{
    U16 old_index = 0;

    if( uartsta->txbusy ) return;

    old_index = uartsta->putdataindex;

    if( ( old_index + 1 ) >= SIZE_BUF_TX )
    {
        uartsta->putdataindex = 0;
    }
    else
    {
        uartsta->putdataindex = old_index + 1;
    }

    if( uartsta->uartnum == 0 )
    {
        uartsta->txbusy = TRUE;
        R_UART0_Send( &( uartsta->putdata[ old_index ] ), 1 );
    }

    return 0;
}

U8 MyUART_Puts( UartStatus* uartsta, U8 *pdata, U16 size )
{
    U8 bufbusy = 0;
    U16 now_index = 0;
    U16 old_index = 0;
    int i;

    now_index = uartsta->txdataindex;
    old_index = uartsta->putdataindex;

    for( i = 0; i < size; i++ )
    {
        uartsta->putdata[ now_index ] = *pdata;
        pdata++;

        if( ++now_index >= SIZE_BUF_TX ) now_index = 0;

        if( now_index != old_index )
        {
            uartsta->txdataindex = now_index;
        }
        else
        {
            bufbusy = TRUE;
        }
        __WDTC( );
    }
    if( !bufbusy )
    {
        MyUART_TxCheck( uartsta );
    }

    return bufbusy;
}

void Tx_Cpl_Call_Back( UartStatus* uartsta )
{
    uartsta->txbusy = FALSE;
    MyUART_TxCheck( uartsta );
}

void LinDataRefreshFun( void )
{
    U16 Current;
    U8  syserror;

    Current = LinParamBak.current - u16LoadCurrent;
    if( (Current < (U16)(-100)) && (Current > 100) )
    {
        LinParamBak.current = u16LoadCurrent;
        u8NeedSend = TRUE;
    }

    if( LinParamBak.runsta != u8RunSta )
    {
        LinParamBak.runsta = u8RunSta;
        u8NeedSend = TRUE;
    }

    if( LinParamBak.systemp != u8SendTemp )
    {
        LinParamBak.systemp = u8SendTemp;
        u8NeedSend = TRUE;
    }

    if( LinParamBak.door != u8OutDoorSignl )
    {
        LinParamBak.door = u8OutDoorSignl;
        u8NeedSend = TRUE;
    }

    if( LinParamBak.lamp != u8LampOn )
    {
        LinParamBak.lamp = u8LampOn;
        u8NeedSend = TRUE;
    }

    if( LinParamBak.settemp != u8TempSet )
    {
        LinParamBak.settemp = u8TempSet;
        u8NeedSend = TRUE;
    }

    syserror = 0x00;
    if( RunErrFlag.Err.LOAD_ERR ) syserror |= 0x01;
    if( Temp.err ) syserror |= 0x02;
    if( RunErrFlag.Err.FAN_OUT_ERR ) syserror |= 0x04;
    if( RunErrFlag.Err.FAN_IN_ERR ) syserror |= 0x08;
    if( RunErrFlag.Err.TEMP_DIFF_ERR ) syserror |= 0x10;
    if( RunErrFlag.Err.RESPONE_ERR ) syserror |= 0x20;
    if( LinParamBak.err != syserror )
    {
        LinParamBak.err = syserror;
        u8NeedSend = TRUE;
    }
}

void CommunicationFun( void )
{
    U16 DataSize;

    DataSize = MyUART_Gets( &UartToCar, UartRxData );
    if( DataSize )
    {
        if( UartRxData[0]==0xBC )
        {
            IVFWOnOffReq = UartRxData[1];
            if( UartRxData[2] == 0 )
            {
                IVFWModReq = RUN_COOL;
            }
            else if( UartRxData[2] == 1 )
            {
                IVFWModReq = RUN_HEAT;
            }
            IVFTSetReq = UartRxData[3];
        }

        if( !strncmp( ( const char * )( UartRxData ), "Start Test\n", 10 ) )
        {
            u16SelfVerifyStep = 0;
            u8SysWorkSta = SYS_WORK_TEST;
        }
        else if( !strncmp( ( const char * )( UartRxData ), "End Test\n", 8 ) )
        {
            u16SelfVerifyStep = 0;
            if( u8SysWorkSta == SYS_WORK_TEST )
                u8SysWorkSta = SYS_WORK_INIT;

        }
        else if( !strncmp( ( const char * )( UartRxData ), "GO STEP:", sizeof("GO STEP:")-1 ) )
        {
            u16SelfVerifyStep = ( UartRxData[ 8 ] - 0x30 )*10 + ( UartRxData[ 9 ] - 0x30 );
        }
    }

    if( u8NeedSend )
    {
        UartTxData[0] = 0xCB;
        UartTxData[1] = u8RunSta;
        UartTxData[2] = u8SendTemp;
        UartTxData[3] = u8OutDoorSignl;
        UartTxData[4] = u8LampOn;
        UartTxData[5] = (U8)((u16LoadCurrent+50)/100);
        UartTxData[6] = LinParamBak.err;
        UartTxData[7] = u8TempSet;
        UartTxData[8] = SerialNumber[0];
        UartTxData[9] = SerialNumber[1];
        UartTxData[10] = SerialNumber[2];
        UartTxData[11] = SerialNumber[3];
        UartTxData[12] = SerialNumber[7];
        UartTxData[13] = SerialNumber[6];
        UartTxData[14] = SerialNumber[5];
        UartTxData[15] = SerialNumber[4];

        MyUART_Puts( &UartToCar, &UartTxData[0], 8 );
        u8NeedSend = 0;
    }
}
