/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file main_application.c
 This file defines the main application
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management & IOT Communication Units
 @author	    Mürsel Yildiz
 @date		    2017-01-14 (YYY-MM-DD)
 **************************************************************************/

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include "main_application.h"
#include "gpio_twi.h"
#include "communication_handler.h"
#include "power_control.h"

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/

/***************************************************************************
 * MACROS
 ***************************************************************************/

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
const int start_up_wait = 300;   // Start up wait duration after power up

/***************************************************************************
 * FUNCTION DEFINITIONS
 ***************************************************************************/


/**
 * @brief  Initialization function for the ports
            PB0: Output     3v3 Output activation
            PB1: Input      AdapterStatus_IN, i.e. charging status
            PB2: Input      ADXL_INT, i.e. interrupt pin from ADXL Vibration Sensor
            PB3: Input      Programming interface
            PB4: Input      Programming interface
            PB5: Input      Programming interface
            PB6: Input      External CLK interface / do not touch
            PB7: Input      External CLK interface / do not touch

            PC0: Input
            PC1: Input
            PC2: Input
            PC3: Input      This pin is used for tracking the MODEM_STATUS pin
            PC4: Input      ATM_SDA, this will be set for i2c interface
            PC5: Input      ATM_SCL, this will be set for i2c interface
            PC6: Input      Programming interface

            PD0: Input      ATM_RXD
            PD1: Output     ATM_TXD
            PD2: Output     ATM_FALL_INTO_SPECIAL_STATE1 / imx communication
            PD3: Input      ATM_FALL_INTO_SPECIAL_STATE2 / imx communication
            PD4: Output     IMX_PSWITCH
            PD5: Output     IMX_RECOVERY
            PD6: Input      Can be ignored
            PD7: Output     MODEM_PWRKEY_SIG

            ADC6: Analog In ATM_ADC6_BattStatus
            ADC7: Ignore
 * @param
 ***********************************************************************/

void set_ports(void)
{
    /*
     * Set port B
     */
    DDRB |= (1 << PB0);
    DDRB &= ~(1 << PB1);
    DDRB &= ~(1 << PB2);
    /*
     * Set port D
     */
    DDRD &= ~(1 << PD0);
    DDRD |= (1 << PD1);
    DDRD |= (1 << PD2);
    DDRD &= ~(1 << PD3);
    DDRD |= (1 << PD4);
    DDRD |= (1 << PD5);
    DDRD &= ~(1 << PD6);
    DDRD |= (1 << PD7);
    /*
     * set the PORTD to low
     */
    PORTD &= ~(1 << PD7);
    /*
     * set the PC3 as input, activate internal pull up
     */
    DDRC &= ~(1 << DDC3);
}


/**
 * @brief  Initialization function for the configuration of the global variables
 * @param
 ***********************************************************************/

void initiate_global_variables(void)
{
    /*
     * reset sleep cycle and counter
     */
    sleep_counter = 0;
    sleep_cycle_counter = 0;
    
    /*
     * Clear interrupts
     */
    cli();
    
    /*
     * Set state of the ports, i.e. input / output / analog in, etc.
     */
    set_ports();
    
    /*
     * Initiate i2c slave mode
     */
    prepare_port_for_i2c();
    
    /*
     * set ATM_FALL_INTO_SPECIAL_STATE1 to low to high
     */
    set_ATM_special_state_low();
    set_ATM_special_state_high();
    sleep_counter_factor = 1;
    
    /*
     * put the sleep_counter_factor to the related register
     */
    set_twi_regs_buf_withposition(sleep_counter_factor, CURRENT_SLEEP_DURATION_POSITION);
    
    /*
     * set the modem power state to 0, this is at the boot up time
     */
    modem_power_state = 0;
    set_twi_regs_buf_withposition(modem_power_state, MODEM_POWER_STATE_POSITION);
    
    /*
     * Enable the interrupts.
     */
    sei();

}


int main(void)
{
    /*
     * Initiate global variables...
     */
    initiate_global_variables();
    
    /*
     * Wait for 300 milliseconds before powering up the imx233
     * this is due to stabilization of the power lines.
     */
    _delay_ms(start_up_wait);
    
    /*
     * Power on the imx
     * Note that for the first time boot ups, the modem is not
     * started, this is due to certain conditions where we need to
     * charge a battery and the atmega is newly started, which may
     * cause a brownout on the battery pins of the imx.
     */
    imx_on();
    
    /*
     * The main application infinite loop
     */    
    while(1) {
        power_off();
    }
    
    return 0;
}






