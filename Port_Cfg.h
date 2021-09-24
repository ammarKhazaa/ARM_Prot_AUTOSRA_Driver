 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Kassem
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)


/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_OFF)

/* Pre-compile option for Port_SetPinDirection API */
#define PORT_SET_PIN_DIRECTION_API 		    	TRUE

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API  

/* Number of Pins excluding JTAG Pins */
#define PINS_NUMBER_EXCEPT_JTAG 				39

/*Defines for ports for code readability*/
#define PORTA (0U)
#define PORTB (1U)
#define PORTC (2U)
#define PORTD (3U)
#define PORTE (4U)
#define PORTF (5U)

/*Defines for pins for code readability*/
#define PIN0 (0U)
#define PIN1 (1U)
#define PIN2 (2U)
#define PIN3 (3U)
#define PIN4 (4U)
#define PIN5 (5U)
#define PIN6 (6U)
#define PIN7 (7U)

/*Definitions For Modes*/
/***********************/
	   /*Mode*/    /*Value to be written in the control register*/
#define DIO 		(10U)
#define ADC 		(0U)
#define UART 		(1U)
#define UART1	 	(2U)
#define SSI 		(2U)
#define I2C 		(3U)
#define CAN0 		(3U)
#define CAN 		(8U)
#define PWM0 		(4U)
#define PWM1 		(5U)
#define QEI 		(6U)
#define GPT 		(7U)
#define USB 		(8U)
#define AC  		(9U)
#define TRD 		(14U)

/*Masks Definitions for code readability*/
#define BIT_MASK 	(0x01)
#define BYTE_MASK	 (0x0F)



#endif /* DIO_CFG_H */
