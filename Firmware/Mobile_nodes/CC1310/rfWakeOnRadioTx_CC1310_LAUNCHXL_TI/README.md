Example Summary
---------------
This examples showcases the Wake-on-Radio (WoR) functionality of the CC1310 to
significantly lower the power consumption of an RF link. It shows how to use
the RF Driver to schedule automatic wake-ups in the future and send messages
with long preambles.

This example is intended to be used together with the rfWakeOnRadioRx example.

Peripherals Exercised
---------------------
* `Board_LED1`- Toggled when data is transmitted over the RF interface
* `Board_BUTTON0` - Press to send packet

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide (http://www.ti.com/lit/SPRUHU7C).
For convenience, a short summary is also shown below.

    | Development Boards | Notes                                               |
    | ================== | =================================================== |
    | CC1310DK           | Board_LED1 is the "LED1" LED                        |
    |                    | Board_BUTTON0 is "Board_KEY_UP"                     |
    | ------------------ | --------------------------------------------------- |
    | CC1310_LAUNCHXL    | Board_LED1 is the "Green" LED                       |
    |                    | Board_BUTTON0 is "Board_BTN1"                       |
    | ------------------ | --------------------------------------------------- |

Example Usage
-------------
Run the example on one of the boards above, this will be the TX board. Pressing
Board_BUTTON0 will trigger a transmit. Every time a packet is sent,
Board_LED1 will toggle.

Start the rfWakeOnRadioRx companion example on another board (RX board).
Board_LED1 on the RX board should now be blinking.

With the RX board running, press Board_BUTTON0 on the TX board. Both Board_LED1
on the TX board and Board_LED1 on the RX board should now toggle.

Basic configuration
--------------------------
It is possible to use the Code Export feature of SmartRF Studio with the
Packet TX / RX tab selected to export settings for this example. This example
has been tested mainly with 50kbit/s.

The wakeup interval is set using the WOR_WAKEUPS_PER_SECOND define at the top
of the rfWakeOnRadioTx.c file. Make sure that this is set to the same in both
the RX and TX part of the Wake-on-Radio example.

Application Design Details
--------------------------
The complete Wake-on-Radio example suite is based on the principle of
duty-cycling the radio and entering RX just as much as necessary to detect
a packet.

This TX example is very similar to the rfPacketTX example except that it uses
the CMD_PROP_TX_ADV to be able to send a long preamble.

The length of the preamble is dynamically calculated based on the
WOR_WAKEUPS_PER_SECOND define at the top of the rfWakeOnRadioTx.c file.

For more details about the general operation of the Wake-on-Radio example
suite, please see the rfWakeOnRadioRx README.

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

## References
* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.
