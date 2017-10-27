#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include "EthernetClientUtils.h"
#include "ftpClient.h"

//comment out next line to update SD from FTP
//#define REFRESH

#ifndef REFRESH
  EthernetServer web_server(85);
#endif

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  if(SD.begin(4) == 0)
    Serial.println(F("SD init fail"));          
  
  setupEthernet();

#ifndef REFRESH
  web_server.begin();
#endif
}


void loop() {
#ifdef REFRESH
  IPAddress ftp_server( 192,168,0,11 );
  byte inChar = Serial.read();
  if(inChar == 'f')
  {
    if(doFTP(ftp_server)) 
      Serial.println(F("FTP OK"));
    else 
      Serial.println(F("FTP FAIL"));
  }
  
  if(inChar == 'r')  
    readSD();  
#else  
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient http_client = web_server.available();
  if (http_client) {
    while (http_client.connected()) {
      handleRequest(http_client);
    }  
    delay(1);
    http_client.stop();
  }
#endif
 
}








