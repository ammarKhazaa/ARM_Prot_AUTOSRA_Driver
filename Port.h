 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/
/*set ione input, set one output,disable ulternative,diable analog ,enable digital, one pull_up*/
#ifndef PORT_H
#define PORT_H

#include "Common_Macros.h"
#include "Std_Types.h"
#include "Port_Cfg.h"


/* Port for the company in the AUTOSAR*/
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (130U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port_SetPinDirection */
#define PORT_SET_PIN_DIRECTION_SID           (uint8)0x00

/* Service ID for Port_Init */
#define PORT_INIT_SID                		 (uint8)0x01

/* Service ID for Port_SetPinMode*/
#define PORT_SET_PIN_MODE_SID                (uint8)0x02

/* Service ID for Port_GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID            (uint8)0x03

/* Service ID for Port_RefreshPortDirection */
#define PORT_REFRESH_PORT_DIRECTION_SID      (uint8)0x04

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN 							(uint8)0x0A

/* Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE         	    (uint8)0x0B

/* DET code to report API Port_Init service called with wrong parameter*/
#define PORT_E_PARAM_CONFIG 					    (uint8)0x0C

/* DET code to report API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_PARAM_INVALID_MODE				    (uint8)0x0D

/*
 *API Port_SetPinMode service called when mode is unchangeable.
 */
#define PORT_E_MODE_UNCHANGEABLE    		        (uint8)0x0E

/*
 *API service called without module initialization
 */
#define PORT_E_UNINIT                 			    (uint8)0x0F

/* DET code to report APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER 					    (uint8)0x10

   
/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000


/*Clock Gating Register*/
#define RCGC2_REGISTER_ADDRESS			  0X400FE108

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/*Symbolic name of a Port Pin number (43 pins)*/
typedef uint8 Port_PinType;


/* Description: Enum to hold PIN direction INPUT =0/OUTPUT=1*/
typedef enum
{
    PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;


typedef uint8 Port_PinModeType;
/* Description: Structure to configure each individual PIN wich will ysha8al al port driver:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the direction of pin --> INPUT or OUTPUT
 *      4. the internal resistor --> Disable, Pull up or Pull down
 *      5. Member initial_value will determine the initial value of the pin in case of output pin.
 *      6.Member pin_mode_changeable will determine if the pin mode can be changed after initialization.
 *      7.Member pin_direction_changeable will determine if the pin direction can be changed after initialization. 
*/
typedef struct 
{
    uint8 port_num;                  /*1 no. of port*/ 
    uint8 pin_num;                   /*2 no of pin*/   
    Port_PinDirectionType direction; /*3 direction in/out*/
    Port_PinModeType pin_mode;       /*4 reisitor off (pull up/down in case input pin)*/   
    Port_InternalResistor resistor;  
    uint8 initial_value;             /*5 intital value wich is the responssiple if port(in case output pin)*//*dio write /read in real time*/
    uint8 pin_mode_changeable; //STD_ON OR STD_OFF
    uint8 pin_direction_changeable; //STD_ON OR STD_OFF
}Port_ConfigChannel;/*our driver one function that setup pin as GPIO when i call it i send address of this structure */


typedef struct
{
	Port_ConfigChannel Channels[PINS_NUMBER_EXCEPT_JTAG];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_SetupGpioPin
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
void Port_Init(
	const Port_ConfigType* ConfigPtr );
/************************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction
************************************************************************************/
/*Precompile option to keep or remove Port_SetPinDirection function*/
#if (PORT_SET_PIN_DIRECTION_API == TRUE) 
void Port_SetPinDirection( 
	Port_PinType Pin,
	Port_PinDirectionType Direction );
#endif


/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non-Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection(void);


/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non-Reentrant
* Parameters (in): versioninfo
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
/*Precompile option to keep or remove Port_GetVersionInfo function*/
#ifdef PORT_VERSION_INFO_API
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Mode
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
void Port_SetPinMode( 
	Port_PinType Pin, 
	Port_PinModeType Mode );


/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Dio and other modules */
extern const Port_ConfigType Port_Configuration;

/* Extern Port_Status to be used by button_init and led_init functions
   This will help in following the layering system of AUTOSAR by not including
   port.h and dio.h in the app*/
extern uint8 Port_Status;

#endif /* PORT_H */
