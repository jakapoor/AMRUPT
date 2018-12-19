/*
 * Copyright (c) 2015-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met
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
#include "EasyLink.h"

/* TI Drivers */
#include <smartrf_settings/smartrf_settings_predefined.h>
#include <smartrf_settings/smartrf_settings.h>

/* BIOS Header files */
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#ifndef USE_DMM
#include <ti/drivers/rf/RF.h>
#include "Board.h"
#else
#include <dmm/dmm_rfmap.h>
#include "board.h"
#endif //USE_DMM


/* XDCtools Header files */
#include <xdc/runtime/Error.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_data_entry.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)
#include DeviceFamily_constructPath(driverlib/chipinfo.h)
#include DeviceFamily_constructPath(inc/hw_ccfg.h)
#include DeviceFamily_constructPath(inc/hw_ccfg_simple_struct.h)

union setupCmd_t{
    rfc_CMD_PROP_RADIO_DIV_SETUP_t divSetup;
    rfc_CMD_PROP_RADIO_SETUP_t setup;
};

//Primary IEEE address location
#define EASYLINK_PRIMARY_IEEE_ADDR_LOCATION   0x500012F0
//Secondary IEEE address location
#define EASYLINK_SECONDARY_IEEE_ADDR_LOCATION 0x0001FFC8

#define EASYLINK_RF_EVENT_MASK  ( RF_EventLastCmdDone | \
             RF_EventCmdAborted | RF_EventCmdStopped | RF_EventCmdCancelled | \
             RF_EventCmdPreempted )

#define EASYLINK_RF_CMD_HANDLE_INVALID -1

#define EasyLink_CmdHandle_isValid(handle) (handle >= 0)

/***** Prototypes *****/
static EasyLink_TxDoneCb txCb;
static EasyLink_ReceiveCb rxCb;
#if (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
static EasyLink_GetRandomNumber getRN;
#endif // (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))

/***** Variable declarations *****/

static RF_Object rfObject;
static RF_Handle rfHandle;

//Rx buffer includes data entry structure, hdr (len=1byte), dst addr (max of 8 bytes) and data
//which must be aligned to 4B
#if defined(__TI_COMPILER_VERSION__)
    #pragma DATA_ALIGN (rxBuffer, 4);
        static uint8_t rxBuffer[sizeof(rfc_dataEntryGeneral_t) + 1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH];
#elif defined(__IAR_SYSTEMS_ICC__)
    #pragma data_alignment = 4
        static uint8_t rxBuffer[sizeof(rfc_dataEntryGeneral_t) + 1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH];
#elif defined(__GNUC__)
        static uint8_t rxBuffer[sizeof(rfc_dataEntryGeneral_t) + 1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH];
#else
    #error This compiler is not supported.
#endif

static dataQueue_t dataQueue;
static rfc_propRxOutput_t rxStatistics;

//Tx buffer includes hdr (len=1byte), dst addr (max of 8 bytes) and data
static uint8_t txBuffer[1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH];

//Addr size for Filter and Tx/Rx operations
//Set default to 1 byte addr to work with SmartRF
//studio default settings
static uint8_t addrSize = 1;

//Indicating that the API is initialized
static uint8_t configured = 0;
//Indicating that the API suspended
static uint8_t suspended = 0;

//RF Params alowing configuration of the inactivity timeout, which is the time
//it takes for the radio to shut down when there are no commands in the queue
static RF_Params rfParams;
static bool rfParamsConfigured = 0;

//Flag used to indicate the multi client operation is enabled
static bool rfModeMultiClient = false;

//Async Rx timeout value
static uint32_t asyncRxTimeOut = 0;

//local commands, contents will be defined by modulation type
static union setupCmd_t EasyLink_cmdPropRadioSetup;
static rfc_CMD_FS_t EasyLink_cmdFs;
static RF_Mode EasyLink_RF_prop;
static rfc_CMD_PROP_TX_t EasyLink_cmdPropTx;
static rfc_CMD_PROP_RX_ADV_t EasyLink_cmdPropRxAdv;
#if (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
static rfc_CMD_PROP_CS_t EasyLink_cmdPropCs;
#endif // (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))

// The table for setting the Rx Address Filters
static uint8_t addrFilterTable[EASYLINK_MAX_ADDR_FILTERS * EASYLINK_MAX_ADDR_SIZE] = {0xaa};

//Mutex for locking the RF driver resource
static Semaphore_Handle busyMutex;

//Handle for last Async command, which is needed by EasyLink_abort
static RF_CmdHandle asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;

/* Set Default parameters structure */
static const EasyLink_Params EasyLink_defaultParams = {
    .ui32ModType            = EasyLink_Phy_50kbps2gfsk,
    .pClientEventCb         = NULL,
    .nClientEventMask       = 0,
    .pGrnFxn                = (EasyLink_GetRandomNumber)rand
};

static EasyLink_Params EasyLink_params;


void EasyLink_Params_init(EasyLink_Params *params)
{
    *params = EasyLink_defaultParams;
}

//Callback for Async Tx complete
static void txDoneCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    EasyLink_Status status;

    //Release now so user callback can call EasyLink API's
    Semaphore_post(busyMutex);
    asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;

    if (e & RF_EventLastCmdDone)
    {
        status = EasyLink_Status_Success;
    }
    else if ( (e & RF_EventCmdAborted) || (e & RF_EventCmdCancelled) || (e & RF_EventCmdPreempted) )
    {
        status = EasyLink_Status_Aborted;
    }
    else
    {
        status = EasyLink_Status_Tx_Error;
    }

    if (txCb != NULL)
    {
        txCb(status);
    }
}

