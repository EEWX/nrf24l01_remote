///////////////////////////////////////////////////////////////////////////////
//
// IAR C/C++ Compiler V2.20.1.176 for STM8                20/Feb/2019  21:20:04
// Copyright 2010-2015 IAR Systems AB.
// Standalone license - IAR Embedded Workbench for STMicroelectronics STM8
//
//    Source file  =  C:\Users\wesley\Desktop\JBC STM8L151\User\main.c
//    Command line =  
//        "C:\Users\wesley\Desktop\JBC STM8L151\User\main.c" -e -Om --no_unroll
//        --no_inline --no_tbaa --no_cross_call --debug --code_model small
//        --data_model medium -o "C:\Users\wesley\Desktop\JBC
//        STM8L151\Debug\Obj" --dlib_config "C:\Program Files (x86)\IAR
//        Systems\Embedded Workbench 7.3\stm8\LIB\dlstm8smn.h" -D STM8L15X_MD
//        -lC "C:\Users\wesley\Desktop\JBC STM8L151\Debug\List" -lA
//        "C:\Users\wesley\Desktop\JBC STM8L151\Debug\List" -I
//        "C:\Users\wesley\Desktop\JBC STM8L151\STM8L15x_StdPeriph_Driver\inc\"
//        -I "C:\Users\wesley\Desktop\JBC STM8L151\User\"
//    List file    =  C:\Users\wesley\Desktop\JBC STM8L151\Debug\List\main.s
//
///////////////////////////////////////////////////////////////////////////////

        RTMODEL "__SystemLibrary", "DLib"
        RTMODEL "__code_model", "small"
        RTMODEL "__core", "stm8"
        RTMODEL "__data_model", "medium"
        RTMODEL "__rt_version", "4"

        PUBLIC main
        
          CFI Names cfiNames0
          CFI StackFrame CFA SP DATA
          CFI Resource A:8, XL:8, XH:8, YL:8, YH:8, SP:16, CC:8, PC:24, PCL:8
          CFI Resource PCH:8, PCE:8, ?b0:8, ?b1:8, ?b2:8, ?b3:8, ?b4:8, ?b5:8
          CFI Resource ?b6:8, ?b7:8, ?b8:8, ?b9:8, ?b10:8, ?b11:8, ?b12:8, ?b13:8
          CFI Resource ?b14:8, ?b15:8
          CFI ResourceParts PC PCE, PCH, PCL
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 1
          CFI DataAlign 1
          CFI ReturnAddress PC CODE
          CFI CFA SP+2
          CFI A Undefined
          CFI XL Undefined
          CFI XH Undefined
          CFI YL Undefined
          CFI YH Undefined
          CFI CC Undefined
          CFI PC Concat
          CFI PCL Frame(CFA, 0)
          CFI PCH Frame(CFA, -1)
          CFI PCE SameValue
          CFI ?b0 Undefined
          CFI ?b1 Undefined
          CFI ?b2 Undefined
          CFI ?b3 Undefined
          CFI ?b4 Undefined
          CFI ?b5 Undefined
          CFI ?b6 Undefined
          CFI ?b7 Undefined
          CFI ?b8 SameValue
          CFI ?b9 SameValue
          CFI ?b10 SameValue
          CFI ?b11 SameValue
          CFI ?b12 SameValue
          CFI ?b13 SameValue
          CFI ?b14 SameValue
          CFI ?b15 SameValue
          CFI EndCommon cfiCommon0
        
// C:\Users\wesley\Desktop\JBC STM8L151\User\main.c
//    1 /**
//    2   ******************************************************************************
//    3   * @file    Project/STM8L15x_StdPeriph_Template/main.c
//    4   * @author  MCD Application Team
//    5   * @version V1.6.1
//    6   * @date    30-September-2014
//    7   * @brief   Main program body
//    8   ******************************************************************************
//    9   * @attention
//   10   *
//   11   * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
//   12   *
//   13   * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
//   14   * You may not use this file except in compliance with the License.
//   15   * You may obtain a copy of the License at:
//   16   *
//   17   *        http://www.st.com/software_license_agreement_liberty_v2
//   18   *
//   19   * Unless required by applicable law or agreed to in writing, software 
//   20   * distributed under the License is distributed on an "AS IS" BASIS, 
//   21   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   22   * See the License for the specific language governing permissions and
//   23   * limitations under the License.
//   24   *
//   25   ******************************************************************************
//   26   */
//   27 
//   28 /* Includes ------------------------------------------------------------------*/
//   29 #include "stm8l15x.h"
//   30 
//   31 /** @addtogroup STM8L15x_StdPeriph_Template
//   32   * @{
//   33   */
//   34 
//   35 /* Private typedef -----------------------------------------------------------*/
//   36 /* Private define ------------------------------------------------------------*/
//   37 /* Private macro -------------------------------------------------------------*/
//   38 /* Private variables ---------------------------------------------------------*/
//   39 /* Private function prototypes -----------------------------------------------*/
//   40 
//   41 /* Private functions ---------------------------------------------------------*/
//   42 
//   43 /**
//   44   * @brief  Main program.
//   45   * @param  None
//   46   * @retval None
//   47   */

        SECTION `.near_func.text`:CODE:REORDER:NOROOT(0)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function main
        CODE
//   48 void main(void)
//   49 {
//   50   /* Infinite loop */
//   51   while (1)
main:
??main_0:
        JRA       L:??main_0
//   52   {
//   53   }
//   54 }
          CFI EndBlock cfiBlock0

        SECTION VREGS:DATA:REORDER:NOROOT(0)

        END
//   55 
//   56 #ifdef  USE_FULL_ASSERT
//   57 
//   58 /**
//   59   * @brief  Reports the name of the source file and the source line number
//   60   *   where the assert_param error has occurred.
//   61   * @param  file: pointer to the source file name
//   62   * @param  line: assert_param error line source number
//   63   * @retval None
//   64   */
//   65 void assert_failed(uint8_t* file, uint32_t line)
//   66 { 
//   67   /* User can add his own implementation to report the file name and line number,
//   68      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//   69 
//   70   /* Infinite loop */
//   71   while (1)
//   72   {
//   73   }
//   74 }
//   75 #endif
//   76 
//   77 /**
//   78   * @}
//   79   */
//   80 
//   81 /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
// 
// 2 bytes in section .near_func.text
// 
// 2 bytes of CODE memory
//
//Errors: none
//Warnings: none
