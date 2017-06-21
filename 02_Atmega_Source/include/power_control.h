#ifndef POWER_CONTROL_H
#define POWER_CONTROL_H

/* ************************************************************************
 * Copyright (C) 2016 VeNSeY Technology
 *
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * The code contains VeNSeY Technology Confidential Proprietary
 * Information.
 * ************************************************************************/

/** @file main_application.h
 
 @project       VeNSeY IoT Board 1
 @subsystem     Power Management
 @author	    Muersel Yildiz
 @date		    2016-03-01 (YYY-MM-DD)
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
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/power.h>

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/
#define SLEEP_COUNTER_MAX       60
#define SLEEP_COUNTER_MINUTE    2
#define SLEEP_CYCLE_MAX         1

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
int sleep_counter;
int sleep_cycle_counter;
int sleep_counter_factor;
uint8_t modem_power_state;

/***************************************************************************
 * TYPE
 ***************************************************************************/

/***************************************************************************
 * FUNCTION PROTOTYPE
 ***************************************************************************/

void power_off(void);
void led_on(void);
void led_off(void);
void modem_on(void);
void modem_off(void);
void imx_on_off_toggle(void);
void imx_on(void);
void imx_off(void);
void set_ATM_special_state_high(void);
void set_ATM_special_state_low(void);
uint8_t get_modem_status(void);

/***************************************************************************
 * MACROS
 ***************************************************************************/

#endif /* POWER_CONTROL_H */

