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

void changeLed(int8_t aNumero)
{
	digitalWrite(LedPin[aNumero-1], !digitalRead(LedPin[aNumero-1]));	
}

void changeLed(int8_t aNumero, bool aEtat)
{
	digitalWrite(LedPin[aNumero-1], aEtat);	
}

bool etatLed(int8_t aNumero)
{
	return digitalRead(LedPin[aNumero-1])==ON;
}

bool toutAllume()
{
	//test pour voir si toutes leds sont allumées :
	int8_t nombre = 0;
	for (int8_t i = 1; i<=BTN_NBR; i++)
		nombre += (etatLed(i)==ON);
	if ( nombre == BTN_NBR )
		return true;
	return false;
}

BTN Bouton[BTN_NBR];
int8_t BoutonActuel;

void lireBoutons()
{
	BoutonActuel = 0;
	
	for (uint8_t i = 0; i < BTN_NBR; i++) 
		Bouton[i].update(!digitalRead(BtnPin[i]));
	
	for (uint8_t i = 0; i < BTN_NBR; i++) {
		if (Bouton[i].state(BTN_CLICK)) {
			BoutonActuel = i+1;
			return;
		}
	}
}

/******** fonction de mise en veille *************************************************/
YASM Veille;

void veille()
{
	if (BoutonActuel)
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
		position = 1;
	
	if ( Chenillard.periodic(ChenDelai) ) {
		if ( position > (BTN_NBR + ChenNbr) ) {
			//si on est arrivé au bout de la chenille, on stoppe le chenillard
			Chenillard.stop();
			//et on remet le dernière led utilisée dans son état initial
			changeLed(1);
			//puis on quitte avant de rallumer un truc
			return;
		}
		
		//basculement de l'état de la led courante
		if ( position < BTN_NBR+1 )
			changeLed(position);
		else if ( position == BTN_NBR+1 )
			changeLed(1);
		
		//extinction de la dernière led de la chenille
		int8_t temp = (position - ChenNbr);
		if ( (temp) > 0)
			if ( temp < BTN_NBR+1 )
				changeLed(temp);
			else 
				if ( temp == BTN_NBR+1 )
				changeLed(1);
			
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
		changeLed(10);
		changeLed(1);
		changeLed(2);
		changeLed(3);
		changeLed(9);
	}
	
	switch (BoutonActuel) {
		case 1:
			Programme.next(prg_jeu2);
			break;
		case 2:
			Programme.next(prg_jeu3);
			break;
		case 3:
			Programme.next(prg_jeu4);
			break;
		case 4:
			//Programme.next(prg_jeu4);
			break;
		case 5:
			//Programme.next(prg_jeu5);
			break;
		case 6:
			//Programme.next(prg_jeu6);
			break;
		case 7:
			//Programme.next(prg_jeu7);
			break;
		case 8:
			//Programme.next(prg_jeu8);
			break;
		case 9:
			Programme.next(prg_jeu6);
			break;
		case 10:
			Programme.next(prg_jeu1);
			break;
	}
	

}


void prg_jeu1()
{	
	//allume/éteint chaque led
	
	if (Programme.isFirstRun()) 
		toutesLeds(OFF);
	
	if ( BoutonActuel ) 
		changeLed(BoutonActuel);
	
	if ( toutAllume() ) {
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
		for (uint8_t i=0; i<(random(9)+1); i++)
			changeLed( random(9)+1 ); //ici on fait random(9)+1 pour etre entre 1 et 10 au lieu de 0 et 9
		chenillard(50,6);
	}
	
	if ( BoutonActuel ) {
		chenillard(400, BoutonActuel, false);
		Programme.next(prg_jeu2, true); //remise à zéro des compteurs de temps
	}
	
	if ( Programme.elapsed(180E3))
		//si 3 minutes (180*10^3 ms) sont écoulées sans actions, on quitte le jeu
		Programme.next(prg_init);
	
	if ( toutAllume() ) {
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
		position=1;
	}
	
	if ( position > 10 ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
	
	if ( BoutonActuel == position ) {
		changeLed(position, ON);
		position++;
		return; //ici il faut quitter immédiatement sinon le test suivant annule l'action en cours !
	}
	
	if ( BoutonActuel && (BoutonActuel == (position-1)) ) {
		position--;
		changeLed(position, OFF);
	}
	

}

void prg_jeu4()
{
	//allumer les deux leds de l'intérieur pour autoriser celle de l'extérieur
	if (Programme.isFirstRun()) {
		chenillard(20,10);
		toutesLeds(OFF);
	}
	
	if (BoutonActuel ) {
		if ( (BoutonActuel & 1) ) { 
			//si le numero du bouton est impair (car en binaire le bit de poids faible est toujours 0 pour un nombre pair)
			if (BoutonActuel>1) {
				if ( etatLed(BoutonActuel-1) && etatLed(BoutonActuel+1) ) 
					changeLed(BoutonActuel); 
			}
			else
				if ( etatLed(BTN_NBR) && etatLed(BoutonActuel+1) ) 
					changeLed(BoutonActuel);
		}
		else 
			//ici le bouton est pair
			changeLed(BoutonActuel);
		
		//et on vérifie que aucune led impaire n'est allumée si les deux paires qui l'encadrent ne le sont pas (pour l'éteindre si on a éteint une deux deux autres)
		for (int8_t i=1; i<BTN_NBR+1; i+=2)
			if( !(etatLed(i==1?BTN_NBR:i-1) && etatLed(i+1)) )
				changeLed(i,OFF);
		
	}
	
	if ( toutAllume() ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
}

void prg_jeu5()
{
	//par couleurs
	Programme.next(prg_init);
}

void prg_jeu6()
{
	//addition : allumer 10, puis quand on allume un autre bouton, il faut ensuite allumer le complémentaire pour faire 10, sinon ça se re-éteinds
	
	static int8_t boutonPrecedent;
	
	if (Programme.isFirstRun()) {
		toutesLeds(OFF);
		//changeLed(10,ON);
		boutonPrecedent = 0;
	}
	
	if (BoutonActuel){
		if (!etatLed(BoutonActuel)) {
			if (boutonPrecedent+BoutonActuel==10)
				changeLed(BoutonActuel);
			else {
				changeLed(BoutonActuel);
				changeLed(boutonPrecedent);
			}
		}
		else
			changeLed(BoutonActuel);
		boutonPrecedent=BoutonActuel;
	}
	
	if ( toutAllume() ) {
		delay(400); //sinon c'est pas beau
		animationFin();
		Programme.next(prg_init);
	}
	
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
