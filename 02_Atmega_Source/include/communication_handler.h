#ifndef COMMUNICATION_HANDLER_H
#define COMMUNICATION_HANDLER_H

/* ************************************************************************
 * Copyright (C) 2016 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file gpio_twi.h
 This file defines i2c communication related functionalities.
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management Communication Service
 @author	    Muersel Yildiz
 @date		    2017-03-10 (YYY-MM-DD)
 **************************************************************************/

/** @mainpage
 *
 *  @section intro_sec Introduction
 *
 */

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/
#define REG_BUF_SIZE 77

/*
 * Regarding the registers, the following input positions are defined.
 */
#define MODEM_POWER_STATE_POSITION 76
#define CURRENT_SLEEP_DURATION_POSITION 75

/*
 * available commands
 */
#define SET_TEST_LED_HIGH 0x01
#define SET_TEST_LED_LOW 0x02
#define POWER_DOWN_MODEM 0x03
#define POWER_UP_MODEM 0x04
#define TOGGLE_PSWITCH 0x05
#define SET_PSWITCH_TO_HIGH 0x06
#define SET_PSWITCH_TO_LOW 0x07
#define POWER_DOWN_3V3_EXTERNAL 0x08
#define POWER_UP_3V3_EXTERNAL 0x09
#define INCREMENT_REG_BUF_POSITION 0x0A
#define DECREMENT_REG_BUF_POSITION 0x0B
#define RESET_REG_BUF_POSITION 0x0C
#define FILL_REG_BUF_HANDSHAKE_RQSTR 0x0D
#define FILL_REG_BUF_HANDSHAKE_RQSTE 0x0E
#define FILL_REG_BUF_HANDSHAKE_HDR 0x0F
#define FILL_REG_BUF_HANDSHAKE_RQST_AR 0x10
#define FILL_REG_BUF_HANDSHAKE_RQST_SH 0x11
#define RESET_MODEM 0x12
#define POWER_OFF 0x13
#define READ_BATT_TO_FIRST_REG 0x14
#define INCREMENT_SLEEP_COUNTER_FACTOR 0x15
#define DECREMENT_SLEEP_COUNTER_FACTOR 0x16
#define GET_MODEM_POWER_STATE 0x17
#define GET_CURRENT_SLEEP_DURATION 0x18

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
volatile uint16_t send_double_byte;
volatile uint8_t send_single_byte;
volatile uint8_t send_buff[REG_BUF_SIZE];
volatile uint8_t reg_buf_position;

/***************************************************************************
 * TYPE
 ***************************************************************************/

/***************************************************************************
 * FUNCTION PROTOTYPE
 ***************************************************************************/
void handle_command(uint8_t command);
void init_twi_related_regs_bufs(void);
int set_twi_regs_buf_withposition(uint8_t value, uint8_t position);
void toggle_test_led(void);
void set_test_led_low(void);
void set_test_led_high(void);
void reset_reg_buf_pos_if_necessary(void);
void increment_regbuf_position(void);
void decrement_regbuf_position(void);
void set_regbuf_position(uint8_t new_position);
void reset_regbuf_position(void);
void power_down_3v3External(void);
void power_up_3v3External(void);


/***************************************************************************
 * MACROS
 ***************************************************************************/



#endif /* COMMUNICATION_HANDLER_H */
