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

/*---------------------------------------------------------------
 * Helper Functions
 *-------------------------------------------------------------*/
void move(void) {
        if (rx_data == FORWARD) {
        TPM0_C3V = 0.99 * TPM0->MOD;
        TPM1_C0V = 0;
        TPM0_C0V = 0.99 * TPM0->MOD;
        TPM0_C2V = 0;
        TPM0_C5V = 0.99 * TPM0->MOD;
        TPM0_C1V = 0;
        TPM2_C0V = 0.99 * TPM2->MOD;
        TPM2_C1V = 0;
        osDelay(500);
    } else if (rx_data == REVERSE) {
          Move back
        TPM0_C3V = 0;
        TPM1_C0V = 0.99 * TPM1->MOD;
        TPM0_C0V = 0;
        TPM0_C2V = 0.99 * TPM0->MOD;
        TPM0_C5V = 0;
        TPM0_C1V = 0.99 * TPM0->MOD;
        TPM2_C0V = 0;
        TPM2_C1V = 0.99 * TPM2->MOD;
        osDelay(500);
    } else if (rx_data == LEFT) {
        // Rotate left
        TPM0_C3V = 0.99 * TPM0->MOD;
        TPM1_C0V = 0;
        TPM0_C0V = 0.99 * TPM0->MOD;
        TPM0_C2V = 0;
        TPM0_C5V = 0;
        TPM0_C1V = 0.99 * TPM0->MOD;
        TPM2_C0V = 0;
        TPM2_C1V = 0.99 * TPM2->MOD;
      osDelay(500);
    } else if (rx_data == RIGHT) {
        // Rotate right
        TPM0_C3V = 0;
        TPM1_C0V = 0.99 * TPM1->MOD;
        TPM0_C0V = 0;
        TPM0_C2V = 0.99 * TPM0->MOD;
        TPM0_C5V = 0.99 * TPM0->MOD;
        TPM0_C1V = 0;
        TPM2_C0V = 0.99 * TPM2->MOD;
        TPM2_C1V = 0;
        osDelay(500);
    } else if (rx_data == FORWARD_LEFT) {
        // Move front-left
        TPM0_C3V = 0.99 * TPM0->MOD;
        TPM1_C0V = 0;
        TPM0_C0V = 0.99 * TPM0->MOD;
        TPM0_C2V = 0;
        TPM0_C5V += 0.7 * TPM0->MOD;
        TPM0_C1V = 0;
        TPM2_C0V += 0.7 * TPM2->MOD;
        TPM2_C1V = 0;
      osDelay(500);
    } else if (rx_data == FORWARD_RIGHT) {
        // Move front-right
        TPM0_C3V += 0.7 * TPM0->MOD;
        TPM1_C0V = 0;
        TPM0_C0V += 0.7 * TPM0->MOD;
        TPM0_C2V = 0;
        TPM0_C5V = 0.99 * TPM0->MOD;
        TPM0_C1V = 0;
        TPM2_C0V = 0.99 * TPM2->MOD;
        TPM2_C1V = 0;
        osDelay(500);
    } else if (rx_data == REVERSE_LEFT) {
        // Move back-left
        TPM0_C3V = 0;
        TPM1_C0V = 0.9 * TPM1->MOD;
        TPM0_C0V = 0;
        TPM0_C2V = 0.9 * TPM0->MOD;
        TPM0_C5V = 0;
        TPM0_C1V = 0.7 * TPM0->MOD;
        TPM2_C0V = 0;
        TPM2_C1V = 0.7 * TPM2->MOD;
      osDelay(500);
    } else if (rx_data == REVERSE_RIGHT) {
        // Move back-right
        TPM0_C3V = 0;
        TPM1_C0V = 0.7 * TPM1->MOD;
        TPM0_C0V = 0;
        TPM0_C2V = 0.7 * TPM0->MOD;
        TPM0_C5V = 0;
        TPM0_C1V = 0.8 * TPM0->MOD;
        TPM2_C0V = 0;
        TPM2_C1V = 0.8 * TPM2->MOD;
        osDelay(500);
    } else {
        TPM0_C3V = 0;
        TPM1_C0V = 0;
        TPM0_C0V = 0;
        TPM0_C2V = 0;
        TPM0_C5V = 0;
        TPM0_C1V = 0;
        TPM2_C0V = 0;
        TPM2_C1V = 0;
    }
}

/*---------------------------------------------------------------
 * Application Thread
 *-------------------------------------------------------------*/
void tMotor(void *argument) {
    for (;;) {
        move();
    }
}

/*---------------------------------------------------------------
 * Main function
 *-------------------------------------------------------------*/
int main() {
    SystemCoreClockUpdate();          // System Initialization
    initPWM();                        // Initialize PWM
        
    osKernelInitialize();             // Initialize CMSIS-RTOS
    osThreadNew(tMotor, NULL, NULL);  //Motor Thread
    osKernelStart();                  // Start thread execution
    for (;;) {}
}