#if (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
//Callback for Clear Channel Assessment Done
static void ccaDoneCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    RF_ScheduleCmdParams schParams_prop;
    EasyLink_Status status        = EasyLink_Status_Tx_Error;
    RF_Op* pCmd                   = RF_getCmdOp(h, ch);
    bool bCcaRunAgain             = false;
    static uint8_t be             = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
    static uint32_t backOffTime;

    asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;

    if (e & RF_EventLastCmdDone)
    {
        if(pCmd->status ==  PROP_DONE_IDLE)
        {
            // Carrier Sense operation ended with an idle channel,
            // and the next op (TX) should have already taken place
            // Failure to transmit is reflected in the default status,
            // EasyLink_Status_Tx_Error, being set
            if(pCmd->pNextOp->status == PROP_DONE_OK)
            {
                //Release now so user callback can call EasyLink API's
                Semaphore_post(busyMutex);
                status = EasyLink_Status_Success;
                // Reset the number of retries
                be = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
            }
        }
        else if(pCmd->status == PROP_DONE_BUSY)
        {
            if(be > EASYLINK_MAX_CCA_BACKOFF_WINDOW)
            {
                //Release now so user callback can call EasyLink API's
                Semaphore_post(busyMutex);
                // Reset the number of retries
                be = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
                // CCA failed max number of retries
                status = EasyLink_Status_Busy_Error;
            }
            else
            {
                // The back-off time is a random number chosen from 0 to 2^be,
                // where 'be' goes from EASYLINK_MIN_CCA_BACKOFF_WINDOW
                // to EASYLINK_MAX_CCA_BACKOFF_WINDOW. This number is then converted
                // into EASYLINK_CCA_BACKOFF_TIMEUNITS units, and subsequently used to
                // schedule the next CCA sequence. The variable 'be' is incremented each
                // time, up to a pre-configured maximum, the back-off algorithm is run.
                backOffTime = (getRN() & ((1 << be++)-1)) *
                        EasyLink_us_To_RadioTime(EASYLINK_CCA_BACKOFF_TIMEUNITS);
                // running CCA again
                bCcaRunAgain = true;
                // The random number generator function returns a value in the range
                // 0 to 2^15 - 1 and we choose the 'be' most significant bits as our
                // back-off time in milliseconds (converted to RAT ticks)
                pCmd->startTime = RF_getCurrentTime() + backOffTime;
                // post the chained CS+TX command again while checking
                // for a clear channel (CCA) before sending a packet
                if(rfModeMultiClient)
                {
                    schParams_prop.priority = RF_PriorityHigh;
                    asyncCmdHndl = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropCs,
                                                  &schParams_prop, ccaDoneCallback, EASYLINK_RF_EVENT_MASK);
                }
                else
                {
                    asyncCmdHndl = RF_postCmd(h, (RF_Op*)&EasyLink_cmdPropCs,
                        RF_PriorityHigh, ccaDoneCallback, EASYLINK_RF_EVENT_MASK);
                }
            }
        }
        else
        {
            //Release now so user callback can call EasyLink API's
            Semaphore_post(busyMutex);
            // Reset the number of retries
            be = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
            // The CS command status should be either IDLE or BUSY,
            // all other status codes can be considered errors
            // Status is set to the default, EasyLink_Status_Tx_Error
        }


    }
    else if ( (e & RF_EventCmdAborted) || (e & RF_EventCmdCancelled ) || (e & RF_EventCmdPreempted ) )
    {
        //Release now so user callback can call EasyLink API's
        Semaphore_post(busyMutex);
        // Reset the number of retries
        be = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
        status = EasyLink_Status_Aborted;
    }
    else
    {
        //Release now so user callback can call EasyLink API's
        Semaphore_post(busyMutex);
        // Reset the number of retries
        be = EASYLINK_MIN_CCA_BACKOFF_WINDOW;
        // Status is set to the default, EasyLink_Status_Tx_Error
    }

    if ((txCb != NULL) && (!bCcaRunAgain))
    {
        txCb(status);
    }
}
#endif // (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
    
//Callback for Async Rx complete
static void rxDoneCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    EasyLink_Status status = EasyLink_Status_Rx_Error;
    //create rxPacket as a static so that the large payload buffer it is not
    //allocated from the stack
    static EasyLink_RxPacket rxPacket;
    rfc_dataEntryGeneral_t *pDataEntry;
    pDataEntry = (rfc_dataEntryGeneral_t*) rxBuffer;

    if (e & RF_EventLastCmdDone)
    {
        //Release now so user callback can call EasyLink API's
        Semaphore_post(busyMutex);
        asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;

        //Check command status
        if (EasyLink_cmdPropRxAdv.status == PROP_DONE_OK)
        {
            //Check that data entry status indicates it is finished with
            if (pDataEntry->status != DATA_ENTRY_FINISHED)
            {
                status = EasyLink_Status_Rx_Error;
            }
            else if ( (rxStatistics.nRxOk == 1) ||
                    //or filer disabled and ignore due to addr mistmatch
                    ((EasyLink_cmdPropRxAdv.pktConf.filterOp == 1) &&
                     (rxStatistics.nRxIgnored == 1)) )
            {
                //copy length from pDataEntry
                rxPacket.len = *(uint8_t*)(&pDataEntry->data) - addrSize;
                //copy address from packet payload (as it is not in hdr)
                memcpy(&rxPacket.dstAddr, (&pDataEntry->data + 1), addrSize);
                //copy payload
                memcpy(&rxPacket.payload, (&pDataEntry->data + 1 + addrSize), rxPacket.len);
                rxPacket.rssi = rxStatistics.lastRssi;
                rxPacket.absTime = rxStatistics.timeStamp;

                status = EasyLink_Status_Success;
            }
            else if ( rxStatistics.nRxBufFull == 1)
            {
                status = EasyLink_Status_Rx_Buffer_Error;
            }
            else if ( rxStatistics.nRxStopped == 1)
            {
                status = EasyLink_Status_Aborted;
            }
            else
            {
                status = EasyLink_Status_Rx_Error;
            }
        }
        else if ( EasyLink_cmdPropRxAdv.status == PROP_DONE_RXTIMEOUT)
        {
            status = EasyLink_Status_Rx_Timeout;
        }
        else
        {
            status = EasyLink_Status_Rx_Error;
        }
    }
    else if (e & (RF_EventCmdCancelled | RF_EventCmdAborted | RF_EventCmdPreempted | RF_EventCmdStopped))
    {
        //Release now so user callback can call EasyLink API's
        Semaphore_post(busyMutex);
        asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;

        status = EasyLink_Status_Aborted;
    }

    if (rxCb != NULL)
    {
        rxCb(&rxPacket, status);
    }
}

//Callback for Async TX Test mode
static void asyncCmdCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    Semaphore_post(busyMutex);
    asyncCmdHndl = EASYLINK_RF_CMD_HANDLE_INVALID;
}

