/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file communication_handler.c
 This file defines handler functions for the communication between imx and
 atmega processors
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management Communication Service
 @author	    Muersel Yildiz
 @date		    2017-03-13 (YYY-MM-DD)
 **************************************************************************/

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
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

/***************************************************************************
 * FUNCTION DEFINITIONS
 ***************************************************************************/

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void handle_command(uint8_t command_byte)
{
    switch(command_byte) {
            
        case SET_TEST_LED_HIGH:
            set_test_led_high();
            break;
            
        case SET_TEST_LED_LOW:
            set_test_led_low();
            break;
            
        case POWER_DOWN_MODEM:
            modem_off();
            break;
            
        case POWER_UP_MODEM:
            modem_on();
            break;
            
        case RESET_MODEM:
            modem_on();
            _delay_ms(1000);
            modem_off();
            break;
            
        case TOGGLE_PSWITCH:
            break;
            
        case SET_PSWITCH_TO_HIGH:
            /*
             * Keep the pin high (PD4)
             */
            PORTD |= (1 << PD4);
            /*
             * Keep the pin recovery low (PD5)
             */
            PORTD &= ~(1 << PD5);
            break;
            
        case SET_PSWITCH_TO_LOW:
            /*
             * Keep the pin low (PD4)
             */
            PORTD &= ~(1 << PD4);
            /*
             * Keep the pin recovery low (PD5)
             */
            PORTD &= ~(1 << PD5);
            break;
            
        case POWER_DOWN_3V3_EXTERNAL:
            power_down_3v3External();
            break;
            
        case POWER_UP_3V3_EXTERNAL:
            power_up_3v3External();
            break;
            
        case INCREMENT_REG_BUF_POSITION:
            increment_regbuf_position();
            break;
            
        case DECREMENT_REG_BUF_POSITION:
            decrement_regbuf_position();
            break;
            
        case RESET_REG_BUF_POSITION:
            reset_regbuf_position();
            break;
            
        case FILL_REG_BUF_HANDSHAKE_RQSTR:
            break;
            
        case FILL_REG_BUF_HANDSHAKE_RQSTE:
            break;
            
        case FILL_REG_BUF_HANDSHAKE_HDR:
            break;
            
        case FILL_REG_BUF_HANDSHAKE_RQST_AR:
            break;
            
        case FILL_REG_BUF_HANDSHAKE_RQST_SH:
            break;
            
        case POWER_OFF:
            power_off();
            break;
            
        case READ_BATT_TO_FIRST_REG:
            break;
            
        case INCREMENT_SLEEP_COUNTER_FACTOR:
            sleep_counter_factor = sleep_counter_factor + 1;
            set_twi_regs_buf_withposition(sleep_counter_factor, CURRENT_SLEEP_DURATION_POSITION);
            break;
            
        case DECREMENT_SLEEP_COUNTER_FACTOR:
            if( sleep_counter_factor > 1 )
            {
                sleep_counter_factor = sleep_counter_factor - 1;
                set_twi_regs_buf_withposition(sleep_counter_factor, CURRENT_SLEEP_DURATION_POSITION);
            }
            break;
            
        case GET_MODEM_POWER_STATE:
            set_regbuf_position(MODEM_POWER_STATE_POSITION);
            break;
            
        case GET_CURRENT_SLEEP_DURATION:
            set_regbuf_position(CURRENT_SLEEP_DURATION_POSITION);
            break;
            
        default :
            break;
    }
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void init_twi_related_regs_bufs(void)
{
    int i = 0;
    for (i=0; i<REG_BUF_SIZE; i++)
    {
        send_buff[i] = 0x00 + i;
    }
    reg_buf_position = 0;
    send_double_byte = 0x9999;
    send_single_byte = 0x99;
}

/**
 * @brief
 * @param
 ***********************************************************************/
int set_twi_regs_buf_withposition(uint8_t value, uint8_t position)
{
    if ( position >= REG_BUF_SIZE)
    {
        toggle_test_led(); 
        return -1;
    }
    else
    {
        send_buff[position] = value;
        return 0;
    }
}



/**
 * @brief
 * @param
 ***********************************************************************/
void set_regbuf_position(uint8_t new_position)
{
    reg_buf_position = new_position;
}

/**
 * @brief
 * @param
 ***********************************************************************/
void increment_regbuf_position(void)
{
    reg_buf_position = reg_buf_position + 1;
}

/**
 * @brief
 * @param
 ***********************************************************************/
void decrement_regbuf_position(void)
{
    if( reg_buf_position >=1 )
    {
        reg_buf_position = reg_buf_position - 1;
    }
}

/**
 * @brief
 * @param
 ***********************************************************************/
void reset_regbuf_position(void)
{
    reg_buf_position = 0;
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void toggle_test_led(void)
{
    PORTB ^= (1 << PB0);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_test_led_low(void)
{
    PORTB &= ~(1 << PB0);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void set_test_led_high(void)
{
    PORTB |= (1 << PB0);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void power_down_3v3External(void)
{
    PORTB &= ~(1 << PB0);
}

/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void power_up_3v3External(void)
{
    PORTB |= (1 << PB0);
}


/**
 * @brief  Operational test purposes...
 * @param
 ***********************************************************************/
void reset_reg_buf_pos_if_necessary(void)
{
    if( reg_buf_position >= REG_BUF_SIZE)
    {
        reg_buf_position = 0;
    }
}

