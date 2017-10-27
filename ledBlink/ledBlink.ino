void setup() {  
  pinMode(13, OUTPUT);
  Serial.begin(9600);  
}

void loop() {
  Serial.println("Stop!");  
  
  digitalWrite(13, HIGH);
  
  delay(5000);
  


  Serial.println("GO!");        
  digitalWrite(13, LOW);

  
  delay(1000);
  

  Serial.println("---That's it.---");    
  delay(1000);
}
