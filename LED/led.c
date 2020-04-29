/*---------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *-------------------------------------------------------------*/
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include <MKL25Z4.h>
#include "init.h"
#include <stdbool.h>

/*---------------------------------------------------------------
 * Initialize variables for flags, data, dir and priority level
 *-------------------------------------------------------------*/
uint8_t rx_data = 20;
volatile bool isMoving = false;
bool isStart = false;
volatile int ledCount = 0;

/*---------------------------------------------------------------
 * Helper Functions
 *-------------------------------------------------------------*/
void blink_twice(void) {
    for (int a = 0; a < 2; a++) {
        PTC->PDOR |= green[0] | green[1] | green[2] | green[3];
        PTC->PDOR |= green[4] | green[5] | green[6] | green[7];
        osDelay(100);
        PTC->PDOR = 0;
        osDelay(100);
    }
    osDelay(900);
}

void staticLED(void) {
    PTE->PDOR = MASK(RED);
    PTC->PDOR |= green[0] | green[1] | green[2] | green[3];
    PTC->PDOR |= green[4] | green[5] | green[6] | green[7];
    osDelay(250);
    PTE->PCOR = MASK(RED);
    osDelay(250);
}

void movingLED(void) {
    if (ledCount == 8) {
        ledCount = 0;
    }
    PTE->PDOR = MASK(RED);
    PTC->PDOR = green[ledCount];
    ledCount++; 
    osDelay(500);
    PTE->PCOR = MASK(RED);
    PTC->PDOR = green[ledCount];
    ledCount++;
    osDelay(500);
}

/*---------------------------------------------------------------
 * Application Threads
 *-------------------------------------------------------------*/
// GREEN stationary: 0ms, moving: 500ms
// RED stationary: 250ms, moving: 500ms
void tLED(void *argument) {
    for (;;) {
        if (isStart) {
            isStart = false;
            blink_twice();
        }
        if (!isMoving && rx_data == 0) {
            staticLED();
        } else if (isMoving) {
            movingLED();
        }
    }
}

/*---------------------------------------------------------------
 * Main function
 *-------------------------------------------------------------*/
int main() {
    SystemCoreClockUpdate();          // System Initialization
    initGPIO();                       // Initialize GPIO
        
    osKernelInitialize();             // Initialize CMSIS-RTOS
    osThreadNew(tLED, NULL, NULL);    //LED Thread
    osKernelStart();                  // Start thread execution
    for (;;) {}
}