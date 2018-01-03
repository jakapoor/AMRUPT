/* ===========================================================================
* Code Composer Studio Version:
*       7.3.0.00019
* Edited:
*       12/27/17 (nds64)
* TODO:
*       Line 206
* ==========================================================================*/
/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
#include <stdlib.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>

/* Display */
#include <ti/mw/display/Display.h>

/* Board Header files */
#include "Board.h"

/* RF settings */
#include "smartrf_settings/smartrf_settings.h"


/***** Defines *****/
/* Wake-on-Radio configuration */
#define WOR_WAKEUPS_PER_SECOND 0.2

/* TX number of random payload bytes */
#define PAYLOAD_LENGTH 30

/* WOR Example configuration defines */
#define WOR_PREAMBLE_TIME_RAT_TICKS(x) \
    ((uint32_t)(4000000*(1.0f/(x))))

/* TX task stack size and priority */
#define TX_TASK_STACK_SIZE 1024
#define TX_TASK_PRIORITY   2


/***** Prototypes *****/
static void txTaskFunction(UArg arg0, UArg arg1);
static void initializeTxAdvCmdFromTxCmd(rfc_CMD_PROP_TX_ADV_t* RF_cmdPropTxAdv, rfc_CMD_PROP_TX_t* RF_cmdPropTx);


/***** Variable declarations *****/
/* TX task objects and task stack */
static Task_Params txTaskParams;
Task_Struct txTask;    /* not static so you can see in ROV */
static uint8_t txTaskStack[TX_TASK_STACK_SIZE];

/* TX packet payload (length +1 to fit length byte) and sequence number */
static uint8_t packet[PAYLOAD_LENGTH +1];
static uint16_t seqNumber;

/* RF driver objects and handles */
static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver objects and handles */
static PIN_Handle ledPinHandle;
static PIN_Handle buttonPinHandle;
static PIN_State ledPinState;
static PIN_State buttonPinState;

/* TX Semaphore */
static Semaphore_Struct txSemaphore;
static Semaphore_Handle txSemaphoreHandle;

/* Advanced TX command for sending long preamble */
static rfc_CMD_PROP_TX_ADV_t RF_cmdPropTxAdv;

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] =
{
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config buttonPinTable[] = {
    Board_BUTTON0 | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};


/***** Function definitions *****/
/* Pin interrupt Callback function board buttons configured in the pinTable. */
void buttonCallbackFunction(PIN_Handle handle, PIN_Id pinId) {

    /* Simple debounce logic, only toggle if the button is still pushed (low) */
    CPUdelay((uint32_t)((48000000/3)*0.050f));
    if (!PIN_getInputValue(pinId)) {
        /* Post TX semaphore to TX task */
        Semaphore_post(txSemaphoreHandle);
    }
}

/* TX task initialization function. Runs once from main() */
void txTaskInit()
{
    /* Initialize TX semaphore */
    Semaphore_construct(&txSemaphore, 0, NULL);
    txSemaphoreHandle = Semaphore_handle(&txSemaphore);

    /* Initialize and create TX task */
    Task_Params_init(&txTaskParams);
    txTaskParams.stackSize = TX_TASK_STACK_SIZE;
    txTaskParams.priority = TX_TASK_PRIORITY;
    txTaskParams.stack = &txTaskStack;
    Task_construct(&txTask, txTaskFunction, &txTaskParams, NULL);
}

/* TX task function. Executed in Task context by TI-RTOS when the scheduler starts. */
static void txTaskFunction(UArg arg0, UArg arg1)
{
    /* Initialize the display and try to open both UART and LCD types of display. */
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_BOTH;
    Display_Handle uartDisplayHandle = Display_open(Display_Type_UART, &params);
    Display_Handle lcdDisplayHandle = Display_open(Display_Type_LCD, &params);

    /* Print initial display content on both UART and any LCD present */
    Display_print0(uartDisplayHandle, 0, 0, "Wake-on-Radio TX");
    Display_print1(uartDisplayHandle, 0, 0, "Pkts sent: %u", seqNumber);
    Display_print0(lcdDisplayHandle, 0, 0, "Wake-on-Radio TX");
    Display_print1(lcdDisplayHandle, 1, 0, "Pkts sent: %u", seqNumber);

    /* Setup callback for button pins */
    if (PIN_registerIntCb(buttonPinHandle, &buttonCallbackFunction) != 0) {
        System_abort("Error registering button callback function");
    }

    /* Initialize the radio */
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* Initialize TX_ADV command from TX command */
    initializeTxAdvCmdFromTxCmd(&RF_cmdPropTxAdv, &RF_cmdPropTx);

    /* Set application specific fields */
    RF_cmdPropTxAdv.pktLen = PAYLOAD_LENGTH +1; /* +1 for length byte */
    RF_cmdPropTxAdv.pPkt = packet;
    RF_cmdPropTxAdv.preTrigger.triggerType = TRIG_REL_START;
    RF_cmdPropTxAdv.preTime = WOR_PREAMBLE_TIME_RAT_TICKS(WOR_WAKEUPS_PER_SECOND);

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Enter main TX loop */
    while(1)
    {
        /* Wait for a button press */
        Semaphore_pend(txSemaphoreHandle, BIOS_WAIT_FOREVER);


    /* ===========================================================================
     * TODO: Add in payload information (Checksum, Global Time, etc) - nds64
     * ==========================================================================*/
        /* Create packet with incrementing sequence number and random payload */
        packet[0] = PAYLOAD_LENGTH;
        packet[1] = (uint8_t)(seqNumber++);
        packet[2] = (uint8_t)(RF_getCurrentTime());
        uint8_t i;
        for (i = 3; i < PAYLOAD_LENGTH +1; i++)
        {
            packet[i] = rand();
        }
    /* ==========================================================================*/x

        /* Send packet */
        RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropTxAdv, RF_PriorityNormal, NULL, 0);

        /* Toggle LED */
        PIN_setOutputValue(ledPinHandle, Board_LED1, !PIN_getOutputValue(Board_LED1));
    }
}

/* Copy the basic RX configuration from CMD_PROP_RX to CMD_PROP_RX_SNIFF command. */
static void initializeTxAdvCmdFromTxCmd(rfc_CMD_PROP_TX_ADV_t* RF_cmdPropTxAdv, rfc_CMD_PROP_TX_t* RF_cmdPropTx)
{
    #define RADIO_OP_HEADER_SIZE 14

    /* Copy general radio operation header from TX commmand to TX_ADV */
    memcpy(RF_cmdPropTxAdv, RF_cmdPropTx, RADIO_OP_HEADER_SIZE);

    /* Set command to CMD_PROP_TX_ADV */
    RF_cmdPropTxAdv->commandNo = CMD_PROP_TX_ADV;

    /* Copy over relevant parameters */
    RF_cmdPropTxAdv->pktConf.bFsOff = RF_cmdPropTx->pktConf.bFsOff;
    RF_cmdPropTxAdv->pktConf.bUseCrc = RF_cmdPropTx->pktConf.bUseCrc;
    RF_cmdPropTxAdv->syncWord = RF_cmdPropTx->syncWord;
}


/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions. */
    Board_initGeneral();

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (!ledPinHandle)
    {
        System_abort("Error initializing board LED pins\n");
    }

    /* Open Button pins */
    buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
    if (!buttonPinHandle) {
        System_abort("Error initializing button pins\n");
    }

    /* Initialize task */
    txTaskInit();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