static EasyLink_Status enableTestMode(EasyLink_CtrlOption mode)
{
    EasyLink_Status status = EasyLink_Status_Cmd_Error;
    //This needs to be static as it is used by the RF driver and Modem after
    //this function exits
    static rfc_CMD_TX_TEST_t txTestCmd = {0};
    static rfc_CMD_RX_TEST_t rxTestCmd = {0};

    if((!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    if((mode != EasyLink_Ctrl_Test_Tone)    &&
       (mode != EasyLink_Ctrl_Test_Signal)  &&
       (mode != EasyLink_Ctrl_Rx_Test_Tone))
    {
        return EasyLink_Status_Param_Error;
    }
    
    //Check and take the busyMutex
    if((Semaphore_pend(busyMutex, 0) == FALSE) ||
            (EasyLink_CmdHandle_isValid(asyncCmdHndl)))
    {
        return EasyLink_Status_Busy_Error;
    }

    if((mode == EasyLink_Ctrl_Test_Tone) || (mode == EasyLink_Ctrl_Test_Signal))
    {
        txTestCmd.commandNo = CMD_TX_TEST;
        txTestCmd.startTrigger.triggerType = TRIG_NOW;
        txTestCmd.startTrigger.pastTrig = 1;
        txTestCmd.startTime = 0;

        txTestCmd.config.bFsOff = 1;
        txTestCmd.syncWord = EasyLink_cmdPropTx.syncWord;

        /* WhitenMode
         * 0: No whitening
         * 1: Default whitening
         * 2: PRBS-15
         * 3: PRBS-32
         */
        txTestCmd.config.whitenMode = 2;

        //set tone (unmodulated) or signal (modulated)
        if (mode == EasyLink_Ctrl_Test_Tone)
        {
            txTestCmd.txWord = 0xFFFF;
            txTestCmd.config.bUseCw = 1;
        }
        else
        {
            txTestCmd.txWord = 0xAAAA;
            txTestCmd.config.bUseCw = 0;
        }

        //generate continuous test signal
        txTestCmd.endTrigger.triggerType = TRIG_NEVER;

        /* Post command and store Cmd Handle for future abort */
        asyncCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&txTestCmd,
                                  RF_PriorityNormal, asyncCmdCallback,
                                  EASYLINK_RF_EVENT_MASK);

        /* Has command completed? */
        uint16_t count = 0;
        while (txTestCmd.status != ACTIVE)
        {
            //The command did not complete as fast as expected, sleep for 10ms
            Task_sleep(10000 / Clock_tickPeriod);

            if (count++ > 500)
            {
                //Should not get here, if we did Something went wrong with the
                //the RF Driver, get out of here and return an error.
                //The next command will likely lock up.
                break;
            }
        }

        if (txTestCmd.status == ACTIVE)
        {
            status = EasyLink_Status_Success;
        }
    }
    else // mode is EasyLink_Ctrl_Rx_Test_Tone
    {
        rxTestCmd.commandNo = CMD_RX_TEST;
        rxTestCmd.startTrigger.triggerType = TRIG_NOW;
        rxTestCmd.startTrigger.pastTrig = 1;
        rxTestCmd.startTime = 0;

        rxTestCmd.config.bFsOff = 1;
        // Correlation threshold set to max to prevent sync, as RSSI values 
        // are locked after sync
        rxTestCmd.config.bNoSync = 1;
        rxTestCmd.syncWord = EasyLink_cmdPropRxAdv.syncWord0;

        //detect test signal continuously
        rxTestCmd.endTrigger.triggerType = TRIG_NEVER;

        /* Post command and store Cmd Handle for future abort */
        asyncCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&rxTestCmd,
                                  RF_PriorityNormal, asyncCmdCallback,
                                  EASYLINK_RF_EVENT_MASK);
        if(EasyLink_CmdHandle_isValid(asyncCmdHndl))
        {
            status = EasyLink_Status_Success;
        }
        else
        {
            status = EasyLink_Status_Cmd_Error;
        }
    }

    return status;
}

