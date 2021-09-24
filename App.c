 /******************************************************************************
 *
 * Module: Application
 *
 * File Name: App.c
 *
 * Description: Source file for Application Tasks.
 *
 * Author: Ammar Khazaa
 ******************************************************************************/

#include "App.h"
#include "Button.h"
#include "Led.h"
#include "Dio.h"
#include "port.h"

/* Description: Task executes once to initialize all the Modules */
void Init_Task(void)
{
    /* Initialize PORT Driver */
    /* bazzabat direction & mode before read &write*/
    Port_Init(&Port_Configuration);
    /* Initialize Dio Driver */
    Dio_Init(&Dio_Configuration);

    /* Initialize LED Driver 
    LED_init();
	
    * Initialize BUTTON Driver *
    BUTTON_init();*/
}

/* Description: Task executes every 20 Mili-seconds to check the button state */
/*Os schdual fun. every 20 msec*/
void Button_Task(void)
{
    BUTTON_refreshState();
}

/* Description: Task executes every 40 Mili-seconds to refresh the LED */
void Led_Task(void)
{
    LED_refreshOutput();        /*every 40 msec*/
}

/* Description: Task executes every 60 Mili-seconds to get the button status and toggle the led */
void App_Task(void)
{
    static uint8 button_previous_state = BUTTON_RELEASED;
    static uint8 button_current_state  = BUTTON_RELEASED;
	
    button_current_state = BUTTON_getState(); /* Read the button state */
	
    /* Only Toggle the led in case the current state of the switch is pressed
     * and the previous state is released */ 
    if((button_current_state == BUTTON_PRESSED) && (button_previous_state == BUTTON_RELEASED))   /*pushed*/
    {
	LED_toggle();
    }
	
    button_previous_state = button_current_state;
}
