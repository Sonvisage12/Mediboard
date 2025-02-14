#include <Ticker.h>
#include <PxMatrix.h> //https://github.com/2dom/PxMatrix
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
Ticker display_ticker;
#include <ESP8266WiFi.h>
#include "log.h"

// API details
const char* host = "api.mediboards.io";
const String path = "/api/public/hospitals/bc1999eb-8d4a-4a03-b056-766f87f4a81c/latest-patient";
String count;
const char* ssid = "Infinix";
const char* password = "thankGod";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

//Pin Definition for Nodemcu to HUB75 LED MODULE
#define P_LAT 16 //nodemcu pin D0
#define P_A 5    //nodemcu pin D1
#define P_B 4    //nodemcu pin D2
#define P_C 15   //nodemcu pin D8
#define P_OE 2   //nodemcu pin D4
#define P_D 12   //nodemcu pin D6
#define P_E 0    //nodemcu pin GND // no connection

// PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(128, 16, P_LAT, P_OE, P_A, P_B, P_C, P_D); // Updated for 4 P4 displays in series

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);

uint16 myCOLORS[1] = {myRED};

// ISR for display refresh
void display_updater()
{
  display.display(70);
}

void CheckApi(){
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient https;
   
    // Skip certificate validation
    client.setInsecure();
   
    // Configure timeout
    https.setTimeout(1000);
   
    // Add required headers based on server response
    https.addHeader("Accept", "text/html,application/xhtml+xml,text/plain");
    https.addHeader("Accept-Encoding", "identity"); // Request uncompressed content
    https.addHeader("User-Agent", "ESP8266HTTPClient");
    https.addHeader("Origin", "https://api.mediboards.io");
    https.addHeader("Connection", "keep-alive");
   
    // Begin HTTPS connection
    if (https.begin(client, String("https://") + host + path)) {
      int httpCode = https.GET();
     
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          count  = https.getString();
        }
      }
     
      https.end();
    }
   
    delay(1500);
  }
}
  
void setup() {
  // Define your display layout here, e.g. 1/8 step
  display.begin(16);
  display.clearDisplay();
  display.setTextColor(myRED);
  Serial.begin(115200);
 
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  
  WiFi.mode(WIFI_STA); 
  display_ticker.attach(0.002, display_updater);
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient https;
   
    // Skip certificate validation
    client.setInsecure();
   
    // Configure timeout
    https.setTimeout(7000);
    
    display.clearDisplay();
    display.setCursor(35, 1);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print("MEDIBOARDS ");
    display.setCursor(20, 17);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print("BY SONVISAGE IS TESTINT TO KNOW HOW THE SYSTEM ");
    delay(3000);
   
    // Add required headers based on server response
    https.addHeader("Accept", "text/html,application/xhtml+xml,text/plain");
    https.addHeader("Accept-Encoding", "identity"); // Request uncompressed content
    https.addHeader("User-Agent", "ESP8266HTTPClient");
    https.addHeader("Origin", "https://api.mediboards.io");
    https.addHeader("Connection", "keep-alive");
   
    // Begin HTTPS connection
    if (https.begin(client, String("https://") + host + path)) {
      int httpCode = https.GET();
     
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          count  = https.getString();
        }
      }
     
      https.end();
    }
   
    delay(1000);
  }
}

unsigned long last_draw = 0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  uint16_t text_length = text.length();
  display.clearDisplay();
  display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);
  display.setTextColor(myRED);

  //  5 pixel character width
  for (int xpos = 128; xpos > -(32 + text_length * 5); xpos--) // Adjusted for 128 pixels width
  {
    display.setTextColor(myRED);
    display.clearDisplay();
    display.setCursor(xpos, ypos);
    display.println(text);

    display.setCursor(30, 4);
    display.setTextSize(1);
    display.setTextColor(myRED);
    display.print(" JUTH EMERGENCY WARD ");
    yield();
  }
  
  CheckApi();
  delay(1500);
}

void loop() {
  if(count !=""){
    display.clearDisplay();
    display.setCursor(25, 1);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print(" NEW PATIENT IS ON BED 345STAY TUNE FOR GOOD NEWS ");
    display.setCursor(25, 17);
    display.print(" COUCH " + (count));
    
    delay(15000);
    String txt = ("NEW PATIENT AT COUCH "+ (count));
    scroll_text(17, 0, String(txt), 96, 96, 250);
  }
  else {
    display.clearDisplay();
    display.setCursor(5, 1);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print("WELCOME TO JUTH");
    display.setCursor(12, 17);
    display.print("EMERGENCY WARD ");
    delay(15000);
    String txt = ("  HEALTH AND SERVICE  ");
    scroll_text(17, 0, String(txt), 96, 96, 250);
  }
}
