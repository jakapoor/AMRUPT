; File Name:	ATTiny20_TXer_V1.4.1.asm
; Created:		6/7/2017 
; Modified		6/7/2017 
; Author:		Julian Kapoor
; Contact:		procnias@gmail.com
; Version:		1.4.1
; Software:		Composed in Atmel Studio 7.0.1417
; Chip:			ATtiny20 MCU
; Copyright:	
; Description:	Below is the AVR Assembly code to be flashed to (i.e. to program) the ATtiny20-based transmitter. To program the ATtiny20 microcontroller (MCU) with this code, follow the directions for programming an Arduino UNO board as an In 
;				System Programmer (ISP) using the provided Arduino sketch (.ino, ISP software) and the programming shield (the device that serves as the physical adapter between the Arduino UNO and the transmitter). Alternatively, the Atmel 
;				corporation provides programmers that will interface directly with with their Assembly code editing program (Atmel studio). To modify this program, Atmel Studio is a reasonable choice, although any text editor will work.
;				-The following program operates by setting up the MCU to run in "active" mode for accurate timing of pulse durations (PDs) and inter-pulse-intervals (IPIs), at which time it will source current to an analog radio transmitter circuit,
;					with a carrier frequency determined by the crystal oscillator on the tag, using the codes provided by the user (see INPUTS below) to modulate the signal using on-off keying (OOK) where a 1 is an RF pulse, and a 0 is no pulse.
;					The length of PDs and IPIs have been fixed at 0.25 s, but can be modified in the code (see the section of code labeled XXXXXX). To save power, during and after a transmission (i.e. the 8 bit ID code) the
;					MCU goes into low power mode, which reduces the accuracy of timing by relying on a less accurate, but less power-hungry timing circuit (CLOCK; 128 kHz versus 8 MHz), and by shutting off all unused functions of the MCU 
;					(known as peripherals) and relying on only the slower clock (the 'Watchdog timer') to determine the time between transmissions - the inter-transmission-interval (ITI). Additional functionality (such as support for various types
;					of sensors) can be added on to this program, however additional hardware may be needed (i.e. sensors, data storage integrated circuits [ICs], energy harvesting ICs), as well as modification of the circuit board that holds and 
;					connects these physical components. Possible modifications of the tag's functionality that can be made entirely in the code (i.e. without altering the physical hardware of the tag) include (but are not necessarily limited to) 
;					alteration of the transmitted data (sequences of 0's and 1's) and total number of bits encoding the transmission, durations of the PD, IPI, and ITI, and power-saving settings. 
;				-INPUTS to be provided by the user:
;					3.	ID HEX CODE to indicate the 8 bits (i.e. 1 byte) that make up the unique ID of the tag (this is the data used by the receiver to identify which tag is transmitting). It is VITAL
;						that the codes, and all data associated with them (e.g. individual tagged, date tagged, location, etc.) be kept in a safe place, and that there are no redundancies in the ID codes of tags deployed in an area unless this is
;						specifically desired for a given tagging project. Note that the same IDs could be used for co-occuring animals (e.g. for different sympatric species) if the tags are operating with a different carrier frequency (determined
;						by the crystal soldered to the circuit board on the tag)
;							NO DEFAULT: <HEX examples> 0x35, 0xD7, 0x2F, 0x48, (i.e. '00110101' '11010111' '00101111' '01001000')
; Mods:			This version differs from the previous version 1.3.2 in having a logicl-level high -> on functionality rather than logic-level low -> on functionality. This was implemented to ensure that power brown-outs did not inadvertantly result
;				in the radio turning on and bleeding out the rest of the battery's current. This program should be paired with hardware version 3.1 of the ATtiny20_beeper_HS_small tag.
;===================================================================================================================================================================================================================================================================

;===========================================================================
; 1.	Specify the name of the MCU being used for this project (ATtiny20):=
;===========================================================================
.DEVICE ATtiny20

;================================================================================================================
; 2.	Declare global variables (i.e. labels that contain data that can be used at any point in the code below)=
;================================================================================================================
;		-Variables that MUST be edited by the user
;=================================================
.EQU	ID		=	0x01 ; ID byte contains the 8 bits used to encode the ID transmission.

