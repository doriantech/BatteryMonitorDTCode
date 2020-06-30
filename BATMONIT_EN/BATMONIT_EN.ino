 //Libraries
#include <LiquidCrystal.h>
//LCD Setup
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool lowbatterynoted = false;
bool chargingnoted = true;
bool lse = false;
bool lsed = false;
bool ccn = true;
bool fn = false;

//Get VCC Voltage function
double readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return double(result) / 1000.0D;
}

//ADC to voltage
double readAnalogVoltage(int analogpin) {
  return (double(analogRead(analogpin)) / 1023.0D) * readVcc();
}

//Main functions
void setup() {
  lcd.begin(16,2); //LCD Setup
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW); // To avoid junk noise
  lcd.clear();
    lcd.home(); //Initalize LCD
    lcd.print("    WELCOME!    ");
    lcd.setCursor(0,1);
    lcd.print("1S Li-Po Monitor"); //Print a message
  tone(2, 1000);
    delay(500);
    noTone(2);
     tone(2, 600);
    delay(250);
    noTone(2);
    tone(2, 200);
    delay(250);
    noTone(2);
    tone(2, 400);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // Play a sound
    delay(300);
    tone(2, 500);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // Play a sound
    delay(450);
    lcd.clear();
    lcd.home(); // Prepare for main operation
    analogRead(A1);
    analogRead(A2); // Prepare analog pins
}

void loop() {
  if(lse == false) {
    ccn = true;
  }
  double batteryvolt; // Battery voltage
  boolean charging = false; // Is charging now?
  if(readAnalogVoltage(A2) > 2.0D) {
    batteryvolt = (2*(readAnalogVoltage(A1)) * 2.0D) - 4.2D; // Estimate battery voltage
    charging = true;
  } else {
    batteryvolt = 2*(readAnalogVoltage(A1));
  }
  if(2*readAnalogVoltage(A1) < 3.3D && !lowbatterynoted && !charging && lse) {
    lcd.clear();
    lcd.home(); //Initalize LCD
    lcd.print("LOW BATTERY!");
    lcd.setCursor(0,1);
    lcd.print("Please charge."); //Print a message
    //Low Battery Tone
    tone(2, 125);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // To avoid junk noise
    delay(250);
    noTone(2);
    tone(2, 125);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // To avoid junk noise
    delay(500);
    lcd.clear(); //To avoid mess
    lowbatterynoted = true;
    if(ccn){
    fn = false;
    } else {
      fn = true;
    }
  } else if(2*readAnalogVoltage(A1) >= 3.3D) {
    lowbatterynoted = false;
    fn = true;
  } else {
    fn = false;
  }
  lcd.home();
  lcd.print("Voltage: " + String(batteryvolt,3) + "V   "); // Voltage
  lcd.setCursor(0,1);
  if(charging) {
    if(readAnalogVoltage(A1) >= 4.17D) {
      lcd.print("Fully charged."); // Fully charged.
    } else {
      lcd.print("Charging...   "); // Charging now!
    }
    if(!chargingnoted) {
      //Charging Battery Tone
    tone(2, 1000);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // To avoid junk noise
    chargingnoted = true;
    }
    lowbatterynoted = false;
  } else {
      lcd.print("              "); // Blanked
    if(chargingnoted == true && lsed == true && fn) {
      //Charger unplugged Tone
    tone(2, 500);
    delay(250);
    noTone(2);
    pinMode(2,OUTPUT);
    digitalWrite(2,LOW); // To avoid junk noise
    ccn = false;
    } else {
      if(lsed){
        ccn = true;
      }
    }
    chargingnoted = false;
  }
  delay(250); // To not mix numbers in 16x2 LCD.
  lsed = lse;
  lse = true;
  }
