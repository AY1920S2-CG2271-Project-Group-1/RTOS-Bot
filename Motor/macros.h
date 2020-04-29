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