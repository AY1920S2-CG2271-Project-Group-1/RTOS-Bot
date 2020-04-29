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
bool isAudioStart = false;
volatile bool isEnd = false;
volatile int ledCount = 0;

// Priority level for tbrain
const osThreadAttr_t AN = {
    .priority = osPriorityAboveNormal
};

osSemaphoreId_t rxSem;
osEventFlagsId_t startFlag;

volatile unsigned int clk = 4800000/128;

/*---------------------------------------------------------------
 * Tune array for start and end of run
 *-------------------------------------------------------------*/
    int tune[] = {
        NOTE_GS5, NOTE_B5, NOTE_CS6, NOTE_B5,
        NOTE_GS5, NOTE_FS5, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_E5,
        NOTE_E6, NOTE_GS6,
        NOTE_CS5, NOTE_E5, NOTE_FS5, NOTE_GS5, NOTE_GS5, NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_FS5,
        NOTE_B4, NOTE_CS5, NOTE_E5, NOTE_CS5, NOTE_FS5
    };

    int tune_duration[] = {
        250, 250, 500, 750,
        250, 250, 250, 250, 250, 250,
        250, 500,
        250, 250, 250, 250, 250, 250, 250, 250, 250,
        125, 125, 250, 250, 750
    };
	
/*---------------------------------------------------------------
 * Song array to be played continuously throughout the run
 *-------------------------------------------------------------*/	
    int song[] = {
    NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_E5, 
        NOTE_FS5, NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_A4, NOTE_A4,
        
    NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_CS5, NOTE_D5,
        NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_D5, NOTE_A4, NOTE_FS5,
        
    NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_E5,
        NOTE_FS5, NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_A4, NOTE_A4,
        
    NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_B4, NOTE_C5, NOTE_A5, NOTE_E5,
        NOTE_FS5
    };

    int song_duration[] = {
        125, 125, 250, 250, 250, 125, 125, 250, 250, 250, 125, 125, 250, 250, 500, 125, 250, 250, 250,
       125, 125, 250, 250, 250, 125, 125, 250, 250, 250, 125, 125, 250, 250, 125, 125, 125, 125, 250, 250, 250,
    125, 125, 250, 250, 250, 125, 125, 250, 250, 250, 125, 125, 250, 250, 500, 125, 250, 250, 250,
       125, 125, 250, 250, 250, 250, 250, 250, 250, 250,500, 500, 500
    };

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

void playTune(void) {
    // Play the tune (Platinum Disco Nisemonogatari OP 3) at start and end of run
    for (int i = 0; i < 26; i++) {
        TPM1->MOD = (int)(clk/tune[i]);
        TPM1_C1V = (int)(0.5 * TPM1->MOD);
        osDelay(tune_duration[i]);
    }
    osDelay(1000);
    isAudioStart = false;
}

void playSong() {
    // Play the song (Pokemon Red/Blue/Yellow Route 1 Theme) continuously
    for (int i = 0; i < 72; i++) {
        TPM1->MOD = (int)(clk/song[i]);
        TPM1_C1V = (int)(0.5 * TPM1->MOD);
        osDelay(song_duration[i]);
    }
}

/*---------------------------------------------------------------
 * Application Threads
 *-------------------------------------------------------------*/
// GREEN stationary: 0ms, moving: 500ms
// RED stationary: 250ms, moving: 500ms
void tLED(void *argument) {
    for (;;) {
        osEventFlagsWait(startFlag, 0x00000001, osFlagsWaitAny, osWaitForever);
        if (isStart) {
            isStart = false;
            blink_twice();
            osEventFlagsSet(startFlag, 0x00000002);
            }
        if (!isMoving && rx_data == 0) {
                staticLED();
        } else if (isMoving) {
            movingLED();
        }
    }
}

void tAudio(void *argument) {
    for (;;) {
        osEventFlagsWait(startFlag, 0x00000002, osFlagsWaitAny, osWaitForever);
        if (isAudioStart || isEnd) {
            playTune();
            if (isEnd) {
                break;
            }
        }
        if (!isAudioStart && rx_data <= 9) {
            playSong();
        }
    }
}

void tBrain(void *argument) {
    for(;;) {
        osSemaphoreAcquire(rxSem, osWaitForever);
        if (rx_data == 10) {
            osEventFlagsSet(startFlag, 0x00000001);
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
            osEventFlagsSet(startFlag, 0x00000003);
        }
    }
}

void tMotor(void *argument) {
    for (;;) {
        osEventFlagsWait(startFlag, 0x00000003, osFlagsWaitAll, osWaitForever);
        move();
    }
}

/*---------------------------------------------------------------
 * UART IRQ handler
 *-------------------------------------------------------------*/
void UART2_IRQHandler() {
    NVIC_ClearPendingIRQ(UART2_IRQn);
    if (UART2->S1 & UART_S1_RDRF_MASK) {
        rx_data = UART2->D;
        osSemaphoreRelease(rxSem);
    }
}

/*---------------------------------------------------------------
 * Main function
 *-------------------------------------------------------------*/
int main() {
    SystemCoreClockUpdate();          // System Initialization
    initPWM();                        // Initialize PWM
    initAudio();                      // Initialize PWM for Audio
    initGPIO();                       // Initialize GPIO
    initUART2();                      // Initialize UART2
    __enable_irq();
    
    rxSem = osSemaphoreNew(1,0,NULL);
    startFlag = osEventFlagsNew(NULL);
    
    osKernelInitialize();             // Initialize CMSIS-RTOS
    osThreadNew(tBrain, NULL, NULL);  //Decoder Control Thread
    osThreadNew(tMotor, NULL, NULL);  //Motor Thread
    osThreadNew(tAudio, NULL, NULL);  //Audio Thread
    osThreadNew(tLED, NULL, NULL);    //LED Thread
    osKernelStart();                  // Start thread execution
    for (;;) {}
}