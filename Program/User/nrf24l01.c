#include "stm8l15x.h"
#include "nrf24l01.h"
#include "atomtimer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK STM32F407???
//NRF24L01????	   
//????@ALIENTEK
//????:www.openedv.com
//????:2014/5/9
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
    
uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xA5,0xA5,0xA5,0xA5,0xA5}; //????
uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xA5,0xA5,0xA5,0xA5,0xA5}; //????

uint8_t nrf_tx_pipe[6][TX_PLOAD_WIDTH];

uint8_t nrf_rx_pipe[6][RX_PLOAD_WIDTH];

static NRF24L01_Flag_TypeDef nrf_flags;
static NRF24L01_Status_TypeDef nrf_status;

//???24L01?IO?
void NRF24L01_Init(void)
{  
  char cnt = 0;
  GPIO_Init(NRF24L01_CE_PORT,NRF24L01_CE_PIN,GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(NRF24L01_nCS_PORT,NRF24L01_nCS_PIN,GPIO_Mode_Out_PP_High_Fast);
  
  GPIO_Init(NRF24L01_SCK_PORT,NRF24L01_SCK_PIN,GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(NRF24L01_MOSI_PORT,NRF24L01_MOSI_PIN,GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(NRF24L01_MISO_PORT,NRF24L01_MISO_PIN,GPIO_Mode_In_FL_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1,ENABLE);
  SPI_Init(SPI1,SPI_FirstBit_MSB,SPI_BaudRatePrescaler_32,SPI_Mode_Master,SPI_CPOL_Low,SPI_CPHA_1Edge,SPI_Direction_2Lines_FullDuplex,SPI_NSS_Soft,7);
  
  GPIO_Init(NRF24L01_IRQ_PORT,NRF24L01_IRQ_PIN,GPIO_Mode_In_PU_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Falling);
  
  SPI_Cmd(SPI1,ENABLE);
  atomTimerDelay(10);

  NRF24L01_CE_RESET();	
  while(NRF24L01_Check())
  {
    atomTimerDelay(1);
    cnt++;
    if(cnt > 20)
    {
      break;
    }
  }
  NRF24L01_RF_Config(0,NRF24L01_PA_Power_0dBm,NRF24L01_DataRate_1Mbps,DISABLE);
  NRF24L01_DataPipeConfig(NRF24L01_pipe0,RX_ADDRESS,5,RX_PLOAD_WIDTH,ENABLE,ENABLE);
  NRF24L01_SetTX_Address(TX_ADDRESS,5);
  NRF24L01_CRC_Config(NRF24L01_CRC_16,ENABLE);
  NRF24L01_ModeManage(NRF24L01_Mode_TX);
}
//??24L01????
//???:0,??;1,??	
uint8_t NRF24L01_Check(void)
{
	static uint8_t buf[5]={1,2,3,4,5};
	uint8_t i; 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5);//??5??????.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //???????  
	for(i=0;i<5;i++)
        {
          if(buf[i]!=TX_ADDRESS[i])break;
        }
	if(i!=5)return 1;//??24L01??	
	return 0;		 //???24L01
}	 	 
//SPI????
//reg:???????
//value:????
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
   	NRF24L01_NCS_RESET();                 //??SPI??
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
        SPI_SendData(SPI1,reg);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        status = SPI_ReceiveData(SPI1);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
  	SPI_SendData(SPI1,value);      //???????
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
  	NRF24L01_NCS_SET();                 //??SPI??	   
  	return(status);       			//?????
}
//??SPI????
//reg:??????
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
 	NRF24L01_NCS_RESET();          //??SPI??
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
  	SPI_SendData(SPI1,reg);   //??????
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        reg_val = SPI_ReceiveData(SPI1);	                        //Dummy data
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
        SPI_SendData(SPI1,NOP);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        reg_val = SPI_ReceiveData(SPI1);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
  	NRF24L01_NCS_SET();          //??SPI??		    
  	return(reg_val);           //?????
}	
//??????????????
//reg:???(??)
//*pBuf:????
//len:????
//???,??????????? 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,u8_ctr;	   
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
  	NRF24L01_NCS_RESET();           //??SPI??
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
        SPI_SendData(SPI1,reg);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        status = SPI_ReceiveData(SPI1);	                        //Dummy data
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
        SPI_SendData(SPI1,NOP);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        status = SPI_ReceiveData(SPI1);	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
        {
          while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
          SPI_SendData(SPI1,NOP);
          while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
          pBuf[u8_ctr] = SPI_ReceiveData(SPI1);//????
        }
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
  	NRF24L01_NCS_SET();       //??SPI??
  	return status;        //????????
}
//?????????????
//reg:???(??)
//*pBuf:????
//len:????
//???,???????????
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,u8_ctr;	 
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
 	NRF24L01_NCS_RESET();          //??SPI??
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
	SPI_SendData(SPI1,reg);
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
        status = SPI_ReceiveData(SPI1);
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
        {
          while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
          SPI_SendData(SPI1,*pBuf++); //????
        }
        while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY) == SET);
	NRF24L01_NCS_SET();       //??SPI??
	return status;          //????????
}				   
//??NRF24L01??????
//txbuf:????????
//???:??????
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 //	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi???10.5Mhz(24L01???SPI???10Mhz)   
	NRF24L01_CE_RESET();
        NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//????TX BUF  32???
 	NRF24L01_CE_SET();	   