EasyLink_Status EasyLink_init(EasyLink_Params *params)
{
    if (params == NULL)
    {
		EasyLink_Params_init(&EasyLink_params);
    } else 
    {
		memcpy(&EasyLink_params, params, sizeof(EasyLink_params));
    }
	

    if (configured)
    {
        //Already configure, check and take the busyMutex
        if (Semaphore_pend(busyMutex, 0) == FALSE)
        {
            return EasyLink_Status_Busy_Error;
        }

        RF_close(rfHandle);
    }

    if (!rfParamsConfigured)
    {
        RF_Params_init(&rfParams);
        //set default InactivityTimeout to 1000us
        rfParams.nInactivityTimeout = EasyLink_ms_To_RadioTime(1);
        //configure event callback
        if(EasyLink_params.pClientEventCb != NULL && EasyLink_params.nClientEventMask != 0){
            rfParams.pClientEventCb = EasyLink_params.pClientEventCb;
            rfParams.nClientEventMask = EasyLink_params.nClientEventMask;
        }

        rfParamsConfigured = 1;
    }
    
#if (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
    // Assign the random number generator function pointer to the global 
    // handle, if it is NULL any function that employs it will return a 
    // configuration error
    getRN = EasyLink_params.pGrnFxn;
    
    // Configure the EasyLink Carrier Sense Command
    memset(&EasyLink_cmdPropCs, 0, sizeof(rfc_CMD_PROP_CS_t));
    EasyLink_cmdPropCs.commandNo                = CMD_PROP_CS;
    EasyLink_cmdPropCs.rssiThr                  = EASYLINK_CS_RSSI_THRESHOLD_DBM;
    EasyLink_cmdPropCs.startTrigger.triggerType = TRIG_NOW;
    EasyLink_cmdPropCs.condition.rule           = COND_STOP_ON_TRUE;  // Stop next command if this command returned TRUE,
                                                            // End causes for the CMD_PROP_CS command:
                                                            // Observed channel state Busy with csConf.busyOp = 1:                            PROP_DONE_BUSY        TRUE
                                                            // 0bserved channel state Idle with csConf.idleOp = 1:                            PROP_DONE_IDLE        FALSE
                                                            // Timeout trigger observed with channel state Busy:                              PROP_DONE_BUSY        TRUE
                                                            // Timeout trigger observed with channel state Idle:                              PROP_DONE_IDLE        FALSE
                                                            // Timeout trigger observed with channel state Invalid and csConf.timeoutRes = 0: PROP_DONE_BUSYTIMEOUT TRUE
                                                            // Timeout trigger observed with channel state Invalid and csConf.timeoutRes = 1: PROP_DONE_IDLETIMEOUT FALSE
                                                            // Received CMD_STOP after command started:                                       PROP_DONE_STOPPED     FALSE
    EasyLink_cmdPropCs.csConf.bEnaRssi          = 0x1; // Enable RSSI as a criterion
    EasyLink_cmdPropCs.csConf.busyOp            = 0x1; // End carrier sense on channel Busy
    EasyLink_cmdPropCs.csConf.idleOp            = 0x0; // Continue carrier sense on channel Idle
    EasyLink_cmdPropCs.csEndTrigger.triggerType = TRIG_REL_START; // Ends at a time relative to the command started
	EasyLink_cmdPropCs.csEndTime                = EasyLink_us_To_RadioTime(EASYLINK_CHANNEL_IDLE_TIME_US);
#endif //(defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))

#if (defined(FEATURE_OAD_ONCHIP))
	EasyLink_params.ui32ModType = EasyLink_Phy_Custom;
#endif

    if (EasyLink_params.ui32ModType == EasyLink_Phy_Custom)
    {
        if(ChipInfo_GetChipType() == CHIP_TYPE_CC2650)
        {
            memcpy(&EasyLink_cmdPropRadioSetup.setup, &RF_cmdPropRadioDivSetup, sizeof(rfc_CMD_PROP_RADIO_SETUP_t));
        }
        else
        {
            memcpy(&EasyLink_cmdPropRadioSetup.divSetup, &RF_cmdPropRadioDivSetup, sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
        }
        memcpy(&EasyLink_cmdFs, &RF_cmdFs, sizeof(rfc_CMD_FS_t));
        memcpy(&EasyLink_RF_prop, &RF_prop, sizeof(RF_Mode));
        memcpy(&EasyLink_cmdPropRxAdv, RF_pCmdPropRxAdv_preDef, sizeof(rfc_CMD_PROP_RX_ADV_t));
        memcpy(&EasyLink_cmdPropTx, &RF_cmdPropTx, sizeof(rfc_CMD_PROP_TX_t));
    }
    else if ( (EasyLink_params.ui32ModType == EasyLink_Phy_50kbps2gfsk) && (ChipInfo_GetChipType() != CHIP_TYPE_CC2650) )
    {
        memcpy(&EasyLink_cmdPropRadioSetup.divSetup,
                RF_pCmdPropRadioDivSetup_fsk,
                sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
        memcpy(&EasyLink_cmdFs, RF_pCmdFs_preDef, sizeof(rfc_CMD_FS_t));
        memcpy(&EasyLink_RF_prop, RF_pProp_fsk, sizeof(RF_Mode));
        memcpy(&EasyLink_cmdPropRxAdv, RF_pCmdPropRxAdv_preDef, sizeof(rfc_CMD_PROP_RX_ADV_t));
        memcpy(&EasyLink_cmdPropTx, RF_pCmdPropTx_preDef, sizeof(rfc_CMD_PROP_TX_t));
    }
    else if ( (EasyLink_params.ui32ModType == EasyLink_Phy_625bpsLrm) && (ChipInfo_GetChipType() != CHIP_TYPE_CC2650) )
    {
        memcpy(&EasyLink_cmdPropRadioSetup.divSetup,
                RF_pCmdPropRadioDivSetup_lrm,
                sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
        memcpy(&EasyLink_cmdFs, RF_pCmdFs_preDef, sizeof(rfc_CMD_FS_t));
        memcpy(&EasyLink_RF_prop, RF_pProp_lrm, sizeof(RF_Mode));
        memcpy(&EasyLink_cmdPropRxAdv, RF_pCmdPropRxAdv_preDef, sizeof(rfc_CMD_PROP_RX_ADV_t));
        memcpy(&EasyLink_cmdPropTx, RF_pCmdPropTx_preDef, sizeof(rfc_CMD_PROP_TX_t));
    }
    else if ( (EasyLink_params.ui32ModType == EasyLink_Phy_2_4_200kbps2gfsk) && (ChipInfo_GetChipType() == CHIP_TYPE_CC2650) )
    {
        memcpy(&EasyLink_cmdPropRadioSetup.setup,
                RF_pCmdPropRadioSetup_2_4G_fsk,
                sizeof(rfc_CMD_PROP_RADIO_SETUP_t));
        memcpy(&EasyLink_cmdFs, RF_pCmdFs_preDef, sizeof(rfc_CMD_FS_t));
        memcpy(&EasyLink_RF_prop, RF_pProp_2_4G_fsk, sizeof(RF_Mode));
        memcpy(&EasyLink_cmdPropRxAdv, RF_pCmdPropRxAdv_preDef, sizeof(rfc_CMD_PROP_RX_ADV_t));
        memcpy(&EasyLink_cmdPropTx, RF_pCmdPropTx_preDef, sizeof(rfc_CMD_PROP_TX_t));
    }

    else if ( (EasyLink_params.ui32ModType == EasyLink_Phy_5kbpsSlLr) && (ChipInfo_GetChipType() != CHIP_TYPE_CC2650) )
    {
        memcpy(&EasyLink_cmdPropRadioSetup.divSetup,
                RF_pCmdPropRadioDivSetup_sl_lr,
                sizeof(rfc_CMD_PROP_RADIO_DIV_SETUP_t));
        memcpy(&EasyLink_cmdFs, RF_pCmdFs_preDef, sizeof(rfc_CMD_FS_t));
        memcpy(&EasyLink_RF_prop, RF_pProp_sl_lr, sizeof(RF_Mode));
        memcpy(&EasyLink_cmdPropRxAdv, RF_pCmdPropRxAdv_preDef, sizeof(rfc_CMD_PROP_RX_ADV_t));
        memcpy(&EasyLink_cmdPropTx, RF_pCmdPropTx_preDef, sizeof(rfc_CMD_PROP_TX_t));
    }
    else
    {
        if (busyMutex != NULL)
        {
            Semaphore_post(busyMutex);
        }
        return EasyLink_Status_Param_Error;
    }
#if !(defined(DeviceFamily_CC26X0R2))
    if (rfModeMultiClient)
    {
        EasyLink_RF_prop.rfMode = RF_MODE_MULTIPLE;
    }
#endif //defined(DeviceFamily_CC26X0R2)

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &EasyLink_RF_prop,
            (RF_RadioSetup*)&EasyLink_cmdPropRadioSetup.setup, &rfParams);

    //Set Rx packet size, taking into account addr which is not in the hdr
    //(only length can be)
    EasyLink_cmdPropRxAdv.maxPktLen = EASYLINK_MAX_DATA_LENGTH +
            EASYLINK_MAX_ADDR_SIZE;
    EasyLink_cmdPropRxAdv.pAddr = addrFilterTable;
    addrSize = 1;
    EasyLink_cmdPropRxAdv.addrConf.addrSize = addrSize; //Set addr size to the
                                                        //default
    EasyLink_cmdPropRxAdv.pktConf.filterOp = 1;  // Disable Addr filter by
                                                 //default
    EasyLink_cmdPropRxAdv.pQueue = &dataQueue;   // Set the Data Entity queue
                                                 // for received data
    EasyLink_cmdPropRxAdv.pOutput = (uint8_t*)&rxStatistics;

    //Set the frequency
    RF_runCmd(rfHandle, (RF_Op*)&EasyLink_cmdFs, RF_PriorityNormal, 0, //asyncCmdCallback,
            EASYLINK_RF_EVENT_MASK);

    //set default asyncRxTimeOut to 0
    asyncRxTimeOut = 0;

    //Create a semaphore for blocking commands
    Semaphore_Params semParams;
    Error_Block eb;

    // init params
    Semaphore_Params_init(&semParams);
    Error_init(&eb);

    // create semaphore instance if not already created
    if (busyMutex == NULL)
    {
        busyMutex = Semaphore_create(0, &semParams, &eb);
        if (busyMutex == NULL)
        {
            return EasyLink_Status_Mem_Error;
        }

        Semaphore_post(busyMutex);
    }
    else
    {
        //already configured and taken busyMutex, so release it
        Semaphore_post(busyMutex);
    }

    configured = 1;

    return EasyLink_Status_Success;
   
}

EasyLink_Status EasyLink_setFrequency(uint32_t ui32Frequency)
{
    EasyLink_Status status = EasyLink_Status_Cmd_Error;
    //uint64_t ui64FractFreq;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if (Semaphore_pend(busyMutex, 0) == FALSE)
    {
        return EasyLink_Status_Busy_Error;
    }

    /* Set the frequency */
    EasyLink_cmdFs.frequency = (uint16_t)(ui32Frequency / 1000000);
    EasyLink_cmdFs.fractFreq = (uint16_t) (((uint64_t)ui32Frequency -
            ((uint64_t)EasyLink_cmdFs.frequency * 1000000)) * 65536 / 1000000);

    /* Run command */
    RF_EventMask result = RF_runCmd(rfHandle, (RF_Op*)&EasyLink_cmdFs,
            RF_PriorityNormal, 0, EASYLINK_RF_EVENT_MASK);

    if (result & RF_EventLastCmdDone)
    {
        status = EasyLink_Status_Success;
    }

    Semaphore_post(busyMutex);

    return status;
}

uint32_t EasyLink_getFrequency(void)
{
    uint32_t freq_khz;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }

    freq_khz = EasyLink_cmdFs.frequency * 1000000;
    freq_khz += ((((uint64_t)EasyLink_cmdFs.fractFreq * 1000000)) / 65536);

    return freq_khz;
}

EasyLink_Status EasyLink_setRfPower(int8_t i8TxPowerdBm)
{
    EasyLink_Status status = EasyLink_Status_Cmd_Error;
    rfc_CMD_SCH_IMM_t immOpCmd = {0};
    rfc_CMD_SET_TX_POWER_t cmdSetPower = {0};
    uint8_t txPowerIdx;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if (Semaphore_pend(busyMutex, 0) == FALSE)
    {
        return EasyLink_Status_Busy_Error;
    }

    immOpCmd.commandNo = CMD_SCH_IMM;
    immOpCmd.startTrigger.triggerType = TRIG_NOW;
    immOpCmd.startTrigger.pastTrig = 1;
    immOpCmd.startTime = 0;

    cmdSetPower.commandNo = CMD_SET_TX_POWER;

    if (i8TxPowerdBm < rfPowerTable[0].dbm)
    {
        i8TxPowerdBm = rfPowerTable[0].dbm;
    }
    else if (i8TxPowerdBm > rfPowerTable[rfPowerTableSize-1].dbm )
    {
        i8TxPowerdBm = rfPowerTable[rfPowerTableSize-1].dbm;
    }

    //if max power is requested then the CCFG_FORCE_VDDR_HH must be set in
    //the ccfg
#if (CCFG_FORCE_VDDR_HH != 0x1)
    if (i8TxPowerdBm == rfPowerTable[rfPowerTableSize-1].dbm)
    {
        //Release the busyMutex
        Semaphore_post(busyMutex);
        return EasyLink_Status_Config_Error;
    }
#endif

    for (txPowerIdx = 0; txPowerIdx < rfPowerTableSize; txPowerIdx++)
    {
        if (i8TxPowerdBm >= rfPowerTable[txPowerIdx].dbm)
        {
            cmdSetPower.txPower = rfPowerTable[txPowerIdx].txPower;
            EasyLink_cmdPropRadioSetup.setup.txPower = rfPowerTable[txPowerIdx].txPower;
        }
    }

    //point the Operational Command to the immediate set power command
    immOpCmd.cmdrVal = (uint32_t) &cmdSetPower;

    // Send command
    RF_CmdHandle cmd = RF_postCmd(rfHandle, (RF_Op*)&immOpCmd,
            RF_PriorityNormal, 0, EASYLINK_RF_EVENT_MASK);

    RF_EventMask result = RF_pendCmd(rfHandle, cmd, RF_EventLastCmdDone);

    if (result & RF_EventLastCmdDone)
    {
        status = EasyLink_Status_Success;
    }

    //Release the busyMutex
    Semaphore_post(busyMutex);

    return status;
}

EasyLink_Status EasyLink_getRfPower(int8_t *pi8TxPowerdBm)
{
    uint8_t txPowerIdx;
    int8_t txPowerdBm = 0xff;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }

    for (txPowerIdx = 0; txPowerIdx < rfPowerTableSize; txPowerIdx++)
    {
        if (rfPowerTable[txPowerIdx].txPower == EasyLink_cmdPropRadioSetup.setup.txPower)
        {
            txPowerdBm = rfPowerTable[txPowerIdx].dbm;
            continue;
        }
    }

    //if CCFG_FORCE_VDDR_HH is not set max power cannot be achieved
#if (CCFG_FORCE_VDDR_HH != 0x1)
    if (txPowerdBm == rfPowerTable[rfPowerTableSize-1].dbm)
    {
        txPowerdBm = rfPowerTable[rfPowerTableSize-2].dbm;
    }
#endif

    *pi8TxPowerdBm = txPowerdBm;
    return EasyLink_Status_Success;
}

