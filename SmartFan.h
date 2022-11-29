// UPDATE 2021 - 01 -14 5H29
/*
  SmartFan.h - Library for smart controlling fan according to temperature detected.
  Created by Anas Zouadi, December 20, 2020.
  Released into the private domain
  copyright © 2020 all rights reserved
*/
#ifndef SmartFan_h
#define SmartFan_h

#include "Arduino.h"
#include <avr/io.h> //header to enable data flow control over pins. Defines pins, ports, etc.
#include <stdlib.h>
#include <stdio.h>
#include <avr/delay.h>
#include<avr/interrupt.h>

class Dht{
  public:
  	Dht(volatile uint8_t &PINx,int DHT11_PIN);
  	//Dht(int DHT11_PIN);
  	void DHT11_start (volatile uint8_t &PORTx,volatile uint8_t &DDRx);
//void DHT11_start ();
  	void check_response (void);
  	uint8_t Receive_data (void);
  private:
  	volatile uint8_t &_PINx;
	int _DHT11_PIN;
};

class Fan{
  public:
	void timer0_init(void);
	void port_init(volatile uint8_t &DDRx,int PORTxx);

};
class  ControlPanel{
  public:
	ControlPanel(bool STstate,bool alrmState,int ledPORTx0,int ledPORTx1,int ledPORTx2);
	void cp_setup(volatile uint8_t &DDRx,volatile uint8_t &PORTx,int BP_PORTx3,int BP_PORTx2);
	char stateCP(volatile uint8_t &PORTx);
	bool getSTstate();
	void setSTstate(bool STstate);
	bool getAlrmState();
	void setAlrmState(bool alrmState);		
  private:
	bool _STstate;
	bool _alrmState;
	int _ledPORTx0;
	int _ledPORTx1;
	int _ledPORTx2;
};
void BdeB_delay_ms(int temps);
#endif 
