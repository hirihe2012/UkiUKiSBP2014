//This program is Test for Devide for Ballon burster on UkiUkiSBP2014
//The version is 1.0a
// by Mujirushi Seisakusyo IRIE Ltd. (2014July10)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 


/* Altitude of balloon bursting */
static const double AltiBurstBalloon =  25000.0 ; // unit is metars [m]
double tempD;

/*
   This sample demonstrates TinyGPS++'s capacity for extracting custom
   fields from any NMEA sentence.  TinyGPS++ has built-in facilities for
   extracting latitude, longitude, altitude, etc., from the $GPGLL and 
   $GPRMC sentences.  But with the TinyGPSCustom type, you can extract
   other NMEA fields, even from non-standard NMEA sentences.

   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(RX) and 3(TX).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;


#define _ON 1
#define _OFF 0
unsigned long StartTime;
 
int potpin0 = 0;  // analog pin0 used for Cutter Position of pauseing to connect the potentiometer
int potpin1 = 1;  // analog pin1 used for Cutter Position of cutting to connect the potentiometer
int potpin2 = 2;  // analog pin2 used for timer to connect the potentiometer
//int val0,val1,val2;    // variable to read the value from the analog pin 
int val0=170;
int val1=20;
int val2=500;    // variable to read the value from the analog pin 

int led_out = 13; // LED connected to digital pin 13
int mode_sw = 4; // mode SW connected to digital pin 4

unsigned long prev_time = 0;
unsigned long chg_time = 500;

void Balloon_Burster(int times)
{
  int i;
   myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 
//  sounds();
  
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
/*
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
*/

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
        if(i>2){i=0; 
           tone(12,880,50);
        }
    }
}


void LEDchikachika(int times)
{
   int i;
   int ftone;
   
   i=times; 
   while(i)
   if(millis() - prev_time > 70){
    
     prev_time = millis(); i--;
      if(digitalRead(led_out) == LOW)
        digitalWrite(led_out,HIGH);
      else
        digitalWrite(led_out,LOW);
    }
    
    if(times<4) 
     tone(12,100,100);// tone(12,880,100);  
    else 
     tone(12,880,70);
 
     
    digitalWrite(led_out,LOW);
}

void Startsound(int timesound)
{
  int i;
  
 for(i=0;i<timesound;i++)
 {
 tone(12,440,200);
 delay(300);
 }
 
}


// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/*
   By declaring TinyGPSCustom objects like this, we announce that we
   are interested in the 15th, 16th, and 17th fields in the $GPGSA 
   sentence, respectively the PDOP (F("positional dilution of precision")),
   HDOP (F("horizontal...")), and VDOP (F("vertical...")).

   (Counting starts with the field immediately following the sentence name, 
   i.e. $GPGSA.  For more information on NMEA sentences, consult your
   GPS module's documentation and/or http://aprs.gids.nl/nmea/.)

   If your GPS module doesn't support the $GPGSA sentence, then you 
   won't get any output from this program.
*/

TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

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


void setup() 
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("UsingCustomFields.ino"));
  Serial.println(F("Demonstrating how to extract any NMEA field using TinyGPSCustom"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  Serial.println(F("UkiUkiSBP2014 using TinyGPSCustom"));
  Serial.print(F("Testing UkiUkiSBP2014  v.1.2b  ")); 
  Serial.println(F("by Mujirushi Seisakusyo IRIE Ltd."));
  Serial.println();
  
  pinMode(led_out,OUTPUT);
  pinMode(mode_sw,INPUT);
  
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
// if(digitalRead(mode_sw) == _ON) //モードスイッチがオンならば、
//     Balloon_Burster(5);  //最初の５回は試し動作を行う。
     
 StartTime = millis();
     sounds();
   myservo.attach(9);
  myservo.write(val0);  
   delay(1000);
   myservo.detach();
}

void loop() 
{
  
   // LEDchika();


  // Every time anything is updated, print everything.
  if (gps.altitude.isUpdated() || gps.satellites.isUpdated() ||
    pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated())
  {
    Serial.print(F("ALT="));   Serial.print(gps.altitude.meters()); 
    Serial.print(F(" PDOP=")); Serial.print(pdop.value()); 
    Serial.print(F(" HDOP=")); Serial.print(hdop.value()); 
    Serial.print(F(" VDOP=")); Serial.print(vdop.value());
    Serial.print(F(" SATS=")); Serial.print(gps.satellites.value());
    tempD =(double)gps.altitude.meters();
      LEDchikachika((int)gps.satellites.value()); 

   if( tempD < AltiBurstBalloon){
      Serial.println(F(" Not burned"));
   }
   else{
     Serial.println(F(" Just burn !"));
     sounds();
    Balloon_Burster(20);  // 50回動作させる
    
    myservo.write(val0); 
   delay(1000); 
    myservo.detach();
    sounds();
    
    }
  
  }


    
  while (ss.available() > 0)
    gps.encode(ss.read());




}

