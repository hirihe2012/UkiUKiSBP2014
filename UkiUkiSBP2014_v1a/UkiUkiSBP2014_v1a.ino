//This program is Test for Devide for Ballon burster on UkiUkiSBP2014
//The version is 1.0a
// by Mujirushi Seisakusyo IRIE Ltd. (2014July10)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 


/* Altitude of balloon bursting */
static const double AltiBurstBalloon =  26000.0 ; // unit is metars [m]
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
int val0,val1,val2;    // variable to read the value from the analog pin 

int led_out = 13; // LED connected to digital pin 13
int mode_sw = 4; // mode SW connected to digital pin 4

unsigned long prev_time = 0;
unsigned long chg_time = 500;

void Balloon_Burster(int times)
{
  int i;
  for(i=0;i<times;i++)
  {
    val2 = analogRead(potpin2);
    val0 = analogRead(potpin0);      
    val0 = map(val0, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    myservo.write(val0);   
    delay(val2);  Serial.print(F(" giko"));
    val1 = analogRead(potpin1);      
    val2 = map(val1, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    myservo.write(val1);   
    delay(val2);   Serial.print(F(" giko  ~"));
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

void LEDchikachika(int times)
{
   int i;
   
   i=times;
   while(i)
   if(millis() - prev_time > 100){
      prev_time = millis(); i--;
      if(digitalRead(led_out) == LOW)
        digitalWrite(led_out,HIGH);
      else
        digitalWrite(led_out,LOW);
    }
    
    digitalWrite(led_out,LOW);
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
  Serial.print(F("Testing UkiUkiSBP2014  v.1.0a  ")); 
  Serial.println(F("by Mujirushi Seisakusyo IRIE Ltd."));
  Serial.println();
  
  pinMode(led_out,OUTPUT);
  pinMode(mode_sw,INPUT);
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
 if(digitalRead(mode_sw) == _ON) //モードスイッチがオンならば、
     Balloon_Burster(5);  //最初の５回は試し動作を行う。
     
 StartTime = millis();
}

void loop() 
{
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
     Balloon_Burster(50);  // 50回動作させる
   }
  
  }

  while (ss.available() > 0)
    gps.encode(ss.read());




}

