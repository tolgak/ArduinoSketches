int buttonState;
int previousButtonState = LOW;

int ledState = HIGH;
int buttonPin = 2;
const int ledPin = 13;

long previousDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
//  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != previousButtonState){
    previousDebounceTime = millis();
  }

//  if ((millis() - previousDebounceTime) > debounceDelay) {  
    if (reading != buttonState){
      buttonState = reading;
      if (buttonState == HIGH){
        ledState = !ledState;
      }
    }
//  }
  
  digitalWrite(ledPin, ledState);
  previousButtonState = reading;

}
