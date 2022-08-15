/*
 * Copyright (c) 2010, Kelvin Lawson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>

#include "atom.h"
#include "atomport-private.h"
#include "atomport-tests.h"
#include "atomtests.h"
#include "atomtimer.h"
#include "uart.h"
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

#include "lcd19264.h"
#include "system.h"
#include "encoder.h"
#include "ui.h"
#include "nrf24l01.h"
#include "ppm.h"
#include "adc.h"
   #include "joystick.h"
/* Constants */

/*
 * Idle thread stack size
 *
 * This needs to be large enough to handle any interrupt handlers
 * and callbacks called by interrupt handlers (e.g. user-created
 * timer callbacks) as well as the saving of all context when
 * switching away from this thread.
 *
 * In this case, the idle stack is allocated on the BSS via the
 * idle_thread_stack[] byte array.
 */
#define IDLE_STACK_SIZE_BYTES       128


/*
 * Main thread stack size
 *
 * Note that this is not a required OS kernel thread - you will replace
 * this with your own application thread.
 *
 * In this case the Main thread is responsible for calling out to the
 * test routines. Once a test routine has finished, the test status is
 * printed out on the UART and the thread remains running in a loop
 * flashing a LED.
 *
 * The Main thread stack generally needs to be larger than the idle
 * thread stack, as not only does it need to store interrupt handler
 * stack saves and context switch saves, but the application main thread
 * will generally be carrying out more nested function calls and require
 * stack for application code local variables etc.
 *
 * With all OS tests implemented to date on the STM8, the Main thread
 * stack has not exceeded 384 bytes. To allow all tests to run we set
 * a minimum main thread stack size of 204 bytes. This may increase in
 * future as the codebase changes but for the time being is enough to
 * cope with all of the automated tests.
 */
#define MAIN_STACK_SIZE_BYTES       256
#define ANALOG_STACK_SIZE_BYTES       256
#define DISPLAY_STACK_SIZE_BYTES       128
#define COMMUNICATION_STACK_SIZE_BYTES       128
/*
 * Startup code stack
 *
 * Some stack space is required at initial startup for running the main()
 * routine. This stack space is only temporarily required at first bootup
 * and is no longer required as soon as the OS is started. By default
 * Cosmic sets this to the top of RAM and it grows down from there.
 *
 * Because we only need this temporarily you may reuse the area once the
 * OS is started, and are free to use some area other than the top of RAM.
 * For convenience we just use the default region here.
 */


/* Local data */

/* Application threads' TCBs */
static ATOM_TCB main_tcb;
static ATOM_TCB analog_tcb;
static ATOM_TCB display_tcb;
static ATOM_TCB communication_tcb;
/* Main thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t main_thread_stack[MAIN_STACK_SIZE_BYTES];
NEAR static uint8_t analog_thread_stack[ANALOG_STACK_SIZE_BYTES];
NEAR static uint8_t display_thread_stack[DISPLAY_STACK_SIZE_BYTES];
NEAR static uint8_t communication_thread_stack[COMMUNICATION_STACK_SIZE_BYTES];
/* Idle thread's stack area (large so place outside of the small page0 area on STM8) */
NEAR static uint8_t idle_thread_stack[IDLE_STACK_SIZE_BYTES];


/* Forward declarations */
static void main_thread_func (uint32_t param);
static void analog_thread_func (uint32_t param);
static void display_thread_func (uint32_t param);
static void communication_thread_func (uint32_t param);
/**
 * \b main
 *
 * Program entry point.
 *
 * Sets up the STM8 hardware resources (system tick timer interrupt) necessary
 * for the OS to be started. Creates an application thread and starts the OS.
 *
 * If the compiler supports it, stack space can be saved by preventing
 * the function from saving registers on entry. This is because we
 * are called directly by the C startup assembler, and know that we will
 * never return from here. The NO_REG_SAVE macro is used to denote such 
 * functions in a compiler-agnostic way, though not all compilers support it.
 *
 */
