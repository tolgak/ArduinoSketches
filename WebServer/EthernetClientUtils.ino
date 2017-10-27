#include <SPI.h>
#include <Ethernet.h>
#include "EthernetClientUtils.h"

// see page at
// http://playground.arduino.cc/Code/WebServer   

void setupEthernet() {
  const byte mac[]     = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
  const byte ip[]      = { 192, 168, 0, 38 }; // default ip. does not work if this is put into Flash
  const byte myDns[]   = { 8, 8, 8, 8 };
  const byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
  const byte subnet[]  = { 255, 255, 255, 0 }; //subnet mask
  
  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin((uint8_t*)mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the Ethernet device not using DHCP:
    Ethernet.begin((uint8_t*)mac, ip, myDns, gateway, subnet);
  }  
  Serial.print("I am at ");
  Serial.println(Ethernet.localIP());
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void printHTTPHeader(EthernetClient client)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("User-Agent: Arduino");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
//  client.println("Refresh: 5");         // refresh the page automatically every 5 sec
  client.println();  
}


void handleRequest(EthernetClient client) 
{     
  Serial.println("new client");
  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  File webFile;
  
  while (client.connected() && client.available()) {
    char c = client.read();
    Serial.write(c);
    
    // if you've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, so you can send a reply
    if (c == '\n' && currentLineIsBlank) {
      printHTTPHeader(client);

      
      // send web page
      webFile = SD.open("index.htm"); // open web page file
      if (webFile) {
        while(webFile.available()) {
          client.write(webFile.read()); // send web page to client
        }
        webFile.close();
      }
      break;

      
    }
    
    if (c == '\n') {
      // you're starting a new line
      currentLineIsBlank = true;
    } else if (c != '\r') {
      // you've gotten a character on the current line
      currentLineIsBlank = false;
    }
  }
  
  // give the web browser time to receive the data
  delay(1);
  
  // close the connection:
  client.stop();
  
  Serial.println("client disconnected");
}
