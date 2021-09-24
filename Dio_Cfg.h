 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Dio Driver
 *
 * Author: Ammar Khazaa
 ******************************************************************************/

#ifndef DIO_CFG_H
#define DIO_CFG_H

/*
 * Module Version 1.0.0
 */
#define DIO_CFG_SW_MAJOR_VERSION              (1U)
#define DIO_CFG_SW_MINOR_VERSION              (0U)
#define DIO_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define DIO_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define DIO_DEV_ERROR_DETECT                (STD_ON)    /* i choose devolpment error so get ti by on*/
                                                        /* i choose devolpment error on gui of tool*/
/* Pre-compile option for Version Info API */
#define DIO_VERSION_INFO_API                (STD_OFF)

/* Pre-compile option for presence of Dio_FlipChannel API */
#define DIO_FLIP_CHANNEL_API                (STD_ON)

/* Number of the configured Dio Channels continer*/
#define DIO_CONFIGURED_CHANNLES              (2U)               /*i choose 2 pins so the config code with 2  size of
                                                                 array of structure yaly gowah configrration structure bta3y
                                                                fy rasmh lal array*/

/* Channel Index in the array of structures in Dio_PBcfg.c */
/*i make container to each port 
                                may could group or channel */
/*named channek_id continer*/
#define DioConf_LED1_CHANNEL_ID_INDEX        (uint8)0x00     /*so from array hagat al led index 0 w 7agat al switch index 01*/  
#define DioConf_SW1_CHANNEL_ID_INDEX         (uint8)0x01     /*lma a7b akteb 3 led a3at index0 & resd switch ab3at index1
                                                              f hwa go access struture index dh w ygeep me gwah al port no. w channel no.*/

/* DIO Configured Port ID's  */
/*here i can add new continer to chang the port to another port*/
/*i configer according to my need*/
/*i make container to each port 
                                may could group or channel */
#define DioConf_LED1_PORT_NUM                (Dio_PortType)5 /* PORTF */ /*form array put  here 5 and 1 in DioConf_LED1_CHANNEL_NUM    LED*/
#define DioConf_SW1_PORT_NUM                 (Dio_PortType)5 /* PORTF */ /*form array put  here 5 and 4 in DioConf_LED1_CHANNEL_NUM    switch*/

/* DIO Configured Channel ID's */
/*no. of pin*/
/*chang no. here to chang place f pin and regenarat again beacause iit is generated code*/
/*static code is numbers*/
#define DioConf_LED1_CHANNEL_NUM             (Dio_ChannelType)1 /* Pin 1 in PORTF 
                *'if i want chaang led to blue led at pf3 save build and code will run'
                * ana b3adel fe al configrashen al static code thabet
                *meezt had al kalam reduce devolpment of next project
                * same mcal if same microcontroler so i confegrated same thing befor
even not same micro controler any dio has same continers
maybe i get an old project and modefy configration to new project
I NEED AFHAM CONTINERS AND PARAMETERS W BAS*/
#define DioConf_SW1_CHANNEL_NUM              (Dio_ChannelType)4 /* Pin 4 in PORTF */

#endif /* DIO_CFG_H */
/*so dio.c & dio.h did not have any change i just make all change i confgration*/