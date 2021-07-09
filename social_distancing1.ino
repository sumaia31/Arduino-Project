#include <Adafruit_NeoPixel.h>
#include<LiquidCrystal.h>
#include <Servo.h>

int ledPin= 3;
int ledNo= 12;

Adafruit_NeoPixel strip= Adafruit_NeoPixel(ledNo,ledPin,NEO_RGB+NEO_KHZ800);

LiquidCrystal lcd(12, 11, 5, 4, 9, 7);

int buzzerPin= 2;
int echoPin= 6;
int trigPin= 5;
int minDistance = 100;
int maxDistance = 300;


long duration;
int distance;
int pos = 0;

int onOffTime;
int IRSense;
int autoOffTrigger=0;

void setup() 
{
  pinMode(10, OUTPUT); 
  pinMode(8, OUTPUT);
  lcd.begin(16, 2);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  
  strip.begin();
  for(int i = 0; i < ledNo; i++)
  {
   strip.setPixelColor(i,strip.Color(0,0,0));
  }
  strip.show();
  Serial.begin(9600);
}

void sanitize_hand() {
  int IRSense= analogRead(A0); 
  int onOffTime= analogRead(A1);
  
  int time=map(onOffTime,0,1023,0,10);
  Serial.println("IR: "+String(IRSense));
  Serial.println("pot: "+String(onOffTime)+ ": "+String(time));
 
  if(IRSense >78) 
  {
    digitalWrite(8,1);
  }
  else 
  {
    digitalWrite(8,0);
  }
  if(IRSense >78 && autoOffTrigger==0)
  {
  	digitalWrite(10,1);
  	delay(time*1000);
    delay(500);
  	digitalWrite(10,0);
    autoOffTrigger=1;
    //Serial.println("Water is given for hand wash!");
  }
  else if(IRSense <78)
  {
    autoOffTrigger=0;
  }
}



void socialDistance() {
  int distance = calcDistance();
  Serial.println(distance);
  int ledsToGlow = map(distance, minDistance, maxDistance, ledNo, 1);
  Serial.println(ledsToGlow);
  if(ledsToGlow == 12)
  {
    digitalWrite(buzzerPin, HIGH);
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
  }
  for(int i = 0; i < ledsToGlow; i++)
  {
    if(i < 4)
    {
      strip.setPixelColor(i,strip.Color(50,0,0));//green,red,blue
    }
    else if(i >= 4 && i < 8)
    {
      strip.setPixelColor(i,strip.Color(50,0,50));//green,red,blue
    }
    else if(i >= 8 && i < 12)
    {
      strip.setPixelColor(i,strip.Color(0,50,0));//green,red,blue
    }
  }
  for(int i = ledsToGlow; i < ledNo; i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,0));
  }
  strip.show();
  delay(50);
}


void LED() {
  lcd.setCursor(0,0);
  lcd.print ("Unsafe Distance ");
  delay (100);
  lcd.setCursor(0,1);
  lcd.print ("Be 3 Feet Apart!");
  delay (100);
  lcd.setCursor (0,0);
  lcd.print ("COVID must stop!");
  delay (500);
  lcd.setCursor (0,1);
  lcd.print ("Wash Your HAND!!");
  delay (500);
}




void loop() 
{
  LED();
  socialDistance();
  sanitize_hand();
}

int calcDistance()
{
  long distance, duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration/29/2;
  if(distance >= maxDistance)
  {
    distance = maxDistance;
  }
  if(distance <= minDistance)
  {
    distance = minDistance;
  }
  return distance;
}