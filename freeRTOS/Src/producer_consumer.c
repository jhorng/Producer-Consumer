/*
 * producer_consumer.c
 *
 *  Created on: Aug 10, 2017
 *      Author: Jaan Horng
 */

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

extern osThreadId task01Handle;
extern osThreadId task02Handle;
extern osMessageQId itemQueueHandle;
extern osMutexId queueMutHandle;
extern osSemaphoreId slotSemHandle;
extern osSemaphoreId itemSemHandle;

osEvent product;

void produce(uint8_t value){
	xSemaphoreTake(slotSemHandle, portMAX_DELAY);

    osMutexWait(queueMutHandle, portMAX_DELAY);
    if(osThreadGetId() == task01Handle){
    	printf("task1: %d\n", value);
    }
    osMessagePut(itemQueueHandle, value, 1000);
	osMutexRelease(queueMutHandle);

	xSemaphoreGive(itemSemHandle);
}

void consume(){
	xSemaphoreTake(itemSemHandle, portMAX_DELAY);

	/*if(osThreadGetId() == task02Handle){
		printf("task2");
	}*/
	osMutexWait(queueMutHandle, portMAX_DELAY);
	product = osMessageGet(itemQueueHandle, 500);
	osMutexRelease(queueMutHandle);

	xSemaphoreGive(slotSemHandle);
}
