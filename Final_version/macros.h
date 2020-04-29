#include "MKL25Z4.h"

/*----------------------------------------------------------------------------
 * Constants for all the PWM pins
 *---------------------------------------------------------------------------*/
#define PTD3_Pin 3 // right forward TPM0_CH3
#define PTA12_Pin 12 // TPM1_CH0
#define PTD0_Pin 0 // right backward TPM0_CH0
#define PTD2_Pin 2 // TPM0_CH2
#define PTD5_Pin 5 // left forward TPM0_CH5
#define PTD1_Pin 1 // TPM0_CH1
#define PTB2_Pin 2 // left backward TPM2_CH0
#define PTB3_Pin 3 // TPM2_CH1

#define PTB1_Pin 1 // Port B's TPM1_CH1 Audio

/*----------------------------------------------------------------------------
 * Constants for all the Motor pins
 *---------------------------------------------------------------------------*/
#define MASK(x) (1 << (x))
#define MOVEM(x) (x & 0x08)

#define FORWARD 0x01
#define REVERSE 0x02
#define LEFT 0x03
#define RIGHT 0x04
#define FORWARD_LEFT 0x05
#define FORWARD_RIGHT 0x06
#define REVERSE_LEFT 0x07
#define REVERSE_RIGHT 0x08

/*----------------------------------------------------------------------------
 * Constants and variables for all the LED pins
 *---------------------------------------------------------------------------*/
#define GC1 7 // PortC Pin 7
#define GC2 0 // PortC Pin 0
#define GC3 3 // PortC Pin 3
#define GC4 4 // PortC Pin 4
#define GC5 5 // PortC Pin 5
#define GC6 6 // PortC Pin 6
#define GC7 10 // PortC Pin 10
#define GC8 11 // PortC Pin 11

#define RED 21 // PortE Pin 21

int green[10] = {MASK(GC1), MASK(GC2), MASK(GC3), MASK(GC4), MASK(GC5),
                                            MASK(GC6), MASK(GC7), MASK(GC8)};

/*----------------------------------------------------------------------------
 * Configure UART2 function template
 *---------------------------------------------------------------------------*/
#define BAUD_RATE 9600
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

/*----------------------------------------------------------------------------
 * Constants for all the notes
 *---------------------------------------------------------------------------*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978