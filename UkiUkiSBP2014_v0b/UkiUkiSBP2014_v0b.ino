//This program is Test for Devide for Ballon burster on UkiUkiSBP2014
//The version is 0.1b  
// by Mujirushi Seisakusyo IRIE Ltd. (2014July10)

static const long BurstTmie_ms = 300000;  //10min x 60 x 1000 = 600000

//Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 


#define _ON 1
#define _OFF 0

unsigned long StartTime;
 
int potpin0 = 0;  // analog pin0 used for Cutter Position of pauseing to connect the potentiometer
int potpin1 = 1;  // analog pin1 used for Cutter Position of cutting to connect the potentiometer
int potpin2 = 2;  // analog pin2 used for timer to connect the potentiometer
int val0,val1,val2;    // variable to read the value from the analog pin 

int led_out = 13; // LED connected to digital pin 13
int mode_sw = 4; // mode SW connected to digital pin 4

unsigned long prev_time = 0;
unsigned long chg_time = 200;

void Balloon_Burster(int times)
{
  int i;
  for(i=0;i<times;i++)
  {
    val2 = analogRead(potpin2);
    val0 = analogRead(potpin0);      
    val0 = map(val0, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    myservo.write(val0);   
    delay(val2);
    val1 = analogRead(potpin1);      
    val2 = map(val1, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    myservo.write(val1);   
    delay(val2);   
  }
  
}
 
void LEDchika(void)
{
   if(millis() - prev_time > chg_time){
      prev_time = millis();
      if(digitalRead(led_out) == LOW)
        digitalWrite(led_out,HIGH);
      else
        digitalWrite(led_out,LOW);
    }
}

void setup() 
{ 
  pinMode(led_out,OUTPUT);
  pinMode(mode_sw,INPUT);
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
 if(digitalRead(mode_sw) == _ON) //モードスイッチがオンならば、
     Balloon_Burster(5);  //最初の５回は試し動作を行う。
     
 StartTime = millis();
} 
 
void loop() 
{ 
  while(1)
  {
    LEDchika();  // LED chika chika 
   if( millis() - StartTime > BurstTmie_ms)  break; // Time up for Bursting !
  }
  
  Balloon_Burster(50);  // 50回動作させる
  
}