;================================================================================================================
;To alter the inter-pulsetrain-interval (ITI), set the bits located in the code below. Following is the watchdog timer prescaler which determines the ITI (default 8 s). 4 bits: WDP3[1], WDP2[0], WDP1[0], WDP0[1], or 0x09 in HEX. See page 34 of MCU datasheet.
;		-Variables that should not normally be edited by the user
;================================================================
.DEF	temp   		=	r16	 ; define r16 as the default general purpose temporary register
.DEF	databyte	=	r19	 ; define r19 as the default RF pulsetrain code register

;=============================================================================================================
;=3.	Declare that what follows is the 'Code Segment' and identify where in memory the program should start=
;=============================================================================================================
.CSEG ; code section
.ORG	0x0000 ; the starting address (or origin) of the whole program
	;Interrupt vectors
	rjmp	RESET			; upon power-up of the MCU, or any form of reset (resets should not occur here though) do a Relative JuMP (RJMP) to the code labeled RESET. This is the 'main' section of the code for setup.
	reti	;isr_int0		; external interrupt request 0
	reti	;isr_pcint0		; pin change interrupt request 0
	reti	;isr_pcint1		; pin change interrupt request 1
	rjmp	isr_wdt			; watchdog timeout
	reti	;TIM1_CAPT		; timer/counter1 input capture
	reti    ;TIM1_COMPA		; Timer/Counter1 Compare Match A
	reti	;TIM1_COMPB		; Timer/Counter1 Compare Match B
	reti	;TIM1_OVF		; Timer/Counter1 Overflow
	reti	;TIM0_COMPA		; Timer/Counter0 Compare Match A
	reti	;TIM0_COMPB		; Timer/Counter0 Compare Match B
	reti	;TIM0_OVF		; Timer/Counter0 Overflow
	reti	;ANA_COMP		; Analog Comparator
	reti	;ADC			; ADC Conversion Complete
	reti	;TWI_SLAVE		; Two-Wire Interface
	reti	;SPI Serial		; Peripheral Interface
	reti	;QTRIP			; Touch Sensing
 
	;Interrupt service routines
isr_wdt:
	rcall	WD_OFF			; once a wdt interrupt fires, the interrupt will call this ISR and immediately go to the code to turn off the WDT to prevent further WDT interrupts until reactivated
	reti					; return and enable interrupts

