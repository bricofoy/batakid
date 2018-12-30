/*************************************************************************************
 *                                                                                   *
 *       Jeu type "batak" minimaliste pour enfant sur base arduino                   *
 *                                                                                   *
 *************************************************************************************/

#include "yasm.h" // http://forum.arduino.cc/index.php?topic=525092
#include "btn.h"
#include <Streaming.h> // https://github.com/janelia-arduino/Streaming



/****définitions relatives aux leds et aux boutons************************************/
#define ON HIGH
#define OFF LOW
#define BTN_NBR 10
const int8_t BtnPin[BTN_NBR] = {44, 42, 40, 38, 36, 34, 32, 30, 26, 28};
const int8_t LedPin[BTN_NBR] = {45, 43, 41, 39, 37, 35, 33, 31, 27, 29};

void toutesLeds(bool aEtat)
{
	for (int8_t i = 0; i < BTN_NBR; i++)
		digitalWrite(LedPin[i], aEtat);
}

void changeLed(int aNumero)
{
	digitalWrite(LedPin[aNumero], !digitalRead(LedPin[aNumero]));	
}

BTN Bouton[BTN_NBR];
int8_t BoutonActuel;

void lireBoutons()
{
	BoutonActuel = -1;
	
	for (uint8_t i = 0; i < BTN_NBR; i++) 
		Bouton[i].update(!digitalRead(BtnPin[i]));
	
	for (uint8_t i = 0; i < BTN_NBR; i++) {
		if (Bouton[i].state(BTN_CLICK)) {
			BoutonActuel = i;
			return;
		}
	}
}

/******** fonction de mise en veille *************************************************/
YASM Veille;

void veille()
{
	if (BoutonActuel != -1)
		Veille.next(veille,true); // si un bouton est appuyé on re-rentre dans l'état pour remettre à zéro le compteur
		
		if ( Veille.elapsed(600E3)) 
			//Si on est dans l'état depuis 10 minutes (600*10^3millisecondes) sans actions
			//sur les boutons, alors on met le circuit en veille : 
			faisDodo();
}

#include <avr/sleep.h> // https://playground.arduino.cc/Learning/arduinoSleepCode
void faisDodo()
{
	animationFin();
	animationFin();
	toutesLeds(OFF);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
	sleep_enable(); 
	sleep_mode(); // adieu mondre cruel !
}

/****** fonction de chenillard ******************************************************/
YASM Chenillard;
unsigned long ChenDelai;
int8_t ChenNbr;

void chenillard(unsigned long aDelai, int8_t aNombre = 1, bool aAttendreFin = true)
{
	if ( aDelai == 0 ) {
		Chenillard.stop();
		return;
	}
	ChenDelai = aDelai;
	ChenNbr = aNombre;
	Chenillard.next(chen);
	if ( aAttendreFin )
		while ( Chenillard.run() ) {}
}

void chen()
{
	static uint8_t position;
	
	if ( Chenillard.isFirstRun() )
		position = 0;
	
	if ( Chenillard.periodic(ChenDelai) ) {
		if ( position >= (BTN_NBR + ChenNbr) ) {
			//si on est arrivé au bout de la chenille, on stoppe le chenillard
			Chenillard.stop();
			//et on remet le dernière led utilisée dans son état initial
			changeLed(0);
			//puis on quitte avant de rallumer un truc
			return;
		}
		
		//basculement de l'état de la led courante
		if ( position < BTN_NBR )
			changeLed(position);
		else if ( position == BTN_NBR )
			changeLed(0);
		
		//extinction de la dernière led de la chenille
		int8_t temp = (position - ChenNbr);
		if ( (temp) > -1)
			if ( temp < BTN_NBR )
				changeLed(temp);
			else if ( temp == BTN_NBR )
				changeLed(0);
			
			position++;
	}
}

void animationFin()
{
	toutesLeds(OFF);
	delay(60);
	toutesLeds(ON);
	delay(60);
	toutesLeds(OFF);
	delay(60);
	toutesLeds(ON);
	delay(60);
	toutesLeds(OFF);
	delay(60);
	toutesLeds(ON);
	delay(60);
	toutesLeds(OFF);
	chenillard(30,2);
	chenillard(30,10);
}



