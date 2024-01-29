#include <Servo.h>
#include <Arduino.h>
 
 Servo myservo; 

 const int analogPin = A0;
 const int servoPin = 9;

 float calculate_dist_cm(float value){
   // Aufgabe 2: Bilden Sie die digitale Spannungsrepräsentation 
   //            auf einen Abstandswert ab
   // TODO

  float voltage = value * (5.0 / 1023.0); // Umrechnung in Volt
  float distance = 13.0 * pow(voltage, -1.0);    // Formel anpassen
  //float distance = 29.988 * pow(voltage, -1.173);

  return distance;
 }

 void make_scan(int start, int end, int step){
   // Aufgabe 0: Übernehmen Sie Zeile 41 - 50 and 52 - 59
   //            in eine Funktion
  
   float val_v, val_cm;

   if (start < end){
     Serial.print("rechts:");
   } else {
     Serial.print("links:");
   }
  
   int pos = start;
   while (pos != end) { // goes from start degrees to end degrees
     // in steps of 1 degree
     // Aufgabe 0: Prozedurale Umsetzung eines Schrittes
     myservo.write(pos);             
     delay(15);                       
     val_v = analogRead(analogPin);  
     val_cm = calculate_dist_cm(val_v);
     Serial.print(pos);
     Serial.print(",");
     Serial.print(val_cm);
     Serial.print(";");

     pos = end>start ? pos+step : pos-step; 
   }
   Serial.println("");

   // TODO
   // 
   // Aufgabe 1: Ersetzen Sie die Arduino AnalogRead()
   //            Methode
 }

 void setup() {
   pinMode(analogPin, INPUT);
   pinMode(servoPin, OUTPUT);

   Serial.begin(9600); 
   //Serial.println("Los geht's");
   delay(1000);

   myservo.attach(servoPin); 

   // Rechts
   make_scan(0, 180, 1);

   // Links
   make_scan(180, 0, 1);
 }

 void loop(){
 }