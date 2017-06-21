#ifndef GPIO_TWI_H
#define GPIO_TWI_H

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
#define SLAVE_ADRESS 0x05
#define SCL_TIMEOUT 1000

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/

/***************************************************************************
 * TYPE
 ***************************************************************************/

/***************************************************************************
 * FUNCTION PROTOTYPE
 ***************************************************************************/
void prepare_port_for_i2c(void);
void wait_for_scl_to_high(void);
void wait_for_scl_to_low(void);
void set_sda_to_high(void);
void set_sda_to_low(void);
void set_sda_as_input(void);
void disable_interrupt(void);
void reset_reg_buf_pos_if_necessary(void);
uint8_t read_sda(void);

/***************************************************************************
 * MACROS
 ***************************************************************************/
#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#endif /* GPIO_TWI_H */
