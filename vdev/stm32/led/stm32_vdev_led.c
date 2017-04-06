#include "vdev.h"
#if VDEV_SUPPORT_LED == 1
#include "stm32f4xx.h"

vdev_status_t posix_vdev_led_init(
        _IN_ uint32_t id)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);   

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_on(
        _IN_ uint32_t id)
{
	if (id == 0) {
		GPIOD->BSRRL = GPIO_Pin_12;
	}
    else {
		GPIOD->BSRRL = GPIO_Pin_13;
    }

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_off(
        _IN_ uint32_t id)
{
	if (id == 0) {
		GPIOD->BSRRH = GPIO_Pin_12;
	}
    else {
		GPIOD->BSRRH = GPIO_Pin_13;
    }

    return VDEV_STATUS_SUCCESS;
}

vdev_status_t posix_vdev_led_toggle(
        _IN_ uint32_t id)
{
	if (id == 0) {
		GPIOD->ODR ^= GPIO_Pin_12;
	}
    else {
		GPIOD->ODR ^= GPIO_Pin_13;
    }

    return VDEV_STATUS_SUCCESS;
}

void vdev_led_api_install(vdev_led_api_t *api)
{
    api->init   = posix_vdev_led_init;
    api->on     = posix_vdev_led_on;
    api->off    = posix_vdev_led_off;
    api->toggle = posix_vdev_led_toggle;

}
#endif

