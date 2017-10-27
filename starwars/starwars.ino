 #define NOTE_C4 262
 #define NOTE_D4 294
 #define NOTE_E4 330
 #define NOTE_F4 349
 #define NOTE_G4 392
 #define NOTE_A4 440
 #define NOTE_B4 494
 #define NOTE_C5 523

const int kPinSpeaker = 5;

void setup()
 {
 pinMode(kPinSpeaker, OUTPUT);
 }

void loop()
{
 tone(kPinSpeaker, NOTE_C4);
 delay(1000);
 tone(kPinSpeaker, NOTE_G4);
 delay(1000);
 tone(kPinSpeaker, NOTE_F4);
 delay(250);
 tone(kPinSpeaker, NOTE_E4);
 delay(250);
 tone(kPinSpeaker, NOTE_D4);
 delay(250);
 tone(kPinSpeaker, NOTE_C5);
 delay(1000);
 tone(kPinSpeaker, NOTE_G4);
 delay(500);
 tone(kPinSpeaker, NOTE_F4);
 delay(250);
 tone(kPinSpeaker, NOTE_E4);
 delay(250);
 tone(kPinSpeaker, NOTE_D4);
 delay(250);
 tone(kPinSpeaker, NOTE_C5);
 delay(1000);
 tone(kPinSpeaker, NOTE_G4);
 delay(500);
 tone(kPinSpeaker, NOTE_F4);
 delay(250);
 tone(kPinSpeaker, NOTE_E4);
 delay(250);
 tone(kPinSpeaker, NOTE_F4);
 delay(250);
 tone(kPinSpeaker, NOTE_D4);
 delay(2000);
  
 noTone(kPinSpeaker);

 delay(250);
}
