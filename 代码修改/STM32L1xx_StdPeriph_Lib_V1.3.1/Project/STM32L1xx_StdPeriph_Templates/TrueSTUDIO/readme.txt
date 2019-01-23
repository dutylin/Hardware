/**
  @page truestudio TrueSTUDIO Project Template for STM32L1xx devices
 
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   This sub-directory contains all the user-modifiable files needed to
  *          create a new project linked with the STM32L1xx Standard Peripherals
  *          Library and working with TrueSTUDIO software toolchain.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  * 
  ******************************************************************************
  @endverbatim
 
 @par Directory contents
 
 - .cproject/.project: A pre-configured project file with the provided library
                       structure that produces an executable image with TrueSTUDIO.

 - STM32L1xxxx_FLASH.ld: This file is the TrueSTUDIO linker script used to 
                         place program code (readonly) in internal FLASH and
                         data (readwrite, Stack and Heap)in internal SRAM. 
                         You can customize this file to your need.
                                                          
 @par How to use it ?

 - Open the TrueSTUDIO toolchain.
 - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
   directory.
 - Click on File->Import, select General->'Existing Projects into Workspace' 
   and then click "Next". 
 - Browse to the TrueSTUDIO workspace directory and select the project: 
     - STM32L1XX_MD(STM32L1xxxBxx): to configure the project for STM32L Ultra Low Power Medium-density devices.
                                    You can use STMicroelectronics STM32L152-EVAL board to run this project. 

     - STM32L1XX_MDP(STM32L1xxxCxx): to configure the project for STM32L Ultra Low Power Medium-density Plus devices.
                                    You can use STMicroelectronics STM32L152C-Discovery board to run this project.

     - STM32L1XX_HD(STM32L1xxxDxx): to configure the project for STM32L Ultra Low Power High-density devices.
                                    You can use STMicroelectronics STM32L152D-EVAL board to run this project.

     - STM32L1XX_XL(STM32L1xxxExx): to configure the project for STM32L Ultra Low Power XL-density devices.
                                    You can use STMicroelectronics STM32L152xE-Nucleo board to run this project.

 - Rebuild all project files: Select the project in the "Project explorer" 
   window then click on Project->build project menu.
 - Run program: Select the project in the "Project explorer" window then click 
   Run->Debug (F11)

@note
- Ultra Low Power Medium-density devices: - STM32L151x6xx, STM32L151x8xx, STM32L151xBxx, STM32L152x6xx,
                                            STM32L152x8xx, STM32L152xBxx, STM32L151x6xxA, STM32L151x8xxA,
                                            STM32L151xBxxA, STM32L152x6xxA, STM32L152x8xxA and STM32L152xBxxA
                                          - STM32L100x6xx, STM32L100x8xx and STM32L100xBxx
- Ultra Low Power Medium-density Plus devices: - STM32L151xCxx, STM32L152xCxx and STM32L162xCxx 
                                               - STM32L100xCxx
- Ultra Low Power High-density devices: STM32L151xDxx, STM32L152xDxx and STM32L162xDxx
- Ultra Low Power XL-density devices: STM32L151xExx, STM32L152xExx and STM32L162xExx

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

