void setup() {  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);  
}

void loop() {
  Serial.println("Stop!");  
  
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  
  delay(5000);
  
  Serial.println("Ready!");    
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);

  delay(1000);

  Serial.println("GO!");        
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  
  delay(5000);
  

  Serial.println("Slow down!");        
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  
  delay(1000);
  Serial.println("---That's it.---");    
  delay(1000);
}