EasyLink_Status EasyLink_getRssi(int8_t *pi8Rssi)
{
    if((!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }

	*pi8Rssi = RF_getRssi(rfHandle);
	
    return EasyLink_Status_Success;
}

EasyLink_Status EasyLink_getAbsTime(uint32_t *pui32AbsTime)
{
    if((!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }

    *pui32AbsTime = RF_getCurrentTime();
    
    return EasyLink_Status_Success;
}

EasyLink_Status EasyLink_transmit(EasyLink_TxPacket *txPacket)
{
    EasyLink_Status status = EasyLink_Status_Tx_Error;
    RF_ScheduleCmdParams schParams_prop;
    RF_CmdHandle cmdHdl;
    uint32_t cmdTime;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if (Semaphore_pend(busyMutex, 0) == FALSE)
    {
        return EasyLink_Status_Busy_Error;
    }

    if (txPacket->len > EASYLINK_MAX_DATA_LENGTH)
    {
        return EasyLink_Status_Param_Error;
    }

    memcpy(txBuffer, txPacket->dstAddr, addrSize);
    memcpy(txBuffer + addrSize, txPacket->payload, txPacket->len);

    //packet length to Tx includes address
    EasyLink_cmdPropTx.pktLen = txPacket->len + addrSize;
    EasyLink_cmdPropTx.pPkt = txBuffer;

    if(EasyLink_params.ui32ModType == EasyLink_Phy_5kbpsSlLr)
    {
        /* calculate the command time:
         * (len + pre-able + phy len + address) * 8bits / 5kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 5;
    }
    else //assume 50kbps
    {
        /* calculate the command time:
         * (len + pre-able + syncword + len + address) * 8bits / 50kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 50;
    }

    if (txPacket->absTime != 0)
    {
        EasyLink_cmdPropTx.startTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropTx.startTrigger.pastTrig = 1;
        EasyLink_cmdPropTx.startTime = txPacket->absTime;
        schParams_prop.endTime = EasyLink_cmdPropTx.startTime + EasyLink_ms_To_RadioTime(cmdTime);
    }
    else
    {
        EasyLink_cmdPropTx.startTrigger.triggerType = TRIG_NOW;
        EasyLink_cmdPropTx.startTrigger.pastTrig = 1;
        EasyLink_cmdPropTx.startTime = 0;
        schParams_prop.endTime = RF_getCurrentTime() + EasyLink_ms_To_RadioTime(cmdTime);
    }

    // Send packet
    if(rfModeMultiClient)
    {
        schParams_prop.priority = RF_PriorityHigh;
        cmdHdl = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropTx,
                    &schParams_prop, 0, EASYLINK_RF_EVENT_MASK);
    }
    else
    {
        cmdHdl = RF_postCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropTx,
            RF_PriorityHigh, 0, EASYLINK_RF_EVENT_MASK);
    }

    // Wait for Command to complete
    RF_EventMask result = RF_pendCmd(rfHandle, cmdHdl, EASYLINK_RF_EVENT_MASK);

    if (result & RF_EventLastCmdDone)
    {
        status = EasyLink_Status_Success;
    }

    //Release the busyMutex
    Semaphore_post(busyMutex);

    return status;
}

EasyLink_Status EasyLink_transmitAsync(EasyLink_TxPacket *txPacket, EasyLink_TxDoneCb cb)
{
    EasyLink_Status status = EasyLink_Status_Tx_Error;
    RF_ScheduleCmdParams schParams_prop;
    uint32_t cmdTime;

    //Check if not configure or already an Async command being performed
    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if ( (Semaphore_pend(busyMutex, 0) == FALSE) || (EasyLink_CmdHandle_isValid(asyncCmdHndl)) )
    {
        return EasyLink_Status_Busy_Error;
    }

    if (txPacket->len > EASYLINK_MAX_DATA_LENGTH)
    {
        return EasyLink_Status_Param_Error;
    }

    //store application callback
    txCb = cb;

    memcpy(txBuffer, txPacket->dstAddr, addrSize);
    memcpy(txBuffer + addrSize, txPacket->payload, txPacket->len);

    //packet length to Tx includes address
    EasyLink_cmdPropTx.pktLen = txPacket->len + addrSize;
    EasyLink_cmdPropTx.pPkt = txBuffer;

    if(EasyLink_params.ui32ModType == EasyLink_Phy_5kbpsSlLr)
    {
        /* calculate the command time:
         * (len + pre-able + phy len + address) * 8bits / 5kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 5;
    }
    else //assume 50kbps
    {
        /* calculate the command time:
         * (len + pre-able + syncword + len + address) * 8bits / 50kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 50;
    }

    if (txPacket->absTime != 0)
    {
        EasyLink_cmdPropTx.startTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropTx.startTrigger.pastTrig = 1;
        EasyLink_cmdPropTx.startTime = txPacket->absTime;
        schParams_prop.endTime = EasyLink_cmdPropTx.startTime + EasyLink_ms_To_RadioTime(cmdTime);
    }
    else
    {
        EasyLink_cmdPropTx.startTrigger.triggerType = TRIG_NOW;
        EasyLink_cmdPropTx.startTrigger.pastTrig = 1;
        EasyLink_cmdPropTx.startTime = 0;
        schParams_prop.endTime = RF_getCurrentTime() + EasyLink_ms_To_RadioTime(cmdTime);
    }

    // Send packet
    if(rfModeMultiClient)
    {
        schParams_prop.priority = RF_PriorityHigh;
        asyncCmdHndl = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropTx,
            &schParams_prop, txDoneCallback, EASYLINK_RF_EVENT_MASK);
    }
    else
    {
        asyncCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropTx,
            RF_PriorityHigh, txDoneCallback, EASYLINK_RF_EVENT_MASK);
    }

    if (EasyLink_CmdHandle_isValid(asyncCmdHndl))
    {
        status = EasyLink_Status_Success;
    }

    //busyMutex will be released by the callback

    return status;
}

#if (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
EasyLink_Status EasyLink_transmitCcaAsync(EasyLink_TxPacket *txPacket, EasyLink_TxDoneCb cb)
{
    EasyLink_Status status = EasyLink_Status_Tx_Error;
    RF_ScheduleCmdParams schParams_prop;
    uint32_t cmdTime;

    //Check if not configure or already an Async command being performed, or 
    //if a random number generator function is not provided
    if ( (!configured) || suspended || (!getRN))
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if ( (Semaphore_pend(busyMutex, 0) == FALSE) || (EasyLink_CmdHandle_isValid(asyncCmdHndl)) )
    {
        return EasyLink_Status_Busy_Error;
    }
    if (txPacket->len > EASYLINK_MAX_DATA_LENGTH)
    {
        return EasyLink_Status_Param_Error;
    }

    //store application callback
    txCb = cb;

    memcpy(txBuffer, txPacket->dstAddr, addrSize);
    memcpy(txBuffer + addrSize, txPacket->payload, txPacket->len);

    // Set the Carrier Sense command attributes
    // Chain the TX command to run after the CS command
    EasyLink_cmdPropCs.pNextOp        = (rfc_radioOp_t *)&EasyLink_cmdPropTx;

    //packet length to Tx includes address
    EasyLink_cmdPropTx.pktLen = txPacket->len + addrSize;
    EasyLink_cmdPropTx.pPkt = txBuffer;

    if(EasyLink_params.ui32ModType == EasyLink_Phy_5kbpsSlLr)
    {
        /* calculate the command time:
         * (len + pre-able + phy len + address) * 8bits / 5kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 5;
    }
    else //assume 50kbps
    {
        /* calculate the command time:
         * (len + pre-able + syncword + len + address) * 8bits / 50kbps */
        cmdTime = ((EasyLink_cmdPropTx.pktLen + 10 + addrSize) * 8) / 50;
    }

    if (txPacket->absTime != 0)
    {
        EasyLink_cmdPropCs.startTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropCs.startTrigger.pastTrig = 1;
        EasyLink_cmdPropCs.startTime = txPacket->absTime;
        schParams_prop.endTime = EasyLink_cmdPropCs.startTime + EasyLink_ms_To_RadioTime(cmdTime);
    }
    else
    {
        EasyLink_cmdPropCs.startTrigger.triggerType = TRIG_NOW;
        EasyLink_cmdPropCs.startTrigger.pastTrig = 1;
        EasyLink_cmdPropCs.startTime = 0;
        schParams_prop.endTime = RF_getCurrentTime() + EasyLink_ms_To_RadioTime(cmdTime);
    }

    // Check for a clear channel (CCA) before sending a packet
    if(rfModeMultiClient)
    {
        schParams_prop.priority = RF_PriorityHigh;
        asyncCmdHndl = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropCs,
            &schParams_prop, ccaDoneCallback, EASYLINK_RF_EVENT_MASK);
    }
    else
    {
        asyncCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropCs,
            RF_PriorityHigh, ccaDoneCallback, EASYLINK_RF_EVENT_MASK);
    }

    if (EasyLink_CmdHandle_isValid(asyncCmdHndl))
    {
        status = EasyLink_Status_Success;
    }

    //busyMutex will be released by the callback

    return status;
}
#endif // (defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC13X2))
    
