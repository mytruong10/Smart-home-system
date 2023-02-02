#include <Servo.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "GeNkDDj74GhbnIEPKsz6QHvnmEGfw9GY"; /* Blynk auth token */
char ssid[] = "final_project"; /* WiFi credentials */
char pass[] = "6334d6fd"; /* WiFi password */

#define buzzerPin D0
#define flamePin D4 /* Input for Infrared Flame sensor */
#define pirPin D3 /* Input for PIR HC-S501 */
#define bedroomLight D6
#define fan D7
#define kitchenLight D2
#define livingroomLight D1 
DHT dht(D5, DHT11); /* Connect DHT11 to D5*/

Servo servo;
SimpleTimer timer;

int flameValue; /* Place to store read Infrared Flame value */
int pirValue; /* Place to store read PIR Value */


void setup()
{
digitalWrite(buzzerPin, LOW);
Serial.begin(115200);
delay(10);

Blynk.begin(auth, ssid, pass);
servo.attach(D8);
pinMode(bedroomLight, OUTPUT);     
digitalWrite(bedroomLight, LOW);
pinMode(kitchenLight, OUTPUT);     
digitalWrite(kitchenLight, LOW);
pinMode(livingroomLight, OUTPUT);     
digitalWrite(livingroomLight, LOW);
pinMode(fan, OUTPUT);     
digitalWrite(fan, LOW);

pinMode(buzzerPin, OUTPUT); 
pinMode(flamePin, INPUT);
pinMode(pirPin, INPUT);

dht.begin();
  timer.setInterval(3000, sendSensor); /* Setup a function to be called every three second */
}


void loop()
{
getFlameValue();
getPirValue();
Blynk.run();
timer.run(); // Initiates SimpleTimer
}

/***************************************************
* Auto door
**************************************************/

BLYNK_WRITE(V0){
  servo.write(param.asInt());
}
BLYNK_WRITE(V2)
{
  servo.write(90);
}
BLYNK_WRITE(V3)
{
  servo.write(0);
}

/***************************************************
* Get Infrared Flame data
**************************************************/
void getFlameValue(void)
{
flameValue = digitalRead(flamePin);
if (flameValue==0)
{
Serial.println(flameValue);
Serial.println("==> FIRE DETECTED");
Blynk.notify("FIRE DETECTED");
digitalWrite(buzzerPin, HIGH);
delay(3000);
digitalWrite(buzzerPin, LOW);
}
else{
  Serial.println("SAFE");
  }
delay(500);

}


/***************************************************
* Get PIR data
**************************************************/
void getPirValue(void)
{
pirValue = digitalRead(pirPin);
if (pirValue)
{
Serial.println(pirValue);
Serial.println("==> HUMAN DETECTED ");
Blynk.notify("HUMAN DETECTED");
}
else{
  Serial.println("NO DETECTION");
  }
delay(500);
}




/***************************************************
* Get DHT11 data
**************************************************/
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}
