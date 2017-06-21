#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

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
 @subsystem     Power Management & IOT Communication Units
 @author	    Muersel Yildiz
 @date		    2017-03-07 (YYY-MM-DD)
 **************************************************************************/

/** @mainpage
 *
 *  @section intro_sec Introduction
 *
 */

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/
#define F_CPU 16000000UL

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/

/***************************************************************************
 * TYPE
 ***************************************************************************/

/***************************************************************************
 * FUNCTION PROTOTYPE
 ***************************************************************************/
void initiate_global_variables(void);
void set_ports(void); 


/***************************************************************************
 * MACROS
 ***************************************************************************/


#endif /* MAIN_APPLICATION_H */
