/*===========================================================================*/
/* Module :  RLIN_driver.h                                                   */
/* Version:  V1.00                                                           */
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
/*                                                                           */
/*===========================================================================*/

/***********************************************************************************************************************
* File Name    : RLIN_Driver.h
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for PORT module.
* Creation Date: 15.07.2013
***********************************************************************************************************************/
#ifndef RLIN_DRIVER_H_
#define RLIN_DRIVER_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef RLIN_driver_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define RLIN_DateBuffer     0x06D8

#define TestTxID        0xC1    //ID 0X01
#define TestRxID        0x42    //ID 0X02
#define SubID           0x92    //ID 0X12
#define PubID           0x08    //ID 0X08
#define ID10            0x50
#define ID11            0x11
#define DIAG_RX         0x3C    //ID 0X3C
#define DIAG_TX         0x7D    //ID 0X3D

EXTERN U8 LinTxData[8];
EXTERN U8 LinRxData[8];
EXTERN U8 DiagRxData[8];


EXTERN void Clear_DataBuffer(void);
EXTERN uint8_t Get_LinData_Byte( void );
EXTERN uint8_t Get_reponse_RxData(uint8_t * RxData);
EXTERN void RLIN_Slave_Init(void);      /* init Slave RLIN0*/
EXTERN void RLIN_Slave_HeaderReceive(void);
EXTERN void RLIN_Slave_Transmit(uint8_t id,uint8_t* databuf,uint8_t Data_length);
EXTERN void RLIN_Slave_Receive(uint8_t id,uint8_t Data_length);
EXTERN void RLIN_Slave_NoResponse(void);

#endif

