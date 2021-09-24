 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"

/*
Precompile option to check if error checking is on.
If the the checking is on it keeps the det code.
*/
#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigChannel * Port_Channels = NULL_PTR;
uint8 Port_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
/*direction ,mode ,out init ,input pull */
/*tt3mal before dio i can not read /write before pin ttzabat mn port init*/
/*port init ta5od structure mn config t loop 3a 43 pin */
void Port_Init(const Port_ConfigType* ConfigPtr )
{
/*
Precompile option to check if error checking is on.
If the the checking is on it keeps the det code.
*/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        /*Report DET error*/
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
    }
    else
#endif
    {
        /*
         * Set the module state to initialized and point to the PB configuration structure using a global pointer.
         * This global pointer is global to be used by other functions to read the PB configuration structures
         */
        Port_Status   = PORT_INITIALIZED;
        Port_Channels = ConfigPtr->Channels; /* address of the first Channels structure --> Channels[0] */

        volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
        volatile uint32 delay = 0;

        volatile uint32 * PCTRreg_Ptr = NULL_PTR; /*Pointer to hold the Current Control Register Address*/

        int i;
        for(i=0 ; i<PINS_NUMBER_EXCEPT_JTAG ; i++)
        {
            switch(Port_Channels[i].port_num) /*Checking the current port number and saving its address to Port_Ptr*/
            {
                case  PORTA: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                         break;

                case  PORTB: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                         break;

                case  PORTC: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                         break;

                case  PORTD: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                         break;
      
                case  PORTE: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                         break;

                case  PORTF: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                         break;
                default:        break; /*default case to obey MISTA rules*/
            }

            /* Enable clock for PORT and allow time for clock to start*/
            if(BIT_IS_CLEAR(( *(volatile uint32 *)RCGC2_REGISTER_ADDRESS), Port_Channels[i].port_num))
            {
                SET_BIT(( *(volatile uint32 *)RCGC2_REGISTER_ADDRESS), Port_Channels[i].port_num);
                delay = SYSCTL_REGCGC2_REG; /*delay*/
            }


            /*use the unlock and commit register incase of PD7 & PF0 turn on pf0*/
            if( ((Port_Channels[i].port_num == PORTD) && (Port_Channels[i].pin_num == PIN7)) || ((Port_Channels[i].port_num == PORTF) && (Port_Channels[i].pin_num == PIN0)) ) /* PD7 or PF0 */
            {
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Channels[i].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            /*only this pin seted other m7toteen 1 mosbaqan*/
            }

            /*switch case to set the direction of the pin (input or output)*/
            switch(Port_Channels[i].direction)
            {
            case PORT_PIN_IN: CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[i].pin_num); /*clear the corresponding bit in the direction reg in case of input*/
                            break;

            case PORT_PIN_OUT: SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[i].pin_num); /*set the corresponding bit in the direction reg in case of output*/
                            break;
            default:        break; /*default case to obey MISRA rules*/
            }

            /*switch case to set the resistor type of the pin (pull up - pull down - off)*/
            switch(Port_Channels[i].resistor)
            {
            case OFF: CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET ) , Port_Channels[i].pin_num); /*clearing the corresponding bit in the pull up register*/
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET ) , Port_Channels[i].pin_num); /*clearing the corresponding bit in the pull down register*/
                            break;

            case PULL_UP: SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET ) , Port_Channels[i].pin_num); /*setting the corresponding bit in the pull up register*/
                            break;

            case PULL_DOWN: SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET ) , Port_Channels[i].pin_num); /*setting the corresponding bit in the pull down register*/
                            break;
            default:        break; /*default case to obey MISRA rules*/
            }

            /*Saving the Current Control Register Address in the PCTR_Ptr*//*turn off analog*/
            PCTRreg_Ptr = (volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET );

            if((Port_Channels[i].pin_mode) == DIO ) /*DIO pin*/
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[i].pin_num); /*clearing the corresponding bit in the analog register*/

                *PCTRreg_Ptr = *PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[i].pin_num)*4))); //Clear Control bits

                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[i].pin_num); /*clearing the corresponding bit in the alternate register*/
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[i].pin_num); /*setting the corresponding bit in the digital register*/
                if((Port_Channels[i].direction) == PORT_PIN_OUT)
                {
                    volatile uint32* DataReg_Ptr= (volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET); /*pointer to point to the current data register*/
                    *DataReg_Ptr = ((*DataReg_Ptr&(~(BIT_MASK<<(Port_Channels[i].pin_num))))|(((Port_Channels[i].initial_value)&(BIT_MASK))<<(Port_Channels[i].pin_num))); /*setting the initial value of the pin*/
                }
                
            }
            else if((Port_Channels[i].pin_mode) == ADC) /*ADC pin
                                                          in analog :enable analog ,disable digtal ,clear ultanativ,clear control 
                                                          in digita: enable digtal,disable ultanativ GPIO,enable ultanativ any other mode (put no. mode in control register)
                                                          */
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[i].pin_num);/*clearing the corresponding bit in the digital register*/

                *PCTRreg_Ptr = *PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[i].pin_num)*4))); //Clear Control bits

                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[i].pin_num); /*clearing the corresponding bit in the alternate register*/
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[i].pin_num); /*setting the corresponding bit in the analog register*/

                volatile uint32* DataReg_Ptr= (volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET); /*pointer to point to the current data register*/
                *DataReg_Ptr = ((*DataReg_Ptr&(~(BIT_MASK<<(Port_Channels[i].pin_num))))|(((Port_Channels[i].initial_value)&(BIT_MASK))<<(Port_Channels[i].pin_num))); /*setting the initial value of the pin*/

            }
            else /*any other mode (in control)*/
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[i].pin_num);/*clearing the corresponding bit in the analog register*/
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[i].pin_num);/*setting the corresponding bit in the digital register*/
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[i].pin_num);/*setting the corresponding bit in the alternate register*/
                /*Masking and setting the mode of the current pin in the right bits in the control register*/
                *PCTRreg_Ptr = ((*PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[i].pin_num)*4))))|(((Port_Channels[i].pin_mode)&(BYTE_MASK))<<((Port_Channels[i].pin_num)*4)));

                volatile uint32* DataReg_Ptr= (volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET); /*pointer to point to the current data register*/
                *DataReg_Ptr = ((*DataReg_Ptr&(~(BIT_MASK<<(Port_Channels[i].pin_num))))|(((Port_Channels[i].initial_value)&(BIT_MASK))<<(Port_Channels[i].pin_num))); /*setting the initial value of the pin*/

            }

        }
    }
}


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
/*chang dirction in run time chang mode in run time*/
#if (PORT_SET_PIN_DIRECTION_API == TRUE)