EasyLink_Status EasyLink_receive(EasyLink_RxPacket *rxPacket)
{
    EasyLink_Status status = EasyLink_Status_Rx_Error;
    RF_EventMask result;
    rfc_dataEntryGeneral_t *pDataEntry;
    RF_CmdHandle rx_cmd;
    RF_ScheduleCmdParams schParams_prop;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if (Semaphore_pend(busyMutex, 0) == FALSE)
    {
        return EasyLink_Status_Busy_Error;
    }

    pDataEntry = (rfc_dataEntryGeneral_t*) rxBuffer;
    //data entry rx buffer includes hdr (len-1Byte), addr (max 8Bytes) and data
    pDataEntry->length = 1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH;
    pDataEntry->status = 0;
    dataQueue.pCurrEntry = (uint8_t*)pDataEntry;
    dataQueue.pLastEntry = NULL;
    EasyLink_cmdPropRxAdv.pQueue = &dataQueue;               /* Set the Data Entity queue for received data */
    EasyLink_cmdPropRxAdv.pOutput = (uint8_t*)&rxStatistics;

    if (rxPacket->absTime != 0)
    {
        EasyLink_cmdPropRxAdv.startTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropRxAdv.startTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.startTime = rxPacket->absTime;
    }
    else
    {
        EasyLink_cmdPropRxAdv.startTrigger.triggerType = TRIG_NOW;
        EasyLink_cmdPropRxAdv.startTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.startTime = 0;
    }

    if (rxPacket->rxTimeout != 0)
    {
        EasyLink_cmdPropRxAdv.endTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropRxAdv.endTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.endTime = RF_getCurrentTime() + rxPacket->rxTimeout;
    }
    else
    {
        EasyLink_cmdPropRxAdv.endTrigger.triggerType = TRIG_NEVER;
        EasyLink_cmdPropRxAdv.endTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.endTime = 0;
    }

    //Clear the Rx statistics structure
    memset(&rxStatistics, 0, sizeof(rfc_propRxOutput_t));

    if(rfModeMultiClient)
    {
        /* assume high priority */
        schParams_prop.priority = RF_PriorityHigh;
        schParams_prop.endTime = EasyLink_cmdPropRxAdv.endTime;

        rx_cmd = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropRxAdv,
                    &schParams_prop, 0, EASYLINK_RF_EVENT_MASK);
    }
    else
    {
        rx_cmd = RF_postCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropRxAdv,
            RF_PriorityHigh, 0, EASYLINK_RF_EVENT_MASK);
    }

    /* Wait for Command to complete */
    result = RF_pendCmd(rfHandle, rx_cmd, RF_EventLastCmdDone);

    if (result & RF_EventLastCmdDone)
    {
        //Check command status
        if (EasyLink_cmdPropRxAdv.status == PROP_DONE_OK)
        {
            //Check that data entry status indicates it is finished with
            if (pDataEntry->status != DATA_ENTRY_FINISHED)
            {
                status = EasyLink_Status_Rx_Error;
            }
            //check Rx Statistics
            else if ( (rxStatistics.nRxOk == 1) ||
                     //or  filer disabled and ignore due to addr mistmatch
                     ((EasyLink_cmdPropRxAdv.pktConf.filterOp == 1) &&
                      (rxStatistics.nRxIgnored == 1)) )
            {
                //copy length from pDataEntry (- addrSize)
                rxPacket->len = *(uint8_t*)(&pDataEntry->data) - addrSize;
                //copy address
                memcpy(rxPacket->dstAddr, (&pDataEntry->data + 1), addrSize);
                //copy payload
                memcpy(&rxPacket->payload, (&pDataEntry->data + 1 + addrSize), (rxPacket->len));
                rxPacket->rssi = rxStatistics.lastRssi;

                status = EasyLink_Status_Success;
                rxPacket->absTime = rxStatistics.timeStamp;
            }
            else if ( rxStatistics.nRxBufFull == 1)
            {
                status = EasyLink_Status_Rx_Buffer_Error;
            }
            else if ( rxStatistics.nRxStopped == 1)
            {
                status = EasyLink_Status_Aborted;
            }
            else
            {
                status = EasyLink_Status_Rx_Error;
            }
        }
        else if ( EasyLink_cmdPropRxAdv.status == PROP_DONE_RXTIMEOUT)
        {
            status = EasyLink_Status_Rx_Timeout;
        }
        else
        {
            status = EasyLink_Status_Rx_Error;
        }
    }

    //Release the busyMutex
    Semaphore_post(busyMutex);

    return status;
}

