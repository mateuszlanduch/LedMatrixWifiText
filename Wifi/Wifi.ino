/*
Matrix LED 8x32
*/

//Declare libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "EEPROM.h"
//#include <LEDMatrixDriver.hpp>
//Headers
#include "mainpage.h"
//#include "font.h"
//#include "mat_functions.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR  // Make Arduino Due happy
#endif

#define PIN 26

//provide your own WiFi SSID and password
const char* ssid = "Yourssid";
const char* password = "Yourpassword";

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(40, 8, PIN,
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 255, 255), matrix.Color(0,255, 255), matrix.Color(255, 0, 255), matrix.Color(255, 255, 0)
};

//Create WebServer instance
WebServer server(80);

int x = matrix.width();
int pass = 0;
//Initialize message to display
String message = "";
int len = message.length();
int len2 = 30;
int len3 = len + len2;


void setup() {
  //This uses EEPROM to store previous message
  //Initialize EEPROM

  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  //For debugging
  Serial.begin(115200);
  //Use ESP32 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
  //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
    server.send(200, "text/html", page);
  });
  //POST data handler
  server.on("/dataHandler", HTTP_POST, dataHandler);
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
  //At first start, read previous message from EEPROM
  message = EEPROM.readString(0);
  //int len = message.length();
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);
  matrix.setTextColor(colors[0]);
  //Print stored message to serial port for checking
  Serial.print("Stored message: ");
  Serial.println(message);
  matrix.print("Hello World");
}

//function for handling POST data

void dataHandler() {
  String msg = server.arg("message");  //message from POST data
  Serial.print("Received: ");          //print to serial terminal
  Serial.println(msg);
  //msg.toUpperCase();  //all incoming string is converted to uppercase since no font for small cases
  message = msg;
  EEPROM.writeString(0, message);      //store received message to EEPROM
  EEPROM.commit();                     //commit the save
  server.sendHeader("Location", "/");  //redirect client to home page
  server.send(303);                    //redirect http code
}

void loop() {
  //int len = message.length();         //get message length
  server.handleClient();  //make the ESP32 respond to web clients
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(message);
  //if(len > 100) return; 175
  if (--x < -160) {
    x = matrix.width();
    if (++pass >= 7) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(90);
  
}