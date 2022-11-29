// UPDATE 2021 - 01 -14 5H29
/*
  SmartFan.cpp - Library for smart controlling fan according to temperature detected.
  Created by Anas Zouadi, December 20, 2020.
  Released into the private domain
  copyright © 2020 all rights reserved
*/
#include "SmartFan.h"

Dht::Dht(volatile uint8_t &PINx,int DHT11_PIN){
	_PINx=PINx;
	_DHT11_PIN=DHT11_PIN;
}
void Dht::DHT11_start (volatile uint8_t &PORTx,volatile uint8_t &DDRx)
{
 DDRx |= (1<<_DHT11_PIN);  // set the pin as output

  PORTx |= (1<<_DHT11_PIN); //high
  _delay_ms(100);
 //send request
 PORTx &= ~(1<<_DHT11_PIN);   // pull the pin low
 _delay_ms(18);   // wait for 18ms

     PORTx |= (1<<_DHT11_PIN);    // pull the pin high
    DDRx &= ~(1<<_DHT11_PIN);   // set as input
    _delay_us (40);   // wait for 40us    
}
void Dht::check_response (void)
{
  //check start condition 1
  if((PIND & (1<<_DHT11_PIN))) {
    return -1;
  }
  _delay_us(80);
  //check start condition 2
  if(!(PIND & (1<<_DHT11_PIN))) {
    return -1;
  }
  _delay_us(80);
}
uint8_t Dht::Receive_data (void)
{
 uint8_t i=0,j;
 for (j=0;j<8;j++)
 {
  while ((PIND & (1<<_DHT11_PIN)) == 0);   // wait for the pin to go high
  _delay_us (30);   // wait for 40 us
  if ((PIND & (1<<_DHT11_PIN)) == 0)   // if the pin is low 
  {
   i&= ~(1<<(7-j));   // write 0
  }
  else i|= (1<<(7-j));  // if the pin is high, write 1
  while (PIND & (1<<_DHT11_PIN));  // wait for the pin to go low
 }
 return i;
}

// Timer0, PWM  Initialization
void Fan::timer0_init()
{ // fast pwm mode (MODE 3, SEE TAB.14-8 PAGE 86)
  // set up timer OC0A,OC0B pin in toggle mode and CTC mode ()
  TCCR0A |= (1 << COM0A1)|(1 << WGM00)|(1 << WGM01);
  // set up timer with prescaler = 256 (TCCR0B = 0b00000100, CS02=1, CS01=0, CS00 = 0)
  TCCR0B |= (1 << CS02);
  // initialize counter
  TCNT0 = 0;
  // initialize compare value (digital pin 6 (pwm) on Arduino borad, PIND6 ON ATEMEGA32)
  OCR0A = 0;
}

// Ports  Initialization


void Fan::port_init(volatile uint8_t &DDRx,int PORTxx)
{
  DDRx |=(1<<PORTxx);  // PD6-ENA (OC0A)
  //PORTD=0b00000000; // Set all pins of PORTC low 

}
ControlPanel::ControlPanel(bool STstate,bool alrmState,int ledPORTx0,int ledPORTx1,int ledPORTx2)
{
_STstate=STstate;
_alrmState=alrmState;
_ledPORTx0=ledPORTx0;
_ledPORTx1=ledPORTx1;
_ledPORTx2=ledPORTx2;
}
void ControlPanel::cp_setup(volatile uint8_t &DDRx,volatile uint8_t &PORTx,int BP_PORTx3,int BP_PORTx2){
	// _ledPORTx2 = green, _ledPORTx0 =red, _ledPORTx1 =yellow
  // LED PB2 green
  DDRx |= (1<<_ledPORTx2);
  //PORTB |=(1<<PORTB2);

  // LED PB1 yellow
  DDRx |= (1<<_ledPORTx1);
  //PORTB |=(1<<PORTB1);

   // LED PB0 red
  DDRx |= (1<<_ledPORTx0);
  //PORTB |=(1<<PORTB0);

  // pull-up push-button pin2, pin3
  PORTx |= (1<<BP_PORTx3);
  PORTx |= (1<<BP_PORTx2);
  
  // interrupt setting 
  //EICRA &= ((0<<ISC11)|(0<<ISC10)); // sense control, the low level ok=f INT1 generates an interrupt request.
  EIMSK |= ((1<<INT1)|(1<<INT0));
  sei(); // enabled global interrept 	
}
char ControlPanel::stateCP(volatile uint8_t &PORTx ){
  // PORTB2 = green, PORTB1 =yellow , PORTB0 =red
  // _ledPORTx2 = green, _ledPORTx1 =yellow, _ledPORTx0 =red
  char ledCC='r'; // ledCodeColor
  if(!_alrmState)
  if(!_STstate){ PORTx |= (1<<_ledPORTx0); PORTx &= ~(1<<_ledPORTx2); PORTx &= ~(1<<_ledPORTx1);ledCC='r';}
  else { PORTx |= (1<<_ledPORTx2); PORTx &= ~(1<<_ledPORTx0); PORTx &= ~(1<<_ledPORTx1);ledCC='g';}
  else { PORTx ^= (1<<_ledPORTx1);  PORTx |= (1<<_ledPORTx0);PORTx &= ~(1<<_ledPORTx2);ledCC='y';}
  //_delay_ms(100);
  return (ledCC);
}

bool ControlPanel::getSTstate(){
	return _STstate;
}
void ControlPanel::setSTstate(bool STstate){
	_STstate=STstate;
}
bool ControlPanel::getAlrmState(){
	return _alrmState;
}
void ControlPanel::setAlrmState(bool alrmState){
	_alrmState=alrmState;
}
void BdeB_delay_ms(int temps)
{   
  TCCR0B = 0b00000101; // Clock Select Mode
  int cnt=0;
  int nbr=temps/12.8;
  while(cnt<=nbr)
  {
    if(TCNT0>=200)
    {
      cnt++;
      TCNT0=0;
    }
  }
  cnt=0;
}
