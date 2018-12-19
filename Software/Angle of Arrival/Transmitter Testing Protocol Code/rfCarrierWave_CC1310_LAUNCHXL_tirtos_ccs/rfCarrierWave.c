/*
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/

/* EasyLink API Header files */
#include "easylink/EasyLink.h"

/* TI Drivers */
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/rf/RF.h>

/* Board Header files */ 
#include "Board.h"

/* Application Header files */
#include "smartrf_settings/smartrf_settings.h"


/***** Defines *****/

/***** Prototypes *****/

/***** Variable declarations *****/

static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver handle */
static PIN_Handle pinHandle;
static PIN_State pinState;

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] = {
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_PIN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#if defined __CC1352R1_LAUNCHXL_BOARD_H__
    Board_DIO30_RFSW | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
    PIN_TERMINATE
};

/***** Function definitions *****/

void *mainThread(void *arg0)
{
    /* Configure the radio for Proprietary mode */
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* In order to achieve +14dBm output power, make sure .txPower = 0xa73f, and
    that the define CCFG_FORCE_VDDR_HH = 0x1 in ccfg.c */

    /* Open LED pins */
    pinHandle = PIN_open(&pinState, pinTable);

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Send CMD_FS and wait until it has completed */
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Explicitly configure CW (1) or Modulated (0). Default modulated mode is PRBS-15. */
    RF_cmdTxTest.config.bUseCw = 0;

    // Set a relative end trigger 500ms after execution start.
    RF_cmdTxTest.endTrigger.triggerType = TRIG_REL_START;
    RF_cmdTxTest.endTime = EasyLink_ms_To_RadioTime(20000);

    //Set LED indicating Modulated Signal
    PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);
    PIN_setOutputValue(pinHandle, Board_PIN_LED2, 0);

    /* Send CMD_TX_TEST for random code which sends for 15 seconds */
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdTxTest, RF_PriorityNormal, NULL, 0);

    /* Send CMD_FS and wait until it has completed */
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Explicitly configure CW (1) or Modulated (0). Default modulated mode is PRBS-15. */
    RF_cmdTxTest.config.bUseCw = 1;

    // Set back to infinite mode.
    RF_cmdTxTest.endTrigger.triggerType = 0x1;
    RF_cmdTxTest.endTime = 0x00000000;

    //Set LED indicating CW Signal
    PIN_setOutputValue(pinHandle, Board_PIN_LED1, 0);
    PIN_setOutputValue(pinHandle, Board_PIN_LED2, 1);

    /* Send CMD_TX_TEST for continuous wave which sends infinitely */
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdTxTest, RF_PriorityNormal, NULL, 0);

    /* Should never come here */
    while (1);
}
