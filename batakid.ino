/*************************************************************************************
 *                                                                                   *
 *     Jeu type "batak" minimaliste pour enfant sur base arduino                     *
 *                                                                                   *
 *************************************************************************************/

#include "yasm.h"
#include "btn.h"

#define BTN_NBR 10
const byte ledPin[BTN_NBR]={13,12,11,10,9,8,7,6,5,4};
const byte btnPin[BTN_NBR]={};

bool ledState[BTN_NBR]={0};

void setup
{
	for (byte i=0; i<BTN_NBR; i++) {
		pinMode(ledPin[i],OUTPUT);
		pinMode(btnPin[i],INPUT_PULLUP);
	}
}
