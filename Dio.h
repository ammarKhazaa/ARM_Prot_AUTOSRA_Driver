 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Dio Driver
 *
 * Author: Ammar Khazaa
 ******************************************************************************/

#ifndef DIO_H
#define DIO_H

/* Id for the company in the AUTOSAR
 * for example Mohamed Tarek's ID = 1000 :) */
#define DIO_VENDOR_ID    (1000U)

/* Dio Module Id */
/*fun. tba3 det_error take MODULE_ID & INSTANCE_ID*/
#define DIO_MODULE_ID    (120U)

/* Dio Instance Id */
#define DIO_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define DIO_SW_MAJOR_VERSION           (1U)
#define DIO_SW_MINOR_VERSION           (0U)
#define DIO_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_AR_RELEASE_MAJOR_VERSION   (4U)
#define DIO_AR_RELEASE_MINOR_VERSION   (0U)
#define DIO_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Dio Status
 */
#define DIO_INITIALIZED                (1U)
#define DIO_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"
/*then check version*/
/* AUTOSAR checking between Std Types and Dio Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Dio Pre-Compile Configuration Header file */
#include "Dio_Cfg.h"
/*cfg #defin pre compil configration generated code and maby has bug so check on its AUTOSAR & SW version*/
/* AUTOSAR Version checking between Dio_Cfg.h and Dio.h files */
#if ((DIO_CFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (DIO_CFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (DIO_CFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Dio_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Dio_Cfg.h and Dio.h files 
make sure static and generated code have same version*/
#if ((DIO_CFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION)\
 ||  (DIO_CFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION)\
 ||  (DIO_CFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
  #error "The SW version of Dio_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for DIO read Channel */
#define DIO_READ_CHANNEL_SID           (uint8)0x00

/* Service ID for DIO write Channel */
#define DIO_WRITE_CHANNEL_SID          (uint8)0x01

/* Service ID for DIO read Port */
#define DIO_READ_PORT_SID              (uint8)0x02

/* Service ID for DIO write Port */
#define DIO_WRITE_PORT_SID             (uint8)0x03

/* Service ID for DIO read Channel Group */
#define DIO_READ_CHANNEL_GROUP_SID     (uint8)0x04

/* Service ID for DIO write Channel Group */
#define DIO_WRITE_CHANNEL_GROUP_SID    (uint8)0x05

/* Service ID for DIO GetVersionInfo */
#define DIO_GET_VERSION_INFO_SID       (uint8)0x12

/* Service ID for DIO Init Channel */
#define DIO_INIT_SID                   (uint8)0x10

/* Service ID for DIO flip Channel */
#define DIO_FLIP_CHANNEL_SID           (uint8)0x11

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/*ch7*/
/* DET code to report Invalid Channel */
#define DIO_E_PARAM_INVALID_CHANNEL_ID (uint8)0x0A

/* Dio_Init API service called with NULL pointer parameter */
#define DIO_E_PARAM_CONFIG             (uint8)0x10

/* DET code to report Invalid Port */
#define DIO_E_PARAM_INVALID_PORT_ID    (uint8)0x14

/* DET code to report Invalid Channel Group */
#define DIO_E_PARAM_INVALID_GROUP      (uint8)0x1F

/*
 * The API service shall return immediately without any further action,
 * beside reporting this development error.
 */
#define DIO_E_PARAM_POINTER             (uint8)0x20

/*
 * API service used without module initialization is reported using following
 * error code (Not exist in AUTOSAR 4.0.3 DIO SWS Document. "user guid" det_error
 */
#define DIO_E_UNINIT                   (uint8)0xF0
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/*ch8*/
/* Type definition for Dio_ChannelType used by the DIO APIs no. of pin 0 to 42 */
typedef uint8 Dio_ChannelType;

/* Type definition for Dio_PortType used by the DIO APIs */
typedef uint8 Dio_PortType;

/* Type definition for Dio_LevelType used by the DIO APIs 1/0 high or low*/
typedef uint8 Dio_LevelType;

/* Type definition for Dio_PortLevelType used by the DIO APIs uint8 according no. of pins "read write on port"*/
typedef uint8 Dio_PortLevelType;

/* Structure for Dio_ChannelGroup */
typedef struct
{
  /* Mask which defines the positions of the channel group */
  uint8 mask;/*no. of pin access 11100000*/
  /* Position of the group from LSB */
  uint8 offset;/*pin start form*/
  /* This shall be the port ID which the Channel group is defined. */
  Dio_PortType PortIndex;/*no. of port*/
} Dio_ChannelGroupType;/*limitation same port and wara ba3d*/

typedef struct
{
	/* Member contains the ID of the Port that this channel belongs to */
	Dio_PortType Port_Num;
	/* Member contains the ID of the Channel     no. pin*/
	Dio_ChannelType Ch_Num;
}Dio_ConfigChannel;

/* Data Structure required for initializing the Dio Driver */
typedef struct Dio_ConfigType
{
	Dio_ConfigChannel Channels[DIO_CONFIGURED_CHANNLES];
} Dio_ConfigType; /*array of struct type Dio_ConfigChannel:2 information of pin(no.port & n. of pin) its size depends on number of pins u configered*/

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for DIO read Channel API */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/* Function for DIO write Channel API */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/* Function for DIO read Port API */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* Function for DIO write Port API */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* Function for DIO Initialization API */
void Dio_Init(const Dio_ConfigType * ConfigPtr);

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/* Function for DIO flip channel API */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

/* Function for DIO Get Version Info API */
#if (DIO_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Dio and other modules */
extern const Dio_ConfigType Dio_Configuration;

/* Extern Dio_Status to be used by button_init and led_init functions
   This will help in following the layering system of AUTOSAR by not including
   port.h and dio.h in the app*/
extern uint8 Dio_Status;
#endif /* DIO_H */
