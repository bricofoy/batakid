/*************************************************************************************
 *                                                                                   *
 *     Jeu type "batak" minimaliste pour enfant sur base arduino                     *
 *                                                                                   *
 *************************************************************************************/

#include "yasm.h"
#include "btn.h"

#define ON true
#define OFF false
#define BTN_NBR 10
const int8_t LedPin[BTN_NBR]={13,12,11,10,9,8,7,6,5,4};
const int8_t BtnPin[BTN_NBR]={};

bool LedState[BTN_NBR]={0};

YASM ChenSM;
unsigned long ChenDelai;
int8_t ChenNbr;
void chenillard(unsigned long aDelai, int8_t aNombre=1, bool aAttendreFin=true)
{
	ChenDelai = aDelai;
	ChenNbr = aNombre;
	ChenSM.next(chen);
	if( aAttendreFin )
		while( ChenSM.run() );
}

void chen()
{
	static int8_t position;
	
	if( ChenSM.isFirstRun() )
		position = 0;

	if( position = (BTN_NBR+ChenNbr-1) ) {
		//si on est arrivé au bout de la chenille, on stoppe le chenillard
		ChenSM.stop();
		//et on remet le dernière led utilisée dans son état initial
		digitalWrite(LedPin[BTN_NBR-1], !digitalRead(LedPin[BTN_NBR-1]));
		//puis on quitte avant de rallumer un truc
		return;
	}
	
	//basculement de l'état de la led courante
	if( position<BTN_NBR )
		digitalWrite(LedPin[position], !digitalRead(LedPin[position]));
	
	//extinction de la dernière led de la chenille
	int8_t condition = (position-ChenNbr);
	if( (condition) > -1)
		digitalWrite(LedPin[condition], !digitalRead(LedPin[condition]));
}


void setup
{
	for(int8_t i=0; i<BTN_NBR; i++) {
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
	for(int8_t i=0; i<BTN_NBR; i++)
		digitalWrite(LedPin[i], LedState[i]);
}

void toutesLeds(bool aEtat)
{
	for(int8_t i=0; i<BTN_NBR; i++)
		LedState[i]=aEtat;
	affiche();
}

void loop
{
	
}
