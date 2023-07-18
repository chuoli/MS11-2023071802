/*===========================================================================*/
/* Module :  RLIN_driver.c                                                   */
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
* File Name    : RLIN_driver.c
* Device(s)    : R5F10PPJ
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements device driver for PORT module.
* Creation Date: 15.07.2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#define RLIN_driver_SRC_

#include "r_cg_macrodriver.h"
#include "Typedef.h"
#include "RLIN_driver.h"
#include "Main_App.h"



/***********************************************************************************************************************
* Function Name: RLIN_Slave_Init(void)
* Description  : This function initializes the RLIN Slave node, setting clock supply,baud rate,ect.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void RLIN_Slave_Init(void)
{
   
  LCHSEL = 0x00;      /*  Selects RLIN0 */
  PER2  |= 0x04;      /*  Enable input clock supply RLIN0*/
  LINCKSEL=0x00;      /*  selects the  fclk=48MHz  clock to RLIN0.*/
  LWBR0 =  0x35;      /*  0X74.b3-b1=010: Prescaler Clock Selcet 48/4, bit sampling count select b7-b4=0011 : 4 sampling. */
  LMD0  |=  0x02;        /*  b1b0=10; LIN Slave mode (Auto baud rate),*/
  LUSC0 = 0x07;
  //LBRP00 = 0x67;      /*  lower 8bit : 0X67=103D, Baud rate= 32M/ (103+1)*16= 19230 bps*/
  //LBRP01 = 0x00;      /*  upper 8 bits in the 16bit counter of the baud rate prescaler*/
  
  LIN0RVCIF = 0U;      /*  Clear interrupt request signal */
  LIN0TRMIF = 0U;      /*  Clear interrupt request signal */
  LIN0WUPIF = 0U;      /*  Clear interrupt request signal */
  LIN0RVCMK = 0U;      /*  interrupt servicing enable */
  LIN0TRMMK = 0U;      /*  interrupt servicing enable */
  LIN0WUPMK = 0U;      /*  interrupt servicing enable */
  ISC   |=  0X04;
  LIE0  |=  0x0F;      /*  Enable successful response/wake-up reception interrupt, enable all interrupt*/
  LEDE0 |=  0x99;      /*  Enable error detection */
  
  /*Header format setting*/
  LMD0  |=  0x10;        /*  transmission interrupt,sucessful reception interrupt..., The noise filter is enable.*/
  LBFC0 =  0x00;        /*  Reception break of 9.5/10 or more Tbits*/
  LSC0  =  0x24;        /*  Response space 4bit; inter-byte space 2bit;*/
  LWUP0 =  0x30;        /*  Wake-up Transmission low width 4 bits.*/
  LIDB0&=  0x00;        /*  Clear the ID buffer */
 
  LINCKSEL|=0x10;        /*  Enable RLIN0 engine clock supply,*/

  __NOP_X5( );
  __NOP_X5( );

  LCUC0 = 0x03;
  LTRC0|= 0x01;
}

/***********************************************************************************************************************
* Function Name: RLIN_Slave_HeaderReceive(void)
* Description  : This function is setting in slave mode, enable header reception is started.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void RLIN_Slave_HeaderReceive(void)
{
  
  LCUC0 = 0x03;          /* 01: RLIN rest mode is canceled; 03:RLIN operation mode  */
  LTRC0|= 0x01;          /* FTS=1; Header reception or wake up transmission/reception is started.*/
}


/***********************************************************************************************************************
* Function Name: RLIN_Slave_Transmit(void)
* Description  : This function seting data buffer for response transmission start
* Arguments    : uint8_t* databuf    : variable array data. 
                 uint8_t Data_length : transmit data length.
* Return Value : None
***********************************************************************************************************************/

void RLIN_Slave_Transmit(uint8_t id,uint8_t* databuf,uint8_t Data_length)
{ 
  uint8_t i;
  uint16_t  Databuf_adr;   
  Clear_DataBuffer();
  if(id == DIAG_TX)
  {
    LDFC0=0x10;                    /*b5=0:Classic checksum mode; b4=1:transmission*/
  }
  else
  {
    LDFC0=0x30;                    /*b5=1:enhanced checksum mode; b4=1:transmission*/
  }
  LDFC0|=Data_length;            /* b4-b0=Data_length: response data lengh select byte*/
  Databuf_adr=RLIN_DateBuffer;   /* get the data buffer address*/
  for(i=0;i<Data_length;i++)             /* setting tansmission data to date buffer*/
  {
    *((uint8_t *)(Databuf_adr+i))=databuf[i];  
  }    
  LTRC0=0x02;                     /*setting RTS=1;Response transmission start*/   
}

/***********************************************************************************************************************
* Function Name: RLIN_Slave_Receive(void)
* Description  : This function clear data buffer for response reception start
* Arguments    : uint8_t Data_length : receive data length.
* Return Value : None
***********************************************************************************************************************/

void RLIN_Slave_Receive(uint8_t id,uint8_t Data_length)
{
    Clear_DataBuffer();
    if( id == DIAG_RX )
    {
      LDFC0=0x00;                   /*b5=0:Classic checksum mode; b4=0:Reception*/
    }
    else
    {
      LDFC0=0x20;                   /*b5=1:enhanced checksum mode; b4=0:Reception*/
    }
    LDFC0|=Data_length;           /* b4-b0=Data_length: response data lengh select byte*/
    LTRC0=0x02;                   /*setting RTS=1,response reception is started*/
}
void RLIN_Slave_NoResponse(void) 
{
  LTRC0=0x00;                     /* setting LNRR=0, No response request*/
}




/***********************************************************************************************************************
* Function Name: Clear_DataBuffer
* Description  : This function setting all data buffer to some value
* Arguments    : uint8_t x : setting data buff value
* Return Value : None
***********************************************************************************************************************/

void Clear_DataBuffer()
{   
  uint8_t i;
  uint16_t Databuf_adr;
  Databuf_adr=RLIN_DateBuffer;
  for(i=0;i<8;i++)
  {
   *((uint8_t *)(Databuf_adr+i))=0U;
  } 

}

/***********************************************************************************************************************
* Function Name: Get_reponse_RxData
* Description  : This function get data buffer value to a variable array  
* Arguments    : uint8_t * RxData : a avriable array for store Data
* Return Value : None
***********************************************************************************************************************/
uint8_t  Get_reponse_RxData(uint8_t * RxData)
{
  uint8_t i,k;
  uint16_t Databuf_adr;
  k=LDFC0&0x0F;
  Databuf_adr=RLIN_DateBuffer;
  for(i=0;i<k;i++)
  {
    RxData[i]=(*((uint8_t *)(Databuf_adr+i)));  
  } 
  return TRUE;
}

uint8_t Get_LinData_Byte( void )
{
    uint8_t data;
    uint16_t Databuf_adr;

    Databuf_adr = RLIN_DateBuffer;

    data = (*((uint8_t *)Databuf_adr));

    return data;
}


