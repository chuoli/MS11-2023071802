/*===========================================================================*/
/* Module :  RLIN_driver_user.c                                              */
/* Version:  V1.01                                                           */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2012 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose: main routine of QB-R5F10PMF-TB startup sample                    */
/*                                                                           */
/*===========================================================================*/
/* In case of any question please do not hesitate to contact:                */
/*                                                                           */
/*        Technical Product Support                                          */
/*                                                                           */
/*        Renesas Electronics Europe GmbH                                    */
/*        Arcadiastrasse 10                                                  */
/*        D-40472 Duesseldorf, Germany                                       */
/*                                                                           */
/*        e-mail: device_support.rl78-eu@lm.renesas.com                      */
/*        FAX:   +49 - (0)211 / 65 03 - 12 79                                */
/*                                                                           */
/*===========================================================================*/
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:          R5F10PPJ                                    */
/*              Target Hardware: QB-R5F10PPJ-TB                              */
/*              IDE:             IAR Systems                                 */
/*                               Embedded Workbench for RL78 V1.30.x         */
/*                                                                           */
/*===========================================================================*/
/* History:                                                                  */
/*              V1.00: Initial version                                       */
/*              V1.01: 1st revision, control of LIE0 changed                                                             */
/*===========================================================================*/

/***********************************************************************************************************************
* File Name    : RLIN_driver_user.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for Interrupt module.
* Creation Date: 02.08.2013
***********************************************************************************************************************/

#pragma interrupt INTLIN0TRM RLIN0_Transmission_interrupt
#pragma interrupt INTLIN0RVC RLIN0_Reception_interrupt
#pragma interrupt INTLIN0STA RLIN0_Status_interrupt
#pragma interrupt INTLIN0WUP RLIN0_Wakeup_interrupt

#define Communication_SRC_

#include "Main_App.h"
#include "Typedef.h"
#include "RLIN_driver.h"
#include "SystemCtrl.h"
#include "AD_NTC.h"
#include <string.h>

uint8_t GetIDbuffer;
uint8_t GiagID;
uint8_t Slave_RxData3[8];            /*reception data store array*/
uint8_t Slave_TxData[]={0x55,0xC0};  /*Transmission data store array*/

uint8_t ID11_TxData[8];
uint8_t const ID10_TxData[] = {0x50,0x00,0x00,0x03,0x45,0x20,0x01,0x00};

