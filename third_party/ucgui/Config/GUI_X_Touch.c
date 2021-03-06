/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"
#include "vdev.h"


vdev_lcd_api_t *pLcdApi = NULL;


void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}

int  GUI_TOUCH_X_MeasureX(void) {
    vdev_status_t ret;
    uint16_t x, y;

    if (NULL == pLcdApi) {
        pLcdApi = (vdev_lcd_api_t *)vdev_api_get(VDEV_API_LCD);
    }

    ret = pLcdApi->touch_get_xy(0, &x, &y);
    if (ret) {
        return 0xFFFF; 
    }
    else {
        return x; 
    }
}

int  GUI_TOUCH_X_MeasureY(void) {
    vdev_status_t ret;
    uint16_t x, y;

    if (NULL == pLcdApi) {
        pLcdApi = (vdev_lcd_api_t *)vdev_api_get(VDEV_API_LCD);
    }

    ret = pLcdApi->touch_get_xy(0, &x, &y);
    if (ret) {
        return 0xFFFF; 
    }
    else {
        return y; 
    }
}


