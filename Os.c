 /******************************************************************************
 *
 * Module: Os
 *
 * File Name: Os.c
 *
 * Description: Source file for Os Scheduler.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/

#include "Os.h"
#include "App.h"
#include "Button.h"
#include "Led.h"
#include "Gpt.h"

/* Enable IRQ Interrupts ... This Macro enables IRQ interrupts by clearing the I-bit in the PRIMASK. */
#define Enable_Interrupts()    __asm("CPSIE I")

/* Disable IRQ Interrupts ... This Macro disables IRQ interrupts by setting the I-bit in the PRIMASK. */
#define Disable_Interrupts()   __asm("CPSID I")

/* Global variable store the Os Time (flag)*/
static uint8 g_Time_Tick_Count = 0;

/* Global variable to indicate the the timer has a new tick */
static uint8 g_New_Time_Tick_Flag = 0;

/*********************************************************************************************/
void Os_start(void)
{
    /* Global Interrupts Enable */
    Enable_Interrupts();
    
    /* 
     * Set the Call Back function to call Os_NewTimerTick
     * this function will be called every SysTick Interrupt (20ms)
     */
    SysTick_SetCallBack(Os_NewTimerTick);

    /* Start SysTickTimer to generate interrupt every OS_BASE_TIME= 20ms */
    SysTick_Start(OS_BASE_TIME);

    /* Execute the Init Task */
    Init_Task();

    /* Start the Os Scheduler  its fun. while(1) and u stay here*/
    Os_Scheduler();
    /* 
     timer count firest 20 msec u interupt this code & leve it go to tnafez ISR in ISR ba3mel call_back function 
      call_back function set flag by 1 indecation that call_back happend ther is 20 msec 3dat & increasing count b 20
so code sha8al lma flag=1 ,flag =0 :whil(1)& if conditon b false 

  1 timer tick(call_back)where flag=0
  2 cut cod sha8al ISR 
  3 in ISR cal call_back (tick+1 & increasing count by 20& flag=1 )
  4 @20 check buuton task rest flag0 
  5 @40 flag=1 &check buuton & refresh led &flag=0
  6 @60 flag=1 & activate button (its task state)& flag=0
  7 @120 crose section between 2 taskes clear timeing
      */
}

/*********************************************************************************************/
void Os_NewTimerTick(void)
{
    /* Increment the Os time by OS_BASE_TIME */
    g_Time_Tick_Count   += OS_BASE_TIME;

    /* Set the flag to 1 to indicate that there is a new timer tick */
    g_New_Time_Tick_Flag = 1;
}

/*********************************************************************************************/
void Os_Scheduler(void)
{
    while(1)
    {
	/* Code is only executed in case there is a new timer tick */
	if(g_New_Time_Tick_Flag == 1)
	{
	    switch(g_Time_Tick_Count)
            {
		case 20:
		case 100:
			    Button_Task();
			    g_New_Time_Tick_Flag = 0;
			    break;
		case 40:
		case 80:
			    Button_Task();
			    Led_Task();
			    g_New_Time_Tick_Flag = 0;
			    break;
		case 60:
			    Button_Task();
			    App_Task();
			    g_New_Time_Tick_Flag = 0;
			    break;
		case 120:
			    Button_Task();
			    App_Task();
			    Led_Task();
			    g_New_Time_Tick_Flag = 0;
			    g_Time_Tick_Count = 0;
			    break;
            }
	}
    }

}
/*********************************************************************************************/
