#include <Ticker.h>
#include <PxMatrix.h> //https://github.com/2dom/PxMatrix
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
Ticker display_ticker;
#include <ESP8266WiFi.h>
#include "log.h"
//#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
// API details
const char* host = "api.mediboards.io";
const String path = "/api/public/hospitals/f62ce993-58ef-43f9-a6ff-a62d023b80ab/latest-patient";
String count;
const char* ssid = "Multiverse";
const char* password = "praiseGod";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = SECRET_CH_ID_WEATHER_STATION;
unsigned int temperatureFieldNumber = 2;

// Counting channel details
unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;
unsigned int counterFieldNumber = 1; 

//Pin Definition for Nodemcu to HUB75 LED MODULE
#define P_LAT 16 //nodemcu pin D0
#define P_A 5    //nodemcu pin D1
#define P_B 4    //nodemcu pin D2
#define P_C 15   //nodemcu pin D8
#define P_OE 2   //nodemcu pin D4
#define P_D 12   //nodemcu pin D6
#define P_E 0    //nodemcu pin GND // no connection
//no clk pin no e pin in hub 75

// PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(256, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);
//PxMATRIX display(64, 64, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16 myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};

//Goto this site for converting image http://www.rinkydinkelectronics.com/t_imageconverter565.php




// ISR for display refresh
void display_updater()
{

  display.display(70);

}


  
void setup() {

  // Define your display layout here, e.g. 1/8 step
  display.begin(16);
  display.clearDisplay();
  display.setTextColor(myRED);
 Serial.begin(115200);
  // Connect to WiFi
}

unsigned long last_draw = 0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
 
  uint16_t text_length = text.length();
    //Serial.print("[HTTPS] begin...\n");
    display.clearDisplay();
  display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
  display.setTextSize(1);
  display.setRotation(0);
  display.setTextColor(myRED);

  //  5 pixel character width
  for (int xpos = 170; xpos > -(32 + text_length * 5); xpos--)
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
  //display.clearDisplay();
 
   
    // Wait before next request
    Serial.println("\n exiting the api");
    Serial.println(count);
    delay(1500);

}


void loop() {
//count="B12";
  if(count !=""){
    display.clearDisplay();
    display.setCursor(25, 1);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print(" NEW PATIENT OUR NEW PATIENT JUST CAME IN AND ARE ASSIGNED TO ");
    display.setCursor(2, 17);
    display.print("" + (count));
    Serial.println(count);
    
     delay(15000);
  //display.setCursor(1, 1);
     String txt = ("NEW PATIENT AT "+ (count));
  //This will display the scrolling text.
    // scroll_text(17, 0, String(txt), 96, 96, 250);
     //scroll_text1(17, 0, String(txt), 96, 96, 250);
    // scroll_text(17, 0, String(txt), 96, 96, 250);
    //scroll_text(y-pos, delay, "TEXT", R, G, B); 
    }
    else { Serial.println(count);
      display.clearDisplay();
    display.setCursor(5, 1);
    display.setTextSize(2);
    display.setTextColor(myRED);
    display.print("WELCOME TO JUTH");
    display.setCursor(12, 17);
    display.print("EMERGENCY WARD ");
    //CheckApi();
     delay(15000);
  //display.setCursor(1, 1);
     String txt = ("  HEALTH AND SERVICE  ");
  //This will display the scrolling text.
    // scroll_text(17, 0, String(txt), 96, 96, 250);
     //scroll_text(17, 0, String(txt), 96, 96, 250);
     //scroll_text(17, 0, String(txt), 96, 96, 250);
    //scroll_text(y-pos, delay, "TEXT", R, G, B); 
      }
}