EasyLink_Status EasyLink_receiveAsync(EasyLink_ReceiveCb cb, uint32_t absTime)
{
    EasyLink_Status status = EasyLink_Status_Rx_Error;
    rfc_dataEntryGeneral_t *pDataEntry;
    RF_ScheduleCmdParams schParams_prop;

    //Check if not configure of already an Async command being performed
    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //Check and take the busyMutex
    if ( (Semaphore_pend(busyMutex, 0) == FALSE) || (EasyLink_CmdHandle_isValid(asyncCmdHndl)) )
    {
        return EasyLink_Status_Busy_Error;
    }

    rxCb = cb;

    pDataEntry = (rfc_dataEntryGeneral_t*) rxBuffer;
    //data entry rx buffer includes hdr (len-1Byte), addr (max 8Bytes) and data
    pDataEntry->length = 1 + EASYLINK_MAX_ADDR_SIZE + EASYLINK_MAX_DATA_LENGTH;
    pDataEntry->status = 0;
    dataQueue.pCurrEntry = (uint8_t*)pDataEntry;
    dataQueue.pLastEntry = NULL;
    EasyLink_cmdPropRxAdv.pQueue = &dataQueue;               /* Set the Data Entity queue for received data */
    EasyLink_cmdPropRxAdv.pOutput = (uint8_t*)&rxStatistics;

    if (absTime != 0)
    {
        EasyLink_cmdPropRxAdv.startTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropRxAdv.startTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.startTime = absTime;
    }
    else
    {
        EasyLink_cmdPropRxAdv.startTrigger.triggerType = TRIG_NOW;
        EasyLink_cmdPropRxAdv.startTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.startTime = 0;
    }

    if (asyncRxTimeOut != 0)
    {
        EasyLink_cmdPropRxAdv.endTrigger.triggerType = TRIG_ABSTIME;
        EasyLink_cmdPropRxAdv.endTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.endTime = RF_getCurrentTime() + asyncRxTimeOut;
    }
    else
    {
        EasyLink_cmdPropRxAdv.endTrigger.triggerType = TRIG_NEVER;
        EasyLink_cmdPropRxAdv.endTrigger.pastTrig = 1;
        EasyLink_cmdPropRxAdv.endTime = 0;
    }

    //Clear the Rx statistics structure
    memset(&rxStatistics, 0, sizeof(rfc_propRxOutput_t));

    if(rfModeMultiClient)
    {
        /* assume high priority */
        schParams_prop.priority = RF_PriorityHigh;
        schParams_prop.endTime = EasyLink_cmdPropRxAdv.endTime;

        asyncCmdHndl = RF_scheduleCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropRxAdv,
                    &schParams_prop, rxDoneCallback, EASYLINK_RF_EVENT_MASK);
    }
    else
    {
        asyncCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&EasyLink_cmdPropRxAdv,
            RF_PriorityHigh, rxDoneCallback, EASYLINK_RF_EVENT_MASK);
    }

    if (EasyLink_CmdHandle_isValid(asyncCmdHndl))
    {
        status = EasyLink_Status_Success;
    }
    else
    {
        //Callback will not be called, release the busyMutex
        Semaphore_post(busyMutex);

    }

    //busyMutex will be released in callback

    return status;
}

