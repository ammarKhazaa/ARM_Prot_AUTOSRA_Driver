/******************************************************************************
 *
 * Module: Led
 *
 * File Name: Led.h
 *
 * Description: Header file for Led Module.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/
#ifndef LED_H
#define LED_H

#include "Std_Types.h"
/*pre compil mesh 7 8ayr w how sha8al*/
/* Set the led ON/OFF according to its configuration Positive logic or negative logic */
#define LED_ON  STD_HIGH
#define LED_OFF STD_LOW

/* Set the LED Port */
/* if i want chang place of led*/
/*any pin must configer in 2 places PORT and DIO*/
#define LED_PORT DioConf_LED1_PORT_NUM

/* Set the LED Pin Number */
#define LED_PIN_NUM DioConf_LED1_CHANNEL_NUM
/*configration type :
                     refrence parametr,number(no.pins,no.ports),check box(on/off as in div_error)string(call_back fun.)
                     refrence parametr: its value parameter at another module at (dio(pin))
                                                                                */
/* Description: 1. Fill the led configurations structure 
 *              2. Set the PIN direction which the led is connected as OUTPUT pin
 *		3. Initialize the led ON/OFF according to the initial value 
 *
void LED_init(void);
*/
/* Description: Set the LED state to ON */
void LED_setOn(void);

/* Description: Set the LED state to OFF */
void LED_setOff(void);

/*Description: Toggle the LED state */
void LED_toggle(void);

/* Description: Refresh the LED state */
void LED_refreshOutput(void);

#endif /* LED_H */
