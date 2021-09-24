 /******************************************************************************
 *
 * Module: Gpt
 *
 * File Name: Gpt.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - SysTick Timer Driver.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/
/*gpt in AUTOSAR has 2 mode CONTINOUS : u make tick and will give u interupt as u make tick & ONE SHOTE : once give u interupt the timer stop*/
/*AUTOSAR malosh da3wa eta bt3d keef comper mod voerflow mode de 7aga HW almohm t3dly my time 
tayb lma y5alas 3ad 7ykol fun. ana 2oltelo 3aleha fe al configration 3 nam of function 
as code is  generated be ykon member of structure el pointer to fun. dh value of this pointer address fun. u give in configer
law name of ur fun. not mawgoda it is syntex error ana b write this fun.*/
#include "Gpt.h"        /*every .c include its .h*/
#include "tm4c123gh6pm_registers.h"     /*include file registers 3shan ast3mel systick registers
                                      weee can make systick_regisers bas 3shan driver mesh AUTOSAR 3amelt kolo fe file wa7ed*/

#define SYSTICK_PRIORITY_MASK  0x1FFFFFFF /*mask clear bits of priority 3shan ad5al priority bl OR*/
#define SYSTICK_INTERRUPT_PRIORITY  3           /*3atti pririty 3*/
#define SYSTICK_PRIORITY_BITS_POS   29          /*n7t priority last 3 bits 29 30 31 3shan take 3 and sheftleft 29*/

/* Global pointer to function used to point upper layer functions
 * to be used in Call Back
 *in begun its valeu set null
 *volatil because i used insidfunctions and ISR
 *static :i used in this file only */
static void (*g_SysTick_Call_Back_Ptr)(void) = NULL_PTR;

/************************************************************************************
* Service Name: SysTick_Handler
* Description: SysTick Timer ISR
************************************************************************************/
void SysTick_Handler(void)
{
    /*reason for condition
    when it is not null law zabatna al call back ana hato fe al2wal b null 
    ana 3amel condition dh 3shan maa y3mel exception al fadi*/
    /* Check if the Timer0_setCallBack is already called "3shan law nasy call_back 7tkon vleo b null code exception in ARM"*/
    if(g_SysTick_Call_Back_Ptr != NULL_PTR)     /*if ur pointer != null e3mel *g_SysTick_Call_Back_Ptrbeen() in this way i call back_fun.*/
    {           /*null i cant put pointr to garpeg &meaning its value not null so alcll_back etzabatet*/
        (*g_SysTick_Call_Back_Ptr)(); /* call the function in the scheduler using call-back concept */
    }
    /* No need to clear the trigger flag (COUNT) bit ... it cleared automatically by the tivac(in ARM) only this flag*/
}
/*how knw timer come
put break point in systick start
 and lock for all register if every thig like what i want
put break point at interupt walk step step 


*/
/************************************************************************************
* Service Name: SysTick_Start
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Tick_Time - Time in miliseconds
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer configuration to count n miliseconds:
*              - Set the Reload value
*              - Enable SysTick Timer with System clock 16Mhz
*              - Enable SysTick Timer Interrupt and set its priority
************************************************************************************/
void SysTick_Start(uint16 Tick_Time)
{
    SYSTICK_CTRL_REG    = 0;                         /* Disable the SysTick Timer by Clear the ENABLE Bit *//*close control register*/
    SYSTICK_RELOAD_REG  = 15999 * Tick_Time;         /* Set the Reload value to count n miliseconds *//*a3d 1 msec multi by n. of msec*/
    SYSTICK_CURRENT_REG = 0;                         /* Clear the Current Register value *//*began from zero*/
    /* Configure the SysTick Control Register 
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    SYSTICK_CTRL_REG   |= 0x07;         /*3shan ash8al al ragel dh ma3mel lh enable interupt bta3oh w asha8l b systemclock*/
    /* Assign priority level 3 to the SysTick Interrupt */
    /*                          priority register& with mask               |                   priority shiftleft by position*/
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);
}

/************************************************************************************
* Service Name: SysTick_Stop
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop the SysTick Timer.
************************************************************************************/
void SysTick_Stop(void)
{
    /*clear all control ana momken close pins enable and enough but if i want starte i configer this register as zero */
    SYSTICK_CTRL_REG = 0; /* Disable the SysTick Timer by Clear the ENABLE Bit */
}

/************************************************************************************
* Service Name: SysTick_SetCallBack             
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Ptr2Func - Call Back function address
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer call back
************************************************************************************/
void SysTick_SetCallBack(void(*Ptr2Func)(void)) /*when Os call this fun. systick send address fun. yaly hwa 7 ycall lma y5alas 3ad*/
{
    g_SysTick_Call_Back_Ptr = Ptr2Func; /*systick do not 3arf meen bysta5demoh some one send fun. lma a5als acallha(its address with me)
ycallha ezay when tmer finsh counting he sending interupt dh SysTick_Handler foo2*/
}
/*(i will needed in os : os will stsrt systick)timers for y3dloh time bta3oh so gpt in lower layer & 3ayz yklem al os yly fy upperlayer
 da 7a ytem 3an tareek callback fun.
Os kaman when start systick timer b ydeloh esm fun. 3andoh(wich is address firest place)
f ystaqbloh how fe pointer bs local pointer h yro7 
f al systick 3shan ygeep al value de 3shan y3raf yrd 3 Os
f 3amalt variable tany global*/