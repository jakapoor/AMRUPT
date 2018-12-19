Example Summary
---------------
The carrier wave (CW) example sends a continuous carrier wave or pseudo-random
modulated signal on a fixed frequency. The frequency and other RF settings can
be modified using SmartRF Studio.

Peripherals Exercised
---------------------
N/A

Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
Run the example.

Application Design Details
--------------------------
This examples consists of a single task and the exported SmartRF Studio radio
settings.

The default frequency is 868.0 MHz (433.92 MHz for the CC1350-LAUNCHXL-433). 
In order to change frequency, modify the smartrf_settings file. This can be 
done using the code export feature in Smart RF Studio, or directly in the file.

To switch between carrier wave (1) and modulated signal (0) set the following
in the code (CW is set as default):

    RF_cmdTxTest.config.bUseCw = 1; // CW
    RF_cmdTxTest.config.bUseCw = 0; // modulated signal

In order to achieve +14 dBm output power, make sure that the define
CCFG_FORCE_VDDR_HH = 0x1 in ccfg.c

When the task is executed it:

1. Configures the radio for Proprietary mode
2. Explicitly configures CW (1) or Modulated (0). Default modulated mode is
   PRBS-15
3. Gets access to the radio via the RF drivers RF_open
4. Sets up the radio using CMD_PROP_RADIO_DIV_SETUP command
5. Sets the frequency using CMD_FS command
6. Sends the CMD_TX_TEST command to start sending the CW or Pseudo-random
   signal forever

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.
