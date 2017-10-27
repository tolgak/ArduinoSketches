// Basic Arduino Web Server version 0.1 
// 27 May 2015
// References and further information at:
// http://startingelectronics.org/software/arduino/web-server/basic-01/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// maximum length of file name including path
#define FILE_NAME_LEN  20
// HTTP request type
#define HTTP_invalid   0
#define HTTP_GET       1
#define HTTP_POST      2

// file types
#define FT_HTML       0
#define FT_ICON       1
#define FT_CSS        2
#define FT_JAVASCRIPT 3
#define FT_JPG        4
#define FT_PNG        5
#define FT_GIF        6
#define FT_TEXT       7
#define FT_INVALID    8

// pin used for Ethernet chip SPI chip select
#define PIN_ETH_SPI   10

// the media access control (ethernet hardware) address for the shield:
const byte mac[] PROGMEM = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//the IP address for the shield:
const byte ip[] = { 192, 168, 0, 23 };  // does not work if this is put into Flash
// the router's gateway address:
const byte gateway[] PROGMEM = { 192, 168, 0, 1 };
// the subnet:
const byte subnet[] PROGMEM = { 255, 255, 255, 0 };

EthernetServer server(85);

void setup() {
  // deselect Ethernet chip on SPI bus
  pinMode(PIN_ETH_SPI, OUTPUT);
  digitalWrite(PIN_ETH_SPI, HIGH);
  
  Serial.begin(115200);       // for debugging
  
  if (!SD.begin(4)) {
    return;  // SD card initialization failed
  }
  Ethernet.begin((uint8_t*)mac, ip, gateway, subnet);
  server.begin();  // start listening for clients
}

void loop() {
  // if an incoming client connects, there will be bytes available to read:
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (ServiceClient(&client)) {
        // received request from client and finished responding
        break;
      }
    }  // while (client.connected())
    delay(1);
    client.stop();
  }  // if (client)
}// void loop()

bool ServiceClient(EthernetClient *client)
{
  static boolean currentLineIsBlank = true;
  char cl_char;
  File webFile;
  // file name from request including path + 1 of null terminator
  char file_name[FILE_NAME_LEN + 1] = {0};  // requested file name
  char http_req_type = 0;
  char req_file_type = FT_INVALID;
  const char *file_types[] = {"text/html", "image/x-icon", "text/css", "application/javascript", "image/jpeg", "image/png", "image/gif", "text/plain"};
  
  static char req_line_1[40] = {0};  // stores the first line of the HTTP request
  static unsigned char req_line_index = 0;
  static bool got_line_1 = false;

  if (client->available()) {   // client data available to read
    cl_char = client->read();
    
    if ((req_line_index < 39) && (got_line_1 == false)) {
      if ((cl_char != '\r') && (cl_char != '\n')) {
        req_line_1[req_line_index] = cl_char;
        req_line_index++;
      }
      else {
        got_line_1 = true;
        req_line_1[39] = 0;
      }
    }
    
    if ((cl_char == '\n') && currentLineIsBlank) {
      // get HTTP request type, file name and file extension type index
      http_req_type = GetRequestedHttpResource(req_line_1, file_name, &req_file_type);
      if (http_req_type == HTTP_GET) {         // HTTP GET request
        if (req_file_type < FT_INVALID) {      // valid file type
          webFile = SD.open(file_name);        // open requested file
          if (webFile) {
            // send a standard http response header
            client->println(F("HTTP/1.1 200 OK"));
            client->print(F("Content-Type: "));
            client->println(file_types[req_file_type]);
            client->println(F("Connection: close"));
            client->println();
            // send web page
            while(webFile.available()) {
              int num_bytes_read;
              char byte_buffer[64];
              // get bytes from requested file
              num_bytes_read = webFile.read(byte_buffer, 64);
              // send the file bytes to the client
              client->write(byte_buffer, num_bytes_read);
            }
            webFile.close();
          }
          else {
            // failed to open file
          }
        }
        else {
          // invalid file type
        }
      }
      else if (http_req_type == HTTP_POST) {
        // a POST HTTP request was received
      }
      else {
        // unsupported HTTP request received
      }
      req_line_1[0] = 0;
      req_line_index = 0;
      got_line_1 = false;
      // finished sending response and web page
      return 1;
    }
    if (cl_char == '\n') {
      currentLineIsBlank = true;
    }
    else if (cl_char != '\r') {
      currentLineIsBlank = false;
    }
  }  // if (client.available())
  return 0;
}

// extract file name from first line of HTTP request
char GetRequestedHttpResource(char *req_line, char *file_name, char *file_type)
{
  char request_type = HTTP_invalid;  // 1 = GET, 2 = POST. 0 = invalid
  char *str_token;
  
  *file_type = FT_INVALID;
  
  str_token =  strtok(req_line, " ");    // get the request type
  if (strcmp(str_token, "GET") == 0) {
    request_type = HTTP_GET;
    str_token =  strtok(NULL, " ");      // get the file name
    if (strcmp(str_token, "/") == 0) {
      strcpy(file_name, "index.htm");
      *file_type = FT_HTML;
    }
    else if (strlen(str_token) <= FILE_NAME_LEN) {
      // file name is within allowed length
      strcpy(file_name, str_token);
      // get the file extension
      str_token = strtok(str_token, ".");
      str_token = strtok(NULL, ".");
      
      if      (strcmp(str_token, "htm") == 0) {*file_type = 0;}
      else if (strcmp(str_token, "ico") == 0) {*file_type = 1;}
      else if (strcmp(str_token, "css") == 0) {*file_type = 2;}
      else if (strcmp(str_token, "js")  == 0) {*file_type = 3;}
      else if (strcmp(str_token, "jpg") == 0) {*file_type = 4;}
      else if (strcmp(str_token, "png") == 0) {*file_type = 5;}
      else if (strcmp(str_token, "gif") == 0) {*file_type = 6;}
      else if (strcmp(str_token, "txt") == 0) {*file_type = 7;}
      else {*file_type = 8;}
    }
    else {
      // file name too long
    }
  }
  else if (strcmp(str_token, "POST") == 0) {
    request_type = HTTP_POST;
  }

  return request_type;
}

