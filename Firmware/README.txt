# ===========================================================================
# Written by:
#   Nathan Shen (nds64@cornell.edu)
# Edited:
#		12/27/17
# ===========================================================================

This README will provide my approach, reasoning for selecting each code section, and where it fits in to the overall project.
A step-by-step guide on how to get the code running is provided at the end of this README.

The specifications that were given to us by Julian Kapoor is as follows:
1. Mobile nodes should sleep just under 5s.
2. Wake long enough to receive a single countdown signal from a ground node. 
3. Once they have received a countdown signal, and the checksum demonstrates a good link, update global time.
4. Listen for rest of commands. 
5. Once the commands are received: use global time, received command, and pre-programmed transmission time delay to send ID during the ground node’s ~5 min RX period.

To tackle the first and second specifications, I found example code in TI Code Composer Studio (CCS). The folders were rfWakeOnRadioRx and rfWakeonRadioTx. Run the example on one of the boards, this will be the RX board. Board_LED0 will blink on this board for every wakeup. Start the rfWakeOnRadioTx companion example on another board (TX board) and press Board_BUTTON0 on that board to send a packet. Board_LED1 on the RX board should now toggle for every button press. These examples use the sleep and wakeup functionality of the CC1310 by scheduling automatic wake-ups in the future. The default code schedules a wakeup every 0.5 seconds. To change the wakeup time to meet the specification of 5s, we want to change line 58 in both rfWakeOnRadioRx.c and rfWakeOnRadioTx.c to 0.2. This makes it so that the transceiver will wake up once every 5 seconds. 
The TI documentation outlines a typical radio physical layer packet format [4].


    |  Preamble  |  Sync Word  |  Length Byte  |  Payload  |    CRC    |
    --------------------------------------------------------------------
    |   4 byte   |   4 byte    |    1 byte     |  X bytes  |  2 bytes  |


From research, the preamble is almost always 101010... with a length that can change (we will use 4 bytes until we have a reason to change the length). The idea is that the receiver can "hear" this, and set the gain and other things correctly. However, this is technically not needed for bit synchronization. The sync word is given to us by TI and unless there is a very compelling reason, we should not change it since TI has tested the sync word to reduce the likelihood of false positives. In the default code, the payload length is 30. This can be set in rfWakeOnRadioTx on line 61. To change the payload, this can be done in rfWakeOnRadioTx on lines 204-212. To receive the payload can be found in rfWakeOnRadioRx lines 387-393. 

We are able to obtain the current time from the MCU by calling a built in TI function RF_getCurrentTime(). We can call this function from the transmitter and include the time into the payload (which I already implemented). Then, the receiver (tag) can read from the packet that it received and update its current time. I do not believe we have to account for the transmission delay because we do not need millisecond precision. Currently, I have not tested any payload commands besides including the Global Time. The code section that takes care and makes sure that the receiver wakes up long enough is found in line 336-354. 

TO DO: 
1.	Add in payload (Tx Command such as Checksum, Global Time, etc.) information (lines 204-212 in rfWakeOnRadioTx and lines 387-393 in rfWakeOnRadioRX).
2.	Add in another state in rfWakeOnRadioRx that sleeps for a pre-programmed transmission time delay (Specification 6). Currently the receivers will always sleep for 5s. The code segment that controls the sleep time is: RF_cmdPropRxSniff.startTime += WOR_WAKE_UP_INTERVAL_RAT_TICKS(WOR_WAKEUPS_PER_SECOND);
3.	Sync and update Global Time from the payload (Tx Command).

While working on the previous specifications, I stumbled upon rfListenBeforeTalk, which is also found in TI CCS. This project outlines how to check if the channel is busy before transmission. I was thinking on incorporating this code with rfWakeOnRadioTx/Rx as a fail-safe, in case our countdown signal is incorrect or too long; it will not interfere with another signal. 


How to get the code running:
1. 	Download TI Code Composer Studio (CCS) Integrated Development Environment (IDE) - http://www.ti.com/tool/CCSTUDIO
2. 	Download TI Real Time Operating System (RTOS) - http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/index.html
3. 	Open CCS
4. 	View -> Resource Explorer -> Software -> SimpleLink CC13x0 SDK -> Examples -> Development Tools -> CC1310 LaunchPad -> TI Drivers
5. 	Open the project you want to run (i.e. rFWakeOnRadioRx, rfWakeOnRadioTx)
6. 	For this example, we'll open rFPacketRX -> TI-RTOS -> CCS Compiler
7. 	Select rfPacketRx. Near the top right, select the "Import to IDE" button.
8. 	The rFPacketRX project will be imported to the right under Project Explorer. Select the project so that it is [Active]. 
9. 	Connect the CC1310 Launchpad to your computer and select Run -> Debug
10.	The code should now be running on a CC1310. To program rFPacketTX, repeat steps 4-9.
