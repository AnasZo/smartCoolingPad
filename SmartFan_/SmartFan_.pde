  // Smart fan update (v1) : 2021 - 01 - 16 20h24 cela fonctionne avec le systeme complet        
import processing.serial.*;    // Importing the serial library to communicate with the Arduino 
import controlP5.*;

ControlP5 pb;

PFont font ;

Serial myPort;      // Initializing a vairable named 'myPort' for serial communication

String led_color = null;   // Variable for changing the background color

float _width1 = 0.0;
float _width2 = 0.0;
float _width3 = 0.0;

float _height1 = 0.0;
float _height2 = 0.0;
float _height3 = 0.0;

float circleSize = 30.0;

PFont fontB;           // Declare Font object

String str_g="g";
String str_red="r";
String str_y="y";
String colo=null;

// Declare & Set LED positions
int led1Xpos=310;   // Red LED 1 X position
int led1Ypos=150; // Red LED 1 Y position
int led2Xpos=210; // Red LED 2 X position
int led2Ypos=150; // Red LED 2 Y position
int led3Xpos=100; // Red LED 3 X position
int led3Ypos=150; // Red LED 3 Y position

String rpm;

void setup () {
background (255);   // Initial background color, when we will open the serial window 
font = createFont("Cambria Math", 18);

 pb = new ControlP5(this);
 
 
 pb.addButton("SS").setPosition(300, 320)
 .setSize(70, 70)
 .setFont(font);
 
 pb.addButton("AR").setPosition(400, 320)
 .setSize(70, 70)
 .setFont(font);

_width1= (width/2)+100;  
_width2= (width/2); 
_width3= (width/2)-100;  

_height1= (height/3);  
_height2= (height/3); 
_height3= (height/3);  

size (500,  500);     // Size of the serial window, you can increase or decrease as you want

myPort  =  new Serial (this, "COM6",  9600); // Set the com port and the baud rate according to the Arduino IDE


myPort.bufferUntil ('\n');   // Receiving the data from the Arduino IDE

fontB = createFont("Century Gothic Gras", 25); //Choose & Create font


delay(4000); // waiting etablished serial connection

} 

void draw ( ) {
background(255);
fill(0, 0, 0);  
textFont(fontB);
textSize(30);
textAlign(CENTER);

text("Smart Fan Dashboard", 20,20,width,height);

line(0, height-420, width, height-420);

line(0, height/2, width, height/2);

line(width-220, height/2, width-220, height-50);

line(0, height-50, width, height-50);


text("Air-Fan Speed ", width/2-100, height/2+60);

textSize(16);
text("rpm", width/2+60-120, height/2+150);

textSize(16);
fill(255, 0, 0); 
//LED-1 Title, rests on top of LED-1 for identification.
text("LED-Red", led1Xpos+40, led1Ypos-30);   

fill(255, 128, 0);
//LED-2 Title , rests on top of LED-2 for identification.
text("LED-Orange", led2Xpos+40, led2Ypos-30);   

fill(0, 128,0 );
//LED-3 Title , rests on top of LED-3 for identification.
text("LED-Green", led3Xpos+40, led3Ypos-30); 
  
if (led_color != null) {
    String[] list = split(led_color, ',');
    colo=list[0];
    rpm=list[1];
    
if(colo.equals(str_red) == true) {stopState(); } 
//yellow
  if(colo.equals(str_y) == true) { alarmState();}  
//green
  if(colo.equals(str_g) == true) {runState(); } 
  
  
textSize(40);
fill(0, 0,0);
text(rpm, width/2-100, height/2+150);

}


}


void serialEvent( Serial myPort) {

led_color = myPort.readStringUntil('\n');
if (led_color != null) {
  led_color = trim(led_color);
  println(led_color);

    //println(bpm);
  }
}

void SS(){

myPort.write('b');

}
void AR(){

myPort.write('c');

}
void stopState(){
drawEllipse(_width1,_height1,255,0,0);
drawEllipse(_width2,_height2,0,0,0); 
drawEllipse(_width3,_height3,0,0,0);

}
void alarmState(){
drawEllipse(_width1,_height1,255,0,0);
drawEllipse(_width2,_height2,255,128,0);
drawEllipse(_width3,_height3,0,0,0);

}
void runState(){
drawEllipse(_width1,_height1,0,0,0);
drawEllipse(_width2,_height2,0,0,0); 
drawEllipse(_width3,_height3,0,128,0);

}
void drawEllipse(float _width,float _height,int c1,int c2,int c3) {
  // 1. on remplit notre forme en bleu. (rouge, vert, bleu).
  fill(c1, c2, c3);
  // 2. on enlève le contour
 // noStroke();

  // 3. on dessine l'ellipse au millieu
  ellipse(_width, _height, circleSize, circleSize);
}
void drawSquare(float x,float y,float extent,int c1,int c2,int c3) {
  // 1. on remplit notre forme en bleu. (rouge, vert, bleu).
  fill(c1, c2, c3);
  // 2. on enlève le contour
 // noStroke();

  // 3. on dessine l'ellipse au millieu
  square(x, y, extent);
}
