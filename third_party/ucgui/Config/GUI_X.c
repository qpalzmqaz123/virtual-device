/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-II interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include "GUI_Private.h"
#include "vdev.h"

#if VDEV_SIMULATION_TYPE == 1
#include <unistd.h>
#endif

/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/

static vdev_os_api_t *pOsApi = NULL;
static vdev_os_mutex_t Mutex;
static vdev_os_event_t Event;

/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int  GUI_X_GetTime (void) 
{
    return (int)pOsApi->get_time();
}


void  GUI_X_Delay (int period) 
{
    pOsApi->msleep(period);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
void GUI_X_ExecIdle (void) 
{
    pOsApi->msleep(1);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void  GUI_X_InitOS (void)
{ 
    pOsApi = (vdev_os_api_t *)vdev_api_get("os");

    pOsApi->mutex_create(&Mutex);
    pOsApi->event_create(&Event);
}


void  GUI_X_Lock (void)
{ 
    pOsApi->mutex_lock(Mutex);
#if VDEV_SIMULATION_TYPE == 1
    usleep(1); /* UCGUI bug ? */
#endif
}


void  GUI_X_Unlock (void)
{ 
    pOsApi->mutex_unlock(Mutex);
}


U32  GUI_X_GetTaskId (void) 
{ 
    return (U32)pOsApi->get_task_id();
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/


void GUI_X_WaitEvent (void) 
{
    pOsApi->event_wait(&Event);
}


void GUI_X_SignalEvent (void) 
{
    pOsApi->event_set(&Event);
}

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/


void GUI_X_Init (void) 
{
}

