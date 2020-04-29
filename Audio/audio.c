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
 * Initialize variables
 *-------------------------------------------------------------*/
uint8_t rx_data = 20;
bool isAudioStart = false;
volatile bool isEnd = false;

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
 * Application Thread
 *-------------------------------------------------------------*/
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

/*---------------------------------------------------------------
 * Main function
 *-------------------------------------------------------------*/
int main() {
    SystemCoreClockUpdate();          // System Initialization
    initAudio();                      // Initialize PWM for Audio
        
    osKernelInitialize();             // Initialize CMSIS-RTOS
    osThreadNew(tAudio, NULL, NULL);  //Audio Thread
    osKernelStart();                  // Start thread execution
    for (;;) {}
}