void Port_SetPinDirection( 
    Port_PinType Pin, //range 0 .. 43
    Port_PinDirectionType Direction )
{
/*
Precompile option to check if error checking is on.
If the the checking is on it keeps the det code.
*/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if(Port_Status  == PORT_NOT_INITIALIZED)
    {
        /*Report DET error*/
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
    }
    else /*Port Initialized*/
#endif
    {
    /*
    Precompile option to check if error checking is on.
    If the the checking is on it keeps the det code.
    */
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
        if(Pin < 0  || Pin > 43)
        {
            /*Report DET error*/
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
        }
        else
    #endif
        {
            if(Pin >= 16 && Pin <= 19) //JTAG Pins
            {
                //Do Nothing
            }
            else
            {
                if(Pin > 19) //converting from range 0 .. 43 to 0 .. 39 
                {
                    Pin = Pin - 4; 
                }
                else
                {
                    //Do Nothing
                }
                if(Port_Channels[Pin].pin_direction_changeable == STD_ON ) /*Checking if the pin direction is changable before changing it*/
                {

                    volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */


                    switch(Port_Channels[Pin].port_num) /*Checking the current port number and saving its address to Port_Ptr*/
                    {
                        case  PORTA: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                                 break;

                        case  PORTB: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                                 break;

                        case  PORTC: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                                 break;

                        case  PORTD: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                                 break;
              
                        case  PORTE: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                                 break;

                        case  PORTF: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                                 break;
                        default:        break; /*default case to obey MISRA rules*/
                    }

                    /*use the unlock and commit register incase of PD7 & PF0*/
                    if( ((Port_Channels[Pin].port_num == PORTD) && (Port_Channels[Pin].pin_num == PIN7)) || ((Port_Channels[Pin].port_num == PORTF) && (Port_Channels[Pin].pin_num == PIN0)) ) /* PD7 or PF0 */
                    {
                    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Channels[Pin].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
                    }

                    /*switch case to set the direction of the pin (input or output)*/
                    switch(Direction)
                    {
                    case PORT_PIN_IN: CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[Pin].pin_num); /*clear the corresponding bit in the direction reg in case of input*/
                                    break;

                    case PORT_PIN_OUT: SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[Pin].pin_num);/*set the corresponding bit in the direction reg in case of output*/
                                    break;
                    default:        break; /*default case to obey MISRA rules*/
                    }
                }
            /*
            Precompile option to check if error checking is on.
            If the the checking is on it keeps the det code.
            */
            #if (PORT_DEV_ERROR_DETECT == STD_ON)
                else if(Port_Channels[Pin].pin_direction_changeable == STD_OFF) //Pin Direction Not Changable
                {
                    /*Report DET error*/
                    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
                }
            #endif

            }

        }

    }
}

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
void Port_RefreshPortDirection(void)
{
/*
Precompile option to check if error checking is on.
If the the checking is on it keeps the det code.
*/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if(Port_Status  == PORT_NOT_INITIALIZED)
    {
        /*Report DET error*/
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
    }
    else /*Port Initialized*/
#endif
    {
        volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */

        int i;
        for(i=0 ; i<PINS_NUMBER_EXCEPT_JTAG ; i++)
        {
            if(Port_Channels[i].pin_direction_changeable == STD_OFF) /*Refreshes pins with pin_direction_changeable set to OFF only*/
            {
                switch(Port_Channels[i].port_num)
                {
                    case  PORTA: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                             break;

                    case  PORTB: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                             break;

                    case  PORTC: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                             break;

                    case  PORTD: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                             break;
          
                    case  PORTE: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                             break;

                    case  PORTF: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                             break;
                    default:        break; /*default case to obey MISRA rules*/
                }


                if( ((Port_Channels[i].port_num == PORTD) && (Port_Channels[i].pin_num == PIN7)) || ((Port_Channels[i].port_num == PORTF) && (Port_Channels[i].pin_num == PIN0)) ) /* PD7 or PF0 */
                {
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Channels[i].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
                }

                /*switch case to set the direction of the pin (input or output)*/
                switch(Port_Channels[i].direction)
                {
                case PORT_PIN_IN: CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[i].pin_num);/*clear the corresponding bit in the direction reg in case of input*/
                                break;

                case PORT_PIN_OUT: SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Port_Channels[i].pin_num);/*set the corresponding bit in the direction reg in case of output*/
                                break;
                default:        break; /*default case to obey MISRA rules*/
                }
            }
            else /*Skip pins with pin_direction_changeable set to ON */
            {
                //Do Nothing
            }

        }

    }
}


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
#ifdef PORT_VERSION_INFO_API
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
/*
Precompile option to check if error checking is on.
If the the checking is on it keeps the det code.
*/
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if(Port_Status  == PORT_NOT_INITIALIZED)
    {
        /*Report DET error*/
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
    }
    else /*Port Initialized*/
