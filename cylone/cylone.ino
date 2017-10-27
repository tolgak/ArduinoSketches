const int startPin = 4;
const int numLEDs = 10;
int currentLED = 0;
int direction = 1;

void setup() {
  
  for (int i = startPin; i < startPin + numLEDs; i++) {
    pinMode(i, OUTPUT);
  }

}

void loop() {
  digitalWrite(startPin + currentLED, HIGH);
  delay(200);
  digitalWrite(startPin + currentLED, LOW);
  currentLED += direction;
  if (currentLED < 1) {
    direction = 1;
  }
  if (currentLED >= numLEDs -1){
    direction = -1;
  }

}
