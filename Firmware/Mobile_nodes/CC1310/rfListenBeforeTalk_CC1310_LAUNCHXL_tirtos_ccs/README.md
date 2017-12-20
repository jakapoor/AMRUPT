Listen Before Talk TX Example
=============================

Example Summary
---------------

The Listen Before Talk (LBT) TX example illustrates how to implement a simple,
proprietary LBT algorithm using command chaining.

When sending a packet, the radio first enters RX mode using CMD_PROP_CS. If
the channel is IDLE (the RSSI is below RSSI_THRESHOLD) for IDLE_TIME_US, the
the radio enters TX and transmits a packet. If the channel is BUSY (RSSI above
RSSI_THRESHOLD), the radio enters RX again to check the channel once more.
This is repeated max CS_RETRIES_WHEN_BUSY number of times. The command chain
will either finish with a packet being sent (if the channel is IDLE), or after
checking the channel CS_RETRIES_WHEN_BUSY times.

Default Settings:

 - CS_RETRIES_WHEN_BUSY: 10
 - RSSI_THRESHOLD:       -90 dBm
 - IDLE_TIME_US:         5000 us


Peripherals Exercised
---------------------

* `Board_PIN_LED1` - Toggled for every packet sent.

Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
The example code can be tested with SmartRF Studio or the rfPacketRX examples
as the receiver, and a jammer at 868.0 MHz (433.92 MHz for the 
CC1350-LAUNCHXL-433 board).


Application Design Details
--------------------------
The command chain contains the following commands:

 - CMD_NOP
 - CMD_PROP_CS
 - CMD_COUNT_BRANCH
 - CMD_PROP_TX

The following image shows the control flow:

--------------------------------------------------------------------------------

                                   -------------
                                   |           |
                                   |  CMD_NOP  |
                                   |           |
                                   -------------
                                         |
                                         |
                                         |
                                 -----------------
         PROP_DONE_IDLE          |               |
    ---------<-------------------|  CMD_PROP_CS  |----<---
    |                            |               |       |
    |                            -----------------       |
    |                                     |              |
    |                      PROP_DONE_BUSY |    RF_cmdCountBranch.counter-- > 0
    |                                     |              |
    |                          ----------------------    |
    |                          |                    |    |
    |                          |  CMD_COUNT_BRANCH  |-->--
    |                          |                    |    |
    |                          ----------------------    |
    |                                    .               |
    |                                    .    RF_cmdCountBranch.counter-- = 0
    |                                    .               |
    |                            -----------------       |
    |                            |               |       |
    --------->-------------------|  CMD_PROP_TX  |       |
                                 |               |       |
                                 -----------------       |
                                        |                |
                                        |--------<--------
                                        |
                            Last command in chain done

--------------------------------------------------------------------------------

The carrier sense command CMD_PROP_CS is prepended by a CMD_NOP in order to
set an absolute trigger time TIRG_ABSTIME for the whole chain. All other
commands trigger immediately after each other. If CMD_PROP_CS indicates a busy
channel it is restarted by CMD_COUNT_BRANCH for maximum
RF_cmdCountBranch.counter times. Only if CMD_PROP_CS observes an IDLE channel,
CMD_PROP_TX is executed.

On a successful transmission, Board_PIN_LED1 toggles and the sequence number in
the packet is incremented. If CMD_PROP_TX can not be executed due to an always
BUSY channel, the command chain is restarted again after PACKET_INTERVAL_MS
which is 200 ms by default.

Once triggered, all commands runs entirely on the RF core and do not involve
the main CPU. The CC13xx enters Standby mode in-between packet
transmissions. The following image shows the characteristic current
consumption profile when channel is IDLE (the radio is in RX for 5 ms):

              TX                                             TX
             ----                                           ----
             |  |                                           |  |
             |  |                                           |  |
             |  |                                           |  |
             |  |                                           |  |
          RX |  |                                        RX |  |
          ----  |                                        ----  |
          |     |                                        |     |
          |     |             Standby                    |     |
    ------      ----------------------------------------      ----------------
          <--------------------------------------------->
                              200 ms


If the channel is BUSY, the CMD_PROP_CS command is called 10 times. The
synthesizer is kept on in between the commands.

With the default settings it takes ~250 us to determine if the channel is BUSY
or not. Considering a worst case of 10 retries, the execution of the whole
chain takes around 5.6 ms, including CMD_NOP and CMD_COUNT_BRANCH.

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.