//	while(NRF24L01_ReadIRQ() == SET);//??????
//	sta=NRF24L01_Read_Reg(STATUS);  //?????????	   
//	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??TX_DS?MAX_RT????
//	if(sta&MAX_TX)//????????
//	{
//		NRF24L01_Write_Reg(FLUSH_TX,0xff);//??TX FIFO??? 
//		return MAX_TX; 
//	}
//	if(sta&TX_OK)//????
//	{
//		return TX_OK;
//	}
	return 0xff;//????????
}

//uint8_t NRF24L01_RxPacket(void)
//{
//	uint8_t sta;		    							   
//	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi???10.5Mhz(24L01???SPI???10Mhz) 
//       
//	 
//        NRF24L01_Read_Buf(RD_RX_PLOAD,nrf_rx_pipe[(sta&0x0e)>>1],RX_PLOAD_WIDTH);//???
//	if(sta&RX_OK)//?????
//	{
//		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//????
//		NRF24L01_Write_Reg(FLUSH_RX,0xff);//??RX FIFO??? 
//		return 0; 
//	}
//        NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??TX_DS?MAX_RT????
//       // NRF24L01_Write_Reg(FLUSH_RX,0xff);//??RX FIFO??? 
//	return 1;//???????
//}					    


void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_RESET();	  
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//?RX????
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //????0?????    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//????0?????  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);	     //??RF????		  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//????0??????? 	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x26);//??TX????,0db??,250kbps,???????   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//???????????;PWR_UP,EN_CRC,16BIT_CRC,???? 
	NRF24L01_CE_SET(); //CE??,?????? 
}						 
 


void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE_RESET();	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//?TX???? 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //??TX????,??????ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //????0?????    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //????0?????  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0xfa);//??????????:500us + 86us;????????:10?
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,0);       //??RF???40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x26);  //??TX????,0db??,250kbps,???????   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //???????????;PWR_UP,EN_CRC,16BIT_CRC,????,??????
	NRF24L01_CE_SET();//CE??,10us?????
}

/***************************************

*****************************************/
void NRF24L01_ModeManage(NRF24L01_Mode_TypeDef rf_mode)
{
  uint8_t temp;

  temp = NRF24L01_Read_Reg(NRF_READ_REG + CONFIG);
  
  switch(rf_mode)
  {
    case NRF24L01_Mode_PowerDown:
      temp &= 0xfd;
      NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
      nrf_status = NRF24L01_Status_PDN;
      break;
    case NRF24L01_Mode_StandbyI:
      
      if((temp & 0x02) == 0)    //last mode in power down
      {
        temp |= 0x02;
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
        atomTimerDelay(5);
      }
      else
      {
        temp |= 0x02;
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
        //Delay_Us(200);
        atomTimerDelay(1);
      }
      NRF24L01_CE_RESET();
      nrf_status = NRF24L01_Status_STANDBY;
      break;
    case NRF24L01_Mode_StandbyII:
    case NRF24L01_Mode_TX:
      
      if((temp & 0x02) == 0)    //last mode in power down
      {
        temp |= 0x02;
        temp &= 0xfe;
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
        atomTimerDelay(1);
      }
      else
      {
        temp |= 0x02;
        temp &= 0xfe;
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
        NRF24L01_CE_SET();
       // Delay_Us(200);
        atomTimerDelay(1);
      }
      
      nrf_status = NRF24L01_Status_TX_IDLE;
      break;
    case NRF24L01_Mode_RX:
      temp |= 0x03;
      NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,temp);
      NRF24L01_CE_SET();
      nrf_status = NRF24L01_Status_RX_IDLE;
      //Delay_Us(200);
      atomTimerDelay(1);
      break;
    default:
    break;
  }
  
  
}

