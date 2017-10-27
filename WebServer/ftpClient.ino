 /*
   FTP passive client for IDE v1.0.1 and w5100/w5200
   Posted October 2012 by SurferTim
   Modified 6 June 2015 by SurferTim
*/

#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetClient ftp_client;
EthernetClient ftp_dclient;

char outBuf[128];
char outCount;

File fh;
char fileName[13] = "index.htm";

byte doFTP(IPAddress ftp_server)
{
  SD.remove(fileName);
  fh = SD.open(fileName,FILE_WRITE);
  if(!fh)
  {
    Serial.println(F("SD open fail"));
    return 0;    
  }
  Serial.println(F("SD opened"));


  if (ftp_client.connect(ftp_server,21)) {
    Serial.println(F("Command connected"));
  } 
  else {
    fh.close();
    Serial.println(F("Command connection failed"));
    return 0;
  }
  if(!eRcv()) return 0;

  ftp_client.println(F("USER myuser"));
  if(!eRcv()) return 0;

  ftp_client.println(F("PASS Sakura3x"));
  if(!eRcv()) return 0;

  ftp_client.println(F("SYST"));
  if(!eRcv()) return 0;

  ftp_client.println(F("Type I"));
  if(!eRcv()) return 0;

  ftp_client.println(F("PASV"));
  if(!eRcv()) return 0;

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL)
    {
      Serial.println(F("Bad PASV Answer"));    
    }
  }

  unsigned int hiPort,loPort;

  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;

  Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  Serial.println(hiPort);

  if (ftp_dclient.connect(ftp_server,hiPort)) {
    Serial.println(F("Data connected"));
  } 
  else {
    Serial.println(F("Data connection failed"));
    ftp_client.stop();
    fh.close();
    return 0;
  }

  ftp_client.print(F("RETR "));
  ftp_client.println(fileName);
  if(!eRcv())
  {
    ftp_dclient.stop();
    return 0;
  }

  Serial.println(F("Writing to SD"));
  while(ftp_dclient.connected())
  {
    while(ftp_dclient.available())
    {
      char c = ftp_dclient.read();
      fh.write(c);      
      Serial.write(c); 
    }
  }
  ftp_dclient.stop();
  Serial.println(F("Data disconnected"));


  ftp_client.println(F("QUIT"));
  if(!eRcv()) return 0;

  ftp_client.stop();
  Serial.println(F("Command disconnected"));

  fh.close();
  Serial.println(F("SD closed"));
  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;

  while(!ftp_client.available()) delay(1);

  respCode = ftp_client.peek();

  outCount = 0;

  while(ftp_client.available())
  {  
    thisByte = ftp_client.read();    
    Serial.write(thisByte);

    if(outCount < 127)
    {
      outBuf[outCount] = thisByte;
      outCount++;      
      outBuf[outCount] = 0;
    }
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;

  ftp_client.println(F("QUIT"));

  while(!ftp_client.available()) delay(1);

  while(ftp_client.available())
  {  
    thisByte = ftp_client.read();    
    Serial.write(thisByte);
  }

  ftp_client.stop();
  Serial.println(F("Command disconnected"));
  fh.close();
  Serial.println(F("SD closed"));
}

void readSD()
{
  fh = SD.open(fileName,FILE_READ);

  if(!fh)
  {
    Serial.println(F("SD open fail"));
    return;    
  }

  while(fh.available())
  {
    Serial.write(fh.read());
  }

  fh.close();
}

