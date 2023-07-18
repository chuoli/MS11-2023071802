
#ifndef Uart_H_
#define Uart_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef Uart_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define SIZE_BUF_RX   64
#define SIZE_BUF_TX   64

typedef struct
{
    U8 uartnum;
    
    U8 rxdata;
    U16 rxsize;
    U16 rxdataindex;
    U16 getdataindex;

    U8 txbusy;
    U16 txdataindex;
    U16 putdataindex;

    U8 putdata[ SIZE_BUF_TX ];
    U8 getdata[ SIZE_BUF_RX ];
} UartStatus;
EXTERN UartStatus UartToCar;

EXTERN U8 UartRxData[ SIZE_BUF_RX ];
EXTERN U8 UartTxData[ SIZE_BUF_TX ];
EXTERN U8 u8NeedSend;


EXTERN void MyUART_Init( void );
EXTERN U8 MyUART_Getc( UartStatus * uartsta, U8 * pdata );
EXTERN U16 MyUART_Gets( UartStatus * uartsta, U8 * pdata );
EXTERN U16 MyUART_GetFrames( UartStatus * uartsta, U8 * pdata );
EXTERN U8 MyUART_Puts( UartStatus * uartsta, U8 * pdata, U16 size );
EXTERN void MyUART_TxCheck( UartStatus* uartsta );
EXTERN void Tx_Cpl_Call_Back( UartStatus * uartsta );
EXTERN void Rx_Byte_Call_Back( UartStatus * uartsta, U8 data );

EXTERN void LinDataRefreshFun( void );
EXTERN void CommunicationFun( void );

#endif

