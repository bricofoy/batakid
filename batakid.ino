/*************************************************************************************
 *                                                                                   *
 *     Jeu type "batak" minimaliste pour enfant sur base arduino                     *
 *                                                                                   *
 *************************************************************************************/

#include "yasm.h"
#include "btn.h"

#define ON HIGH
#define OFF LOW
#define BTN_NBR 10
const byte LedPin[BTN_NBR]={13,12,11,10,9,8,7,6,5,4};
const byte BtnPin[BTN_NBR]={};

bool LedState[BTN_NBR]={0};

void setup
{
	for (byte i=0; i<BTN_NBR; i++) {
		pinMode(LedPin[i],OUTPUT);
		pinMode(BtnPin[i],INPUT_PULLUP);
	}
	
	chenillard(50);
	toutesLeds(ON);
	delay(50);
	toutesLeds(OFF);
}

void affiche()
{ 
	for (byte i=0; i<BTN_NBR; i++)
		digitalWrite(LedPin[i], LedState[i]);
}

void toutesLeds(bool aEtat)
{
	for (byte i=0; i<BTN_NBR; i++)
		LedState[i]=aEtat;
	affiche();
}

void loop
{
	
}