/***************************************
rf_channel: 0-63 -> 2.4 + rf_channel x 1MHz
RF_cont_state: continuous carrier transmit
*****************************************/
void NRF24L01_RF_Config(uint8_t rf_channel,NRF24L01_PA_Power_TypeDef pa_power,NRF24L01_DataRate_TypeDef rf_data_rate,FunctionalState RF_cont_state)
{
  uint8_t temp = 0x00;
 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,rf_channel);
  
  temp |= pa_power;
  temp |= rf_data_rate;
  
  if(RF_cont_state)
  {
    temp |= 0x80;
  }
  else
  {
    temp &= 0x7f;
  } 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,temp);
}


/*
for Pipe0 and pipe1, the address is 5 bytes, for pipe2 - pipe5, only LSByte is effective, MSBytes are shared with pipe1
pipe_rx_bytes : 0-32, 0: pipe not used, 1-32 bytes
*/
void NRF24L01_DataPipeConfig(NRF24L01_pipe_TypeDef pipe,uint8_t *pipe_rx_address,uint8_t pipe_rx_adr_width,uint8_t pipe_rx_bytes,FunctionalState AutoACK_NewState,FunctionalState PipeNewState)
{
  uint8_t temp,pipe_index;
  pipe_index = 0x01 << pipe;
  
  temp = NRF24L01_Read_Reg(NRF_READ_REG + EN_AA);
  if(AutoACK_NewState)
  {
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA, temp | pipe_index);
  }
  else
  {
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,temp & ~pipe_index);
  }
  
  if(pipe < 2)
  {
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0 + pipe,(uint8_t*)pipe_rx_address,pipe_rx_adr_width);
  }
  else  //only one byte address
  {
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_ADDR_P0 + pipe,*pipe_rx_address);
  }
  
  temp = NRF24L01_Read_Reg(NRF_READ_REG + EN_RXADDR);
  if(PipeNewState)
  {
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0+pipe, pipe_rx_bytes);
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR, temp | pipe_index);
  }
  else
  {
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,temp & ~pipe_index);
  }
}

void NRF24L01_CRC_Config(NRF24L01_CRC_TypeDef crc,FunctionalState crc_status)
{
  uint8_t temp;
  temp = NRF24L01_Read_Reg(NRF_READ_REG + CONFIG);
  temp &= 0xfb;
  if(crc_status)
  {
    temp |= 0x08;
  }
  else
  {
    temp &= 0xf7;
  }
  
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, temp | crc);
}

void NRF24L01_SetTX_Address(uint8_t *tx_address,uint8_t tx_adr_width)
{
  NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR,(uint8_t*)tx_address,tx_adr_width);
}

uint8_t NRF24L01_GetPendingFlags(void)
{
  uint8_t temp;
  temp = NRF24L01_Read_Reg(NRF_READ_REG + STATUS);
  return(temp&0x70);
}

void NRF24L01_ClearPendingFlag(NRF24L01_Flag_TypeDef rf_flag)
{
  uint8_t temp;
  temp = NRF24L01_Read_Reg(NRF_READ_REG + STATUS);
  NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,temp | rf_flag);
}

void NRF24L01_IRQHandler(void)
{
   nrf_flags = NRF24L01_GetPendingFlags(); 
  if(nrf_flags & NRF24L01_Flag_RX_DR)
  {
    nrf_status = NRF24L01_Status_RX_BUSY;
    
  }
  
  if(nrf_flags & NRF24L01_Flag_TX_DS)
  {
    nrf_status = NRF24L01_Status_TX_IDLE;
  }
  
  if(nrf_flags & NRF24L01_Flag_MAX_RT)
  {
    nrf_status = NRF24L01_Status_TX_ERROR;
    NRF24L01_Write_Reg(FLUSH_TX,0xff);
  }
  
  if(nrf_flags & NRF24L01_Flag_FIFO_FULL)
  {
    nrf_status = NRF24L01_Status_TX_ERROR;
    NRF24L01_Write_Reg(FLUSH_TX,0xff);
  }
  
  NRF24L01_ClearPendingFlag(NRF24L01_Flag_TX_DS | NRF24L01_Flag_RX_DR | NRF24L01_Flag_MAX_RT);
  nrf_status = NRF24L01_Status_TX_IDLE;
}