/***** Programme principal ********************/
YASM Programme;

void prg_init()
{
	if (Programme.isFirstRun())
		toutesLeds(OFF);
	
	if (Programme.periodic(700)) {
		changeLed(9);
		changeLed(0);
		changeLed(1);
		changeLed(2);
	}
	
	switch (BoutonActuel) {
		case 9:
			Programme.next(prg_jeu1);
			break;
		case 0:
			Programme.next(prg_jeu2);
			break;
		case 1:
			Programme.next(prg_jeu3);
			break;
		case 2:
			Programme.next(prg_jeu4);
			break;
		/*case 3:
			//Programme.next(prg_jeu5);
			break;
		case 4:
			//Programme.next(prg_jeu6);
			break;
		case 5:
			//Programme.next(prg_jeu7);
			break;
		case 6:
			//Programme.next(prg_jeu8);
			break;
		case 7:
			//Programme.next(prg_jeu9);
			break;
		case 8:
			//Programme.next(prg_jeu10);
			break;*/
	}
	

}

void prg_jeu1()
{	
	//allume/éteint chaque led
	
	if (Programme.isFirstRun()) 
		toutesLeds(OFF);
	
	if ( BoutonActuel != -1 ) 
		changeLed(BoutonActuel);
	
	//test pour voir si toutes leds allumées :
	int8_t nombre = 0;
	for (int8_t i = 0; i<BTN_NBR; i++)
		nombre += (digitalRead(LedPin[i])==ON);
	if ( nombre == BTN_NBR ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
	

}

void prg_jeu2()
{
	//chenillard avec plus ou moins de leds
	
	if (Programme.isFirstRun()) {
		toutesLeds(OFF);
		for (uint8_t i=0; i<random(9); i++)
			changeLed(random(9));
		chenillard(50,6);
	}
	
	if ( BoutonActuel != -1 ) {
		chenillard(400, BoutonActuel+1, false);
		Programme.next(prg_jeu2, true); //remise à zéro des compteurs de temps
	}
	
	if ( Programme.elapsed(180E3))
		//si 3 minutes (180*10^3 ms) sont écoulées sans actions, on quitte le jeu
		Programme.next(prg_init);
	
	//test pour voir si toutes leds allumées :
	int8_t nombre = 0;
	for (int8_t i = 0; i<BTN_NBR; i++)
		nombre += (digitalRead(LedPin[i])==ON);
	if ( nombre == BTN_NBR ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
}

void prg_jeu3()
{	
	//allume/eteint chaque led dans l'ordre
	static uint8_t position;
	
	if (Programme.isFirstRun()) {
		toutesLeds(OFF);
		position=0;
	}
	
	if ( position == 10 ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
	
	if ( BoutonActuel == position ) {
		digitalWrite(LedPin[position], ON);
		position++;
		return; //ici il faut quitter immédiatement sinon le test suivant annule l'action en cours !
	}
	
	if ( (BoutonActuel != -1) && (BoutonActuel == (position-1)) ) {
		position--;
		digitalWrite(LedPin[position], OFF);
	}
	

}

void prg_jeu4()
{
	//intérieur pour autoriser extérieur
	if (Programme.isFirstRun()) {
		chenillard(20,10);
		toutesLeds(OFF);
	}
	
	if (BoutonActuel != -1) {
		Serial << "bouton ! "<< BoutonActuel << " ";
		if ( !(BoutonActuel & 1) ) { //si le numero du bouton est impair (car en binaire le bit de poids fort est toujours 0 pour un nombre pair)
			changeLed(BoutonActuel);
			Serial << "pair"<<endl;
			
		}
		
	}
}

void prg_jeu5()
{
	//par couleurs
}



void setup()
{
	for (int8_t i = 0; i < BTN_NBR; i++) {
		//paramétrage des broches utilisées
		pinMode(LedPin[i], OUTPUT);
		pinMode(BtnPin[i], INPUT_PULLUP);
	}
	randomSeed(analogRead(A0));
	
	Serial.begin(115200); 
	animationFin();
	Programme.next(prg_init);
	Veille.next(veille);
}

void loop()
{
	lireBoutons();
	Veille.run();
	Programme.run();
	Chenillard.run();
}
