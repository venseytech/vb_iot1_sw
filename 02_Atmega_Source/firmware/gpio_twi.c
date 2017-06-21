/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file gpio_twi.c
 This file defines i2c related functionalities
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management & IOT Communication Units
 @author	    Muersel Yildiz
 @date		    2017-03-10 (YYY-MM-DD)
 **************************************************************************/

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include "gpio_twi.h"
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
volatile uint8_t portbhistory = 0xFF;
volatile uint8_t clk_counter = 0;
volatile uint8_t data_counter = 0;
volatile uint8_t data_reg[8];
volatile uint8_t addr_reg;
volatile uint8_t command_byte;
volatile uint8_t rw_operation;
volatile uint8_t address_was_set = 0;

/***************************************************************************
 * FUNCTION DEFINITIONS
 ***************************************************************************/

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void prepare_port_for_i2c(void)
{
    
    /*
     * Port C is prepared for the PC4 (SDA) and the PC5(SCL)
     */
    DDRC &= ~((1 << DDC4) | (1 << DDC5));
    PORTC &= ~((1 << PORTC4) | (1 << PORTC5));
    
    PCICR |= (1 << PCIE1);     // set PCIE1 to enable PCMSK2 scan
    PCMSK1 |= (1 << PCINT13);  // set PCINT13 to trigger an interrupt on state change (SCL)
    PCMSK1 |= (1 << PCINT12);  // set PCINT12 to trigger an interrupt on state change (SDA)
    /*
     * store current status of the pinc
     */
    portbhistory = PINC;
    /*
     * finally initiate test buffer
     */
    init_twi_related_regs_bufs();
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void disable_interrupt(void)
{
    PCMSK1 &= ~(1 << PCINT13);
    PCMSK1 &= ~(1 << PCINT12);
    PCICR &= ~(1 << PCIE1);     // disable PCIE1 to enable PCMSK0 scan
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void enable_interrupt(void)
{
    PCICR |= (1 << PCIE1);     // set PCIE1 to enable PCMSK2 scan
    PCMSK1 |= (1 << PCINT13);   // set PCINT13 to trigger an interrupt on state change
    PCMSK1 |= (1 << PCINT12);  // set PCINT12 to trigger an interrupt on state change (SDA)
}


/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_sda_as_input(void)
{
    /*
     * Port C is prepared for the PC4 (SDA) and the PC5(SCL)
     */
    DDRC &= ~((1 << DDC4) | (1 << DDC5));
    PORTC &= ~((1 << PORTC4) | (1 << PORTC5));
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void wait_for_scl_to_low(void)
{
    int counter = 0;
    TCCR0A |= (1 << WGM01);
    OCR0A = 0xFF;
    TCCR0B |= (1 << CS01) | (1 << CS00);
    while( ((PINC & (1<<PINC5) ) == (1<<PINC5) )  && ((TIFR0 & (1 << TOV0) ) > 0) && (counter < SCL_TIMEOUT) ) {
        counter ++;
    }
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void wait_for_scl_to_high(void)
{
    int counter = 0;
    TCCR0A |= (1 << WGM01);
    OCR0A = 0xFF;
    TCCR0B |= (1 << CS01) | (1 << CS00);

    while(  ( (PINC & (1<<PINC5)) == 0 ) && ((TIFR0 & (1 << TOV0) ) > 0) && (counter < SCL_TIMEOUT) ) {
        counter ++;
    }
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void wait_for_scl_to_high_with_stop_condition(void)
{
    uint8_t tmp_sda_tracking;
    uint8_t tmp_sda_comparison;
    tmp_sda_tracking = read_sda();
    tmp_sda_comparison = tmp_sda_tracking;
    
    while( ((PINC & (1<<PINC5) ) == (1<<PINC5) )   && (tmp_sda_tracking == tmp_sda_comparison)  ) {
        /*
         * if the change occurs without the PINC5 is set to low, this is a stop condition
         */
        tmp_sda_tracking = read_sda();
    }
}



/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
uint8_t read_sda(void)
{
    if( (PINC & (1<<PINC4)) == (1<<PINC4) )
    {
        /* HIGH pin */
        return 1;
    }
    else
    {
        /* LOW pin */ 
        return 0;
    }
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_sda_to_high(void)
{
    DDRC |= (1 << PC4);
    PORTC |= (1 << PC4);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_sda_to_low(void)
{
    DDRC |= (1 << PC4);
    PORTC &= ~(1 << PC4);
}

/*
 * PC4 (SDA) and the PC5(SCL)
 */
ISR (PCINT1_vect)
{
    uint8_t changedbits;
    uint8_t clk_counter_mask;
    uint8_t send_tmp;
    uint8_t pinVal;
    uint8_t buf_operation = 1;
    
    /*
     * Note that the following call shall be removed, this is for debugging purposes for now.
     */
    changedbits = PINC ^ portbhistory;
    portbhistory = PINC;
    addr_reg = 0;
    rw_operation = 0;
    /*
     * clear the interrupt for the SCL changes in order to prevent conflicts
     */
    disable_interrupt();
    
    if(changedbits & (1 << PINC4))
    {
        if( (PINC & (1<<PINC4)) == 0)
        {
            /*
             * the SDA is set to low
             * check for the SCL, if the SCL is at high, this is a start condition
             */
            if( (PINC & (1<<PINC5)) == (1<<PINC5) )
            {
                /*
                 * This is a start condition as the SCL is still at high
                 */
                wait_for_scl_to_low();
                /*
                 * SCL is set to low
                 * reading the ADDR
                 */
                for (clk_counter = 0; clk_counter<7; clk_counter++)
                {
                    /* HIGH to LOW pin change */
                    wait_for_scl_to_high();
                    pinVal = read_sda();
                    addr_reg = (addr_reg << 1);
                    addr_reg = addr_reg + pinVal;
                    wait_for_scl_to_low();
                }
                /*
                 * check whether the addr_reg includes our address;
                 */
                if( addr_reg == SLAVE_ADRESS )
                {
                    /*
                     * check read write operation...
                     */
                    wait_for_scl_to_high();
                    rw_operation = read_sda();
                    wait_for_scl_to_low();
                    /*
                     * set line to 0 for the ACK
                     */
                    set_sda_to_low();
                    wait_for_scl_to_high();
                    wait_for_scl_to_low();
                    
                    /*
                     * Now it is time to do operation RW
                     */
                    if ( rw_operation == 1){
                        /*
                         * Control the rw_operation and ACK condition for buffer send.
                         */
                        buf_operation = 1;
                        while( buf_operation == 1 ){
                            
                            for (clk_counter = 0; clk_counter<8; clk_counter++)
                            {
                                clk_counter_mask = 7 - clk_counter;
                                send_tmp = (1 << clk_counter_mask);
                                if ( (send_tmp & send_buff[reg_buf_position]) )
                                {
                                    set_sda_to_high();
                                }
                                else
                                {
                                    set_sda_to_low();
                                }
                                wait_for_scl_to_high();
                                wait_for_scl_to_low();
                            }
                            
                            /*
                             * expect ACK or NACK from the master
                             */
                            set_sda_as_input();
                            wait_for_scl_to_high();
                            pinVal = read_sda();
                            /*
                             * Check whether the ACK / NACK is sent
                             */
                            if( pinVal == 0 )
                            {
                                /*
                                 * ACK is received, read buf operation is continued
                                 */
                                buf_operation = 1;
                                increment_regbuf_position(); 
                                reset_reg_buf_pos_if_necessary();
                                wait_for_scl_to_low();
                                
                            }
                            else
                            {
                                /*
                                 * NACK is arrived, read operation shall stop
                                 */
                                buf_operation = 0;
                                wait_for_scl_to_low();
                                wait_for_scl_to_high_with_stop_condition();
                            }
                        }
                        
                    }
                    else
                    {
                        /*
                         * rw_operation is 0, hence we expect command from Master
                         * note that this operation is just for single command operation
                         */
                        set_sda_as_input();
                        for (clk_counter = 0; clk_counter<8; clk_counter++)
                        {
                            /* HIGH to LOW pin change */
                            wait_for_scl_to_high();
                            pinVal = read_sda();
                            command_byte = (command_byte << 1);
                            command_byte = command_byte + pinVal;
                            wait_for_scl_to_low();
                        }
                        
                        /*
                         * send ACK
                         */
                        set_sda_to_low();
                        wait_for_scl_to_high();
                        wait_for_scl_to_low();
                        /*
                         * handle command byte
                         */
                        handle_command(command_byte);
                    }
                }
                else
                {
                    /*
                     * Address is for another slave, waiting for two cycles
                     */
                    wait_for_scl_to_high();
                    wait_for_scl_to_low();
                    wait_for_scl_to_high();
                    wait_for_scl_to_low();
                }
            }
        }
    }
    set_sda_as_input();
    enable_interrupt();
    
}



