/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file power_control.c
 This file defines power management related functionalities.
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management & IOT Communication Units
 @author	    Muersel Yildiz
 @date		    2017-01-14 (YYY-MM-DD)
 **************************************************************************/

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include "power_control.h"
#include "communication_handler.h"

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/

/***************************************************************************
 * MACROS
 ***************************************************************************/

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
char intCounter, adcsra, mcucr1, mcucr2;

/***************************************************************************
 * FUNCTION DEFINITIONS
 ***************************************************************************/


/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void power_off(void)
{
    /*
     * Make sure that the sleep mode is set firstly.
     */
    PRR |= (1<<PRTWI) | (1<<PRSPI) | (1<<PRUSART0);
 
    sleep_enable();
    power_adc_disable();
    power_spi_disable();
    power_twi_disable();
    ADCSRA = 0;                    //disable ADC
    cli();
 
    mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);  //turn off the brown-out detector
    mcucr2 = mcucr1 & ~_BV(BODSE);
    MCUCR = mcucr1;
    MCUCR = mcucr2;
    /*
     * Set the timer for sleep duration
     */
    OCR1A = 0xFFFF;
    
    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A
    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // set prescaler to 1024 and start the timer
    
    /*
     * Timer is set, now set the interrupts
     */
    sei();                         //ensure interrupts enabled so we can wake up again
    sleep_cpu();                   //go to sleep
    
    sleep_disable();
    power_all_enable();

}

/**
 * @brief  Interrupt routine for the timer 0
 * @param
 ***********************************************************************/
ISR (TIMER1_COMPA_vect)
{
    /*
     * Note that two counters are defined in order to increase the resolution 
     * This is for testing purposes
     */
    sleep_counter = sleep_counter + 1;
    /*
     * check the first threshold
     */
    if (sleep_counter >= ( sleep_counter_factor * SLEEP_COUNTER_MAX) ){
        /*
         * second level counter is incremented...
         */
        sleep_cycle_counter = sleep_cycle_counter + 1;
        sleep_counter = 0;
        /*
         * check second level counter
         */
        if ( sleep_cycle_counter >= SLEEP_CYCLE_MAX )
        {
            sleep_cycle_counter = 0;
            /*
             * In order to provide imx that the routine cycle for data exchange is expected,
             * the ATM_FALL_INTO_SPECIAL_STATE1 is set to 1 for a minute, 
             * this will be checked by the sleep_counter and the pin will be set to ground 
             * again...
             */
            set_ATM_special_state_high();
            
        }
    }
    else if ( sleep_counter == SLEEP_COUNTER_MINUTE )
    {
        /*
         * once the sleep_counter is set to 0, for around 1 minute, the ATM_FALL_INTO_SPECIAL_STATE1
         * is kept as high.
         */
        set_ATM_special_state_low();
    }
 
}


/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_ATM_special_state_high(void)
{
    PORTD |= (1 << PD2);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_ATM_special_state_low(void)
{
    PORTD &= ~(1 << PD2);
}


/**
 * @brief  Main IMX Processor power line on
 * @param
 ***********************************************************************/
void imx_on_off_toggle(void)
{
    /*
     * Set the PD4 as output, keep rest as is
     */
    DDRD = DDRD | (0x10);

    /*
     * Keep the pin low
     */
    PORTD = PORTD & (0xEF);
    
    /*
     * Now keep the pin high for 1 seconds,
     */
    PORTD = PORTD | (0x10);
    _delay_ms(200);
    
    /*
     * Keep the pin low
     */
    PORTD = PORTD & (0xEF);
    
}


/**
 * @brief  Main IMX Processor power line on
 * @param
 ***********************************************************************/
void imx_on(void)
{
    /*
     * Keep the pin high (PD4)
     */
    PORTD |= (1 << PD4);
    
    /*
     * Keep the pin recovery low (PD5)
     */
    PORTD &= ~(1 << PD5);
    
    /*
     * wait for a short time duration and keep pin high again.
     */
    _delay_ms(500);
    
    /*
     * Keep the pin low (PD4)
     */
    PORTD &= ~(1 << PD4);
}

/**
 * @brief  Main IMX Processor power line on
 * @param
 ***********************************************************************/
void imx_off(void)
{
    /*
     * Set the PD4 as output, keep rest as is
     */
    DDRD = DDRD | (0x30);
    
    /*
     * Keep the pin low
     */
    PORTD = PORTD & (0xCF);
    
}


/**
 * @brief  Modem Processor power line on
 * @param
 ***********************************************************************/
void modem_on(void)
{
    /*
     * check the modem state first and perform the action accordingly,
     */
    if( get_modem_status() != 1 ){
        /*
         * As we have the POWER KEY based design, it is possible
         * to powe the modem on.
         * This key is at port D pin 7. keep it at low
         */
        PORTD |= (1 << PD7);
        
        /*
         * Set the key low and high for 1 seconds.
         */
        _delay_ms(1000);
        
        /*
         * Now keep the pin low for a while
         */
        PORTD &= ~(1 << PD7);
        
        
        /*
         * finally set the local state variable
         */
        modem_power_state = 1;
        
        /*
         * update the related send regbuf position
         */
        set_twi_regs_buf_withposition(modem_power_state, MODEM_POWER_STATE_POSITION);
    }
}

/**
 * @brief  Modem Processor power line off
 * @param
 ***********************************************************************/
void modem_off(void)
{
    /*
     * check the modem state first and perform the action accordingly,
     */
    if( get_modem_status() != 0 ){
    
        /*
         * Make sure that the pin is at low.
         */
        PORTD &= ~(1 << PD7);
        _delay_ms(200);
        /*
         * As we have the POWER KEY based design, it is possible
         * to powe the modem on.
         * This key is at port D pin 7. keep it at low
         */
        PORTD |= (1 << PD7);
        
        /*
         * Set the key low and high for 1 seconds.
         */
        _delay_ms(1000); 
    
        /*
        * Now keep the pin low for a while
        */
        PORTD &= ~(1 << PD7);
    
        /*
         * finally set the local state variable
         */
        modem_power_state = 0;
        
        /*
         * update the related send regbuf position
         */
        set_twi_regs_buf_withposition(modem_power_state, MODEM_POWER_STATE_POSITION);
     }
}




/**
 *
 * @brief
 * @param
 ***********************************************************************/
uint8_t get_modem_status(void)
{
    /*
     * Check whether the pin3 is at high, which would mean that Modem is on
     */
    if( (PINC & (1<<PINC3) ) == (1<<PINC3) )
    {
        modem_power_state = 1;
    }
    else
    {
        modem_power_state = 0;
    }
    return modem_power_state;
}