#endif
    {
    /*
    Precompile option to check if error checking is on.
    If the the checking is on it keeps the det code.
    */
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
        /* Check if input pointer is not Null pointer */
        if(NULL_PTR == versioninfo)
        {
            /* Report to DET  */
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        }
        else
    #endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
        {
            /* Copy the vendor Id */
            versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
            /* Copy the module Id */
            versioninfo->moduleID = (uint16)PORT_MODULE_ID;
            /* Copy Software Major Version */
            versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
            /* Copy Software Minor Version */
            versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
            /* Copy Software Patch Version */
            versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
        }
    }
}
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
    Port_PinModeType Mode)
{
    /*
    Precompile option to check if error checking is on.
    If the the checking is on it keeps the det code.
    */
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
    if(Port_Status  == PORT_NOT_INITIALIZED)
    {
        /*Report DET error*/
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID , PORT_E_UNINIT);
    }
    else /*Port Initialized*/
#endif
    {
    /*
    Precompile option to check if error checking is on.
    If the the checking is on it keeps the det code.
    */
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
        if(Pin < 0  || Pin > 43)
        {
            /*Report DET error*/
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID , PORT_E_PARAM_PIN);
        }
        else
    #endif
        {
        /*
        Precompile option to check if error checking is on.
        If the the checking is on it keeps the det code.
        */
        #if (PORT_DEV_ERROR_DETECT == STD_ON)
            if(Mode > 14 || Mode < 0 || Mode == 11 || Mode == 12 || Mode == 13)
            {
                /*Report DET error*/
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID , PORT_E_PARAM_INVALID_MODE);
            }
            else
        #endif 
            {
                if(Pin >= 16 && Pin <= 19) //JTAG Pins
                {
                    //Do Nothing
                }
                else
                {
                    if(Pin > 19) //converting from range 0 .. 43 to 0 .. 39 
                    {
                        Pin = Pin - 4; 
                    }
                    else
                    {
                        //Do Nothing
                    }
                    if(Port_Channels[Pin].pin_mode_changeable == STD_ON ) /*Checking if the pin mode is changable before changing it*/
                    {

                        volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */

                        switch(Port_Channels[Pin].port_num) /*Checking the current port number and saving its address to Port_Ptr*/
                        {
                            case  PORTA: Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                                     break;

                            case  PORTB: Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                                     break;

                            case  PORTC: Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                                     break;

                            case  PORTD: Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                                     break;
                  
                            case  PORTE: Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                                     break;

                            case  PORTF: Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                                     break;
                            default:        break; /*default case to obey MISRA rules*/
                        }

                        if( ((Port_Channels[Pin].port_num == PORTD) && (Port_Channels[Pin].pin_num == PIN7)) || ((Port_Channels[Pin].port_num == PORTF) && (Port_Channels[Pin].pin_num == PIN0)) ) /* PD7 or PF0 */
                        {
                        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Channels[Pin].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
                        }

                        volatile uint32 * PCTRreg_Ptr = NULL_PTR; /*Pointer to hold the Current Control Register Address*/

                        /*Saving the Current Control Register Address in the PCTR_Ptr*/
                        PCTRreg_Ptr = (volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET );

                        if((Port_Channels[Pin].pin_mode) == DIO ) /*DIO mode*/
                        {
                            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*clearing the corresponding bit in the analog register*/

                            *PCTRreg_Ptr = *PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[Pin].pin_num)*4))); //Clear Control bits

                            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*clearing the corresponding bit in the alternate register*/
                            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*setting the corresponding bit in the digital register*/
                            
                        }
                        else if((Port_Channels[Pin].pin_mode) == ADC) /*ADC mode*/
                        {
                            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*clearing the corresponding bit in the digital register*/

                            *PCTRreg_Ptr = *PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[Pin].pin_num)*4)));//Clear Control bits

                            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*clearing the corresponding bit in the alternate register*/
                            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*setting the corresponding bit in the analog register*/

                        }
                        else /*another mode*/
                        {
                            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*clearing the corresponding bit in the analog register*/
                            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*setting the corresponding bit in the digital register*/
                            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET ) , Port_Channels[Pin].pin_num);/*setting the corresponding bit in the alternate register*/
                            /*Masking and setting the mode of the current pin in the right bits in the control register*/
                            *PCTRreg_Ptr = ((*PCTRreg_Ptr&(~(BYTE_MASK<<((Port_Channels[Pin].pin_num)*4))))|(((Port_Channels[Pin].pin_mode)&(BYTE_MASK))<<((Port_Channels[Pin].pin_num)*4)));

                        }
                    }
                /*
                Precompile option to check if error checking is on.
                If the the checking is on it keeps the det code.
                */
                #if (PORT_DEV_ERROR_DETECT == STD_ON)
                    else /*Pin mode not changeable*/
                    {
                        /*Report DET error*/
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID , PORT_E_MODE_UNCHANGEABLE);
                    }
               #endif
                }
            }
        }
    }
}
/*
************************************************************************************
* Service Name: Port_SetupGpioPin
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin configuration:
*              - Setup the pin as Digital GPIO pin
*              - Setup the direction of the GPIO pin
*              - Provide initial value for o/p pin
*              - Setup the internal resistor for i/p pin
************************************************************************************
void Port_SetupGpioPin(const Port_ConfigType * ConfigPtr )
{
    volatile uint32 * PortGpio_Ptr = NULL_PTR; * point to the required Port Registers base address *
    volatile uint32 delay = 0; *my register 4 byt*
    
    switch(ConfigPtr->port_num)*acces register on of port*
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; * PORTA Base Address *
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; * PORTB Base Address *
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; * PORTC Base Address *
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; * PORTD Base Address *
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; * PORTE Base Address *
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; * PORTF Base Address *
		 break;
    }
    
    * Enable clock for PORT and allow time for clock to start*
    SYSCTL_REGCGC2_REG |= (1<<ConfigPtr->port_num);
    delay = SYSCTL_REGCGC2_REG;
    *                                   port pd7                  ||            port pf 5 unlock commit reg*
    if( ((ConfigPtr->port_num == 3) && (ConfigPtr->pin_num == 7)) || ((ConfigPtr->port_num == 5) && (ConfigPtr->pin_num == 0)) ) * PD7 or PF0 *
    {
          *casting(offset 4byte)to (offset 1byte)*
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     * Unlock the GPIOCR register committ*   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ConfigPtr->pin_num);  * Set the corresponding bit in GPIOCR register to allow changes on this pin *
    }
    else if( (ConfigPtr->port_num == 2) && (ConfigPtr->pin_num <= 3) ) * PC0 to PC3 *
    {
        * Do Nothing ...  this is the JTAG pins *
        return;
    }
    else
    {
        * Do Nothing ... No need to unlock the commit register for this pin *
    }
    
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , ConfigPtr->pin_num);      * Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin *
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , ConfigPtr->pin_num);             * Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register *
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (ConfigPtr->pin_num * 4));     * Clear the PMCx bits for this pin *
    
    if(ConfigPtr->direction == OUTPUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);                * Set the corresponding bit in the GPIODIR register to configure it as output pin *
        
        if(ConfigPtr->initial_value == STD_HIGH)*out*
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , ConfigPtr->pin_num);          * Set the corresponding bit in the GPIODATA register to provide initial value 1 *
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , ConfigPtr->pin_num);        * Clear the corresponding bit in the GPIODATA register to provide initial value 0 *
        }
    }
    else if(ConfigPtr->direction == INPUT)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);             * Clear the corresponding bit in the GPIODIR register to configure it as input pin *
        
        if(ConfigPtr->resistor == PULL_UP)*set pullup 1 automaticaly turn off puul down*
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , ConfigPtr->pin_num);       * Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin *
        }
        else if(ConfigPtr->resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , ConfigPtr->pin_num);    * Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin *
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , ConfigPtr->pin_num);     * Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin *
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , ConfigPtr->pin_num);   * Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin *
        }
    }
    else
    {
        * Do Nothing *
    }
    
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , ConfigPtr->pin_num);        * Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin *
}*/