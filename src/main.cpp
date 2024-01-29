#include <Servo.h>
#include <Arduino.h>

Servo myservo;

const int analogPin = A0;
const int servoPin = 9;
const int samples = 10;
const int delayTime = 15;

float calculate_dist_cm(float voltage)
{
  // Aufgabe 2: Bilden Sie die digitale Spannungsrepräsentation
  //            auf einen Abstandswert ab

  // Formel zur Berechnung der Entfernung
  float distance = 13.0 * pow(voltage, -1.0); // Diese Formel muss möglicherweise kalibriert werden
  return distance;
}

void make_readout()
{
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)); // Auf Ergebnis warten...
}

float get_voltage(uint8_t channel)
{
  // initialize ADC
  // ADCSRA = ADC Control and Status Register A
  // ADEN = ADC Enable
  // ADPS2:0 = ADC Prescaler Select Bits
  // 128 = 16MHz / 128 = 125kHz
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Setze ADC-Kanal und externe Referenzspannung (5V)
  ADMUX |= channel | (1 << REFS0);
  

  // Dummy-Readout
  make_readout();

  // Richtige Messung
  make_readout();

  // Lesen des Registers "ADCW" (ADC Wert) (berücksichtigt die korrekte Reihenfolge des Lesens der beiden 8-Bit Register)
  uint16_t raw_value = ADCW;

  // Konvertiere ADC-Wert in Spannung
  float voltage = raw_value * (5.0 / 1024.0);

  // ADC deaktivieren
  ADCSRA = 0;

  return voltage;
}

float get_avg_voltage(uint8_t channel, uint8_t samples)
{
  float output = 0.0;

  // Nehme mehrere Proben und berechne durchschnittliche Spannung
  for (int i = 0; i < samples; i++)
  {
    output += get_voltage(channel);
  }

  return output / samples;
}



void make_scan(int start, int end, int step)
{
  // Aufgabe 0: Übernehmen Sie Zeile 41 - 50 and 52 - 59
  //            in eine Funktion

  float val_v, val_cm;
  int left = min(start, end);
  int right = max(start, end);
  int pos = start;

  if (start < end)
  {
    Serial.print("rechts:");
  } else
  {
    Serial.print("links:");;
  }
  
  do  // von start bis end
  {
    // in steps of 1 degree
    // Aufgabe 0: Prozedurale Umsetzung eines Schrittes
    myservo.write(pos);
    delay(delayTime);
    val_v = get_avg_voltage(analogPin, samples);
    val_cm = calculate_dist_cm(val_v);
    Serial.print(pos);
    Serial.print(",");
    Serial.print(val_cm);
    Serial.print(";");

    pos = (end > start) ? pos + step : pos - step;

  } while (pos >= left && pos <= right); // so lange pos zwischen start und end

  Serial.println("");

  //
  // Aufgabe 1: Ersetzen Sie die Arduino AnalogRead()
  //            Methode
}

void setup()
{
  pinMode(analogPin, INPUT);
  pinMode(servoPin, OUTPUT);

  Serial.begin(9600);
  // Serial.println("Los geht's");
  delay(1000);

  myservo.attach(servoPin);

  // Scanne von 0 bis 180 Grad (rechts)
  make_scan(0, 180, 1);

  // Scanne von 180 bis 0 Grad (links)
  make_scan(180, 0, 1);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
