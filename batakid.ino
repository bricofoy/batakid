/*************************************************************************************
 *                                                                                   *
 *     Jeu type "batak" minimaliste pour enfant sur base arduino                     *
 *                                                                                   *
 *************************************************************************************/

#include "yasm.h" // http://forum.arduino.cc/index.php?topic=525092

/****définitions relatives aux leds et aux boutons************************************/
#define ON true
#define OFF false
#define BTN_NBR 10
const int8_t LedPin[BTN_NBR]={13,12,11,10,9,8,7,6,5,4};
const int8_t BtnPin[BTN_NBR]={};
bool LedState[BTN_NBR]={0};

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

uint8_t BoutonActuel=0;

void lireBoutons()
{
	BoutonActuel=0;
	for (uint8_t i=0; i<BTN_NBR; i++) {
		if (digitalRead(BtnPin[i])==LOW) {
			BoutonActuel = i+1;
			return;
		}
	}
}

/****** fonction de chenillard ******************************************************/
YASM ChenSM;

unsigned long ChenDelai;

int8_t ChenNbr;

void chenillard(unsigned long aDelai, int8_t aNombre=1, bool aAttendreFin=true)
{
	if(aDelai==0) {
		ChenSM.stop();
		return;
	}	
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

	if( position = (BTN_NBR-1+ChenNbr) ) {
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
	int8_t temp = (position-ChenNbr);
	if( (temp) > -1)
		digitalWrite(LedPin[temp], !digitalRead(LedPin[temp]));
}

/***** tache de fond*************************/
YASM Programme;

void Prg_init()
{
	if (Programme.isFirstRun()) 
		for (int8_t i=0, i=2, i++) 
			LedState[i]=ON;
	
	switch (BoutonActuel) {
		case 1:
			Programme.next(Prg_jeu1);
			toutesLeds(OFF);
			break;
		case 2:
			Programme.next(Prg_jeu2);
			toutesLeds(OFF);
			break;
		case 3:
			Programme.next(Prg_jeu3);
			toutesLeds(OFF);
			break;
	}
}

void Prg_jeu1()
{
	//allume/éteint chaque led
	if (BoutonActuel)
		LedState[BoutonActuel-1]=!LedState[BoutonActuel-1];
}

void Prg_jeu2()
{
	//chenillard avec plus ou moins de leds
	if (BoutonActuel)
		chenillard(50, BoutonActuel, false);
}

void Prg_jeu3()
{
	//allume/eteint chaque led dans l'ordre
}



void setup()
{
	for(int8_t i=0; i<BTN_NBR; i++) {
		pinMode(LedPin[i],OUTPUT);
		pinMode(BtnPin[i],INPUT_PULLUP);
	}
	
	chenillard(50, 2, true);
	toutesLeds(ON);
	chenillard(50, 4, true);
	toutesLeds(OFF);
	
	Programme.next(Prg_init);
}

void loop()
{
	lireBoutons();
	Programme.run();
	ChenSM.run();
	
	affiche();
}