NO_REG_SAVE void main ( void )
{
    int8_t status;

    /**
     * Note: to protect OS structures and data during initialisation,
     * interrupts must remain disabled until the first thread
     * has been restored. They are reenabled at the very end of
     * the first thread restore, at which point it is safe for a
     * reschedule to take place.
     */

    /* Initialise the OS before creating our threads */
    status = atomOSInit(&idle_thread_stack[0], IDLE_STACK_SIZE_BYTES, TRUE);
    if (status == ATOM_OK)
    {
        /* Enable the system tick timer */
        archInitSystemTickTimer();

        /* Create an application thread */
        status = atomThreadCreate(&main_tcb,
                     TEST_THREAD_PRIO, main_thread_func, 0,
                     &main_thread_stack[0],
                     MAIN_STACK_SIZE_BYTES,
                     TRUE);
        if (status == ATOM_OK)
        {
            /**
             * First application thread successfully created. It is
             * now possible to start the OS. Execution will not return
             * from atomOSStart(), which will restore the context of
             * our application thread and start executing it.
             *
             * Note that interrupts are still disabled at this point.
             * They will be enabled as we restore and execute our first
             * thread in archFirstThreadRestore().
             */
            atomOSStart();
        }
    }

    /* There was an error starting the OS if we reach here */
    while (1)
    {
    }

}

static void analog_thread_func (uint32_t param)
{
  char cnt = 0;
  JoystickInit();
  printf("------------------------------------------\n");
  while(1)
  {
    
    if(SYSTEM_ADC_UpdateFlag)  
    {
      SYSTEM_ADC_UpdateFlag = RESET;
      
      for(cnt = 0;cnt<8;cnt++)
      {
        if(cnt < 4)
        {
          JoystickChannel[cnt].ChannelData = JOYSTICK_Update(ADC_Results+1+cnt,JoystickChannel[cnt].JoystickType,JoystickChannel[cnt].ChannelReverseFlag);
      
        }
        else
        {
          JoystickChannel[cnt].ChannelData = JOYSTICK_Update(ADC_Results+2+cnt,JoystickChannel[cnt].JoystickType,JoystickChannel[cnt].ChannelReverseFlag);
      
        }
      }
      
      //
    }
   
    atomTimerDelay (50);
  }
  
  
}

static void display_thread_func (uint32_t param)
{
  char cnt = 0;
  char menu_last; 
  UI_Init();
  while(1)
  {
    if(menu_last != MenuList.Page)
    {
      LCD_ClearScreen(0);
      menu_last = MenuList.Page;
    }
    switch(MenuList.Page)
    {
    case 0:
      for(cnt = 0;cnt<8;cnt++)
      {
       
          LCD_Put6x8Char(0,cnt,cnt+48);
          if(JoystickChannel[cnt].ChannelData & CHANNEL_NEGATIVE_SIGN)
          {
            LCD_Put6x8Char(12,cnt,'-');
          }
          else
          {
            LCD_Put6x8Char(12,cnt,' ');
          }
          LCD_Put6x8IntNumber(18,cnt,JoystickChannel[cnt].ChannelData & 0x0fff);
          if(JoystickChannel[cnt].ChannelData&AUTO_ZERO_TYPE)   //auto zero type
          {
            if(JoystickChannel[cnt].ChannelData & CHANNEL_NEGATIVE_SIGN)
            {
              LCD_Draw8BitBar(60,127,cnt,(33 - (JoystickChannel[cnt].ChannelData & 0x0fff)/57));
            }
            else
            {
              LCD_Draw8BitBar(60,127,cnt,(33 + (JoystickChannel[cnt].ChannelData & 0x0fff)/57));
            }
            
          }
          else
          {
            LCD_Draw8BitBar(60,127,cnt,(JoystickChannel[cnt].ChannelData & 0x0fff)/66);
          }
          
      }
      break;
    case 1:
      LCD_Put8x16Str(0,4,"Window 1");
      break;
    case 2:
      LCD_Put8x16Str(0,4,"Window 2");
      break;
    case 3:
      LCD_Put8x16Str(0,4,"Window 3");
      break;
    case 4:
      LCD_Put8x16Str(10,4,"Powering off...");
      break;
    default:
      LCD_Put8x16Str(0,4,"Default");
      break;
    }
      
      
    LCD_LightIconBattery((ADC_Results[5]-1862)/186,ENABLE);
    
    atomTimerDelay (20);
  }
  
  
}


