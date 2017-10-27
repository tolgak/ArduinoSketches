#include <LiquidCrystal.h>
#include <Keypad.h>
#include "notes.h"

//const byte ledPin = 6;


const byte cntRow = 4;
const byte cntCol = 3;
char pad [cntRow][cntCol] = {
    {'1', '2', '3'}
  , {'4', '5', '6'}
  , {'7', '8', '9'}
  , {'*', '0', '#'}
};
byte rowPins[cntRow] = {10, 11, 12, 13};
byte colPins[cntCol] = {7, 8, 9};
Keypad myPad = Keypad(makeKeymap(pad), rowPins, colPins, cntRow, cntCol);
static byte kpadState;


const int rs = 1, en = 2, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



int ldrPin   = A0; // select the input pin for LDR
int ldrValue = 0;  // variable to store the value coming from the sensor

int potPin   = A1; // select the input pin for pot
int potValue = 0;  // variable to store the value coming from the sensor

//int buzzPin   = 5; // select the input pin for buzzer
//int melody[] = { _C4, _G3, _G3, _A3, _G3, _SILENCE, _B3, _C4};
//int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4 }; // durations: 4 = quarter, 8 = eighth, etc.:

void setup() {
//  Serial.begin(9600);

//  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin, LOW);

//  pinMode(buzzPin, OUTPUT);
//  tone(buzzPin, 2200, 100);

  lcd.begin(16, 2);  
  lcd.clear();
  myPad.addEventListener(keypadEvent);  // Add an event listener.
  
//  Serial.println("Ready.");
        
}


void loop() {
//  char button = myPad.getKey();
//  if (button) {
//    performFunction(button);
//  }
  lcd.print("Hello World!");
  delay(5000);
}

void performFunction(char code){   
  switch ( code ) {
    case '4':
//      Serial.println("No function on f4");

      break;
          
    case '3':
//      Serial.println("Buzzing");      
//      buzzout();
      break;
          
    case '2':
//      Serial.print("Light level : ");
      readLightLevel();
      break;

    case '1':
//      Serial.print("Pot value : ");
      readPotValue();
      break;
  }  // end switch-case
}

//void buzzout() {
//  // note duration = one second / duration
//  // quarter note = 1000 / 4, eighth note = 1000/8 ...
//  int melodySize (sizeof(melody) / sizeof(int *));
//  for (int noteIndex = 0; noteIndex < melodySize; noteIndex++) {
//    int noteDuration = 1000/noteDurations[noteIndex];
//    tone(buzzPin, melody[noteIndex], noteDuration);  
//    int pauseBetweenNotes = noteDuration * 1.10;
//    delay(pauseBetweenNotes);
//    noTone(buzzPin);
//  }
//}

void readLightLevel() {  
  ldrValue = analogRead(ldrPin); // read the value from the sensor
  int lightLevel = map(ldrValue, 0, 1023, 0, 100);
  
//  Serial.println(lightLevel);  
}

void readPotValue() {  
  potValue = analogRead(potPin);
  int potLevel = map(potValue, 0, 1023, 0, 100);
  
//  Serial.print("value ");
//  Serial.print(potValue);
//  Serial.print(" mapped to ");
//  Serial.println(potLevel);
}

void keypadEvent( KeypadEvent key ) {
  kpadState = myPad.getState( );
  swOnState( key );
} // end numbers keypad events

void swOnState( char key ) {
  switch ( kpadState ) {
    case PRESSED:
//      digitalWrite(ledPin, HIGH);
      break;

    case HOLD:
      break;

    case RELEASED:
//      digitalWrite(ledPin, LOW);
      break;
  }  // end switch-case
}// end switch on state function



