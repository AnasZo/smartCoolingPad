  // Smart fan      
#include "SmartFan.h"
#define DHT11_pin PORTD5
#define PINx PIND
#define  ledRed PORTB0
#define  ledYellow PORTB1 
#define  ledGreen PORTB2

char data[5];
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
bool ss;
bool ar;

Fan fan;
Dht dht(PIND,DHT11_pin);
ControlPanel CP(0,0,ledRed,ledYellow,ledGreen);

int main(){
  
  Serial.begin(9600);
  //_delay_ms(2000); 
  
  fan.port_init(DDRD,PIND6);
  fan.timer0_init();
  CP.cp_setup(DDRB,PORTD,PORTD3,PORTD2);
  

while(1){
  
if(Serial.available()){
    
  char var=Serial.read(); 
   
  if(var=='b'){
    ss = CP.getSTstate();
    ss = !ss;
    CP.setSTstate(ss);
  }
  if(var=='c'){
    //PORTB ^= (1<<PORTB2); 
    ar = CP.getAlrmState();
    ar = !ar;
    CP.setAlrmState(ar);
    }
  }
  
  char ledCC=CP.stateCP(PORTB);

  Serial.print(ledCC);
    
  
        
    dht.DHT11_start(PORTD,DDRD);    // send start pulse 
    dht.check_response();   // receive response 
    
    I_RH=dht.Receive_data();  // store first eight bit in I_RH 
    D_RH=dht.Receive_data();  // store next eight bit in D_RH 
    I_Temp=dht.Receive_data();  // store next eight bit in I_Temp 
    D_Temp=dht.Receive_data();  // store next eight bit in D_Temp 
    CheckSum=dht.Receive_data();// store next eight bit in CheckSum 
  

//    itoa(I_Temp,data,10);
      if(ss && !ar){ 
      if(OCR0A == 0 ){OCR0A = 200;}     // this instruction  start working from 6.6 volts DC power 
      if(I_Temp<10) OCR0A = 50;
      else if(I_Temp>=10 && I_Temp<25) OCR0A = 70;
      else if(I_Temp>=25 && I_Temp<30) OCR0A = 100;
      else if(I_Temp>=30 && I_Temp<35) OCR0A = 150;
      else if(I_Temp>=35 && I_Temp<=40) OCR0A = 200;
      else  OCR0A = 255;
      }
      else OCR0A = 0;
//  Serial.print("Temperature = ");
//  Serial.print(data);
//  Serial.print(".");
//    itoa(D_Temp,data,10);
//  Serial.print(data);
//  Serial.print("C | ");
   
    double duty = double(OCR0A) / 255.0;
    double avv = (duty / 255.0) * 12.0;
    double speed_motor = (8010.0 *avv)/ 12.0; // 12volts 8bits (255) (avv: average voltage) 
    Serial.print(",");    
    Serial.println(int(speed_motor));
    BdeB_delay_ms(1200);
    
  }
 }
    ISR(INT1_vect){
    ss = CP.getSTstate();
    ss = !ss;
    CP.setSTstate(ss);
    }
    ISR(INT0_vect){
    ar = CP.getAlrmState();
    ar = !ar;
    CP.setAlrmState(ar);
    }