uint8_t const DiagTxData0[] = {0x6C,0x06,0xF2,0x00,0x00,0x00,0x00,0x07};
uint8_t const DiagTxData1[] = {0x6C,0x05,0xF2,0x00,0x00,0x00,0x00,0xFF};
uint8_t const DiagTxDataErr[] = {0x6C,0x03,0x7F,0xB2,0x12,0xFF,0xFF,0xFF};
uint8_t const SleepCommand[] = {0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


/***********************************************************************************************************************
* Function Name: RLIN0_Transmission_interrupt
* Description  : This function is RLIN0 Transmission interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//#pragma vector = INTLIN0TRM_vect
__interrupt static void RLIN0_Transmission_interrupt(void)
{
    
  LST0&=0xFE; 

}


/***********************************************************************************************************************
* Function Name: RLIN0_Reception_interrupt
* Description  : This function is RLIN0 Reception interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//#pragma vector = INTLIN0RVC_vect
__interrupt static void RLIN0_Reception_interrupt(void)
{
    uint8_t receive_header_flag;
    uint8_t receive_reponse_flag;
    
    receive_header_flag=LST0 & 0x80;          /* get header reception flag */
    receive_reponse_flag=LST0 & 0X02;         /* get response rception flag*/
    
    GetIDbuffer=LIDB0;

    u16PowerLinTime = 0;

    switch( GetIDbuffer )
    {
    case SubID:
    case ID10:
    case ID11:
    case DIAG_RX:
    case DIAG_TX:
    case PubID:
        if( LEST0 & 0X21 )
        {
            u8LinErrSta = 1;
            LEST0 = 0;
        }
        break;

    default:
        break;
    }

    if( receive_header_flag )
    {
        LST0&=0X7F;    /*clear successful header reception flag*/

        switch( GetIDbuffer )
        {
        case TestRxID:
            RLIN_Slave_Receive(TestRxID,1);
            break;
        case TestTxID:
            if( u8SysWorkSta == SYS_WORK_TEST )
            {
                LinTxData[0] = 0x3A;
                RLIN_Slave_Transmit( TestTxID, LinTxData, 8 );
            }
            break;
            
        case SubID:
            LinTxData[0] = (U8)((u16LoadCurrent+50)/100);
            LinTxData[1] = u8SendTemp;
            LinTxData[2] = LinParamBak.err;
            LinTxData[3] = ((u8RunSta<<6)&0xC0)|(u8TempSet&0x3F);
            LinTxData[4] = ((u8LoadRunSta<<4)&0x10)|((u8LoadRunSta<<3)&0x08)|((u8LampOn<<2)&0x04)|((u8OutFanOn<<1)&0x02)|(u8OutDoorSignl&0x01);
            LinTxData[5] = 0;
            LinTxData[6] = 0;
            LinTxData[7] = (u8LinErrSta<<7)&0x80;
            u8LinErrSta = 0;
            RLIN_Slave_Transmit( SubID, LinTxData, 8 );
            break;

        case ID10:
            RLIN_Slave_Transmit( ID10, ID10_TxData, 8 );
            break;

        case ID11:
            ID11_TxData[0] = SerialNumber[0];
            ID11_TxData[1] = SerialNumber[1];
            ID11_TxData[2] = SerialNumber[2];
            ID11_TxData[3] = SerialNumber[3];
            ID11_TxData[4] = SerialNumber[7];
            ID11_TxData[5] = SerialNumber[6];
            ID11_TxData[6] = SerialNumber[5];
            ID11_TxData[7] = SerialNumber[4];
            RLIN_Slave_Transmit( ID11, ID11_TxData, 8 );
            break;

        case PubID:
            RLIN_Slave_Receive(PubID,8);
            break;

        case DIAG_RX:
            RLIN_Slave_Receive(DIAG_RX,8);
            break;

        case DIAG_TX:
            if( u8DiagTime )
            {
                if(GiagID==0x00)
                {
                    RLIN_Slave_Transmit( DIAG_TX, DiagTxData0, 8 );
                }
                else if(GiagID==0x01)
                {
                    RLIN_Slave_Transmit( DIAG_TX, DiagTxData1, 8 );
                }
                else
                {
                    RLIN_Slave_Transmit( DIAG_TX, DiagTxDataErr, 8 );
                }

                GiagID = 0;
                u8DiagTime = 0;
            }
            break;

        default:
            RLIN_Slave_NoResponse( );
            break;
        }
    /*LIE0 |= 0x80; */  /*enable header reception interrupt*//*revision 1.01*/
    }

    if( receive_reponse_flag )
    {
        LST0 &= 0xFD;      /* clear response reception successful flag*/
        
        switch( GetIDbuffer )
        {
        case TestRxID:
            if( Get_LinData_Byte() == 0xBC ) u8LinSta = TRUE;
            else u8LinSta = FALSE;
            break;
            
        case PubID:
            Get_reponse_RxData( LinRxData );

            VCCDLZCUComFltSts = (LinRxData[2]>>6)&0x01;

            if( VCCDLZCUComFltSts == TRUE )
            {
                IVFWOnOffReq = FALSE;
            }
            else if( VCCDLZCUComFltSts == FALSE )
            {
                IVFWOnOffReq = (LinRxData[1]>>7)&0X01;
                if( ((LinRxData[1]>>5)&0X03) == 0 )
                {
                    IVFWModReq = RUN_COOL;
                }
                else if( ((LinRxData[1]>>5)&0X03) == 1 )
                {
                    IVFWModReq = RUN_HEAT;
                }
                IVFTSetReq = (LinRxData[2] & 0x3F);
            }
            break;

        case DIAG_RX:
            Get_reponse_RxData( DiagRxData );
            if( (DiagRxData[0]==0x6C) || (DiagRxData[0]==0x7F) )
            {
                if( (DiagRxData[1]==0x06) && (DiagRxData[2]==0xB2) )
                {
                    if( (DiagRxData[4]==0xFF&&DiagRxData[5]==0x7F) || (DiagRxData[4]==0x00&&DiagRxData[5]==0x00) )
                    {
                        if( (DiagRxData[6]==0xFF&&DiagRxData[7]==0xFF) || (DiagRxData[6]==0x00&&DiagRxData[7]==0x00) )
                        {
                            u8DiagTime = 5*SECOND;
                            GiagID = DiagRxData[3];
                        }
                    }
                }
            }

            if( !memcmp(SleepCommand,DiagRxData,sizeof(SleepCommand)) )
            {
                u8SleepOrder = 1;
            }
            break;

        default:
            break;
        }
    }


    LTRC0=0x01;    /*enabled header reception interrupt*/
}

/***********************************************************************************************************************
* Function Name: RLIN0_Status_interrupt
* Description  : This function is RLIN0 Status interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

//#pragma vector = INTLIN0_vect
__interrupt static void RLIN0_Status_interrupt(void)
{
    ;
}



/***********************************************************************************************************************
* Function Name: RLIN0_Wakeup_interrupt
* Description  : This function is RLIN0 Wakeup interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
//#pragma vector = INTLIN0WUP_vect
__interrupt static void RLIN0_Wakeup_interrupt(void)
{
    ;
}