;===========================================================================================================================================================================
; 4.	The 'setup' portion of the program which will be run once (when the tag is first powered up), and which will set up the MCU to run through the 'loop' section below=
;===========================================================================================================================================================================
RESET:
	; 4.1	Set up the location on the MCU's memory where temporary information can be stored and manipulated (i.e. the 'stack') *Note that although the ATtiny20 has not implemented the high byte for the stack pointer it must still be written.
	;==============================================================================================================================================================================================================================================
	ldi		temp,			high(RAMEND)		; LoaDs Immediately (LDI) an 8 bit constant (the high byte containing the address of the end of the RAM in the stack) and saves it to register 16 (the first of several designated locations for holding data)
	out		SPH,			temp				; OUTputs the data (here the address) stored in r16 to the Stack Pointer's High (SPH) byte (this sets an 'earmark' for where temporary data can be stored until needed).
	ldi		temp,			low(RAMEND)			; Loads an 8 bit constant (the low byte containing the location of the end of the RAM in the stack) directly to register 16
	out		SPL,			temp				; Stores the address of the stack pointer's low bit from r16 
		
	; 4.2	Power Savings
	;====================

			; 4.2.1	disable digital input buffer (DIDR0), and set all pull-up resistors (internal to the MCU) on unused pins (all but two [PB0 for toggling radio, PA1 for connecting GRND pin to GRND]; for power savings)
			;================================================================================================================================================
	ldi		temp,			0xFF								; The digital input buffer on all pins capable of ADC (analog to digital conversion) are disabled by feeding it ones
	out		DIDR0,			temp								; disable the digital input buffer for all ADC pins [ADC1-7] (otherwise consumes power)
	ldi		temp,			0x00								; set temp register to all zeros for use in de-activating the pull-up resistors of all Analog I/O Pins (Port A)
	out		PUEA,			temp								; set the Pull-Up Enable for the A port pins, to zeroes (i.e. disable the pull-ups)
	ldi		temp,			0xFF								; prepare to set all A port pins to output (1=output, 0=input)
	out		DDRA,			temp								; set data direction by assigning Data Direction Register for Port A pins (DDRA) to the data held in temp
	ldi		temp,			0x00								; prepare to set all A port pins to low (1=HIGH, 0=LOW)
	out		PORTA,			temp								; assign all A port pins to low (ensures that the pins are not leaking current)
	ldi		temp,			(1<<PUEB3)|(1<<PUEB2)|(1<<PUEB1)	; set temp register to ones for activating the pull-up resistors of three of four Digital I/O Pins (Port B). Note that the pin PB0 (the one being used to control the radio) is not being disabled
	out		PUEB,			temp								; set the Pull-Up Enable for the B port pins to ones (i.e. enable the pull-ups)

			; 4.2.2	Disable Brown-Out Detector (Configuration Byte's default values indicate that the BOD is always off, so nothing needs to happen here)
			;=============================================================================================================================================

			; 4.2.3	Disable Analog-to-Digital Converter (ADC) and Analog Comparator (AC) to save power. Neither are being used here, and they consume power
			;==============================================================================================================================================
	ldi		temp,			(1<<ACD)|(0<<ACIE)											; Set Analog Comparator Disable to 1 to disable the AC, and the Analog Comparator Interrupt Enable (ACIE) should be disabled to keep interrupts from interfering with this process
	out		ACSRA,			temp														; Set the Analog Comparator control and Status Register (port A) to the values in temp
	ldi		temp,			0<<ADEN														; Ensure that the Analog to Digital comparator ENable (ADEN) register is set to 0 (0 by default). This must be done before shutting down the ADC in the PRR (see below)
	out		ADCSRA,			temp														; Disable ADC
	ldi		temp,			(1<<PRTWI)|(1<<PRSPI)|(1<<PRTIM1)|(1<<PRTIM0)|(1<<PRADC)	; Disable the following unused functions: Power Reduction Two-Wire Interface, Power Reduction Serial Peripheral Interface, Power Reduction Timer/Counter1, Power Reduction Timer/Counter0, Power Reduction ADC
	out		PRR,			temp														; Set bits in Power Reduction Register (PRR)

			; 4.2.4	Disable Watchdog Timer (WDT) during active mode to save power. (Should already be off, and device will not normally be resetting)
			;==================================================================================================================================================================================================================================================================
	rcall	WD_OFF

			; 4.2.5	Set clock divider (the slower the clock runs the less power it consumes, but the less precise [the not less accurate] the timing of pulses and intervals)
			;================================================================================================================================================================
			; -8 MHz [division factor 1] oscillator's period is 0.125 탎. [0x00 in HEX]
			; -4 MHz [division factor 2] period is 0.25 탎.
			; -2 MHz [DF 4] period 0.5 탎.
			; -1 MHz [DF 8] period 1 탎. This is the chosen rate. 4 bits determine how to set this: CLKPS3[0], CLKPS2[0], CLKPS1[1], and CLKPS0[1]  or 0x03 in HEX. See page 21 of MCU datasheet.
			; -500 kHz [DF 16] period 2 탎. 
			; -250 kHz [DF 32] period 4 탎.
			; -125 kHz [DF 64] period 8 탎.
			; -62.5 kHz [DF 128] period 16 탎.
			; -31.25 kHz [DF 256] period 32 탎. [0x08 in HEX] Clock divided by 256 (multiplies all durations by 256 since clock "appears" to be running 256X more slowly; see above) 
	ldi		temp,			0x03	; Clock divided by 8 (multiplies all durations by 8 since clock "appears" to be running 8X more slowly; see above)
	ldi		r17,			0xD8	; The key for CCP (the signature [11011000 or 0xD8] for Configuration Change Protection Register, which allows the contents of other protected registers to be altered)
	out		CCP,			r17		; Output signature contained in r17 to Configuration Change Protection register, which allows protected changes
	out		CLKPSR,			temp	; Sets the clock pre-scaler to the value held in temp (sets the clock division factor; cannot be set unless CCP is "unlocked" with the signature, so must be done first)

	; 4.3	Initialize port pin to be used for turning on and off the radio frequency (RF) transmissions (i.e. the pulses that encode the PRMBL, SYNC, and ID)
	;=========================================================================================================================================================
	ldi		temp,			1<<PB0	; Prepare to set pin 2 (PB0) to output (1=output, 0=input), all others will remain input by default (<< allows for a single bit to be set independent of others in the address)
	out		DDRB,			temp	; Set data direction by assigning Data Direction Register for Port B pins (DDRB) to the data held in temp (only PB0 has been set)
	ldi		temp,			0<<PB0	; Load PB0 bit with a zero (i.e. OFF; this pin is operating a high-side enhancement mode switch, so a logic 1 of the pin turns on the radio, for simplicity the ID code operates with 1 as a transmission)
	out		PORTB,			temp	; assign PB0 on PORTB to low (regardless of input or output designation, ensures that the pin is not leaking current, and that the radio does not start in the 'on' condition)
	
	; 4.4	Proceed to the main loop
	;==========================================================================================
	rjmp	LOOP					; Relative JuMP to the subroutine labeled "LOOP"

;=================================================================================================================================
; 5.	LOOP: The 'loop' portion of the program which will run from beginning to end repeatedly for as long as the tag has power.=
;=================================================================================================================================
LOOP:
	; 5.1	Read ID byte
	;=====================================
		; 5.1.1	Read bits bitwise, if 1 send a pulse, else do not
	ldi		databyte,		ID			; load high byte of preamble
	rcall	READBYTE					; call the subroutine that reads all 8 bits, one after the other

	; 5.2	Prepare MCU to go to sleep for inter-transmission-interval (ITI): Set watchdog timer into interrupt mode, then go to sleep
	;=================================================================================================================================
	rcall	DELAY_8						; Call the delay subroutine that sets up a long (8 sec [default]) ITI

	; 5.3	Go back to beginning of loop, and repeat
	;===============================================
	rjmp	LOOP						; Upon wake-up go back to the beginning of the loop section 

;========================================================================================
; 6.	SUBROUTINES: Functions that are called repeatedly by other parts of the program.=
;========================================================================================
; Read the ID byte and call the subroutine responsible for evaluating whether each of the 8 bits is a 0 or 1
READBYTE:
	rcall	ONOROFF						; call the subroutine that identifies whether bit 8 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 7 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 6 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 5 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 4 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 3 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 2 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	rcall	ONOROFF						; call the subroutine that identifies whether bit 1 is a 1 (on) or 0 (off) and sends an RF pulse or not (4 cycles)
	ret									; return to the location that called the subroutine (4 cycles)

; Subroutine that determines whether the current bit is a 1 or 0
ONOROFF:
	ldi		r17,			0x80		; Create a mask that identifies the high bit of a byte 10000000 (0x80) (1 cycle)
	and		r17,			databyte	; Determine if the first bit of the byte contained in "temp" is a 1 or 0 (1 cycle)
	brne	PULSE						; If the result in r17 is not == 0 (i.e. is 1), go to PULSE subroutine (2 cycles PULSE, 1 cycle NOPULSE)
	breq	NOPULSE						; Otherwise, it's a 0, go to NOPULSE subroutine (4 cycles)
	ret									; (4 cycles)

; Current bit is high (i.e. pulse)
PULSE:
	ldi		temp,			(1<<PB0)	; Set pin PB0's value to 1 (high=on) and load into temp (1 CYCLE)
	out		PORTB,			temp		; Assign pin PB2's value to output using value in r16 (1 CYCLE) (RF PULSE BEGINS HERE)
	rcall	DELAY_16ms					; Call 16ms delay subroutine
	ldi		temp,			(0<<PB0)	; Set pin PB0's value to 0 (low=off) and load into temp (i.e. end the pulse)
	out		PORTB,			temp		; Assign pin PB2's value to output using value in r16 (Turn off RF pulse)
	rcall	DELAY_8th					; Call 1/8th s delay subroutine (creates detectable gap between consecutive ones)
	lsl		databyte					; Prepare for reading next bit by shifting all bits to the left 1 position in the register (1 cycle)
	ret									; (4 cycles)

; Current bit is low (i.e. no pulse) [REPLACE WITH CODE THAT SIMPLY SKIPS NOPULSE]
NOPULSE:
	ldi		temp,			(0<<PB0)	; Set pin PB0's value to 0 (low=off) and load into temp (1 CYCLE)
	out		PORTB,			temp		; Assign pin PB2's value to output using value in r16 (1 CYCLE) (NO RF PULSE BEGINS HERE)
	rcall	DELAY_8th					; Call 64 ms delay subroutine
	lsl		databyte					; Prepare for reading next bit by shifting all bits to the left 1 position in the register (1 cycle)
	ret									; (4 cycles)

; 8 second delay in Power-down mode with watchdog timer 
DELAY_8: 
	cli																								; prevent global interrupts
	wdr																								; reset the watchdog timer
	wdr																								; Second WDR is required to make even number of calls for WDR (for unknown reason)
	ldi		temp,			0xD8																	; write signature needed for CCP
	out		CCP,			temp																	; "unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(1<<WDP3)|(0<<WDP2)|(0<<WDP1)|(1<<WDP0)|(0<<WDE)|(1<<WDIE)|(0<<WDIF)	; set up watchdog timer so that 1) it is prescaled to time-out every 8 seconds, and 2) it is in interrupt mode (i.e. a time-out will generate an "interrupt" rather than a "reset")
	out		WDTCSR,			temp																	; program register
	sei																								; enable global interrupts so that the WDT can wake up the MCU
	rcall	GO_TO_SLEEP																				; call sleep subroutine
	ret																								; after returning from sleep/watdog shut-off, go back to invoking function

; 8th second delay in power down mode with watchdog timer 
DELAY_8th: 
	cli																								; prevent global interrupts
	wdr																								; reset the watchdog timer
	wdr																								; Second WDR is required to make even number of calls for WDR (for unknown reason)
	ldi		temp,			0xD8																	; write signature needed for CCP
	out		CCP,			temp																	; "unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(0<<WDP3)|(0<<WDP2)|(1<<WDP1)|(1<<WDP0)|(0<<WDE)|(1<<WDIE)|(0<<WDIF)	; set up watchdog timer so that 1) it is prescaled to time-out every 1/8 second, and 2) it is in interrupt mode (i.e. a time-out will generate an "interrupt" rather than a "reset")
	out		WDTCSR,			temp																	; program register
	sei																								; enable global interrupts so that the WDT can wake up the MCU
	rcall	GO_TO_SLEEP																				; call sleep subroutine
	ret																								; after returning from sleep/watdog shut-off, go back to invoking function
	
/*; 64 msecond delay in power down mode with watchdog timer 
DELAY_64ms: 
	cli																								; prevent global interrupts
	wdr																								; reset the watchdog timer
	wdr																								; Second WDR is required to make even number of calls for WDR (for unknown reason)
	ldi		temp,			0xD8																	; write signature needed for CCP
	out		CCP,			temp																	; "unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(0<<WDP3)|(0<<WDP2)|(1<<WDP1)|(0<<WDP0)|(0<<WDE)|(1<<WDIE)|(0<<WDIF)	; set up watchdog timer so that 1) it is prescaled to time-out every 64 msecond, and 2) it is in interrupt mode (i.e. a time-out will generate an "interrupt" rather than a "reset")
	out		WDTCSR,			temp																	; program register
	sei																								; enable global interrupts so that the WDT can wake up the MCU
	rcall	GO_TO_SLEEP																				; call sleep subroutine
	ret																								; after returning from sleep/watdog shut-off, go back to invoking function*/

/*; 32 msecond delay in power down mode with watchdog timer 
DELAY_32ms: 
	cli																								; prevent global interrupts
	;wdr																								; reset the watchdog timer
	wdr																								; Second WDR is required to make even number of calls for WDR (for unknown reason)
	ldi		temp,			0xD8																	; write signature needed for CCP
	out		CCP,			temp																	; "unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(0<<WDP3)|(0<<WDP2)|(0<<WDP1)|(1<<WDP0)|(0<<WDE)|(1<<WDIE)|(0<<WDIF)	; set up watchdog timer so that 1) it is prescaled to time-out every 32 msecond, and 2) it is in interrupt mode (i.e. a time-out will generate an "interrupt" rather than a "reset")
	out		WDTCSR,			temp																	; program register
	sei																								; enable global interrupts so that the WDT can wake up the MCU
	rcall	GO_TO_SLEEP																				; call sleep subroutine
	ret																								; after returning from sleep/watdog shut-off, go back to invoking function*/

; 16 msecond delay in power down mode with watchdog timer 
DELAY_16ms: 
	cli																								; prevent global interrupts
	;wdr																								; reset the watchdog timer
	wdr																								; Second WDR is required to make even number of calls for WDR (for unknown reason)
	ldi		temp,			0xD8																	; write signature needed for CCP
	out		CCP,			temp																	; "unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(0<<WDP3)|(0<<WDP2)|(0<<WDP1)|(0<<WDP0)|(0<<WDE)|(1<<WDIE)|(0<<WDIF)	; set up watchdog timer so that 1) it is prescaled to time-out every 16 msecond, and 2) it is in interrupt mode (i.e. a time-out will generate an "interrupt" rather than a "reset")
	out		WDTCSR,			temp																	; program register
	sei																								; enable global interrupts so that the WDT can wake up the MCU
	rcall	GO_TO_SLEEP																				; call sleep subroutine
	ret																								; after returning from sleep/watdog shut-off, go back to invoking function

; Put MCU to sleep
GO_TO_SLEEP:
	ldi		temp,			0xD8								; Write signature needed for CCP
	out		CCP,			temp								; "Unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(1<<SE)|(0<<SM2)|(1<<SM1)|(0<<SM0)	; The Sleep Enable (SE) bit must be written to 1 to allow the MCU to be put to sleep (Power Down Mode) during the ITI when the "sleep" operation is executed. Set the three bits for encoding the sleep mode of the MCU to '010' -> Power Down Mode (only the WDT remains active)
	out		MCUCR,			temp								; Program register
	sleep														; Enter sleep mode NOW and wait for the time-out generated interrupt, once this happens, the PC will go to the next line of code, which will deactivate global interrupts and the WDT then go to the next line of code.
	rjmp	WD_OFF												; Turn off WDT to prevent further interrupts
	ldi		temp,			0xD8								; Write signature needed for CCP
	out		CCP,			temp								; "Unlock" CCP so that changes to watchdog mode can be made
	ldi		temp,			(0<<SE)								; As a precaution, the Sleep Enable (SE) bit should be written to 0 to prevent the MCU from accidentally being put back to sleep (Power Down Mode) 
	out		MCUCR,			temp								; Program register
	ret															; Return to invoking function

; Turn off watchdog
WD_OFF:	
	cli															; Prevent interrupts from interrupting execution of this code
	wdr															; Reset watchdog timer
	wdr															; Second WDR is required to make even number of calls for WDR (for unknown reason)
	in		temp,			RSTFLR								; Load reset flag register into R16 (temp)
	andi	temp,			0<<WDRF								; Clear watchdog reset flag (must be done before attempting to turn off WDT)
	out		RSTFLR,			temp								; Load the new setting (i.e. cleared WD reset flag) back into RSTFLR
	ldi		temp,			0xD8								; Write signature for change enable of protected I/O register
	out		CCP,			temp								; Load password into register
	ldi		temp,			(0<<WDE)|(0<<WDIE)|(0<<WDIF)		; Within four instruction cycles, turn off watchdog enable and interrupt enable bits
	out		WDTCSR,			temp								; Load the new settings (i.e. cleared WD enable, cleared WD interrupt enable, and cleared [datasheet says 1 = cleared???] WD interrupt flag) back into RSTFLR
	sei															; Re-enable global interrupts
	ret															; Return to invoking function