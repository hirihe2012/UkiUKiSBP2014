//This program is Test for Devide for Ballon burster on UkiUkiSBP2014
//The version is 0.1b  
// by Mujirushi Seisakusyo IRIE Ltd. (2014July10)

static const int BurstTime_min = 10;   //5min
long BurstTmie_ms;  // = 300000;  burstTime_min x 60 x 1000 ms

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
int val0=170, val1=20,val2=1020;    // variable to read the value from the analog pin 

int led_out = 13; // LED connected to digital pin 13
int mode_sw = 4; // mode SW connected to digital pin 4

unsigned long prev_time = 0;
unsigned long chg_time = 200;

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
  
void sounds(){
 for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(12, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(12);
  }
  
}


void Balloon_Burster(int times)
{
  int i;
   myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 
  sounds();
  
//  tone(12,440,300);
  for(i=0;i<times;i++)
  {
    myservo.write(val1);   
    delay(val2);
    myservo.write(val0);   
    delay(val2);   
  }
   myservo.detach();
}
 
void LEDchika(void)
{
  static int i;
  int ftone;
  
   if(millis() - prev_time > chg_time){
      prev_time = millis();
      if(digitalRead(led_out) == LOW)
        digitalWrite(led_out,HIGH);
      else
        digitalWrite(led_out,LOW);
        i++;
        if(i>10){i=0; 
       ftone = (int)((BurstTmie_ms-(millis() - StartTime))/1000 );
         tone(12,880+ftone,100);delay(100); tone(12,880,100);}
    }
}

void Startsound(void)
{
  int i;
  
 for(i=0;i<BurstTime_min;i++)
 {
 tone(12,440,200);
 delay(300);
 }
 
}


void setup() 
{ 
  pinMode(led_out,OUTPUT);
  pinMode(mode_sw,INPUT);
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(val0);
  delay(2000);
  myservo.detach();
  BurstTmie_ms= (long)BurstTime_min * 60 *1000;
  
 //if(digitalRead(mode_sw) == _ON) //モードスイッチがオンならば、
 //    Balloon_Burster(5);  //最初の５回は試し動作を行う。
     
 StartTime = millis();
 Startsound();
 
 
} 
 
void loop() 
{ 
  while(1)
  {
    LEDchika();  // LED chika chika 
   if( millis() - StartTime > BurstTmie_ms)  break; // Time up for Bursting !
  }
  
  Balloon_Burster(50);  // 50回動作させる
  sounds();
  while(1);
}