static void communication_thread_func (uint32_t param)
{
  uint8_t nrf_status,command_received = 0;
  uint8_t cnt;
  while(1)
  {
    
//    JoystickChannel[0].ChannelData = nrf_tx_pipe[0][0]<<8;
//    JoystickChannel[0].ChannelData |= nrf_tx_pipe[0][1];
//    
//    JoystickChannel[1].ChannelData = nrf_tx_pipe[0][2]<<8;
//    JoystickChannel[1].ChannelData |= nrf_tx_pipe[0][3];
//    
//    JoystickChannel[2].ChannelData = nrf_tx_pipe[0][4]<<8;
//    JoystickChannel[2].ChannelData |= nrf_tx_pipe[0][5];
//    
//    JoystickChannel[3].ChannelData = nrf_tx_pipe[0][6]<<8;
//    JoystickChannel[3].ChannelData |= nrf_tx_pipe[0][7];
    
    for(cnt = 0; cnt<8; cnt++)
    {
      nrf_tx_pipe[0][cnt*2] = (JoystickChannel[cnt].ChannelData)>>8;
      nrf_tx_pipe[0][cnt*2+1] = JoystickChannel[cnt].ChannelData &0x00ff;
    }
//    nrf_tx_pipe[0][0] = (JoystickChannel[0].ChannelData &0x0fff)>>8;
//    nrf_tx_pipe[0][1] = JoystickChannel[0].ChannelData &0x00ff;
//    nrf_tx_pipe[0][2] = (JoystickChannel[1].ChannelData &0x0fff)>>8;
//    nrf_tx_pipe[0][3] = JoystickChannel[1].ChannelData &0x00ff;
//    nrf_tx_pipe[0][4] = (JoystickChannel[2].ChannelData &0x0fff)>>8;
//    nrf_tx_pipe[0][5] = JoystickChannel[2].ChannelData &0x00ff;
//    nrf_tx_pipe[0][6] = (JoystickChannel[3].ChannelData &0x0fff)>>8;
//    nrf_tx_pipe[0][7] = JoystickChannel[3].ChannelData &0x00ff;
    
    NRF24L01_TxPacket(nrf_tx_pipe[0]);
   
    atomTimerDelay (50);
  }
  
  
}
/**
 * \b main_thread_func
 *
 * Entry point for main application thread.
 *
 * This is the first thread that will be executed when the OS is started.
 *
 * @param[in] param Unused (optional thread entry parameter)
 *
 * @return None
 */
static void main_thread_func (uint32_t param)
{
    int8_t status;
    uint8_t tx_buf[10]="channelone";
    /* Compiler warnings */
    param = param;
    /* Initialise UART (9600bps) */
    if (uart_init(115200) != 0)
    {
        /* Error initialising UART */
    }

    /* Put a message out on the UART */
    printf ("Go\n");
    ENCODER_Init();
    PPM_Init();
    SYSTEM_Init();
    UI_Init();
    NRF24L01_Init();
    LCD19264_Init();
    Analog_Init();
    
    status = atomThreadCreate(&display_tcb,
                     TEST_THREAD_PRIO, display_thread_func, 0,
                     &display_thread_stack[0],
                     DISPLAY_STACK_SIZE_BYTES,
                     TRUE);
    
    status = atomThreadCreate(&analog_tcb,
                     TEST_THREAD_PRIO, analog_thread_func, 0,
                     &analog_thread_stack[0],
                     ANALOG_STACK_SIZE_BYTES,
                     TRUE);
    
    status = atomThreadCreate(&communication_tcb,
                     TEST_THREAD_PRIO, communication_thread_func, 0,
                     &communication_thread_stack[0],
                     COMMUNICATION_STACK_SIZE_BYTES,
                     TRUE);

    
    while (1)
    {
      
      if(SYSTEM_ReadPowerKey() == RESET)
      {
        atomTimerDelay (100);
        if(SYSTEM_ReadPowerKey() != RESET)
        {
          MenuList.Page++;
          if(MenuList.Page == MenuList.PageMax - 1)
          {
            MenuList.Page = 0;
          }
        }
        else
        {
          atomTimerDelay (500);
          if(SYSTEM_ReadPowerKey()== RESET)
          {
            MenuList.Page = MenuList.PageMax;
            TIM2_CtrlPWMOutputs(DISABLE);
            SYSTEM_PowerOff();
          }
        }
      }
   
//          switch(SystemVariables.SystemKey)
//          {
//            case Key_Push:
//              ENCODER_BeeperBeep(5);
//              NRF24L01_TxPacket(tx_buf);
//              break;
//            case Key_Increase:
//            case Key_Decrease:
//              
//              ENCODER_BeeperClickSound();
//              
//              break;
//            default:
//              break;
//          }
         // SystemVariables.SystemKey = Key_Null;
       //   UI_KeyProcess(&SystemVariables.SystemKey);
        /* Sleep then toggle LED again */
        atomTimerDelay (1);
    }
}