EasyLink_Status EasyLink_abort(void)
{
    EasyLink_Status status = EasyLink_Status_Cmd_Error;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    //check an Async command is running, if not return success
    if (!EasyLink_CmdHandle_isValid(asyncCmdHndl))
    {
        return EasyLink_Status_Aborted;
    }

    //force abort (gracefull param set to 0)
    if (RF_cancelCmd(rfHandle, asyncCmdHndl, 0) == RF_StatSuccess)
    {
        /* If command is cancelled immediately, callback may have set the cmd handle to invalid.
         * In that case, no need to pend.
         */
        if (EasyLink_CmdHandle_isValid(asyncCmdHndl))
        {
            /* Wait for Command to complete */
            RF_EventMask result = RF_pendCmd(rfHandle, asyncCmdHndl, (RF_EventLastCmdDone |
                    RF_EventCmdAborted | RF_EventCmdCancelled | RF_EventCmdStopped));
            if (result & RF_EventLastCmdDone)
            {
                status = EasyLink_Status_Success;
            }
        }
        else
        {
            /* Command already cancelled */
            status = EasyLink_Status_Success;
        }
    }
    else
    {
        status = EasyLink_Status_Cmd_Error;
    }

    return status;
}

EasyLink_Status EasyLink_enableRxAddrFilter(uint8_t* pui8AddrFilterTable, uint8_t ui8AddrSize, uint8_t ui8NumAddrs)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    if ( (!configured) || suspended)
    {
        return EasyLink_Status_Config_Error;
    }
    if ( Semaphore_pend(busyMutex, 0) == FALSE )
    {
        return EasyLink_Status_Busy_Error;
    }

    if ( (pui8AddrFilterTable != NULL) &&
            (ui8AddrSize != 0) && (ui8NumAddrs != 0) &&
            (ui8AddrSize == addrSize) &&
            (ui8NumAddrs <= EASYLINK_MAX_ADDR_FILTERS) )
    {
        memcpy(addrFilterTable, pui8AddrFilterTable, ui8AddrSize * ui8NumAddrs);
        EasyLink_cmdPropRxAdv.addrConf.addrSize = ui8AddrSize;
        EasyLink_cmdPropRxAdv.addrConf.numAddr = ui8NumAddrs;
        EasyLink_cmdPropRxAdv.pktConf.filterOp = 0;

        status = EasyLink_Status_Success;
    }
    else if (pui8AddrFilterTable == NULL)
    {
        //disable filter
        EasyLink_cmdPropRxAdv.pktConf.filterOp = 1;

        status = EasyLink_Status_Success;
    }

    //Release the busyMutex
    Semaphore_post(busyMutex);

    return status;
}

EasyLink_Status EasyLink_setCtrl(EasyLink_CtrlOption Ctrl, uint32_t ui32Value)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;
    switch(Ctrl)
    {
        case EasyLink_Ctrl_AddSize:
            if (ui32Value <= EASYLINK_MAX_ADDR_SIZE)
            {
                addrSize = (uint8_t) ui32Value;
                EasyLink_cmdPropRxAdv.addrConf.addrSize = addrSize;
                status = EasyLink_Status_Success;
            }
            break;
        case EasyLink_Ctrl_Idle_TimeOut:
            rfParams.nInactivityTimeout = ui32Value;
            rfParamsConfigured = 1;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_MultiClient_Mode:
            rfModeMultiClient = (bool) ui32Value;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_AsyncRx_TimeOut:
            asyncRxTimeOut = ui32Value;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_Test_Tone:
            status = enableTestMode(EasyLink_Ctrl_Test_Tone);
            break;
        case EasyLink_Ctrl_Test_Signal:
            status = enableTestMode(EasyLink_Ctrl_Test_Signal);
            break;
        case EasyLink_Ctrl_Rx_Test_Tone:
            status = enableTestMode(EasyLink_Ctrl_Rx_Test_Tone);
            break;
    }

    return status;
}

EasyLink_Status EasyLink_getCtrl(EasyLink_CtrlOption Ctrl, uint32_t* pui32Value)
{
    EasyLink_Status status = EasyLink_Status_Cmd_Error;

    switch(Ctrl)
    {
        case EasyLink_Ctrl_AddSize:
            *pui32Value = addrSize;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_Idle_TimeOut:
            *pui32Value = rfParams.nInactivityTimeout;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_MultiClient_Mode:
            *pui32Value = (uint32_t) rfModeMultiClient;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_AsyncRx_TimeOut:
            *pui32Value = asyncRxTimeOut;
            status = EasyLink_Status_Success;
            break;
        case EasyLink_Ctrl_Test_Tone:
        case EasyLink_Ctrl_Test_Signal:
        case EasyLink_Ctrl_Rx_Test_Tone:
            *pui32Value = 0;
            status = EasyLink_Status_Success;
            break;
    }

    return status;
}

EasyLink_Status EasyLink_getIeeeAddr(uint8_t *ieeeAddr)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    if (ieeeAddr != NULL)
    {
        int i;

        //Reading from primary IEEE location...
        uint8_t *location = (uint8_t *)EASYLINK_PRIMARY_IEEE_ADDR_LOCATION;

        /*
         * ...unless we can find a byte != 0xFF in secondary
         *
         * Intentionally checking all 8 bytes here instead of len, because we
         * are checking validity of the entire IEEE address irrespective of the
         * actual number of bytes the caller wants to copy over.
         */
        for (i = 0; i < 8; i++) {
            if (((uint8_t *)EASYLINK_SECONDARY_IEEE_ADDR_LOCATION)[i] != 0xFF) {
                //A byte in the secondary location is not 0xFF. Use the
                //secondary
                location = (uint8_t *)EASYLINK_SECONDARY_IEEE_ADDR_LOCATION;
                break;
            }
        }

        //inverting byte order
        for (i = 0; i < 8; i++) {
            ieeeAddr[i] = location[8 - 1 - i];
        }

        status = EasyLink_Status_Success;
    }

    return status;
}
