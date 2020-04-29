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
 * Initialize variables for flags, data
 *-------------------------------------------------------------*/
uint8_t rx_data = 20;
volatile bool isMoving = false;
bool isStart = false;
bool isAudioStart = false;
volatile bool isEnd = false;

/*---------------------------------------------------------------
 * Application Thread
 *-------------------------------------------------------------*/
void tBrain(void *argument) {
    for(;;) {
        if (rx_data == 10) {
            isStart = true;
            isAudioStart = true;
            rx_data = 0;
        } else if(rx_data == 9) {
            isEnd = true;
        } else {
            if (rx_data >= 1 && rx_data <= 8) {
                isMoving = true;
            } else {
                isMoving = false;
            }
        }
    }
}

/*---------------------------------------------------------------
 * UART IRQ handler
 *-------------------------------------------------------------*/
void UART2_IRQHandler() {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        rx_data = UART2->D;
    }
}

/*---------------------------------------------------------------
 * Main function
 *-------------------------------------------------------------*/
int main() {
    SystemCoreClockUpdate();          // System Initialization
    initUART2();                      // Initialize UART2
    
    osKernelInitialize();             // Initialize CMSIS-RTOS
    osThreadNew(tBrain, NULL, NULL);  //Decoder Control Thread
    osKernelStart();                  // Start thread execution
    for (;;) {}
}